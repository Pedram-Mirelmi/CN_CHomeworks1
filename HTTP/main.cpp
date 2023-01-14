#include <iostream>
#include <chrono>
#include "HTTPServer.h"
#include <cstring>
#include <sstream>
using namespace std;

int main()
{
    HTTPServer server = HTTPServer("127.0.0.1", 16000, 1);
    server.start();
    return 0;
}
