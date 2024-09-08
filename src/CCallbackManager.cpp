
#include "CCallbackManager.h"

std::unordered_set<AMX *> CCallbackManager::m_AmxInstances;

bool CCallbackManager::IsValidAmx(AMX *amx) {
    return m_AmxInstances.count(amx) == 1;
}

void CCallbackManager::AddAmx(AMX *amx) { m_AmxInstances.insert(amx); }
void CCallbackManager::RemoveAmx(AMX *amx) { m_AmxInstances.erase(amx); }

void CCallbackManager::OnBotConnect() {
    int iIndex = 0;
    for (auto &it : m_AmxInstances) {
        if (!amx_FindPublic(it, "OB_OnBotConnect", &iIndex)) {
            amx_Exec(it, NULL, iIndex);
        }
    }
}

void CCallbackManager::OnBotEnable() {
    int iIndex = 0;
    for (auto &it : m_AmxInstances) {
        if (!amx_FindPublic(it, "OB_OnBotEnable", &iIndex)) {
            amx_Exec(it, NULL, iIndex);
        }
    }
}

void CCallbackManager::OnBotDisable() {
    int iIndex = 0;
    for (auto &it : m_AmxInstances) {
        if (!amx_FindPublic(it, "OB_OnBotDisable", &iIndex)) {
            amx_Exec(it, NULL, iIndex);
        }
    }
}

void CCallbackManager::OnGroupMessage(uint64_t group, uint64_t user_id,
                                      const char *raw,
                                      nlohmann::json *sender_info) {
    int iIndex = 0;
    for (auto &it : m_AmxInstances) {
        if (!amx_FindPublic(it, "OB_OnGroupMessage", &iIndex)) {
            cell addr;

            std::ostringstream ss;

            amx_Push(it, reinterpret_cast<cell>(sender_info));
            amx_PushString(it, &addr, NULL, raw, NULL, NULL);

            ss << user_id;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            ss << group;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            amx_Exec(it, NULL, iIndex);
            amx_Release(it, addr);
        }
    }
}

void CCallbackManager::OnPrivateMessage(uint64_t user_id, const char *raw,
                                        int type,
                                        nlohmann::json *sender_info) {
    int iIndex = 0;
    for (auto &it : m_AmxInstances) {
        if (!amx_FindPublic(it, "OB_OnPrivateMessage", &iIndex)) {
            cell addr;

            std::ostringstream ss;

            amx_Push(it, reinterpret_cast<cell>(sender_info));
            amx_Push(it, type);
            amx_PushString(it, &addr, NULL, raw, NULL, NULL);

            ss << user_id;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            amx_Exec(it, NULL, iIndex);
            amx_Release(it, addr);
        }
    }
}

void CCallbackManager::OnGroupUpload(uint64_t group, uint64_t user_id,
                                     const nlohmann::json &file_info) {
    int iIndex = 0;
    for (auto &it : m_AmxInstances) {
        if (!amx_FindPublic(it, "OB_OnGroupUpload", &iIndex)) {
            cell addr;

            std::ostringstream ss;

            amx_PushString(it, &addr, NULL, file_info.dump().c_str(), NULL,
                           NULL);

            ss << user_id;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            ss << group;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            amx_Exec(it, NULL, iIndex);
            amx_Release(it, addr);
        }
    }
}

void CCallbackManager::OnGroupAdmin(uint64_t group, uint64_t user_id,
                                    int type) {
    int iIndex = 0;
    for (auto &it : m_AmxInstances) {
        if (!amx_FindPublic(it, "OB_OnGroupUpload", &iIndex)) {
            cell addr;

            std::ostringstream ss;

            amx_Push(it, type);

            ss << user_id;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            ss << group;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            amx_Exec(it, NULL, iIndex);
            amx_Release(it, addr);
        }
    }
}

void CCallbackManager::OnGroupDecrease(uint64_t group, uint64_t user_id,
                                       uint64_t admin, int type) {
    int iIndex = 0;
    for (auto &it : m_AmxInstances) {
        if (!amx_FindPublic(it, "OB_OnGroupDecrease", &iIndex)) {
            cell addr;

            std::ostringstream ss;

            amx_Push(it, type);

            ss << admin;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            ss << user_id;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            ss << group;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            amx_Exec(it, NULL, iIndex);
            amx_Release(it, addr);
        }
    }
}
void CCallbackManager::OnGroupIncrease(uint64_t group, uint64_t user_id,
                                       uint64_t admin, int type) {
    int iIndex = 0;
    for (auto &it : m_AmxInstances) {
        if (!amx_FindPublic(it, "OB_OnGroupIncrease", &iIndex)) {
            cell addr;

            std::ostringstream ss;

            amx_Push(it, type);

            ss << admin;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            ss << user_id;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            ss << group;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            amx_Exec(it, NULL, iIndex);
            amx_Release(it, addr);
        }
    }
}
void CCallbackManager::OnGroupBan(uint64_t group, uint64_t user_id,
                                  uint64_t admin, int type, int duration) {
    int iIndex = 0;
    for (auto &it : m_AmxInstances) {
        if (!amx_FindPublic(it, "OB_OnGroupBan", &iIndex)) {
            cell addr;

            std::ostringstream ss;

            amx_Push(it, duration);
            amx_Push(it, type);

            ss << admin;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            ss << user_id;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            ss << group;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            amx_Exec(it, NULL, iIndex);
            amx_Release(it, addr);
        }
    }
}
void CCallbackManager::OnFriendAdd(uint64_t user_id) {
    int iIndex = 0;
    for (auto &it : m_AmxInstances) {
        if (!amx_FindPublic(it, "OB_OnFriendAdd", &iIndex)) {
            cell addr;

            std::ostringstream ss;

            ss << user_id;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            amx_Exec(it, NULL, iIndex);
            amx_Release(it, addr);
        }
    }
}
void CCallbackManager::OnGroupRecall(uint64_t group, uint64_t user_id,
                                     uint64_t admin) {
    int iIndex = 0;
    for (auto &it : m_AmxInstances) {
        if (!amx_FindPublic(it, "OB_OnGroupRecall", &iIndex)) {
            cell addr;

            std::ostringstream ss;

            ss << admin;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            ss << user_id;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            ss << group;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            amx_Exec(it, NULL, iIndex);
            amx_Release(it, addr);
        }
    }
}
void CCallbackManager::OnFriendRecall(uint64_t user_id) {
    int iIndex = 0;
    for (auto &it : m_AmxInstances) {
        if (!amx_FindPublic(it, "OB_OnFriendRecall", &iIndex)) {
            cell addr;

            std::ostringstream ss;

            ss << user_id;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            amx_Exec(it, NULL, iIndex);
            amx_Release(it, addr);
        }
    }
}
void CCallbackManager::OnGroupPoke(uint64_t group, uint64_t user_id) {
    int iIndex = 0;
    for (auto &it : m_AmxInstances) {
        if (!amx_FindPublic(it, "OB_OnFriendRecall", &iIndex)) {
            cell addr;

            std::ostringstream ss;

            ss << user_id;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            ss << group;
            amx_PushString(it, &addr, NULL, ss.str().c_str(), NULL, NULL);
            ss.str("");
            ss.clear();

            amx_Exec(it, NULL, iIndex);
            amx_Release(it, addr);
        }
    }
}