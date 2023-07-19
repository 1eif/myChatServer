//
// Created by leifl on 2023/7/14.
//
#include "chatserver.hpp"
#include "json.hpp"
#include <functional>
#include <string>
#include "chatservice.hpp"

using namespace std;
using namespace placeholders;
using json = nlohmann::json;

// 初始化聊天服务器对象
ChatServer::ChatServer(muduo::net::EventLoop *loop, const muduo::net::InetAddress &listenAddr,
                       const std::string &nameArg) : server_(loop, listenAddr, nameArg), loop_(loop) {

    // 注册链接回调
    server_.setConnectionCallback(bind(&ChatServer::onConnection, this, _1));

    // 消息回调
    server_.setMessageCallback(bind(&ChatServer::onMessage, this, _1, _2, _3));

    // 设置线程数量
    server_.setThreadNum(10);

}

// 启动服务
void ChatServer::start() {
    server_.start();
}

// 上报链接相关信息回调
void ChatServer::onConnection(const TcpConnectionPtr &conn) {

    //客户端断开链接
    if (!conn->connected()) {
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

// 上报读写事件信息回调
void ChatServer::onMessage(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp time) {
    string buf = buffer->retrieveAllAsString();
    json js = json::parse(buf);

    // 要完全解耦网络模块和业务模块

    // 通过js["msgid"] 获取业务handler =》 conn js time
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
    // 执行业务处理
    msgHandler(conn, js, time);
}

