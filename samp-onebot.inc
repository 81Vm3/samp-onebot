#if defined _onebot_included
	#endinput
#endif
#define _onebot_included

// ======================== 函数 ========================

// 开启机器人 (开启WebSocket服务器并开始监听Onebot实现)
native OB_Connect();

// 关闭WebSocket服务器
native OB_Disconnect();

// OB_SendGroupMessage - 发送群消息
native OB_SendGroupMessage(const group[], const message[], escape=0);

// OB_SendPrivateMessage - 发送私信
native OB_SendPrivateMessage(const account[], const message[], escape=0);

native JsonObject:OB_GetJson(const raw_json[]);
native OB_FreeJson(JsonObject:json);
native OB_GetJsonString(JsonObject:json, const path[], out[], out_len);
native OB_DumpJson(JsonObject:json, out[], out_len);

// OB_GetSenderName - 获取消息发送者的名称
stock OB_GetSenderName(JsonObject:json, name[], name_len) {
	OB_GetJsonString(json, "nickname", name, name_len);
}

// OB_GetSenderCard - 获取群名片
stock OB_GetSenderCard(JsonObject:json, card[], card_len) {
	OB_GetJsonString(json, "card", card, card_len);
}

// OB_GetSenderTitle - 获取群专属头衔
stock OB_GetSenderTitle(JsonObject:json, title[], title_len) {
	OB_GetJsonString(json, "title", title, title_len);
}

// OB_GetSenderLevel - 获取等级
stock OB_GetSenderLevel(JsonObject:json, level[], level_len) {
	OB_GetJsonString(json, "level", level, level_len);
}

// OB_SendGroupKick - 踢出群成员
native OB_SendGroupKick(const group[], const user[]);

// OB_GetGroupInfo - 获取群详细信息
// 回调格式: Callback(group_name[], members, max_members, ...)
native OB_GetGroupInfo(const group[], const callback[] = "", const format[] = "", ...);

// OB_GetMemberInfo - 获取群成员详细信息
// 回调格式: Callback(nickname[], card[], join_time, last_send_time, level[], role[], ...)
native OB_GetMemberInfo(const group[], const member_id[], const callback[] = "", const format[] = "", ...);

// OB_GetMemberList - 获取群成员列表
// 回调格式: Callback(nickname[], card[], join_time, last_send_time, level[], role[], ...)
//native OB_GetMemberList(const group[], const callback[] = "", const format[] = "", ...);


// ======================== 回调 ========================
forward OB_OnBotConnect();
forward OB_OnBotEnable();
forward OB_OnBotDisable();

// OB_OnGroupMessage 收到群消息
// sender_info 为json指针，json对象会在函数调用后被销毁
forward OB_OnGroupMessage(group[], user_id[], raw[], JsonObject:sender_info);

// OB_OnPrivateMessage 收到私信
// sender_info 为json指针，json对象会在函数调用后被销毁
// type:
// 0. 好友  
// 1. 群私聊
// 2. 其他
forward OB_OnPrivateMessage(user_id[], raw[], type, JsonObject:sender_info);

// OB_OnGroupUpload 当某人上传了群文件
forward OB_OnGroupUpload(group[], user_id[], file_info[]);

// OB_OnGroupAdmin 管理员事件
// type:
// 0. 设置了管理员
// 1. 取消设置了管理员
forward OB_OnGroupAdmin(group[], user_id[], type);

// OB_OnGroupDecrease 群成员退群事件
// type:
// 0. 退出
// 1. 被踢出
// 2. 机器人被踢出
forward OB_OnGroupDecrease(group[], user_id[], admin[], type);

// OB_OnGroupIncrease 新群成员入群事件
// admin 操作者QQ 如果是主动加群，和user_id一致
// type:
// 0. 管理员已同意入群
// 1. 被踢出
// 2. 管理员邀请入群
forward OB_OnGroupIncrease(group[], user_id[], admin[], type);

// OB_OnGroupBan 某人被禁言
// type:
// 0. 禁言
// 1. 解除禁言
forward OB_OnGroupBan(group[], user_id[], admin[], type, duration);

// OB_OnFriendAdd 新添加好友
forward OB_OnFriendAdd(user_id[]);

// OB_OnGroupRecall 群消息撤回事件
forward OB_OnGroupRecall(group[], user_id[], admin[]);

// OB_OnFriendRecall 好友消息撤回事件
forward OB_OnFriendRecall(user_id[]);

// OB_OnGroupPoke 机器人在群内被戳一戳事件
forward OB_OnGroupPoke(group[], user_id[]);