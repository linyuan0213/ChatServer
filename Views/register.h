#ifndef CHATSERVER_REGISTER_H
#define CHATSERVER_REGISTER_H

/*!
* \file register.h
* \brief 聊天软件注册模块实现
*
* 客户端发送json数据包格式为 \n
* json["reason_type"] \n
* json["username"] \n
* json["password"] \n
*
* 服务器返回json包 \n
* json["reason_type"] \n
* json["status"] \n
* json["message"] \n
*
* process 处理登录信息, response返回登录成功或失败后的信息给客户端
*
* \author linyuan
* \version 1.0.0
* \date 2108年8月
*/

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <string>
#include <iostream>
#include <json/json.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/md5.h>

#include "../Model/sql.h"
#include "view.h"

///
/// \brief 注册模块
///
class Register : public View
{
public:
    Register();
    ~Register();

    std::string process(std::string root) override ;
    std::string response(std::string status) override ;
    std::string MD5(const char * message);
};
#endif //CHATSERVER_REGISTER_H
