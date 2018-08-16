#ifndef CHATSERVER_ADDFRIEND_H
#define CHATSERVER_ADDFRIEND_H

/*!
* \file addfriend.h
* \brief 聊天软件添加好友模块实现
*
* 客户端发送json数据包格式为
* json["reason_type"] \n
* json["username"] \n
* json["friendname"] \n
* json["ispassed"] 0 \n
*
* 服务器返回json包
* json["reason_type"] \n
* json["status"] \n
* json["message"] \n
*
* process 处理登录信息, response返回登录成功或失败后的信息给客户端
*
* \author linyuan
* \version 1.0.0
* \date 2108年8月
* \note ispassed 必须为0,表示等待好友处理
*/

#include <string>
#include <json/json.h>
#include <cstring>
#include <ctime>

#include "../Model/sql.h"
#include "view.h"

///
/// \brief 添加好友
///
class AddFriend : public View
{
public:
    std::string process(std::string root) override;
    std::string response(std::string status) override;
};
#endif //CHATSERVER_ADDFRIEND_H
