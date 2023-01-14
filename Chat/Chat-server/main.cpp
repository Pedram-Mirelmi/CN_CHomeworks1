#include <iostream>
#include "Chat-Server.hpp"
#include "MessageBodyDeserializer.hpp"
#include "NetIOManager.hpp"

using namespace std;

int main()
{
    shared_ptr<ChatServer> server = make_shared<ChatServer>();
    shared_ptr<NetIOManager> netIoManager = make_shared<NetIOManager>("127.0.0.1", 20000, 8);
    shared_ptr<MessageBodyDeserializer> deserializer = make_shared<MessageBodyDeserializer>();

    netIoManager->setDeserializer(std::move(deserializer));
    netIoManager->setMainManager(server);

    server->setNetIOManager(netIoManager);

    server->start();
    netIoManager->addThisThread();
    return 0;
}
