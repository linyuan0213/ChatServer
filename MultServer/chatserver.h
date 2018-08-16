//
// Created by robin on 18-8-12.
//

#ifndef CHATSERVER_CHATSERVER_H
#define CHATSERVER_CHATSERVER_H

/*!
* \file chatserver.h
* \brief 聊天软件具体实现
*
* \author linyuan
* \version 1.0.0
* \date 2108年8月
*/

#include <iostream>
#include "server.h"
#include <vector>

class TestServer : public CoreServer
{

protected:
    //重载各个处理业务的虚函数
    void read_cb(Conn *conn) override ;
    void write_cb(Conn *conn) override ;
    void connect_cb(Conn *conn) override ;
    void close_cb(Conn *conn, short events) override ;

public:
    explicit TestServer(int count);
    ~TestServer() override;

    //退出事件，响应Ctrl+C
    static void quit_cb(int sig, short events, void *data);
    //定时器事件，每10秒向所有客户端发一句hello, world
    static void timeout_cb(int id, int short events, void *data);
};

#endif //CHATSERVER_CHATSERVER_H
