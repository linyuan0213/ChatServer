#ifndef CHATSERVER_GETMESSAGE_H
#define CHATSERVER_GETMESSAGE_H

/*!
* \file getmessage.h
* \brief 聊天软件聊天模块实现
*
* 客户端发送json数据包格式为 \n
* json["reason_type"] \n
* json["classify"] = 1 \n
* json["sender"] \n
* json["receiver"] \n
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

#include <string>
#include <json/json.h>
#include <cstring>
#include <ctime>

#include "../Model/sql.h"
#include "view.h"

///
/// \brief 获取聊天信息
///
class GetMessage : public View
{
    std::string process(std::string root) override ;
    std::string response(std::string status) override ;

};
#endif //CHATSERVER_GETMESSAGE_H
