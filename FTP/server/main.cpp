#include <iostream>
#include <cstring>
#include <chrono>
#include "./Server.h"
#include <asio.hpp>
#include <Config.h>
#include <fstream>
#include <jsoncpp/json/json.h>

using namespace std;


void startServer(Config& config)
{
    Server server("127.0.0.1", config.port, 10, config);
    server.start();
    cin.get();
}

Config init_config(string file_name) {

    Config config;

    // read file
    ifstream file(file_name);

    Json::Reader reader;
    Json::Value js;
    reader.parse(file, js);


    config.port = js["commandChannelPort"].asInt();
    for (int i = 0; i < js["users"].size(); i++) {
        User user;
        user.name = js["users"][i]["user"].asString();
        user.password = js["users"][i]["password"].asString();
        user.remain_storage = stoi(js["users"][i]["size"].asString());
        if (js["users"][i]["admin"].asString() == "true")
            user.is_admin = true;
        else
            user.is_admin = false;

        config.users.push_back(user);
    }
    for (int i=0; i < js["files"].size(); i++) {
        config.admin_files.push_back(js["files"][i].asString());
    }

    return config;
}



int main()
{
    
    Config config = init_config("./Files/config.json");
    cout << "[Read]: config.json\n";
    cout << "[Started]: Server on port (" << config.port << ") ...\n";
    startServer(config);


    return 0;
}

