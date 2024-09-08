#ifndef NATIVES_H
#define NATIVES_H

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

#include <sstream>


extern AMX_NATIVE_INFO PluginNatives[];

#endif