//
// Created by leifl on 2023/7/19.
//

#ifndef CHAT_GROUPMODEL_HPP
#define CHAT_GROUPMODEL_HPP

#include "group.hpp"

class GroupModel {
public:
    // 创建群组
    bool createGroup(Group &group);

    // 加入群组
    void addGroup(int userid, int groupid, string role);

    // 查询用户所在群组信息
    vector<Group> queryGroups(int userid);

    // 根据指定的groupid查询群组用户id列表，除userid自己 用于群聊发消息
    vector<int> queryGroupUsers(int userid, int groupid);
private:
};

#endif //CHAT_GROUPMODEL_HPP
