/*!
* \file main.cpp
* \brief 聊天软件主函数
*
* \author linyuan
* \version 1.0.0
* \date 2108年8月
*/

#include <iostream>
#include <memory>
#include "../Model/sql.h"
#include "../Views/login.h"
#include "../Views/register.h"
#include "../Control/process.h"
#include "../MultServer/server.h"
#include "../MultServer/chatserver.h"
#include "config.h"

#define __DEBUG_ 
int main()
{
	int worker = 0;
	short port = 0;
	std::map<std::string, std::string> conf;

	//配置文件在bin目录下
	load_config("./chat.conf", conf);
	for (auto &i : conf)
	{
		if (i.first == "worker")
			worker = atoi(i.second.c_str());
		if (i.first == "port")
			port = atoi(i.second.c_str());
	}
#ifdef __DEBUG_

	std::cout << "worker:" << worker << std::endl;
	std::cout << "port:" << port << std::endl;	

    std::cout << "pid: " << getpid() << std::endl;
#endif 

    TestServer server(worker);
    server.add_signal_event(SIGINT, TestServer::quit_cb);
    //timeva.addl tv = {1, 0};
    //server.add_timer_event(TestServer::timeout_cb, tv, false);
    server.set_port(port);
    server.start_run();
#ifdef __DEBUG_ 
    std::cout << "done" << std::endl;
#endif 
    return 0;
 

    /*
    Json::Value root;
    root["reason_type"] = 3;
    root["islogin"] = true;
    root["username"] = "Jobs";

    std::string res;
    Process process;
    res = process.handle(root.toStyledString());
    std::cout << res << std::endl;
    */

    return 0;

}

/*
void test1()
{
    Sql mysql;
    mysql.set_mysql_conn(const_cast<char *>("localhost"), const_cast<char *>("root"),
                         const_cast<char *>("Ly19960to13##"), const_cast<char *>("mytest"), 0);

    if (!mysql.open())
    {

        std::cout << mysql.error_num << " : " << mysql.error_info << std::endl;

    }

    // 读取数据
    std::vector<std::vector<std::string> > data;
    std::string sql_str = "SELECT * FROM mytest_tbl";
    mysql.select(sql_str, data);

    // 显示数据
    for (unsigned int i = 0; i < data.size(); ++i)
    {
        for (unsigned int j = 0; j < data[0].size(); ++j)
        {
            std::cout << data[i][j] << "\t\t";
        }
        std::cout << std::endl;

    }
}
 */

/*
void test2()
{
    event_base * base = event_base_new();
    std::shared_ptr<ChatConn> test = std::make_shared<ChatConn>();
    test->bind_event_base(base);
    test->create_event_for_listen("0.0.0.0", 9999);
    event_base_dispatch(base);
}

 void test3()
 {
    Json::Value root;
    root["username"] = "Tom";
    root["passwd"]   = "22334";

    //std::string ret;
    //Login login;
    //ret = login.process(root.toStyledString());
    std::string ret;
    Login login;
    ret = login.process(root.toStyledString());
    std::cout << ret << std::endl;
 }
 */
