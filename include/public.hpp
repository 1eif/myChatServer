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
};
#endif //CHAT_PUBLIC_HPP
