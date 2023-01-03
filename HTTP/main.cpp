#include <iostream>
#include <chrono>
#include "HTTPServer.h"
using namespace std;

int main()
{
//    HTTPServer server = HTTPServer("127.0.0.1", 16000, 8);
//    server.start();

    char* buffer = new char[12];
    memcpy(buffer, "hello HELLO", 12);

    stringstream ss;

    uint8_t x = 0;

    ss.write(buffer, 5);
    ss.write((char*)&x, 1);
    ss.write(buffer+6, 5);

    auto result = ss.str();

    auto hello = result.data();
    auto HELLO = result.data() + 6;

    std::cout << hello << "\n" << HELLO << endl;
    std::cout << ss.tellp() << endl;
    this_thread::sleep_for(chrono::minutes(5));
    return 0;
}
