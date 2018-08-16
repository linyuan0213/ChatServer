#ifndef CHATSERVER_SQL_H
#define CHATSERVER_SQL_H
/*!
* \file sql.h
* \brief 数据库封装
*
* 对数据库模块封装,方便调用
*
* \author linyuan
* \version 1.0.0
* \date 2108年8月
*/

#include <mysql/mysql.h>
#include <string>
#include <vector>


///
/// \brief 数据库连接信息封装
///
typedef struct
{
    char* server;    ///< 服务器ip
    char* user;      ///< 用户名
    char* password;  ///< 密码
    char* database;  ///< 连接的数据库
    int port;        ///< 端口
}MysqlConInfo;

///
/// \brief mysql模块封装
///
class Sql
{
public:
    Sql();
    ~Sql();

    // 设置连接信息
    void set_mysql_conn(char* server, char* username, char* password, char* database, int port);
    bool open();  // 打开连接
    void close(); // 关闭连接

    // 读取数据
    bool select(const std::string& query_str, std::vector<std::vector<std::string> >& data);
    bool query(const std::string& query_str);     // 其他操作
    int get_insert_id(const std::string& query_str);// 插入并获取插入的ID,针对自动递增ID
    void error_message();		 // 错误消息

public:
    int error_num;				 ///< 错误代号
    const char* error_info;       ///< 错误提示

private:
    MysqlConInfo mysql_con_info;	 ///< 连接信息
    MYSQL mysql_instance;		 ///< MySQL对象
    MYSQL_RES *result;			 ///< 用于存放结果


};

#endif //CHATSERVER_SQL_H
