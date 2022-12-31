#pragma once
#include "INetMessageProcessor.hpp"
#include "AsyncServerFramework/AbstractNetIOManager.h"

class NetIOManager : public AbstractNetIOManager
{
    shared_ptr<INetMessageProcessor> m_messageProcessor;
public:
    NetIOManager(std::string&& ip, uint16_t&& port, uint8_t&& ioThreadsCount)
        : AbstractNetIOManager(std::move(ip), std::move(port), std::move(ioThreadsCount))
    {

    }
    // INetWriter interface
public:
    void writeMessage(shared_ptr<HTTPMessage> msg, std::shared_ptr<Session> session) override
    {

    }


protected:
    void onNewConnectionAccepted(shared_ptr<Session> newConnection) override
    {
        // TODO
    }
    void onConnectionClosedByClient(std::error_code ec, shared_ptr<Session> session) override
    {
        // TODO
    }
    void onNewMessageReadCompletely(shared_ptr<Session> session) override
    {
        m_messageProcessor->processNetMessage(session->getTempMessage(), session);
    }
public:
    void setMessageProcessor(const shared_ptr<INetMessageProcessor> &newMessageProcessor)
    {
        m_messageProcessor = newMessageProcessor;
    }

};




