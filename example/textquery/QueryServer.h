#ifndef ECHO_QUERY_SERVER
#define ECHO_QUERY_SERVER 

#include <tiny/TcpServer.h>
#include <tiny/ThreadPool.h>
#include "TextQuery.h"

class QueryServer : NonCopyable
{
    public:
        QueryServer(const Tiny::InetAddress &addr, const std::string &filename);
        void start();
    private:
        void onConnection(const Tiny::TcpConnectionPtr &conn);
        void onMessage(const Tiny::TcpConnectionPtr &conn);

        void runQuery(const std::string &word, const Tiny::TcpConnectionPtr &conn);

        Tiny::TcpServer server_;
        TextQuery query_;  //查询的类
        Tiny::ThreadPool pool_;
};

#endif  /*ECHO_QUERY_SERVER*/
