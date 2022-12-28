#pragma once

#include "../IService.h"
#include "./NetMessages.h"
#include "./INetMessageProcessor.h"
#include "./MessageBodyDeserializer.h"
#include "./NetIOManager.h"

class DemoEchoServer : public IService,
                       public INetMessageProcessor<DemoNetMessageType>,
                       public std::enable_shared_from_this<DemoEchoServer>
{
    NetIOManager m_netIoManager;
public:
    DemoEchoServer(const std::string& ip, uint16_t port, int netIOThreadsCount)
        : INetMessageProcessor<DemoNetMessageType>(),
          m_netIoManager(ip, port, netIOThreadsCount)
    {
        auto deserializer = shared_ptr<INetMessageBodyDeserializer<DemoNetMessageType>>(static_cast<INetMessageBodyDeserializer<DemoNetMessageType>*>(new MessageBodyDeserializer()));
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
    virtual void processNetMessage(shared_ptr<NetMessage<DemoNetMessageType>> netMsg, shared_ptr<Session<DemoNetMessageType>> session) override
    {
        this->m_netIoManager.writeMessage(netMsg, session); // echos the message back
    }
};
