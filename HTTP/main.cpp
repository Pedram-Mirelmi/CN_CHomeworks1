#include <iostream>
#include <chrono>
#include "HTTPServer.h"
#include <cstring>
#include <sstream>
using namespace std;

int main()
{
    HTTPServer server = HTTPServer("127.0.0.1", 16000, 8);
    server.start();
    this_thread::sleep_for(chrono::minutes(5));
    return 0;
}
