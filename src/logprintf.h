#ifndef LOGPRINTF_H
#define LOGPRINTF_H

#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <plugincommon.h>

typedef void (*logprintf_t)(const char* format, ...);

// Declare the logprintf function pointer
//extern logprintf_t logprintf;
void logprintf(const char* format, ...);

// Function to initialize the logprintf function pointer
void InitializeLogprintf(void **ppData);


#endif