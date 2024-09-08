#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#else
#include <iconv.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <memory.h>

#include "utf8.h"
#include "logprintf.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
void utf8_to_gb(const char* src, char* dst, int len) {
    int ret = 0;
    WCHAR* strA;
    int srcLen = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
    if (srcLen <= 0) {
        logprintf("ERROR: Failed to convert from UTF-8 to WideChar.");
        return;
    }
    strA = (WCHAR*)malloc(srcLen * sizeof(WCHAR));
    MultiByteToWideChar(CP_UTF8, 0, src, -1, strA, srcLen);
    int dstLen = WideCharToMultiByte(CP_ACP, 0, strA, -1, NULL, 0, NULL, NULL);
    if (len >= dstLen && dstLen > 0) {
        ret = WideCharToMultiByte(CP_ACP, 0, strA, -1, dst, dstLen, NULL, NULL);
        dst[dstLen - 1] = 0; // Ensure null termination
    } else {
        logprintf("ERROR: Destination buffer is too small.");
    }
    if (ret <= 0) {
        logprintf("ERROR: Conversion from WideChar to GBK failed.");
    }
    free(strA);
}

void gb_to_utf8(const char* src, char* dst, int len) {
    int ret = 0;
    WCHAR* strA;
    int srcLen = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
    if (srcLen <= 0) {
        logprintf("ERROR: Failed to convert from GBK to WideChar.");
        return;
    }
    strA = (WCHAR*)malloc(srcLen * sizeof(WCHAR));
    MultiByteToWideChar(CP_ACP, 0, src, -1, strA, srcLen);
    int dstLen = WideCharToMultiByte(CP_UTF8, 0, strA, -1, NULL, 0, NULL, NULL);
    if (len >= dstLen && dstLen > 0) {
        ret = WideCharToMultiByte(CP_UTF8, 0, strA, -1, dst, dstLen, NULL, NULL);
        dst[dstLen - 1] = 0; // Ensure null termination
    } else {
        logprintf("ERROR: Destination buffer is too small.");
    }
    if (ret <= 0) {
        logprintf("ERROR: Conversion from WideChar to UTF-8 failed.");
    }
    free(strA);
}
#else   //Linux
// starkwong: In iconv implementations, inlen and outlen should be type of size_t not uint, which is different in length on Mac
void utf8_to_gb(const char* src, char* dst, int len)
{
    int ret = 0;
    size_t inlen = strlen(src) + 1;
    size_t outlen = len;

    // duanqn: The iconv function in Linux requires non-const char *
    // So we need to copy the source string
    char* inbuf = (char *)malloc(len);
    char* inbuf_hold = inbuf;   // iconv may change the address of inbuf
                                // so we use another pointer to keep the address
    memcpy(inbuf, src, len);

    char* outbuf = dst;
    iconv_t cd;
    cd = iconv_open("GBK", "UTF-8");
    if (cd != (iconv_t)-1) {
        ret = iconv(cd, &inbuf, &inlen, &outbuf, &outlen);
        if (ret != 0) {
            logprintf("iconv failed err: %s", strerror(errno));
        }

        iconv_close(cd);
    }
    free(inbuf_hold);   // Don't pass in inbuf as it may have been modified
}

void gb_to_utf8(const char* src, char* dst, int len)
{
    int ret = 0;
    size_t inlen = strlen(src) + 1;
    size_t outlen = len;

    // duanqn: The iconv function in Linux requires non-const char *
    // So we need to copy the source string
    char* inbuf = (char *)malloc(len);
    char* inbuf_hold = inbuf;   // iconv may change the address of inbuf
                                // so we use another pointer to keep the address
    memcpy(inbuf, src, len);

    char* outbuf2 = NULL;
    char* outbuf = dst;
    iconv_t cd;

    // starkwong: if src==dst, the string will become invalid during conversion since UTF-8 is 3 chars in Chinese but GBK is mostly 2 chars
    if (src == dst) {
        outbuf2 = (char*)malloc(len);
        memset(outbuf2, 0, len);
        outbuf = outbuf2;
    }

    cd = iconv_open("UTF-8", "GBK");
    if (cd != (iconv_t)-1) {
        ret = iconv(cd, &inbuf, &inlen, &outbuf, &outlen);
        if (ret != 0)
            logprintf("iconv failed err: %s", strerror(errno));

        if (outbuf2 != NULL) {
            strcpy(dst, outbuf2);
            free(outbuf2);
        }

        iconv_close(cd);
    }
    free(inbuf_hold);   // Don't pass in inbuf as it may have been modified
}
#endif