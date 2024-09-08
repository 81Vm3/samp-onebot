
#ifndef _UTF8_H
#define _UTF8_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * UTF-8 to GBK
 * @param src [in]
 * @param dst [out]
 * @param len [in] The most bytes which starting at dst, will be written.
 *
 */
void utf8_to_gb(const char* src, char* dst, int len);

/**
 * GBK to UTF-8
 *
 * @param src [in]
 * @param dst [out]
 * @param len [in] The most bytes which starting at dst, will be written.
 */
void gb_to_utf8(const char* src, char* dst, int len);

#ifdef __cplusplus
}
#endif

#endif  // end of _UTF8_H
