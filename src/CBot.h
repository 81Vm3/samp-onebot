#ifndef CBOT_H
#define CBOT_H

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
	

	#define BOOL    int32_t
	#define DWORD   uint32_t
	#define BYTE    uint8_t
	#define WORD    uint16_t
	#define TRUE    (1)
	#define FALSE   (0)
	#define STDCALL
	#define THISCALL
	#define FASTCALL
	#define MAX_PATH 260
#endif
#include <memory> 

#include <amx/amx.h>
#include <plugincommon.h>

#include <memory>
#include <twobot.hh>
#include <thread>

#include "ThreadPool.h"

class CBot {
private:
    std::unique_ptr<twobot::BotInstance> botInstance;
    ThreadPool *botThread;
    ThreadPool *messageThread;
    bool connected;

public:
    CBot();
    ~CBot();

    void registerEvents();
    bool connect();
    bool disconnect();

    void sendGroupMessage(uint64_t group, std::string plain, bool auto_escape);
    void sendPrivateMessage(uint64_t user, std::string plain, bool auto_escape);
    void sendGroupKick(uint64_t group, uint64_t user);
    void getGroupInfo(uint64_t group, AMX* amx, cell* params);
	void getMemberInfo(uint64_t group, uint64_t user, AMX* amx, cell* params);
	void getMemberList(uint64_t group, AMX* amx, cell* params);

};

#endif