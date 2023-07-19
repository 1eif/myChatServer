//
// Created by leifl on 2023/7/19.
//

#ifndef CHAT_GROUPUSER_HPP
#define CHAT_GROUPUSER_HPP

#include "user.hpp"

class GroupUser : public User{
public:
    void setRole(string role) { this->role = role; }

    string getRole() { return this->role; }
private:
    string role;
};

#endif //CHAT_GROUPUSER_HPP
