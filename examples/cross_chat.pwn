#include <samp-onebot>

new my_game_group[] = "775660140";

public OB_OnBotConnect() {
    printf("脚本成功加载 - 群消息互通");
    OB_SendGroupMessage(my_game_group, "samp-onebot 启动成功!");
}

public OB_OnGroupMessage(group[], user_id[], raw[], JsonObject:sender_info) {
    new szName[256]; // 群友的名字
    new szCard[256]; // 群友的名片
    OB_GetSenderCard(sender_info, szCard, sizeof(szCard));
    OB_GetSenderName(sender_info, szName, sizeof(szName))
    if (strlen(szCard) > 0) {
        // 如果有名片
        format(szName, sizeof(szName), "%s", szCard);
    }
    
    new szMsg[512];
    format(szMsg, sizeof(szMsg), "{E56DFF}[群消息]{ffffff} %s: %s", szName, raw);
    SendClientMessageToAll(-1, szMsg);
}

public OnPlayerText(playerid, text[]) {
    new szName[256];
    GetPlayerName(playerid, szName, sizeof(szName));

    new szMsg[512];
    format(szMsg, sizeof(szMsg), "%s: %s", szName, text);
    OB_SendGroupMessage(my_game_group, szMsg);
    return 1;
}