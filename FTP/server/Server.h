#pragma once

#include "AsyncServerFramework/IService.h"
#include "./NetMessages.h"
#include "NetMessages/AllNetMessages.hpp"
#include "./INetMessageProcessor.h"
#include "./MessageBodyDeserializer.h"
#include "./NetIOManager.h"

class Server : public IService,
                       public INetMessageProcessor<NetMessageType>,
                       public std::enable_shared_from_this<Server>
{
    NetIOManager m_netIoManager;
public:
    Server(const std::string& ip, uint16_t port, int netIOThreadsCount)
        : INetMessageProcessor<NetMessageType>(),
          m_netIoManager(ip, port, netIOThreadsCount)
    {
        auto deserializer = shared_ptr<INetMessageBodyDeserializer<NetMessageType>>(static_cast<INetMessageBodyDeserializer<NetMessageType>*>(new MessageBodyDeserializer()));
        m_netIoManager.setNetMessageDeserializer(deserializer);

        auto processor = shared_ptr<INetMessageProcessor>(static_cast<INetMessageProcessor*>(this));
        m_netIoManager.setNetMessageProcessor(processor);
    }

    // IService interface
public:
    void start() override
    {
        m_netIoManager.start();
    }
    void stop() override
    {
        m_netIoManager.stop();
    }

    // INetMessageProcessor interface
public:
    virtual void processNetMessage(shared_ptr<NetMessage<NetMessageType>> netMsg, shared_ptr<Session<NetMessageType>> session) override
    {
        
    }
};
