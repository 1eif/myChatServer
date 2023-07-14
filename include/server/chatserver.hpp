//
// Created by leifl on 2023/7/14.
//

#ifndef CHAT_CHATSERVER_HPP
#define CHAT_CHATSERVER_HPP

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;

class ChatServer {
public:
    // 初始化聊天服务器对象
    ChatServer(EventLoop *loop, const InetAddress &listenAddr, const string &nameArg);

    // 启动服务
    void start();

private:
    // 上报链接相关信息回调
    void onConnection(const TcpConnectionPtr &);

    // 上报读写事件信息回调
    void onMessage(const TcpConnectionPtr &, Buffer *, Timestamp);

    TcpServer server_;  // 服务器功能呢类对象
    EventLoop *loop_;   // 指向事件循环对象的指针
};

#endif //CHAT_CHATSERVER_HPP
