#include <iostream>
#include <thread>
#include <unistd.h>
#include <chrono>
#include "NetworkHandler.hpp"


using namespace std;

void run(string& host, int port, string& username) {


    NetworkHandler net_handler(host, port, username);

    net_handler.start();
    net_handler.wait_to_connect();
    net_handler.wait_to_acknowledge();
    net_handler.receive_all_message();

    
    for(;;) {            
        string line;
        cout << ">> ";
        getline(cin, line);
        stringstream ss(line);
        string command;
        ss >> command;

        if (command == "list") {
            net_handler.update_user_list();
            // net_handler.print_user_list();
            
        }
        else if (command == "send") {
            string reciever;
            string message;
            ss >> reciever;
            ss >>  message;
            net_handler.send_message(reciever, message);

        }
        else if (command == "exit") {
            cout << "Exiting ...\n";
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