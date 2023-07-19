//
// Created by leifl on 2023/7/15.
//

#ifndef CHAT_USERMODEL_HPP
#define CHAT_USERMODEL_HPP

#include "user.hpp"

// // 提供user表的操作接口方法
class UserModel {
public:
    // User表的增加方法
    bool insert(User &user);

    // 根据用户号码查询用户信息
    User query(int id);

    // 更新用户的状态信息
    bool updateState(User user);
private:
};


#endif //CHAT_USERMODEL_HPP
