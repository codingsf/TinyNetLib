#include "EchoServer.h"
#include <tiny/InetAddress.h>
using namespace Tiny;
using namespace std;
using namespace std::placeholders;

EchoServer::EchoServer(const Tiny::InetAddress &addr)
: server_(addr)
{
    server_.setConnectCallback(bind(&EchoServer::onConnection, this, _1));
    server_.setMessageCallback(bind(&EchoServer::onMessage, this, _1));
    server_.setCloseCallback(bind(&EchoServer::onClose, this, _1));
}

void EchoServer::onConnection(const TcpConnectionPtr &conn)
{
    printf("%s\n", conn->toString().c_str());
    conn->send("hello, welcome to Chat Server.\r\n");
}

void EchoServer::onMessage(const TcpConnectionPtr &conn)
{
    std::string s(conn->receive());
    conn->send(s);
}

void EchoServer::onClose(const TcpConnectionPtr &conn)
{
    printf("%s close\n", conn->toString().c_str());
}