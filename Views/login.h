/*!
* \file login.h
* \brief 聊天软件登录模块实现
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

#ifndef CHATSERVER_LOGIN_H
#define CHATSERVER_LOGIN_H

#include <json/json.h>
#include "view.h"

///
/// \brief 登录模块
///
class Login : public View
{
public:
    Login();
    ~Login();

    std::string process(std::string root) override;
    std::string response(std::string status) override;
    std::string MD5(const char * message);
};


#endif //CHATSERVER_LOGIN_H
