
#include "chatserver.h"
#include <json/json.h>
#include "../Control/process.h"

#define  __DEBUG_ 
///
/// \brief 读取客户端发送信息,处理后,发送给客户端
/// \param conn 客户端连接信息
///
void TestServer::read_cb(Conn *conn)
{

    char buf[1024];
    std::string res;
    memset(buf, 0, sizeof(buf));

    //获取缓冲区数据
    conn->get_read_buffer(buf, sizeof(buf));
#ifdef __DEBUG_ 
	std::cout << "recv: " << std::endl;
	std::cout << buf << std::endl;
#endif
    //处理接收的数据
    Process process;
    res = process.handle(buf);

#ifdef __DEBUG_ 
	std::cout << "send: " << std::endl;
	std::cout << res << std::endl;
#endif
    memset(buf, 0, sizeof(buf));
    strncpy(buf, res.c_str(), res.length());
    conn->add_to_write_buffer(buf, strlen(buf));
}

///
/// \brief 发送数据完成后触发该函数
/// \param conn 客户端连接信息
///
void TestServer::write_cb(Conn *conn)
{

}

///
/// \brief 客户端连接成功后,调用该函数
/// \param conn 客户端连接信息
///
void TestServer::connect_cb(Conn *conn)
{
    //获取连接文件描述符
    TestServer *me = dynamic_cast<TestServer *>(conn->get_thread()->p_tcp_conn);
    std::cout << "new connection " << conn->get_fd() << std::endl;
}

///
/// \brief 客户端连接断开,调用用该函数
/// \param conn 客户端连接信息
/// \param events 客户端断开事件
void TestServer::close_cb(Conn *conn, short events)
{
    TestServer *me = dynamic_cast<TestServer *>(conn->get_thread()->p_tcp_conn);
    //判断是否正常退出
    if (events & BEV_EVENT_EOF)
        std::cout << "connection closed " << conn->get_fd() <<std::endl;
    else if (events & BEV_EVENT_ERROR)
        std::cerr << "some other error" <<std::endl;
}

///
/// \brief 初始化线程数
///
TestServer::TestServer(int count) : CoreServer(count) {}

TestServer::~TestServer()
{

}

///
/// \brief 信号处理函数,Ctrl+C 后丢调用该函数,终止服务器
/// \param sig 信号
/// \param events 返回事件
/// \param data void指针
///
void TestServer::quit_cb(int sig, short events, void *data)
{
    std::cout << "Catch the SIGINT signal, quit in one second" << std::endl;
    //获取this指针数据
    auto *me = static_cast<TestServer*>(data);
    timeval tv = {1, 0};
    //结束整个服务
    me->stop_run(&tv);
}

///
/// \brief 注册定时器后,调用该函数
/// \param id id
/// \param events 返回事件
/// \param data void 指针
///
void TestServer::timeout_cb(int id, int short events, void *data)
{
    auto *me = static_cast<TestServer*>(data);
    char temp[33] = "hello, world\n";
}
