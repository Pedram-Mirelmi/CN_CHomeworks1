#include <iostream>
#include <cstring>
#include <chrono>
#include "./demoServer.h"
#include <asio.hpp>
using namespace std;

void startDemo()
{
    DemoEchoServer server("127.0.0.1", 60001, 10);
    server.start();
    cin.get();
}



int main()
{
    startDemo();
    return 0;
}

