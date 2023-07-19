//
// Created by leifl on 2023/7/19.
//

#include "server/model/FriendModel.hpp"
#include "db.hpp"
//using namespace std;

// 添加好友
bool FriendModel::addFriend(int userid, int friendid) {
    char sql[1024] = {0};

    sprintf(sql, "insert into friend values(%d, %d)", userid, friendid);

    MySQL mysql;

    if (mysql.connect()) {
        mysql.update(sql);
        return true;
    }
    return false;
}

// 删除好友
bool FriendModel::deleteFriend(int userid, int friendid) {
    char sql[1024] = {0};

    sprintf(sql, "delete from friend where userid = %d and friendid = %d", userid, friendid);

    MySQL mysql;

    if (mysql.connect()) {
        mysql.update(sql);
        return true;
    }
    return false;
}

vector<User> FriendModel::query(int userid) {

    vector<User> vec;

    char sql[1024] = {0};
    sprintf(sql, "select a.id, a.name, a.state from user a inner join friend b on b.friendid = a.id where b.userid = %d", userid);

    MySQL mySql;

    if (mySql.connect()) {
        MYSQL_RES *res = mySql.query(sql);
        if (res != nullptr) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr) {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                vec.push_back(user);
            }
            mysql_free_result(res);
        }
    }
    return vec;
}