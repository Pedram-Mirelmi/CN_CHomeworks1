#pragma once

#include "./AsyncServerFramework/AbstractNetIOManager.h"
#include "./NetMessages/AllNetMessages.hpp"
#include "./AsyncServerFramework/io/INetMessageDeserializer.h"
#include "./ChatServer-typedefs.hpp"

class NetIOManager : public AbstractNetIOManager<MessageTypes>
{
    _MD_ptr m_deserializer;
    _MM_ptr m_processer;
    // AbstractNetIOManager interface
public:
    NetIOManager(const string& ip, const uint16_t port, const int& ioThreadsCount)
        : AbstractNetIOManager(ip, port, ioThreadsCount)
    {

    }
    void setDeserializer(const _MD_ptr &newDeserializer)
    {
        m_deserializer = newDeserializer;
    }

    void setMainManager(const _MM_ptr &newProcesser)
    {
        m_processer = newProcesser;
    }

    void addThisThread()
    {
        m_ioContext.run(); // add this thread to the threads that do io stuff
    }
protected:
    void onNewConnectionAccepted(shared_ptr<Session<MessageTypes>> newConnection) override
    {
        // TODO log the new connection
        std::cout << "new connection established: " << newConnection->getSocket().remote_endpoint() << std::endl;
    }
    void onConnectionClosedByClient(std::error_code ec, shared_ptr<Session<MessageTypes>> session) override
    {
        // TODO log the closure
        std::cout << "Connection closed: " << session->getSocket().remote_endpoint()
                  << "\terror: "<< ec.message() << std::endl;
        m_processer->onClientDisconnected(session);
    }
    void onNewMessageReadCompletely(shared_ptr<Session<MessageTypes>> session) override
    {
        auto msg = m_deserializer->deserializeMessage(session->getTempHeader(), session->getMessageInBuffer().data());
        m_processer->processNetMessage(msg, session);
    }

};
