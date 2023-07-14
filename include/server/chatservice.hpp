//
// Created by leifl on 2023/7/14.
//
#ifndef CHAT_CHATSERVICE_HPP
#define CHAT_CHATSERVICE_HPP

#include "json.hpp"
#include <unordered_map>
#include <functional>
#include <muduo/net/TcpConnection.h>

using namespace std;
using namespace muduo;
using namespace muduo::net;

using json = nlohmann::json;

// 处理消息的事件回调方法类型
using MsgHandler = function<void(const TcpConnectionPtr &, json &, Timestamp)>;

// 聊天服务器业务类
class ChatService {
public:
    // 获取单例对象的接口函数
    static ChatService *instance();

    // 获取消息对应处理器
    MsgHandler getHandler(int);

    // 业务
    // 登录
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 注册
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);

private:
    ChatService();

    // 存储消息id和对应业务处理方法
    unordered_map<int, MsgHandler> msgHandlerMap_;
};

#endif //CHAT_CHATSERVICE_HPP
