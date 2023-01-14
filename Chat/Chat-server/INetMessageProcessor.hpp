#include <memory>
#include "./AsyncServerFramework/Session.h"
#include "./AsyncServerFramework/io/BasicNetMessage.h"
#include "./NetMessages/ChatServerMessagesTypes.hpp"

class IMainManager : std::enable_shared_from_this<IMainManager>
{
public:
    virtual void processNetMessage( shared_ptr<NetMessage<MessageTypes>> msg, shared_ptr<Session<MessageTypes>> session) = 0;
    virtual void onClientDisconnected(shared_ptr<Session<MessageTypes>> session) = 0;
};
