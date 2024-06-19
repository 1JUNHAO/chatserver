#include "chatserver.hpp"
#include "chatservice.hpp"
#include "json.hpp"
#include <string>
#include <functional>
#include <iostream>
using namespace std;
using namespace placeholders;
using json = nlohmann::json;

ChatServer::ChatServer(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& nameArg)
        :_server(loop, listenAddr, nameArg), _loop(loop)
{
    //注册连接回调
    _server.setConnectionCallback(bind(&ChatServer::onConnection, this, _1));

    //注册信息回调
    _server.setMessageCallback(bind(&ChatServer::onMessage, this, _1, _2, _3));

    //设置线程数量
    _server.setThreadNum(10);
}

void ChatServer::start()
{
    _server.start();
}

//上报链接相关信息的回调函数
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    //链接失败要释放socketfd资源
    if(!conn->connected())
    {
        ChatService::instance()->clientCloseException(conn);
        //客户端断开连接
        conn->shutdown();
    }
}
//上报读写事件相关信息的回调函数
void ChatServer::onMessage(const TcpConnectionPtr& conn,
                        Buffer* buffer,
                        Timestamp time)
{
    string buf = buffer->retrieveAllAsString();
    //数据反序列化
    json js = json::parse(buf);
    // 通过js["msg_id"]绑定一个回调操作，一个id对应一个操作，不管网络做什么业务，只检索id，通过id获取一个Handler去进行业务处理。就可以完全将网络模块和业务模块区分开操作
    // 把接收到的conn, js, time都传给业务模块去进行操作
    //达到的目的：完全解耦网络模块代码和业务模块代码

    //以下两行代码就完全没有业务代码的实现方法，只需要在服务业务层service的内部把相应的消息和对应的事件回调做一个绑定
    //网络层即可通过消息id得到对应的事件处理器然后进行一个回调
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
    //回调消息绑定好的事件处理器，来执行相应的业务处理
    msgHandler(conn, js, time);
    //好处是不管在service里如何修改，这里的代码都不需要改动
}