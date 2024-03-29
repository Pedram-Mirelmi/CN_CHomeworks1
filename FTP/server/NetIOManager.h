#pragma once
#include "AsyncServerFramework/AbstractNetIOManager.h"
#include "./NetMessageTypes.h"
#include "./INetMessageProcessor.h"

class NetIOManager : public AbstractNetIOManager<NetMessageType>
{
    shared_ptr<INetMessageBodyDeserializer<NetMessageType>> m_netMessageDeserializer;
    shared_ptr<INetMessageProcessor<NetMessageType>> m_netMessageProcessor;
public:
    NetIOManager(const string& ip, const uint16_t port, const int& ioThreadsCount)
        : AbstractNetIOManager<NetMessageType>(ip, port, ioThreadsCount)
    {

    }
     // AbstractNetIOManager interface
    void setNetMessageDeserializer(const shared_ptr<INetMessageBodyDeserializer<NetMessageType> > &newNetMessageDeserializer)
    {
        m_netMessageDeserializer = newNetMessageDeserializer;
    }

    void setNetMessageProcessor(const shared_ptr<INetMessageProcessor<NetMessageType> > &newNetMessageProcessor)
    {
        m_netMessageProcessor = newNetMessageProcessor;
    }
protected:
    void onConnectionClosedByClient(std::error_code ec, shared_ptr<Session<NetMessageType>> session)  override
    {
        // TODO log the closure
        std::cout << "[INFO] Connection closed on port \"" << session->get_client_port() << "\"\n" ;
    }
    void onNewMessageReadCompletely(shared_ptr<Session<NetMessageType>> session) override
    {
        // TODO log the read

        // m_netMessageDeserializer->deserializeBody(session->getTempHeader(), session->getBodyInBuffer());
        // msg->
        m_netMessageProcessor->processNetMessage(m_netMessageDeserializer->deserializeBody(session->getTempHeader(),
                                                                                           session->getBodyInBuffer()),
                                                session->getTempHeader().getMessageType(), session);
    }

    void onNewConnectionAccepted(shared_ptr<Session<NetMessageType> > newConnection) override
    {
        // TODO log the new connection
        std::cout << "[INFO] New Connection on port \"" << newConnection->get_client_port() << "\"\n" ;
    }
};


