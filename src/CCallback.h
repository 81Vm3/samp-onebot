#ifndef CCallback_H
#define CCallback_H

#if defined(WIN32)
#pragma warning(disable : 4201 4206 4127 4100 4305 4127)
#define STDCALL __stdcall
#define THISCALL __thiscall
#define FASTCALL __fastcall
#define SN_TARGET_PS2
#elif defined(LINUX)

#include <pthread.h>
#include <sys/mman.h>
#include <unistd.h>

#define BOOL int32_t
#define DWORD uint32_t
#define BYTE uint8_t
#define WORD uint16_t
#define TRUE (1)
#define FALSE (0)
#define STDCALL
#define THISCALL
#define FASTCALL
#define MAX_PATH 260
#endif
#include <list>
#include <memory>
#include <string>
#include <any>
#include <vector>
#include <variant>

#include <amx/amx.h>
#include <plugincommon.h>

typedef std::vector<std::variant<int, float, std::string>> ResultVector;

class CCallback {
   public:  // type definitions
    typedef std::shared_ptr<CCallback> Callback_t;
    typedef std::list<std::pair<char, std::any>> ParamList_t;

   public:  // constructor / destructor
    CCallback(AMX *amx, int cb_idx, ParamList_t &&params)
        : m_AmxInstance(amx), m_AmxCallbackIndex(cb_idx), m_Params(params) {}
    ~CCallback() = default;

   private:  // variables
    AMX *m_AmxInstance = nullptr;
    int m_AmxCallbackIndex = -1;

    ParamList_t m_Params;

   public:  // functions
    bool Execute(ResultVector results);

   public:  // factory functions
    //param_offset 指的是在pawn在*format之前有多少个参数+1
    static Callback_t Create(AMX *amx, const char *name, const char *format,
                             cell *params, cell param_offset);
};

typedef std::shared_ptr<CCallback> Callback_t;
typedef std::list<std::pair<char, std::any>> ParamList_t;

#endif