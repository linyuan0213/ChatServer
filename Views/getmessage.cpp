//
// Created by robin on 18-8-13.
//

#include "getmessage.h"

/// \brief  从数据库获取聊天信息发送给返回json包
/// \param root 接收者和发送者信息
/// \return 返回json包
std::string GetMessage::process(std::string root)
{
    //接收者 user Tom
    //root["classify"] = 1
    //root["sender"] = John
    //root["receiver"] = Tom
    if(root.empty())
        return nullptr;

    Json::Value json_root;
    json_root.clear();

    //返回数据包
    Json::Value json_return;
    json_return.clear();
    json_return["reason_type"] = TYPE_GET_MESSAGE;

    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();
    JSONCPP_STRING err;
    if (!reader->parse(root.data(), root.data() + root.size(), &json_root, &err))
        return nullptr;

    std::string sender = json_root["sender"].asString();
    std::string receiver = json_root["receiver"].asString();
    int classify = json_root["classify"].asInt();

    if (sender.empty() || receiver.empty())
    {
        json_return["status"] = "error";
        json_return["message"] = "接受信息失败";
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
    //验证
    sprintf(buf, "SELECT friendname FROM friends WHERE friendname = '%s' AND fname = '%s'",
            receiver.c_str(), sender.c_str());
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
        json_return["message"] = "用户不存在";
        return json_return.toStyledString();
    }

    memset(buf, 0, sizeof(buf));
    query_str.clear();

    //获取发送给receiver的信息
    sprintf(buf, "SELECT sender, message, sendtime FROM messages WHERE "
                 "classify = %d AND sender = '%s' AND receiver = '%s' ORDER BY id DESC LIMIT 5",
            classify, sender.c_str(), receiver.c_str());
    query_str = buf;
    data.clear();
    if (mysql.select(query_str, data) == 0)
    {
        std::cerr << "mysql select error" << std::endl;
        json_return["status"] = "error";
        json_return["message"] = "查找用户失败";
        return json_return.toStyledString();
    }

    std::vector<long> timestamp;
    for(auto& i : data)
    {
        timestamp.push_back(metis_strptime(i[2].c_str()));
    }

    int index = max_diif(timestamp);
    for (auto i : timestamp)
    {
        std::cout << i << std::endl;
    }
    std::cout << "index " << index << std::endl;
    //int i = 0;
    for (int j = 0; j < data.size(); ++j)
    {
        json_return["sender"] = data[j][0];
        json_return["message"][index] = data[j][1];
        json_return["sendtime"][index] = data[j][2];

        if (index == 0)
            break;
        index--;
    }

    return json_return.toStyledString();

}

/// \brief 返回给客户端处理信息
/// \param status 接收process返回状态
/// \return 返回状态给客户端
std::string GetMessage::response(std::string status)
{
    return status;
}

/// \brief 比较排序后相邻时间的最大值
/// \param arr 排序好的数组
/// \return 返回超过120s的下标
int GetMessage::max_diif(std::vector<long> &arr)
{
    if (arr.empty())
        return 0;
    int index = 0;
    long max = 0;
    for (long i = 1; i < arr.size(); i++)
    {
        auto diff = arr.at(i - 1) - arr.at(i);
        if (diff > max )
        {
            max = diff;
            index = static_cast<int>(i -1);
            if (max > 120)
                return index;
        }
    }

}

/// \brief 时间转时间戳
/// \param str_time 时间字符串
/// \return 时间戳
long GetMessage::metis_strptime(const char *str_time)
{
    struct tm stm;
    strptime(str_time, "%Y-%m-%d %H:%M:%S",&stm);
    long t = mktime(&stm);
    return t;
}
