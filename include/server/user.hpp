//
// Created by leifl on 2023/7/15.
//
#ifndef CHAT_USER_HPP
#define CHAT_USER_HPP

#include <string>

using namespace std;

// 用户信息映射
class User {
public:

    User(int id = -1, string name = "", string pwd = "", string state = "offline") {
        this->id = id;
        this->name = name;
        this->password = pwd;
        this->state = state;
    }

    void setId(int id) { this->id = id; }

    void setName(string name) { this->name = name; }

    void setPwd(string pwd) { this->password = pwd; }

    void setState(string state) { this->state = state; }

    int getId() { return this->id; }

    string getName() { return this->name; }

    string getPwd() { return this->password; }

    string getState() { return this->state; }

private:
    int id;
    string name;
    string password;
    string state;
};


#endif //CHAT_USER_HPP
