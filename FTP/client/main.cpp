#include "NetworkHandler.hpp"
#include "ResponseHandler.hpp"
using namespace std;



void runClient()
{
    using namespace std;
    auto networkHandler = make_shared<NetworkHandler>("127.0.0.1", 8000);
    auto responseResolver = new ResponseHandler;
    responseResolver->setNetWriter(networkHandler);
    networkHandler->setResponseResolver(shared_ptr<IResponseResolver>(responseResolver));

    networkHandler->start();
    while (true)
    {
        string line;
        getline(cin, line);
        stringstream ss(line);
        string command;
        ss >> command;
        if(command == "user")
        {
            string username;
            ss >> username;
            networkHandler->sendUsername(username);
        }
        else if (command == "pass")
        {
            string password;
            ss >> password;
            networkHandler->sendPassword(password);
        }
        else if (command == "retr")
        {
            string filename;
            ss >> filename;
            networkHandler->sendRetr(filename);
        }
        else if(command == "Upload")
        {
            string filename;
            ss >> filename;
            networkHandler->sendUpload(filename);
        }
        else if(command == "help")
        {
            networkHandler->sendGetHelp();
        }
        else if(command == "quit")
        {
            networkHandler->sendQuit();
            networkHandler->closeConnection();
        }
        else if(command == "connect")
        {
            networkHandler->connectToServer();
        }
        else
        {
            cout << "Unsopported command!" << endl;
        }

    }
}

int main()
{
    runClient();
    return 0;
}
