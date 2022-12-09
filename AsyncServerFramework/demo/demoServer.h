#pragma once
#include "./NetMessages.h"
#include "../AbstractAsyncServer.h"
#include "../INetMessageProcessor.h"
#include "./MessageBodyDeserializer.h"

class DemoEchoServer : public AbstractAsyncServer<DemoNetMessageType>,
                       public INetMessageProcessor<DemoNetMessageType>,
                       public std::enable_shared_from_this<DemoEchoServer>
{
public:
    DemoEchoServer(const std::string& ip, uint16_t port, int netIOThreadsCount)
        : AbstractAsyncServer<DemoNetMessageType>(ip, std::move(port), std::move(netIOThreadsCount)),
          INetMessageProcessor<DemoNetMessageType>()
    {
        m_netIOManager->setMessageProcessor(shared_ptr<INetMessageProcessor<DemoNetMessageType>>(this));
        m_netIOManager->setBodyDeserializer(
                    shared_ptr<INetMessageBodyDeserializer<DemoNetMessageType>>(
                        (INetMessageBodyDeserializer<DemoNetMessageType>*) new MessageBodyDeserializer()));
    }

    // IService interface
public:
    void start() override
    {
        for (auto& service : m_services)
            service->start();
    }
    void stop() override
    {
        for (auto& service : m_services)
            service->stop();
    }

    // INetMessageProcessor interface
public:
    virtual void processNetMessage(shared_ptr<NetMessage<DemoNetMessageType>> netMsg, shared_ptr<Session<DemoNetMessageType>> session) override
    {
        this->m_netIOManager->writeMessage(netMsg, session);
    }
};
