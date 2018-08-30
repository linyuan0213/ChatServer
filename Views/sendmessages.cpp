//
// Created by robin on 18-8-13.
//

#include "sendmessages.h"

/// \brief 将客户端发送信息插入数据库
/// \param root 接收者和发送者信息
/// \return 返回json包
std::string SendMessage::process(std::string root)
{
    //root["reason_type"] = 4
    //root["classify"] = 1
    //root["sender"] =
    //root["receiver"] =
    //root["message"] =

    if(root.empty())
        return nullptr;

    Json::Value json_root;
    json_root.clear();

    //返回数据包
    Json::Value json_return;
    json_return.clear();
    json_return["reason_type"] = TYPE_SEND_MESSAGE;

    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();
    JSONCPP_STRING err;
    if (!reader->parse(root.data(), root.data() + root.size(), &json_root, &err))
        return nullptr;

    std::string sender = json_root["sender"].asString();
    std::string receiver = json_root["receiver"].asString();
    int classify = json_root["classify"].asInt();
    std::string message = json_root["message"].asString();

    if (sender.empty() || receiver.empty())
    {
        json_return["status"] = "error";
        json_return["message"] = "发送用户不存在";
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
    //判断receiver是否存在
    sprintf(buf, "SELECT friendname FROM friends WHERE friendname = '%s' and fname = '%s'",
            receiver.c_str(), sender.c_str());
    query_str = buf;
    std::vector<std::vector<std::string> > data;
    if (mysql.select(query_str, data) == 0)
    {
        std::cerr << "mysql select error" << std::endl;
        json_return["status"] = "error";
        json_return["message"] = "查询数据失败";
        return json_return.toStyledString();
    }

    if (data.empty())
    {
        json_return["status"] = "error";
        json_return["message"] = "发送用户不存在";
        return json_return.toStyledString();
    }

    memset(buf, 0, sizeof(buf));
    query_str.clear();
    //插入发送给receiver的信息
    sprintf(buf, "INSERT INTO messages (classify, sender, receiver, message) "
                 "VALUE (%d, '%s', '%s', '%s')",
            classify, sender.c_str(), receiver.c_str(), message.c_str());
    query_str = buf;
    if (mysql.query(query_str) == 0)
    {
        std::cerr << "error num:" << mysql.error_num << std::endl;
        json_return["status"] = "error";
        json_return["message"] = "插入数据失败";
        return json_return.toStyledString();
    }
	

	// 添加信息标志位,判断信息是否被接收
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "SELECT id FROM messages ORDER BY id DESC LIMIT 1");
	query_str = buf;
	data.clear();
	if (mysql.select(query_str, data) == 0)
	{
        std::cerr << "mysql select error" << std::endl;
        json_return["status"] = "error";
        json_return["message"] = "查询数据失败";
        return json_return.toStyledString();
	}

    sprintf(buf, "INSERT INTO messages_flg (message_id, isreceived) "
                 "VALUE (%s, %d)", data[0][0].c_str() , 0
            );
    query_str = buf;
    if (mysql.query(query_str) == 0)
    {
        std::cerr << "error num:" << mysql.error_num << std::endl;
        json_return["status"] = "error";
        json_return["message"] = "插入数据失败";
        return json_return.toStyledString();
    }

    json_return["status"] = "ok";

    return json_return.toStyledString();
}

/// \brief 返回给客户端处理信息
/// \param status 接收process返回状态
/// \return 返回状态给客户端
std::string SendMessage::response(std::string status)
{
    return status;
}
