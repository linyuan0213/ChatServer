#include "addfriend.h"
#include "../Log/log.h"
/// \brief  将添加好友插入数据库,ispassed为0
/// \param root 用户和要添加的好友
/// \return 返回json包
std::string AddFriend::process(std::string root)
{
    //root["usename"]
    //root["friendname"]
    //root["ispassed"] = 0
    if(root.empty())
        return nullptr;

    Json::Value json_root;
    json_root.clear();

    //返回数据包
    Json::Value json_return;
    json_return.clear();
    json_return["reason_type"] = TYPE_ADD_FRIEND;

    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();
    JSONCPP_STRING err;
    if (!reader->parse(root.data(), root.data() + root.size(), &json_root, &err))
        return nullptr;

    std::string username = json_root["username"].asString();
    std::string friendname = json_root["friendname"].asString();
    int ispassed = json_root["ispassed"].asInt();


    if (username.empty() || friendname.empty())
    {
        json_return["status"] = "error";
        json_return["message"] = "添加好友失败";
        return json_return.toStyledString();
    }

    Sql mysql;
    mysql.set_mysql_conn(const_cast<char *>("localhost"), const_cast<char *>("root"),
                         const_cast<char *>("Ly19960to13##"), const_cast<char *>("chat"), 0);
    if (!mysql.open())
    {

        std::cerr << mysql.error_num << " : " << mysql.error_info << std::endl;
		LOGERROR("mysql open error:%s, errorno: %d", mysql.error_info, mysql.error_num);
    }

    char buf[2048];
    std::string query_str;
    memset(buf, 0, sizeof(buf));
    //验证所添加好友是否存在
    sprintf(buf, "SELECT username FROM users WHERE username = '%s'", friendname.c_str());
    query_str = buf;
    std::vector<std::vector<std::string> > data;
    if (mysql.select(query_str, data) == 0)
    {
        std::cerr << "mysql select error" << std::endl;
        json_return["status"] = "error";
        json_return["message"] = "查询失败";
        return json_return.toStyledString();
    }

    if (data.empty())
    {
        json_return["status"] = "error";
        json_return["message"] = "要添加的用户不存在";
        return json_return.toStyledString();
    }

    memset(buf, 0, sizeof(buf));
    query_str.clear();

    //添加好友
    sprintf(buf, "INSERT INTO friends (fname, friendname, ispassed)  VALUE('%s', '%s', %d)",
            username.c_str(), friendname.c_str(), ispassed);
    query_str = buf;
    if (mysql.query(query_str) == 0)
    {
        std::cerr << "mysql insert error" << std::endl;
        json_return["status"] = "error";
        json_return["message"] = "添加好友失败";
        return json_return.toStyledString();
    }

    json_return["status"] = "ok";
    return json_return.toStyledString();
}

/// \brief 返回给客户端处理信息
/// \param status 接收process返回状态
/// \return 返回状态给客户端
std::string AddFriend::response(std::string status)
{
    return status;
}
