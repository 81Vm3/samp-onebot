
#include <samp-onebot>

new my_game_group[] = "775660140";

public OB_OnBotConnect() {
    printf("脚本成功加载 - 进服消息");
    OB_SendGroupMessage(my_game_group, "samp-onebot 启动成功!");
}

public OnPlayerConnect(playerid){

    new tmp[256], name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    format(tmp, sizeof(tmp), "___________________\n%s 加入了服务器!\n___________________", name);
    OB_SendGroupMessage(my_game_group, tmp);

	#if defined OBF_OnPlayerConnect
		OBF_OnPlayerConnect(playerid);
	#endif
	return 1;
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect OBF_OnPlayerConnect
#if defined OBF_OnPlayerConnect
	forward OBF_OnPlayerConnect();
#endif

public OnPlayerDisconnect(playerid, reason){
    new tmp[256], name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    
    if (reason == 1) {
        format(tmp, 256, "___________________\n%s 离开了服务器\n___________________", name);
        OB_SendGroupMessage(my_game_group, tmp);
    } else if (reason == 0) {
        format(tmp, 256, "___________________\n%s 掉线/崩溃了\n___________________", name);
        OB_SendGroupMessage(my_game_group, tmp);
    } else {
        format(tmp, 256, "___________________\n%s 被踢出了服务器\n___________________", name);
        OB_SendGroupMessage(my_game_group, tmp);
    }
	#if defined OBF_OnPlayerDisconnect
		OBF_OnPlayerDisconnect(playerid, reason);
	#endif
	return 1;
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect OBF_OnPlayerDisconnect
#if defined OBF_OnPlayerDisconnect
	forward OBF_OnPlayerDisconnect();
#endif