

#include "process.h"
#include "../Views/login.h"
#include "../Views/register.h"
#include "../Views/getlist.h"
#include "../Views/sendmessages.h"
#include "../Views/getmessage.h"
#include "../Views/addfriend.h"
#include "../Views/handlefriendrequest.h"
#include "../Views/chatdefine.h"

///
/// \brief 解析json得到不同的模块枚举,调用相关模块
/// \param json 客户端发送的json
/// \return 返回模块处理结果
///
std::string Process::handle(std::string json)
{
    if(json.empty())
        return "error";

    Json::Value json_root;
    json_root.clear();

    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();
    JSONCPP_STRING err;
    if (!reader->parse(json.data(), json.data() + json.size(), &json_root, &err))
        return "error";

    int type = json_root["reason_type"].asInt();
    auto it = chat_map.find(type);

    //调用聊天模块
    std::string res = it->second->process(json);
    res = it->second->response(res);
    return res;
}

Process::Process()
{
    chat_map.insert(std::make_pair(TYPE_LOGIN, std::make_shared<Login>()));
    chat_map.insert(std::make_pair(TYPE_REGISTER, std::make_shared<Register>()));
    chat_map.insert(std::make_pair(TYPE_GET_LIST, std::make_shared<GetList>()));
    chat_map.insert(std::make_pair(TYPE_SEND_MESSAGE, std::make_shared<SendMessage>()));
    chat_map.insert(std::make_pair(TYPE_GET_MESSAGE, std::make_shared<GetMessage>()));
    chat_map.insert(std::make_pair(TYPE_ADD_FRIEND, std::make_shared<AddFriend>()));
    chat_map.insert(std::make_pair(TYPE_HANDLE_ADD_FRIEND_REQUEST, std::make_shared<HnadleRequest>()));
}

