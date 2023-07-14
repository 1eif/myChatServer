//
// Created by leifl on 2023/7/14.
//
#include "chatservice.hpp"
#include "public.hpp"
#include <muduo/base/Logging.h>

using namespace muduo;

// 获取单例对象的接口函数
ChatService *ChatService::instance() {
    static ChatService service;
    return &service;
}

// 注册消息及对应Handler回调
ChatService::ChatService() {
    msgHandlerMap_.insert({LOGIN_MSG, bind(&ChatService::login, this, _1, _2, _3)});
    msgHandlerMap_.insert({REG_MSG, bind(&ChatService::reg, this, _1, _2, _3)});
}

// 获取消息对应处理器
MsgHandler ChatService::getHandler(int msgid) {

    auto it = msgHandlerMap_.find(msgid);

    // 消息没有对应的处理器，返回空的操作
    if (it == msgHandlerMap_.end()) {
        return [=](const TcpConnectionPtr &conn, json &js, Timestamp time) -> void {
            LOG_ERROR << "msgid:" << msgid << "can not find handler!!";
        };

    } else {
        return msgHandlerMap_[msgid];
    }


}

// 业务
// 登录
void ChatService::login(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    LOG_INFO << "login service!!!!";
}

// 注册
void ChatService::reg(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    LOG_INFO << "register service!!!!";
}