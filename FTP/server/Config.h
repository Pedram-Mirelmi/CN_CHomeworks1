#include <iostream>
#include <vector>
using namespace std;


#ifndef CONFIG
#define CONFIG

struct User {
    string name;
    string password;
    bool is_admin;
    int remain_storage;
};


struct Config {
    int port;
    vector<User> users;
    vector<string> admin_files;
};

int get_user(const vector<User>& users, string user_name) {
    for (int i=0; i <  users.size(); i++) 
        if (users[i].name == user_name)
            return i;
    return -1;
}



#endif