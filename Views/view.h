#ifndef CHATSERVER_VIEW_H
#define CHATSERVER_VIEW_H

/*!
* \file view.h
* \brief 视图模块,提供处理模块相同的接口
*
* 作为各个聊天模块的父类,对各个模块具体实现隐藏,提供统一的访问接口,
* 采用MVC模式
*
* \author linyuan
* \version 1.0.0
* \date 2108年8月
*/

#include <iostream>

#include "chatdefine.h"

///
/// \brif 视图模块
///
class View
{
public:
    virtual std::string process(std::string root) = 0;
    virtual std::string response(std::string status) = 0;
};
#endif //CHATSERVER_VIEW_H
