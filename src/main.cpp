#include "main.h"

extern void *pAMXFunctions;

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
    return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) {
    pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
    InitializeLogprintf(ppData);

    CPluginApp &a = CPluginApp::getInstance();
    a.init();
    try {
        a.getConfig().load(std::string("samp-onebot.json"));
        a.printPluginMessage();
    } catch (std::runtime_error &ex) {
        auto s = ex.what();
        logprintf("Cannot load the config: %s", s);
        return true;
    }

    return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload() {
    logprintf(" * samp-onebot was unloaded.");
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx) {
    CCallbackManager::AddAmx(amx);
    return amx_Register(amx, PluginNatives, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx) { 
    CCallbackManager::RemoveAmx(amx);
    return AMX_ERR_NONE; 
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick() {
	
}