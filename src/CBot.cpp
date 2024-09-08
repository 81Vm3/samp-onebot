#include "CBot.h"

#include "CCallback.h"
#include "CCallbackManager.h"
#include "CConfig.h"
#include "CPluginApp.h"
#include "utf8.h"

CBot::CBot() { connected = false; }
CBot::~CBot() { disconnect(); }

void CBot::registerEvents() {
    using namespace twobot;
    using namespace twobot::Event;
    using twobot::ApiSet;

    botInstance->onEvent<ConnectEvent>([this](const ConnectEvent& msg) {
        logprintf("Bot connected.");
        CCallbackManager::OnBotConnect();
    });

    botInstance->onEvent<DisableEvent>([this](const DisableEvent& msg) {
        logprintf("Bot disabled.");
        CCallbackManager::OnBotEnable();
    });

    botInstance->onEvent<EnableEvent>([this](const EnableEvent& msg) {
        logprintf("Bot enabled");
        CCallbackManager::OnBotDisable();
    });

    botInstance->onEvent<GroupMsg>([this](const GroupMsg& msg) {
        if (msg.raw_message.size()) {
            char* p = new char[msg.raw_message.size() * 3];
            utf8_to_gb(msg.raw_message.c_str(), p, msg.raw_message.size() * 3);

            nlohmann::json j(msg.sender);
            CCallbackManager::OnGroupMessage(msg.group_id, msg.user_id, p, &j);

            delete p;
        }
    });

    botInstance->onEvent<PrivateMsg>([this](const PrivateMsg& msg) {
        if (msg.raw_message.size()) {
            char* p = new char[msg.raw_message.size() * 3];
            utf8_to_gb(msg.raw_message.c_str(), p, msg.raw_message.size() * 3);

            nlohmann::json j(msg.sender);
            CCallbackManager::OnPrivateMessage(msg.user_id, p, msg.sub_type,
                                               &j);
            delete p;
        }
    });

    botInstance->onEvent<GroupUploadNotice>([this](
                                                const GroupUploadNotice& msg) {
        CCallbackManager::OnGroupUpload(msg.group_id, msg.user_id, msg.file);
    });

    botInstance->onEvent<GroupAdminNotice>([this](const GroupAdminNotice& msg) {
        CCallbackManager::OnGroupAdmin(msg.group_id, msg.user_id, msg.sub_type);
    });

    botInstance->onEvent<GroupDecreaseNotice>(
        [this](const GroupDecreaseNotice& msg) {
            CCallbackManager::OnGroupDecrease(msg.group_id, msg.user_id,
                                              msg.operator_id, msg.sub_type);
        });

    botInstance->onEvent<GroupInceaseNotice>(
        [this](const GroupInceaseNotice& msg) {
            CCallbackManager::OnGroupIncrease(msg.group_id, msg.user_id,
                                              msg.operator_id, msg.sub_type);
        });

    botInstance->onEvent<GroupBanNotice>([this](const GroupBanNotice& msg) {
        CCallbackManager::OnGroupBan(msg.group_id, msg.user_id, msg.operator_id,
                                     msg.sub_type, msg.duration);
    });

    botInstance->onEvent<FriendAddNotice>([this](const FriendAddNotice& msg) {
        CCallbackManager::OnFriendAdd(msg.user_id);
    });

    botInstance->onEvent<GroupRecallNotice>(
        [this](const GroupRecallNotice& msg) {
            CCallbackManager::OnGroupRecall(msg.group_id, msg.user_id,
                                            msg.operator_id);
        });

    botInstance->onEvent<FriendRecallNotice>(
        [this](const FriendRecallNotice& msg) {
            CCallbackManager::OnFriendRecall(msg.user_id);
        });

    botInstance->onEvent<GroupNotifyNotice>(
        [this](const GroupNotifyNotice& msg) {
            if (msg.sub_type == msg.POKE) {
                CCallbackManager::OnGroupPoke(msg.group_id, msg.user_id);
            }
        });
}

bool CBot::connect() {
    using namespace twobot;
    using namespace twobot::Event;
    using twobot::ApiSet;

    auto config = CPluginApp::getInstance().getConfig();

    if (botInstance.get()) {
        return false;
    } else {
        messageThread = new ThreadPool(config.getThreadCount());
        botThread = new ThreadPool(1);

        botInstance = BotInstance::createInstance(
            Config{config.getHost(), config.getApiPort(), config.getWSPort(),
                   config.getToken()});
        registerEvents();

        logprintf("Started websocket server!");
        botThread->enqueue([&config, this] { botInstance->start(); });
        return true;
    }
}

bool CBot::disconnect() {
    if (!botInstance.get()) {
        return false;
    } else {
        logprintf("Bot shutting down...");
        delete messageThread;
        delete botThread;
        botInstance.release();
        return true;
    }
}

void CBot::sendGroupMessage(uint64_t group, std::string plain,
                            bool auto_escape) {
    if (botInstance.get()) {
        messageThread->enqueue([this, group, plain, auto_escape] {
            botInstance->getApiSet().sendGroupMsg(group, plain, auto_escape);
        });
    }
}

void CBot::sendPrivateMessage(uint64_t user, std::string plain,
                              bool auto_escape) {
    if (botInstance.get()) {
        messageThread->enqueue([this, user, plain, auto_escape] {
            botInstance->getApiSet().sendPrivateMsg(user, plain, auto_escape);
        });
    }
}

void CBot::sendGroupKick(uint64_t group, uint64_t user) {
    if (botInstance.get()) {
        messageThread->enqueue([this, group, user] {
            botInstance->getApiSet().setGroupKick(group, user);
        });
    }
}

void CBot::getGroupInfo(uint64_t group, AMX* amx, cell* params) {
    if (botInstance.get()) {
        char *szCallback, *szFormat;
        amx_StrParam(amx, params[2], szCallback);
        amx_StrParam(amx, params[3], szFormat);

        Callback_t callback = CCallback::Create(amx, szCallback, szFormat, params, 4);
        
        messageThread->enqueue([this, group, callback] {
            auto it = botInstance->getApiSet().getGroupInfo(group);
            if (it.first) {
                using namespace nlohmann;
                json& j = it.second["data"];

                callback->Execute({
                    j["group_name"].get<std::string>(),
                    j["member_count"].get<int>(), 
                    j["max_member_count"].get<int>()
                });
            }
        });
    }
}

void CBot::getMemberInfo(uint64_t group, uint64_t user, AMX* amx, cell* params) {
    if (botInstance.get()) {
        char *szCallback, *szFormat;
        amx_StrParam(amx, params[3], szCallback);
        amx_StrParam(amx, params[4], szFormat);

        Callback_t callback = CCallback::Create(amx, szCallback, szFormat, params, 5);
        messageThread->enqueue([this, group, user, callback] {
            auto it = botInstance->getApiSet().getGroupMemberInfo(group, user);
                printf(it.second.dump().c_str());

            if (it.first) {
                using namespace nlohmann;
                json& j = it.second["data"];

                callback->Execute({
                    j["nickname"].get<std::string>(),
                    j["card"].get<std::string>(), 
                    j["join_time"].get<int>(),
                    j["last_send_time"].get<int>(),
                    j["level"].get<std::string>(),
                    j["role"].get<std::string>()
                });
            }
        });
    }
}

void CBot::getMemberList(uint64_t group, AMX* amx, cell* params) {
    if (botInstance.get()) {
        char *szCallback, *szFormat;
        amx_StrParam(amx, params[2], szCallback);
        amx_StrParam(amx, params[3], szFormat);

        Callback_t callback = CCallback::Create(amx, szCallback, szFormat, params, 4);
        messageThread->enqueue([this, group, callback] {
            auto it = botInstance->getApiSet().getGroupMemberList(group);
            printf(it.second.dump().c_str());
            if (it.first) {
                using namespace nlohmann;
                /* json& j = it.second["data"];
                callback->Execute({
                    j["nickname"].get<std::string>(),
                    j["card"].get<std::string>(), 
                    j["join_time"].get<int>(),
                    j["last_send_time"].get<int>(),
                    j["level"].get<std::string>(),
                    j["role"].get<std::string>()
                }); */
            }
        });
    }
}
