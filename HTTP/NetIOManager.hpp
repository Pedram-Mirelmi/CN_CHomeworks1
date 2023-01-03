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

protected:
    void onNewConnectionAccepted(shared_ptr<Session> newConnection) override
    {
        std::cout << "Accepted a new Connection: \n"
                  << "\tremote_endpoint:" << newConnection->getSocket().remote_endpoint()
                  << "\tlocal_endpoint:" << newConnection->getSocket().local_endpoint() << std::endl;
    }
    void onConnectionClosedByClient(std::error_code ec, shared_ptr<Session> session) override
    {
        std::cout << "Connection closed \n"
                  << "\tremote_endpoint:" << session->getSocket().remote_endpoint()
                  << "\tlocal_endpoint:" << session->getSocket().local_endpoint() << std::endl;
    }
    void onNewMessageReadCompletely(shared_ptr<Session> session) override
    {
        m_messageProcessor->processNetMessage(session->getTempRequest(), session);
    }
public:
    void setMessageProcessor(const shared_ptr<INetMessageProcessor> &newMessageProcessor)
    {
        m_messageProcessor = newMessageProcessor;
    }

};




