#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <functional>
#include <string>
#include <iostream>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;

class Chatserver
{
public:
    Chatserver(EventLoop* loop, //事件循环
                const InetAddress &listenAddr,   //IP+Port
                const string& nameArg)  //服务器名字
                :_server(loop, listenAddr, nameArg),_loop(loop)
            {
                _server.setConnectionCallback(bind(&Chatserver::onConnection, this, _1));
                /*集中于业务代码，这个部分的网络IO代码要让网络库自己工作*/
                // 给服务器注册用户连接的创建和断开回调
                //给服务器注册用户读写事件的回调
                _server.setMessageCallback(bind(&Chatserver::onMessage, this, _1, _2, _3));

                _server.setThreadNum(10);
            }
            void start()
            {
                _server.start();
            }
private:
    //专门处理用户的连接创建和断开， 相当于epoll里listenfd到客户然后accept；
    void onConnection(const TcpConnectionPtr &conn)
    {
        if(conn->connected())
        {
            cout<<conn->peerAddress().toIpPort()<<"->"<<conn->localAddress().toIpPort()<<"status:online"<<endl;
        }
        else{
            cout<<conn->peerAddress().toIpPort()<<"->"<<conn->localAddress().toIpPort()<<"status:offline"<<endl;
            conn->shutdown();
        }
    }
    //专门处理用户读写事件
    void onMessage(const TcpConnectionPtr &conn,  //连接
                            Buffer *buffer,    //缓冲区
                            Timestamp time)  //接收到数据的时间信息
    {
        string buf = buffer->retrieveAllAsString();
        cout<<"recv data:"<<buf<<"time:"<<time.toString()<<endl;
        conn->send(buf);
    }
    TcpServer _server;
    EventLoop* _loop;
};

int main()
{
    EventLoop loop;
    InetAddress addr("127.0.0.1", 6000);
    Chatserver server(&loop, addr, "Chatserver");
    server.start();
    loop.loop();
    return 0;
}