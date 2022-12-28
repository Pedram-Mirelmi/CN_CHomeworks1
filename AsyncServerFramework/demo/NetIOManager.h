#pragma once
#include "../AbstractNetIOManager.h"
#include "./NetMessageTypes.h"
#include "./INetMessageProcessor.h"

class NetIOManager : public AbstractNetIOManager<DemoNetMessageType>
{
    shared_ptr<INetMessageBodyDeserializer<DemoNetMessageType>> m_netMessageDeserializer;
    shared_ptr<INetMessageProcessor<DemoNetMessageType>> m_netMessageProcessor;
public:
    NetIOManager(const string& ip, const uint16_t port, const int& ioThreadsCount)
        : AbstractNetIOManager<DemoNetMessageType>(ip, port, ioThreadsCount)
    {

    }

    // AbstractNetIOManager interface
    void setNetMessageDeserializer(const shared_ptr<INetMessageBodyDeserializer<DemoNetMessageType> > &newNetMessageDeserializer)
    {
        m_netMessageDeserializer = newNetMessageDeserializer;
    }

    void setNetMessageProcessor(const shared_ptr<INetMessageProcessor<DemoNetMessageType> > &newNetMessageProcessor)
    {
        m_netMessageProcessor = newNetMessageProcessor;
    }
protected:
    void onConnectionClosedByClient(std::error_code ec, shared_ptr<Session<DemoNetMessageType>> session)  override
    {
        // TODO log the closure
    }
    void onNewMessageReadCompletely(shared_ptr<Session<DemoNetMessageType>> session) override
    {
        // TODO log the read
        m_netMessageProcessor->processNetMessage(m_netMessageDeserializer->deserializeBody(session->getTempHeader(),
                                                                                           session->getBodyInBuffer()),
                                                 session);
    }

    void onNewConnectionAccepted(shared_ptr<Session<DemoNetMessageType> > newConnection) override
    {
        // TODO log the new connection
    }
};


