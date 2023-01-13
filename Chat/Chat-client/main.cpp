#include <iostream>
#include <thread>
#include <chrono>
#include "NetworkHandler.hpp"

using namespace std;

void run(string& host, int port, string& username) {

    NetworkHandler net_handler(host, port);
    cout << "[INFO] Connecting to server ...\n";
    try {
        net_handler.start();

    }
    catch(...) {
        cout << "[ERROR] Server Not available. Try later\n";
        return;
    }


    // Wait for server to response
    // cout << "[INFO] Connected to " << host << " on port " << port << "\n";
    // for (int i=0; i<11; i++) {
    //     cout << "[INFO] Waiting for server to ACCEPT you ...\n";
    //     if (net_handler.does_server_accept())
    //         break;
    //     else if(i == 10){
    //         cerr << "[INFO] Server do not ACCEPT try again later\n";
    //         return;
    //     }
    //     std::this_thread::sleep_for(std::chrono::seconds(2));   
    // }

    cout << "------------------------------\n\n";
    cout << "Hello \"" << username << "\"\n";
    for(;;) {
        string line;
        cout << ">> ";
        getline(cin, line);
        stringstream ss(line);
        string command;
        ss >> command;
        if (command == "list") {

        }
        else if (command == "send") {

        }
        else if (command == "exit") {
            return;
        }
        else {
            cerr << "[ERROR] Wrong Command\n";
        }
    }
}


int main(int argc, char* argv[]) {

    if (argc != 3)
        cerr << "[ERROR] Wrong command expected argument: [Host:port] [User Name]\n";

    else{
        string host_port = argv[1];
        string host = host_port.substr(0, host_port.find(":"));
        string port_str = host_port.substr(host_port.find(":")+1, host_port.size());
        string username = argv[2];
        int port = stoi(port_str);
        run(host, port, username);        
    }

    return 0;
}