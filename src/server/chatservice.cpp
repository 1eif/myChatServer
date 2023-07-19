//
// Created by leifl on 2023/7/14.
//
#include "chatservice.hpp"
#include "public.hpp"
#include <muduo/base/Logging.h>
#include "user.hpp"
#include <vector>


using namespace std;
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
    msgHandlerMap_.insert({ONE_CHAT_MSG, bind(&ChatService::oneChat, this, _1, _2, _3)});
    msgHandlerMap_.insert({ADD_FRIEND_MSG, bind(&ChatService::addFriend, this, _1, _2, _3)});
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
// 登录   检测密码是否正确
void ChatService::login(const TcpConnectionPtr &conn, json &js, Timestamp time) {

    int id = js["id"];
    string pwd = js["password"];

    User user = userModel_.query(id);

    if (user.getId() == id && user.getPwd() == pwd) {
        if (user.getState() == "online") {
            // 该用户已经登录 不允许重复登录
            LOG_INFO << "do login service failed, user is already online";
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 2;
            response["errmsg"] = "该用户已经登录";
            conn->send(response.dump());
            return;
        } else {
            // 登录成功
            {
                // 互斥锁
                lock_guard<mutex> lock(connMutex_);
                // 登录成功，记录用户连接信息
                userConnMap_.insert({id, conn});
            }

            // 登录成功 更新用户状态信息
            LOG_INFO << "do login service success";

            user.setState("online");
            userModel_.updateState(user);

            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 0;
            response["id"] = user.getId();
            response["name"] = user.getName();

            // 查询该用户是否有离线消息
            vector<string> vec = offlineMessageModel_.query(id);

            if (!vec.empty()) {
                response["offlinemsg"] = vec;
                // 读取该用户的离线消息后，把该用户的所有离线消息删除
                offlineMessageModel_.remove(id);
            }

            // 查询该用户好友信息并返回 并通知好友上线
            vector<User> userVec = friendModel_.query(id);
            if(!userVec.empty()){
                vector<string> vec2;

                for(User &friendUser : userVec){
                    json js;
                    js["id"] = friendUser.getId();
                    js["name"] = friendUser.getName();
                    js["state"] = friendUser.getState();
                    vec2.push_back(js.dump());

                    {
                        lock_guard<mutex> lock(connMutex_);
                        // 通知好友上线信息
                        auto it = userConnMap_.find(friendUser.getId());
                        if (it != userConnMap_.end()) {
                            json js;
                            js["msgid"] = FRIEND_LOGIN_STATE_MSG;
                            js["id"] = user.getId();
                            js["name"] = user.getName();
                            it->second->send(js.dump());

                        }
                    }

                }
                response["friends"] = vec2;

            }
            conn->send(response.dump());
        }

    } else {
        // 登录失败 用户不存在或者密码错误
        LOG_INFO << "do login service failed";
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["errno"] = 1;
        response["errmsg"] = "用户名或密码错误";
        conn->send(response.dump());
    }
}

// 注册   name pwd
void ChatService::reg(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    string name = js["name"];
    string pwd = js["password"];

    User user;
    user.setName(name);
    user.setPwd(pwd);

    bool state = userModel_.insert(user);
    if (state) {
        // 注册成功
        LOG_INFO << "do reg service success";
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 0;
        response["id"] = user.getId();
        conn->send(response.dump());
    } else {
        // 注册失败
        LOG_INFO << "do reg service failed";
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 1;
        conn->send(response.dump());
    }
}

// 处理客户端异常退出
void ChatService::clientCloseException(const TcpConnectionPtr &conn) {

    User user;

    // 互斥锁作用域
    {
        lock_guard<mutex> lock(connMutex_);
        // 找到userConnMap_中对应的用户链接信息并删除
        for (auto it = userConnMap_.begin();  it != userConnMap_.end() ; ++it) {
            if (it->second == conn) {
                user.setId(it->first);
                userConnMap_.erase(it);
                break;
            }
        }
    }

    if(user.getId() != -1) {
        // 更新用户的状态信息
        user.setState("offline");
        userModel_.updateState(user);
    }
}


// 一对一聊天业务
void ChatService::oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    int toid = js["to"].get<int>();

    // 加锁
    {
        lock_guard<mutex> lock(connMutex_);
        auto it = userConnMap_.find(toid);

        if (it != userConnMap_.end()) {
            // 在线，转发消息
            it->second->send(js.dump());
            return;
        }
    }

    // 不在线，存储离线消息
    offlineMessageModel_.insert(toid, js.dump());
}

// 服务器异常 重置
void ChatService::reset() {
    userModel_.resetState();
}

// 添加好友
void ChatService::addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    int id = js["id"].get<int>();
    int friendid = js["friendid"].get<int>();

    friendModel_.addFriend(id, friendid);
}

// 创建群组
void ChatService::createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    int id = js["id"].get<int>();
    string name = js["groupname"];
    string desc = js["groupdesc"];

    Group group(-1, name, desc);
    if(groupModel_.createGroup(group)){
        // 存储群组创建人信息
        groupModel_.addGroup(id, group.getId(), "creator");
    }
}

// 加入群组
void ChatService::addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    int id = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();

    groupModel_.addGroup(id, groupid, "normal");
}

// 群聊天
void ChatService::groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();

    vector<int> useridVec = groupModel_.queryGroupUsers(userid, groupid);

    {
        lock_guard<mutex> lock(connMutex_);
        for (int id : useridVec) {
            auto it = userConnMap_.find(id);
            if (it != userConnMap_.end()) {
                it->second->send(js.dump());
            } else {
                // 存储离线消息
                string msg = js.dump();
                offlineMessageModel_.insert(id, msg);
            }
        }
    }
}