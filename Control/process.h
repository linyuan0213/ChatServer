#ifndef CHATSERVER_PROCESS_H
#define CHATSERVER_PROCESS_H

/*!
* \file process.h
* \brief 聊天软件模块管理
*
* 通过map实现对各个模块的映射,方便拓展模块
*
* \author linyuan
* \version 1.0.0
* \date 2108年8月
*/

#include <iostream>
#include <map>
#include <string>
#include <memory>

#include "../Views/view.h"

/// \brief 模块管理
///
///
class Process
{
public:
    Process();
    std::string handle(std::string json);
    std::map<int, std::shared_ptr<View>> chat_map;  ///< 映射不同模块
};
#endif //CHATSERVER_PROCESS_H
