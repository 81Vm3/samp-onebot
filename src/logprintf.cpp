#include "logprintf.h"

// Define the logprintf function pointer
logprintf_t logprintf_internal;

// Initialize the logprintf function pointer
void InitializeLogprintf(void** ppData) {
    logprintf_internal = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
}

void logprintf(const char* format, ...) {
    if (logprintf_internal == nullptr) {
        return;  
    }

    char buffer[1024];

    va_list args;
    va_start(args, format);

    static char prefix[] = "samp-onebot: ";
    vsnprintf(buffer + sizeof(prefix), sizeof(buffer) - sizeof(prefix), format, args);

    va_end(args);

    snprintf(buffer, sizeof(buffer), "%s %s", prefix, buffer + sizeof(prefix));
    logprintf_internal("%s", buffer);
}
