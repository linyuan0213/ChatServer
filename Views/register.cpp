//
// Created by robin on 18-8-6.
//

#include "register.h"
#include "../Log/log.h"

Register::Register()
{

}

Register::~Register()
{

}

/// \brief 注册用户
/// \param root
/// \return
std::string Register::process(std::string root)
{
    if(root.empty())
        return "error";

    Json::Value json_root;
    json_root.clear();

    //返回响应数据包
    Json::Value json_return;
    json_return.clear();
    json_return["reason_type"] = TYPE_REGISTER;

    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();
    JSONCPP_STRING err;
    if (!reader->parse(root.data(), root.data() + root.size(), &json_root, &err))
        return nullptr;

    std::string username = json_root["username"].asString();
    std::string passwd   = json_root["passwd"].asString();
    if (username.empty() || passwd.empty())
    {
        json_return["status"] = "error";
        json_return["message"] = "账户或密码不能为空";

        return json_return.toStyledString();
    }

    Sql mysql;
    std::string insert_str;
    std::string query_str;
    char buf[100];
    mysql.set_mysql_conn(const_cast<char *>("localhost"), const_cast<char *>("root"),
                         const_cast<char *>("Ly19960to13##"), const_cast<char *>("chat"), 0);

    if (!mysql.open())
    {

        std::cout << mysql.error_num << " : " << mysql.error_info << std::endl;
		LOGERROR("mysql open error:%s, errorno: %d", mysql.error_info, mysql.error_num);

    }

    //判断注册用户是否存在,不存在则注册
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "SELECT username FROM users WHERE username = %s", username.c_str());
    query_str = buf;
    std::vector<std::vector<std::string> > data;
    mysql.select(query_str, data);

    if (!data.empty())
    {
        json_return["status"] = "error";
        json_return["message"] = "用户名已经存在";
        return json_return.toStyledString();
    }


    memset(buf, 0, sizeof(buf));
    sprintf(buf, "INSERT INTO users (username, password) VALUES('%s', '%s');", username.c_str(), MD5(passwd.c_str()).c_str());
    insert_str = buf;
    // 插入数据
    if(mysql.query(insert_str))
    {
        json_return["status"] = "ok";
        return json_return.toStyledString();
    }

    std::cerr << "error num:" << mysql.error_num << std::endl;
    json_return["status"] = "error";
    json_return["message"] = "注册失败";
    return json_return.toStyledString();
}

/// \brief 返回给客户端处理信息
/// \param status 接收process返回状态
/// \return 返回状态给客户端
std::string Register::response(std::string status)
{
    return status;
}

/// \brief 对密码采用md5加密
/// \param message 要加密的字符串
/// \return 返回加密后的字符串
std::string Register::MD5(const char *message)
{
    std::string digest;
    CryptoPP::Weak::MD5 md5;
    CryptoPP::StringSource src(message, true,
                               new CryptoPP::HashFilter(md5, new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest))));
    std::transform(digest.begin(), digest.end(), digest.begin(), ::tolower);
    return digest;
}
