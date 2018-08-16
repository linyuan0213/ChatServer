#ifndef CHATSERVER_HANDLEFRIENDREQUEST_H
#define CHATSERVER_HANDLEFRIENDREQUEST_H

/*!
* \file handlefriendrequest.h
* \brief 聊天软件处理好友添加请求模块实现
*
* 客户端发送json数据包格式为
* json["reason_type"] \n
* json["username"] \n
* json["friendname"] \n
* json["ispassed"] 1 表示同意 0 表示拒绝 \n
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
/// \brief 处理好友请求
///
class HnadleRequest : public View
{
    std::string process(std::string root) override ;
    std::string response(std::string status) override ;

};
#endif //CHATSERVER_HANDLEFRIENDREQUEST_H
