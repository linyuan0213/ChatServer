#ifndef CHATSERVER_CHATDEFINE_H
#define CHATSERVER_CHATDEFINE_H

///\enum 聊天软件各个事件枚举
enum {
    TYPE_LOGIN = 1,
    TYPE_REGISTER = 2,
    TYPE_GET_LIST = 3,
    TYPE_SEND_MESSAGE = 4,
    TYPE_GET_MESSAGE = 5,
    TYPE_ADD_FRIEND = 6,
    TYPE_HANDLE_ADD_FRIEND_REQUEST = 7
};
#endif //CHATSERVER_CHATDEFINE_H
