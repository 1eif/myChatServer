//
// Created by leifl on 2023/7/14.
//

/*
 * server和client的公共文件*/
#ifndef CHAT_PUBLIC_HPP
#define CHAT_PUBLIC_HPP

enum EnMsgType {
    LOGIN_MSG = 1,  // 登录消息
    LOGIN_MSG_ACK, // 登录响应消息
    REG_MSG,     // 注册消息
    REG_MSG_ACK, // 注册响应消息
    ONE_CHAT_MSG, // 聊天消息 一对一
    ADD_FRIEND_MSG, // 添加好友消息
    FRIEND_LOGIN_STATE_MSG, // 好友状态信息
    CREATE_GROUP_MSG, // 创建群组
    ADD_GROUP_MSG, // 加入群组
    GROUP_CHAT_MSG, // 群聊天
};
#endif //CHAT_PUBLIC_HPP
