//
// Created by leifl on 2023/7/19.
//

#ifndef CHAT_FRIENDMODEL_HPP
#define CHAT_FRIENDMODEL_HPP

#include <vector>
#include "user.hpp"

using namespace std;

// 提供好友信息表的操作接口方法
class FriendModel {
public:
    // 添加好友
    bool addFriend(int userid, int friendid);

    // 删除好友
    bool deleteFriend(int userid, int friendid);

    // 查询好友列表
    vector<User> query(int userid);
private:
};

#endif //CHAT_FRIENDMODEL_HPP
