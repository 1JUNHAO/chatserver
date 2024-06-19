#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <unordered_map>
#include <functional>
#include "json.hpp"
#include "UserModel.hpp"
#include <mutex>
#include <muduo/net/TcpConnection.h>
#include "offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"
#include "redis.hpp"
using namespace std;
using namespace muduo::net;
using namespace muduo;
using json = nlohmann::json;

//运用C++11的语法using替代typedef来给已经存在的类型定义一个新的类型名称
//表述处理消息的事件回调方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;
//聊天服务器业务类
//主要实现给msgid实现一个事件回调，就是映射关系，一个id映射一个回调,unordered_map
class ChatService
{
public:
    //获取单例对象的接口函数
    static ChatService* instance();
    //处理登陆业务
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //处理注册业务
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);

    //一对一聊天业务
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //添加好友业务
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 创建群组业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 加入群组业务
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 群组聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);

    //处理注销业务
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //获取消息对应的处理器
    MsgHandler getHandler(int msgid);
    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);

    //从redis消息队列中获取订阅的消息
    void handleRedisSubscribeMessage(int, string);

    //服务器异常，业务重置方法
    void reset();

private:
    ChatService();
    //存储消息Id和对应的业务处理方法
    unordered_map<int, MsgHandler> _msghandlerMap;

    //添加tcp存储连接对象,存储在线用户的通信连接,这个map一定要注意线程安全，因为它在业务中是被多线程调用的，会不断改变登录下线的信息
    unordered_map<int, TcpConnectionPtr> _userConnMap;

    //定义互斥锁，保证userConnnMap的线程安全
    mutex _connMutex;

    //数据操作类对象
    UserModel _userModel;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;

    //Redis操作对象
    Redis _redis;
};


#endif