//
// Created by leifl on 2023/7/14.
//
#ifndef CHAT_CHATSERVICE_HPP
#define CHAT_CHATSERVICE_HPP

#include "json.hpp"
#include <unordered_map>
#include <functional>
#include <muduo/net/TcpConnection.h>
#include "server/model/UserModel.hpp"
#include <mutex>
#include "server/model/OfflinemessageModel.hpp"
#include "server/model/FriendModel.hpp"
#include "server/model/GroupModel.hpp"

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

    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);

    // 一对一聊天业务
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 服务器异常 重置
    void reset();

    // 添加好友
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 创建群组
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 加入群组
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 群聊天
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);

private:
    ChatService();

    // 存储消息id和对应业务处理方法
    unordered_map<int, MsgHandler> msgHandlerMap_;

    // 数据操作类对象
    UserModel userModel_;

    // 存储在线用户的通信连接
    unordered_map<int, TcpConnectionPtr> userConnMap_;

    // 定义互斥锁，保证userConnMap_的线程安全
    mutex connMutex_;

    // 存储离线消息的model对象
    OfflineMessageModel offlineMessageModel_;

    // 存储好友列表的model对象
    FriendModel friendModel_;

    // 存储群组列表的model对象
    GroupModel groupModel_;

};

#endif //CHAT_CHATSERVICE_HPP
