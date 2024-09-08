#include "natives.h"

#include "CBot.h"
#include "CCallback.h"
#include "CPluginApp.h"
#include "logprintf.h"
#include "utf8.h"

cell AMX_NATIVE_CALL OB_Connect(AMX* amx, cell* params) {
    CPluginApp::getInstance().getBot().connect();
    return 1;
}

cell AMX_NATIVE_CALL OB_SendGroupMessage(AMX* amx, cell* params) {
    char *szGroup, *szMessage;
    amx_StrParam(amx, params[1], szGroup);
    amx_StrParam(amx, params[2], szMessage);
    bool escape = static_cast<bool>(params[3]);

    std::stringstream ss;
    ss << szGroup;
    uint64_t group;
    ss >> group;

    int sz = strlen(szMessage);
    char* p = new char[sz * 3];
    gb_to_utf8(szMessage, p, sz * 3);

    CPluginApp::getInstance().getBot().sendGroupMessage(group, p, escape);
    delete p;
    return 1;
}

cell AMX_NATIVE_CALL OB_SendPrivateMessage(AMX* amx, cell* params) {
    char *szAccount, *szMessage;
    amx_StrParam(amx, params[1], szAccount);
    amx_StrParam(amx, params[2], szMessage);
    bool escape = static_cast<bool>(params[3]);

    std::stringstream ss;
    ss << szAccount;
    uint64_t user;
    ss >> user;

    int sz = strlen(szMessage);
    char* p = new char[sz * 3];
    gb_to_utf8(szMessage, p, sz * 3);

    CPluginApp::getInstance().getBot().sendPrivateMessage(user, p, escape);
    delete p;
    return 1;
}

cell AMX_NATIVE_CALL OB_GetJson(AMX* amx, cell* params) {
    using namespace nlohmann;
    char* szRaw;
    amx_StrParam(amx, params[1], szRaw);

    try {
        int len = strlen(szRaw);
        char* p = new char[len * 3];
        gb_to_utf8(szRaw, p, len * 3);

        json* j = new json(json::parse(p));  // Parse the raw string as JSON
        delete p;
        return reinterpret_cast<cell>(j);
    } catch (const json::parse_error& e) {
        // Handle JSON parsing errors
        logprintf("JSON Parsing Error: %s", e.what());
        return 0;  // Return an error code or null pointer
    } catch (const std::exception& e) {
        // Handle other exceptions
        logprintf("Exception: %s", e.what());
        return 0;
    }
}

cell AMX_NATIVE_CALL OB_FreeJson(AMX* amx, cell* params) {
    using namespace nlohmann;
    json* j = reinterpret_cast<json*>(params[1]);

    try {
        delete j;
    } catch (const std::exception& e) {
        logprintf("Exception in OB_FreeJson: %s", e.what());
        return 0;
    }
    return 1;
}

cell AMX_NATIVE_CALL OB_GetJsonString(AMX* amx, cell* params) {
    using namespace nlohmann;
    json* j = reinterpret_cast<json*>(params[1]);

    char* szPath;
    amx_StrParam(amx, params[2], szPath);

    cell* out_addr = 0;
    amx_GetAddr(amx, params[3], &out_addr);

    try {
        std::string out;
        j->at(szPath).get_to(out);  // Access JSON by path and get the string value

        if (out.size()) {
            char* p = new char[out.size() * 3];
            utf8_to_gb(out.c_str(), p, out.size() * 3);
            amx_SetString(out_addr, p, 0, 0, params[4]);
            delete p;
        } else {
            amx_SetString(out_addr, "", 0, 0, params[4]);
        }

    } catch (const json::out_of_range& e) {
        // Handle missing keys in JSON
        logprintf("JSON Out of Range Error: %s", e.what());
        return 0;  // Return an error code
    } catch (const std::exception& e) {
        // Handle other exceptions
        logprintf("Exception: %s", e.what());
        return 0;
    }
    return 1;
}

cell AMX_NATIVE_CALL OB_DumpJson(AMX* amx, cell* params) {
    using namespace nlohmann;
    json* j = reinterpret_cast<json*>(params[1]);

    cell* out_addr = 0;
    amx_GetAddr(amx, params[2], &out_addr);

    std::string s = j->dump();

    char* p = new char[s.size() * 3];
    utf8_to_gb(s.c_str(), p, s.size() * 3);
    amx_SetString(out_addr, p, 0, 0, params[3]);
    delete p;
    return 1;
}

cell AMX_NATIVE_CALL OB_SendGroupKick(AMX* amx, cell* params) {
    char *szGroup, *szUser;
    amx_StrParam(amx, params[1], szGroup);
    amx_StrParam(amx, params[2], szUser);

    std::stringstream ss;
    ss << szGroup;
    uint64_t group;
    ss >> group;

    ss.str("");
    ss.clear();
    ss << szUser;
    uint64_t user;
    ss >> user;

    CPluginApp::getInstance().getBot().sendGroupKick(group, user);
    return 1;
}

cell AMX_NATIVE_CALL OB_GetGroupInfo(AMX* amx, cell* params) {
    char* szGroup;
    amx_StrParam(amx, params[1], szGroup);

    std::stringstream ss;
    ss << szGroup;
    uint64_t group;
    ss >> group;

    CPluginApp::getInstance().getBot().getGroupInfo(group, amx, params);
    return 1;
}

cell AMX_NATIVE_CALL OB_GetMemberInfo(AMX* amx, cell* params) {
    char *szGroup, *szUser;
    amx_StrParam(amx, params[1], szGroup);
    amx_StrParam(amx, params[2], szUser);

    std::stringstream ss;
    ss << szGroup;
    uint64_t group;
    ss >> group;

    ss.str("");
    ss.clear();
    ss << szUser;
    uint64_t user;
    ss >> user;

    CPluginApp::getInstance().getBot().getMemberInfo(group,user, amx, params);
    return 1;
}

cell AMX_NATIVE_CALL OB_GetMemberList(AMX* amx, cell* params) {
    char *szGroup, *szUser;
    amx_StrParam(amx, params[1], szGroup);
    amx_StrParam(amx, params[2], szUser);

    std::stringstream ss;
    ss << szGroup;
    uint64_t group;
    ss >> group;

    CPluginApp::getInstance().getBot().getMemberList(group, amx, params);
    return 1;
}

AMX_NATIVE_INFO PluginNatives[] = {
    {"OB_Connect", OB_Connect},
    {"OB_SendGroupMessage", OB_SendGroupMessage},
    {"OB_SendPrivateMessage", OB_SendPrivateMessage},
    {"OB_GetJson", OB_GetJson},
    {"OB_FreeJson", OB_FreeJson},
    {"OB_GetJsonString", OB_GetJsonString},
    {"OB_DumpJson", OB_DumpJson},
    {"OB_SendGroupKick", OB_SendGroupKick},
    {"OB_GetGroupInfo", OB_GetGroupInfo},
    {"OB_GetMemberInfo", OB_GetMemberInfo},
    {"OB_GetMemberList", OB_GetMemberList},
    {0, 0}};