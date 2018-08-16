#include "handlefriendrequest.h"

/// \brief  将好友列ispassed为1, 用户添加好友
/// \param root 用户和要添加的好友
/// \return 返回json包
std::string HnadleRequest::process(std::string root)
{
    //root["username"] =
    //root["friendname"]
    //root["ispassed"]
    if(root.empty())
        return nullptr;

    Json::Value json_root;
    json_root.clear();

    //返回数据包
    Json::Value json_return;
    json_return.clear();
    json_return["reason_type"] = TYPE_HANDLE_ADD_FRIEND_REQUEST;

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

        std::cout << mysql.error_num << " : " << mysql.error_info << std::endl;

    }

    char buf[2048];
    std::string query_str;
    memset(buf, 0, sizeof(buf));
    //修改ispassed,是否添加好友
    if (ispassed == 1)
    {
        //同意好友添加
        sprintf(buf, "UPDATE friends SET ispassed = '%d' WHERE  fname = '%s' AND friendname = '%s'",
                ispassed, friendname.c_str(), username.c_str());
        query_str = buf;
        if (mysql.query(query_str) == 0)
        {
            std::cerr << "mysql select error" << std::endl;
            json_return["status"] = "error";
            json_return["message"] = "查询失败";
            return json_return.toStyledString();
        }

        //将好友添加到自己好友列表
        memset(buf, 0, sizeof(buf));
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
        json_return["message"] = "添加好友成功";
        return json_return.toStyledString();
    }

    //拒绝好友添加
    memset(buf, 0, sizeof(buf));
    query_str.clear();

    //获取发送给receiver的信息
    sprintf(buf, "DELETE FROM friends WHERE fname = '%s' AND friendname = '%s'",
            friendname.c_str(), username.c_str());
    query_str = buf;
    if (mysql.query(query_str) == 0)
    {
        std::cerr << "mysql insert error" << std::endl;
        json_return["status"] = "error";
        json_return["message"] = "删除好友失败";
        return json_return.toStyledString();
    }

    json_return["status"] = "ok";
    json_return["message"] = "拒绝好友添加";
    return json_return.toStyledString();
}

/// \brief 返回给客户端处理信息
/// \param status 接收process返回状态
/// \return 返回状态给客户端
std::string HnadleRequest::response(std::string status)
{
    return status;
}
