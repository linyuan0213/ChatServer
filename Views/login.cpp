#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "login.h"
#include "../Model/sql.h"
#include <iostream>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/md5.h>
#include <algorithm>

/// 从数据库读取账号,和登录用户匹配
/// \param root
/// \return 成功返回json包 status= "ok",表示成功,"error",出错,message表示出错信息
std::string Login::process(std::string root)
{
    if(root.empty())
        return nullptr;

    //解析json数据包
    Json::Value json_root;
    json_root.clear();

    //封装送返回包
    Json::Value json_return;
    json_root.clear();

    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();
    JSONCPP_STRING err;
    if (!reader->parse(root.data(), root.data() + root.size(), &json_root, &err))
        return nullptr;

    std::string username = json_root["username"].asString();
    std::string passwd   = json_root["passwd"].asString();
    if (username.empty() || passwd.empty())
    {
        json_return["reason_type"] = TYPE_LOGIN;
        json_return["status"] = "error";
        json_return["message"] = "账户或密码不能为空";
        return json_return.toStyledString();
    }

    Sql mysql;
    std::string query_str = "SELECT * FROM users WHERE username='" + username + "' and password=MD5('" + passwd + "')";
    mysql.set_mysql_conn(const_cast<char *>("localhost"), const_cast<char *>("root"),
                         const_cast<char *>("Ly19960to13##"), const_cast<char *>("chat"), 0);

    if (!mysql.open())
    {

        std::cout << mysql.error_num << " : " << mysql.error_info << std::endl;

    }

    // 读取数据
    std::vector<std::vector<std::string> > data;
    mysql.select(query_str, data);

    std::string md5 = MD5(passwd.c_str());
    if (!data.empty())
    {
        if (data[0][1] == username && data[0][2] == md5)
        {
            json_return["reason_type"] = TYPE_LOGIN;
            json_return["status"] = "ok";
            return json_return.toStyledString();
        }
    }

    json_return["reason_type"] = TYPE_LOGIN;
    json_return["status"] = "error";
    json_return["message"] = "账户或密码不匹配";
    return  json_return.toStyledString();

}

/// \brief 返回给客户端处理信息
/// \param status 接收process返回状态
/// \return 返回状态给客户端
std::string Login::response(std::string status)
{
    return status;
}

/// \brief 对密码采用md5加密
/// \param message 要加密的字符串
/// \return 返回加密后的字符串
std::string Login::MD5(const char * message)
{
    std::string digest;
    CryptoPP::Weak::MD5 md5;
    CryptoPP::StringSource src(message, true,
                 new CryptoPP::HashFilter(md5, new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest))));

    std::transform(digest.begin(), digest.end(), digest.begin(), ::tolower);
    return digest;
}

Login::Login()
{

}

Login::~Login()
{

}
