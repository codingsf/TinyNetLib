#include "QueryServer.h"
using namespace std;
using namespace std::placeholders;
using namespace Tiny;


QueryServer::QueryServer(const InetAddress &addr,
                         const string &filename)
    :server_(addr),
     query_(filename),
     pool_(1000, 4)
{
    server_.setConnectCallback(bind(&QueryServer::onConnection, this, _1));
    server_.setMessageCallback(bind(&QueryServer::onMessage, this, _1));
}

void QueryServer::start()
{
    pool_.start(); //先启动线程池
    server_.start();
}

void QueryServer::onConnection(const TcpConnectionPtr &conn)
{
    conn->send("welcome, please input word:\r\n");
}

void QueryServer::onMessage(const TcpConnectionPtr &conn)
{
    string word(conn->receive());
    //runQuery(word, conn);
    //把任务交给线程池
    pool_.addTask(bind(&QueryServer::runQuery, this, word, conn));
}


void QueryServer::runQuery(const string &s, const TcpConnectionPtr &conn)
{
    string word = s;
    if(word.substr(word.size()-2, 2) == "\r\n")
    {
        word.erase(word.size()-1);
        word.erase(word.size()-1);
    }
    string res = query_.runQuery(word);
    conn->send(res + "\r\n");
}

