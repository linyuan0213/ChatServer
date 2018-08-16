#ifndef MULTISERVER_SERVER_H
#define MULTISERVER_SERVER_H
/*!
* \file server.h
* \brief 聊天软件核心模块
*
* 处理连接客户端连接,采用libevent库和线程池作为核心处理模块
* 采用管道进行线程间通信,将管道文件描述符加入事件监听主线程得
* 到客户端文件描述符,通过管道发送给子线程处理
*
* \author linyuan
* \version 1.0.0
* \date 2108年8月
*/
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <errno.h>
#include <signal.h>
#include <ctime>
#include <pthread.h>
#include <fcntl.h>
#include <cassert>
#include <sys/socket.h>

#include <iostream>
#include <list>
#include <thread>
#include <mutex>

#include <event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include "threadpool.h"


class CoreServer;
struct LibeventThread;
class Conn;


///
/// \brief 封装发送和接收模块
///
class Conn
{
    friend class CoreServer;	//在CoreServer中访问Conn

private:
    const int m_fd;             ///<socket的ID
    evbuffer *m_readbuf;        ///<读数据的缓冲区
    evbuffer *m_writebuf;       ///<写数据的缓冲区

    LibeventThread *m_thread;   ///<线程池指针

public:
    explicit Conn(int fd=0);
    ~Conn();

public:
    LibeventThread *get_thread();
    int get_fd();

    //获取可读数据的长度
    int get_read_buffer_len();

    //从读缓冲区中取出len个字节的数据，存入buffer中，若不够，则读出所有数据
    //返回读出数据的字节数
    int get_read_buffer(char *buffer, int len);

    //从读缓冲区中复制出len个字节的数据，存入buffer中，若不够，则复制出所有数据
    //返回复制出数据的字节数
    //buffer中的数据只是原数据的副本
    int copy_read_buffer(char *buffer, int len);

    //获取可写数据的长度
    int get_write_buffer_len();

    //将数据加入写缓冲区，准备发送
    int add_to_write_buffer(char *buffer, int len);

    //将读缓冲区中的数据移动到写缓冲区
    void move_buffer_data();

}; 


///
/// \brief 对线程池封装
///
struct LibeventThread
{
    pthread_t tid;                  //线程的ID
    struct event_base *base;        //libevent的事件处理
    struct event notify_event;      //监听管理的事件
    int notify_receive_fd;          //管道的接收端
    int notify_send_fd;             //管道的发送端

    std::list<Conn *> conn;         //储存每个连接的数据

    CoreServer *p_tcp_conn;         //通过类指针访问回调函数
}; 


///
/// \brief 核心类,创建多线程libevent主框架
///
class CoreServer
{
private:
    static const int EXIT_CODE = -1;       ///<程序结束码
    static const int MAX_SIGNAL = 256;     ///<最大信号数

private:
    int m_thread_num;                      ///<子线程数
    int m_port;                            ///<监听的端口
    LibeventThread *m_main_base;           ///<主线程的event base
    LibeventThread *m_threads;             ///<存储各个子线程信息的数组
    event *m_signal_events[MAX_SIGNAL];    ///<自定义的信号处理
    ThreadPool *pool;                      ///<线程池

private:

    //初始化子线程的数据
    void init_thread(LibeventThread *thread);
    //子线程的入口函数
    static void *worker_thread(void *arg);
    //（主线程收到请求后），对应子线程的处理函数
    static void thread_process(int fd, short which, void *arg);
    //被libevent回调的各个静态函数
    static void on_listen(evconnlistener *listener, evutil_socket_t fd,
                          sockaddr *sa, int socklen, void *user_data);
    static void on_read(struct bufferevent *bev, void *data);
    static void on_write(struct bufferevent *bev, void *data);
    static void on_close(struct bufferevent *bev, short events, void *data);

protected:
    ///
    /// \brief 需要重写,新建连接成功后，调用该函数
    /// \param conn 客户端连接信息
    ///
    virtual void connect_cb(Conn *conn) { }

    ///
    /// \brief 读取完数据后，调用该函数
    /// \param conn 客户端连接信息
    ///
    virtual void read_cb(Conn *conn) { }

    ///
    /// \brief 发送完数据后，调用该函数
    /// \param conn 客户端连接信息
    ///
    virtual void write_cb(Conn *conn) { }

    ///
    /// \brief 断开连接，调用该函数
    /// \param conn 客户端连接信息
    /// \param events 关闭连接事件
    ///
    virtual void close_cb(Conn *conn, short events) { }

public:
    explicit CoreServer(int count);
    virtual ~CoreServer();

    //设置监听的端口号，不需要监听，设置为EXIT_CODE
    void set_port(int por);

    //开始事件循环
    void start_run();
    //tv:时间间隔,tv为空，则立即停止
    void stop_run(timeval *tv);

    //添加和删除信号处理事件
    //sig是信号，ptr为要回调的函数
    bool add_signal_event(int sig, void (*ptr)(int, short, void *));
    bool delete_signal_event(int sig);

    //添加和删除定时事件
    //ptr为要回调的函数
    //tv是间隔时间，
    //once决定是否只执行一次
    event *add_timer_event(void(*ptr)(int, short, void *),
                           timeval tv, bool once);
    bool delete_timer_event(event *ev);
};  




#endif //MULTISERVER_SERVER_H
