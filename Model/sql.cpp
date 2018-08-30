//
// Created by robin on 18-8-4.
//

#include <iostream>
#include "sql.h"
#include "../Log/log.h"

///
/// \brief 构造函数,初始化error_num和error_info
///
Sql::Sql(): error_num(0), error_info("ok")
{
    mysql_library_init(0, nullptr, nullptr);
    mysql_init(&mysql_instance);

    // 设置字符集，否则无法处理中文
    mysql_options(&mysql_instance, MYSQL_SET_CHARSET_NAME, "utf8");

}

///
/// \brief 析构函数,关闭数据库连接
///
Sql::~Sql()
{
    close();
}

///
/// \brief 设置连接信息
/// \param server 服务器ip
/// \param username 用户名
/// \param password 密码
/// \param database 连接的数据库
/// \param port 端口号,0为默认
///
void Sql::set_mysql_conn(char *server, char *username, char *password, char *database, int port)
{
    mysql_con_info.server = server;
    mysql_con_info.user   = username;
    mysql_con_info.password = password;
    mysql_con_info.database = database;
    mysql_con_info.port = port;

}

///
/// \brief 连接数据库
///
bool Sql::open()
{
    if (mysql_real_connect(&mysql_instance, mysql_con_info.server, mysql_con_info.user,
                           mysql_con_info.password, mysql_con_info.database, mysql_con_info.port, 0, 0) != nullptr)
    {
        return true;
    }
    else
    {
        error_message();
        return false;
    }

}

///
/// \brief 关闭数据库连接
///
void Sql::close()
{
    mysql_close(&mysql_instance);
}

///
/// \brief 查询数据库调用select等命令
/// \param query_str 查询命令
/// \param data  查询结果
/// \return 成功返回true
/// \note  需要调用mysql_store_result存储数据
///
bool Sql::select(const std::string &query_str, std::vector<std::vector<std::string> > &data)
{
    if (0 != mysql_query(&mysql_instance, query_str.c_str()))
    {
        error_message();
        return false;
    }

    result = mysql_store_result(&mysql_instance);

    // 行列数
    //int row = mysql_num_rows(result);
    int field = mysql_num_fields(result);

    MYSQL_ROW line = nullptr;
    line = mysql_fetch_row(result);

    std::string temp;
    //std::vector<std::vector<std::string> >().swap(data);
    while (nullptr != line)
    {
        std::vector<std::string> line_data;
        for (int i = 0; i < field; i++)
        {
            if (line[i])
            {
                temp = line[i];
                line_data.push_back(temp);
            }
            else
            {
                temp = "";
                line_data.push_back(temp);
            }
        }
        line = mysql_fetch_row(result);
        data.push_back(line_data);
    }
    return true;
}

///
/// \brief 调用update delete等命令
/// \param query_str 查询命令
/// \return 成功返回true
///
bool Sql::query(const std::string &query_str)
{
    if (0 == mysql_query(&mysql_instance, query_str.c_str()))
    {
        return true;
    }
    error_message();
    return false;
}

///
/// \brief 插入并获取id
/// \param query_str 查询命令
/// \return 成功返回true
///
int Sql::get_insert_id(const std::string &query_str)
{
    if (!query(query_str))
    {
        error_message();
        return -1;
    }
    // 获取ID
    return mysql_insert_id(&mysql_instance);
}


///
/// \brief 出错信息处理
///
void Sql::error_message()
{
    error_num = mysql_errno(&mysql_instance);
    error_info = mysql_error(&mysql_instance);
    std::cerr << "error_info" << error_info << std::endl;
	LOGERROR("error_info: %s, error_num: %d", error_info, error_num);
}


