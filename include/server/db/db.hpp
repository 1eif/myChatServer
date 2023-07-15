//
// Created by leifl on 2023/7/15.
//

#ifndef CHAT_DB_HPP
#define CHAT_DB_HPP

#include <string>
#include <mysql/mysql.h>

using namespace std;

// 数据库操作类
class MySQL {
public:
    // 初始化数据库连接
    MySQL();

    // 释放数据库连接资源
    ~MySQL();

    // 连接数据库
    bool connect();

    // 更新操作
    bool update(string sql);

    // 查询操作
    MYSQL_RES *query(string sql);

    // 获取连接
    MYSQL *getConnection();

private:
    MYSQL *conn_;
};


#endif //CHAT_DB_HPP
