# SAMP-ONEBOT

基于TwoBotFramework在C++中编写的samp插件，允许开发者在pawn中使用OneBot接口

## 原理

一个OneBot机器人接受和传递消息的流程如下：

OneBot SDK -> OneBot实现 -> 社交App平台 (如QQ，微信，Telegram)

在TwoBotFramework C++ SDK的基础上，本插件包装了TwoBot作为pawn语言下的OneBot 的的SDK，允许你简单地在pawn中编写自己的机器人代码

## 配置文件 - samp-onebot.json
```json
{
    "host": "127.0.0.1",
    "ws_port": 8080,
    "api_port": 8081,
    "thread_count": 6,
    "token": ""
}
```
#### 说明:
1. host - 上游Onebot API的地址
2. ws_port - Websocket服务器端口
3. api_port - 上游Onebot API的端口
4. thread_count - 消息线程池的最大线程数量
5. token - API的密码，无则留空
## 示例代码 - 在游戏内显示群消息

```pwn
#include <samp-onebot>

my_game_group[] = "775660140";

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

```

## 编译

#### Windows下编译

```cmd
cd samp-onebot-master
mkdir build
cmake ..
```
之后打开sln文件或者vcproj文件

#### Linux下编译
首先确保你安装了gcc和g++，然后你需要添加x86的库
```bash
sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get install g++-multilib gcc-multilib
sudo apt-get install libssl-dev:i386
sudo apt-get install libc6-dev-i386
```

## 举例函数调用

### OB_Connect()
开启机器人 (开启WebSocket服务器并开始监听Onebot实现)

### OB_SendGroupMessage(const group[], const message[], escape=0)
发送群消息

### OB_SendPrivateMessage(const account[], const message[], escape=0)
发送私信

### OB_GetSenderName(JsonObject:json, name[], name_len) 
获取消息发送者的名称

### OB_GetSenderCard(JsonObject:json, card[], card_len)
获取群名片

## 举例回调


### OB_OnGroupMessage(group[], user_id[], raw[], JsonObject:sender_info);
收到群消息

sender_info 为json指针，json对象会在函数调用后被销毁


### OB_OnPrivateMessage(user_id[], raw[], type, JsonObject:sender_info);
收到私信

sender_info 为json指针，json对象会在函数调用后被销毁

type:

0. 好友  
1. 群私聊
2. 其他


### OB_OnGroupIncrease(group[], user_id[], admin[], type);
OB_OnGroupIncrease 新群成员入群事件

admin 操作者QQ 如果是主动加群，和user_id一致

type:

0. 管理员已同意入群
1. 被踢出
2. 管理员邀请入群