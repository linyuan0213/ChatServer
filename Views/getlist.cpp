//
// Created by robin on 18-8-12.
//

#include "getlist.h"

GetList::GetList()
{

}

GetList::~GetList()
{

}

/// \brief 通过json包返回好友列表
/// \param root 查询好友信息包
/// \return 成功返回好友列表,出错返回错误信息
std::string GetList::process(std::string root)
{
    //root["reason_type"] = 3
    //root["username"]
    //root["islogin"] = true

    if(root.empty())
        return nullptr;

    Json::Value json_root;
    json_root.clear();

    //返回好友列表
    Json::Value json_return;
    json_return.clear();
    json_return["reason_type"] = TYPE_GET_LIST;

    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();
    JSONCPP_STRING err;
    if (!reader->parse(root.data(), root.data() + root.size(), &json_root, &err))
        return nullptr;

    std::string username = json_root["username"].asString();
    bool islogin = json_root["islogin"].asBool();
    if (username.empty() || !islogin)
    {
        json_return["status"] = "error";
        json_return["message"] = "用户未登录";
        return json_return.toStyledString();
    }

    Sql mysql;
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "SELECT friendname, ispassed FROM friends WHERE fname = '%s'", username.c_str());
    std::string query_str = buf;
    mysql.set_mysql_conn(const_cast<char *>("localhost"), const_cast<char *>("root"),
                         const_cast<char *>("Ly19960to13##"), const_cast<char *>("chat"), 0);
    if (!mysql.open())
    {

        std::cout << mysql.error_num << " : " << mysql.error_info << std::endl;

    } 

    // 读取数据
    std::vector<std::vector<std::string> > data;
    mysql.select(query_str, data);

    if (!data.empty())
    {
        int i = 0;
        json_return["status"] = "ok";
		for (auto it = data.begin(); it != data.end(); it++)
		{
		    json_return["ispassed"][i] = (*it)[1];
			json_return["user_list"][i] = (*it)[0];
			i++;
		}

        return json_return.toStyledString();
    }
    json_return["status"] = "error";
    json_return["message"] = "获取用户列表失败";
    return json_return.toStyledString();
}

/// \brief 返回给客户端处理信息
/// \param status 接收process返回状态
/// \return 返回状态给客户端
std::string GetList::response(std::string status)
{
    if (status == "error")
        return "error";
    return status;
}
