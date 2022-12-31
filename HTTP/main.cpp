#include <iostream>
#include <vector>
#include <sstream>
#include <thread>
#include <chrono>
#include "HTTPServer.h"
using namespace std;

int main()
{
    HTTPServer server = HTTPServer("127.0.0.1", 16000, 8);
    server.start();

    cout << "Hello World!" << endl;
    this_thread::sleep_for(chrono::minutes(5));
    return 0;
}
