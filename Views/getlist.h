#ifndef CHATSERVER_GETLIST_H
#define CHATSERVER_GETLIST_H

/*!
* \file getlist.h
* \brief 聊天软件获取好友列表
*
* 客户端发送json数据包格式为\n
* json["reason_type"]\n
* json["username"]\n
* json["islogin"] true 已经登录\n
*
* 服务器返回json包
* json["reason_type"]\n
* json["status"]\n
* json["message"] error\n
* json["user_list"] status = ok\n
* json["ispassed"]\n
*
* process 处理登录信息, response返回登录成功或失败后的信息给客户端
*
* \author linyuan
* \version 1.0.0
* \date 2108年8月
*/

#include <json/json.h>
#include <cstring>

#include "view.h"
#include "../Model/sql.h"

///
/// \brief 获取好友列表
///
class GetList : public View
{
public:
    GetList();
    ~GetList();

    std::string process(std::string root) override;
    std::string response(std::string status) override;
};

#endif //CHATSERVER_GETLIST_H
