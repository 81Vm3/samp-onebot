#include <samp-onebot>
#define strcpy(%0,%1) strcat((%0[0] = '\0', %0), %1)

new my_game_group[] = "775660140";

public OB_OnBotConnect() {
    printf("脚本成功加载 - 机器人命令");
    OB_SendGroupMessage(my_game_group, "samp-onebot 启动成功!");
}

BotSendReply(bool:from_group, const user_id[], const msg[]) {
    if (from_group) {
        OB_SendGroupMessage(my_game_group, msg);
    } else {
        OB_SendPrivateMessage(user_id, msg)
    }
}

BotReceiveMessage(bool:from_group, const user_id[], const cmd[], const args[], JsonObject:sender_info) {
    if (!strcmp(cmd, "在线玩家")) {
        new player_list[512], cache[64], name[MAX_PLAYER_NAME];
        new t = GetMaxPlayers();
        new c = 1;
        for (new i = 0; i < t; i++) {
            if (!IsPlayerConnected(i))
                continue;
            GetPlayerName(i, name, MAX_PLAYER_NAME);
            format(cache, sizeof(cache), "%d. %s\n", c, name);
            strcat(player_list, cache, sizeof(player_list));
            c++;
        }
        new out[600];
        format(out, sizeof(out), "现在有 %d/%d 玩家在线:\n%s", c-1, t, player_list);
        BotSendReply(from_group, user_id, out);
    }
}

public OB_OnGroupMessage(group[], user_id[], raw[], JsonObject:sender_info) {
    new cmd[256];
    new tmp[255];
    new idx;
    cmd = strtok(raw, idx);
    strcpy(tmp, raw[idx], 255 - idx);
    BotReceiveMessage(true, user_id, cmd, tmp, sender_info);
}

public OB_OnPrivateMessage(user_id[], raw[], type, JsonObject:sender_info) {
    new cmd[256];
    new tmp[255];
    new idx;
    cmd = strtok(raw, idx);
    strcpy(tmp, raw[idx], 255 - idx);
    BotReceiveMessage(false, user_id, cmd, tmp, sender_info);
}

stock strtok(const string[], &index) {
	new length = strlen(string);
	while ((index < length) && (string[index] <= ' '))
	{
		index++;
	}

	new offset = index;
	new result[20];
	while ((index < length) && (string[index] > ' ') && ((index - offset) < (sizeof(result) - 1)))
	{
		result[index - offset] = string[index];
		index++;
	}
	result[index - offset] = EOS;
	return result;
}