#ifndef ECHO_SERVER_H
#define ECHO_SERVER_H

#include <boost/noncopyable.hpp>
#include <tiny/TcpServer.h>

class EchoServer : boost::noncopyable
{
public:
    explicit EchoServer(const Tiny::InetAddress &addr);
    void start()
    { server_.start(); }
private:
    void onConnection(const Tiny::TcpConnectionPtr &conn);
    void onMessage(const Tiny::TcpConnectionPtr &conn);
    void onClose(const Tiny::TcpConnectionPtr &conn);

    Tiny::TcpServer server_;
};


#endif //ECHO_SERVER_H