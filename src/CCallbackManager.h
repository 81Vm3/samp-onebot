#ifndef CALLBACKMANAGER_H
#define CALLBACKMANAGER_H

#if defined(WIN32)
#pragma warning(disable : 4201 4206 4127 4100 4305 4127)
#define STDCALL __stdcall
#define THISCALL __thiscall
#define FASTCALL __fastcall
#define SN_TARGET_PS2
#elif defined(LINUX)
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>


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
#include <array>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <sstream>
#include <nlohmann/json.hpp>

#include <amx/amx.h>
#include <plugincommon.h>
#include "logprintf.h"

class CCallbackManager {
   private:  // variables
    static std::unordered_set<AMX *> m_AmxInstances;

   public:  // functions
    static bool IsValidAmx(AMX *amx);

    static void AddAmx(AMX *amx);
    static void RemoveAmx(AMX *amx);

    static void OnBotConnect();
	static void OnBotEnable();
	static void OnBotDisable();
	static void OnGroupMessage(uint64_t group, uint64_t user_id, const char *raw, nlohmann::json* sender_info);
	static void OnPrivateMessage(uint64_t user_id, const char *raw, int type, nlohmann::json* sender_info);
	static void OnGroupUpload(uint64_t group, uint64_t user_id, const nlohmann::json& file_info);
	static void OnGroupAdmin(uint64_t group, uint64_t user_id, int type);

	static void OnGroupDecrease(uint64_t group, uint64_t user_id, uint64_t admin, int type);
	static void OnGroupIncrease(uint64_t group, uint64_t user_id, uint64_t admin, int type);
	static void OnGroupBan(uint64_t group, uint64_t user_id, uint64_t admin, int type, int duration);
	static void OnFriendAdd(uint64_t user_id);
	static void OnGroupRecall(uint64_t group, uint64_t user_id, uint64_t admin);
	static void OnFriendRecall(uint64_t user_id);
	static void OnGroupPoke(uint64_t group, uint64_t user_id);
};
#endif