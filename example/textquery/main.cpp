#include "QueryServer.h"
#include <iostream>
#include <string>
using namespace std;
using namespace Tiny;

int main(int argc, const char *argv[])
{
    QueryServer server(Tiny::InetAddress(8989), "The_Holy_Bible.txt");
    server.start();
    return 0;
}