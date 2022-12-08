#pragma once

#include "./IService.h"
#include "./INetMessageProcessor.h"
#include "./io/NetMessage.h"
#include "./INetWriter.h"
#include "./io/INetMessageBodyDeserializer.h"

using std::shared_ptr;
using std::make_shared;


template<typename MsgType>
class BasicNetIOManager : public IService, public INetWriter<MsgType>, public std::enable_shared_from_this<BasicNetIOManager<MsgType>>
{
    using socket = asio::ip::tcp::socket;
protected:
    std::vector<std::thread> m_ioThreads;
    asio::io_context m_ioContext;

    asio::ip::tcp::acceptor m_acceptor;

    shared_ptr<INetMessageProcessor<MsgType>> m_messageProcessor;
    shared_ptr<INetMessageBodyDeserializer<MsgType>> m_bodyDeserializer;
public:
    BasicNetIOManager(const string& ip, const uint16_t port, const int& ioThreadsCount)
        : IService(), INetWriter<MsgType>(), m_acceptor(m_ioContext, asio::ip::tcp::endpoint(asio::ip::make_address(ip), port))
    {
        m_ioThreads.reserve(ioThreadsCount);
    }
    ~BasicNetIOManager()
    {
        this->stop();
    }

    virtual void onAsyncAccepted(std::error_code ec, socket socket)
    {
        if(!ec)
        {
            char * a;
            shared_ptr<Session<MsgType>> newConnection = make_shared<Session<MsgType>>(std::move(socket));
            asio::async_read(newConnection->getSocket(),
                             asio::buffer(newConnection->getHeaderInBuffer(), sizeof (NetMessageHeader<MsgType>)),
                             std::bind(&BasicNetIOManager::onAsyncReadHeader,
                                       this,
                                       std::placeholders::_1,
                                       std::placeholders::_2,
                                       newConnection)
                             );

        }
        else
        {

        }
    }

    virtual void onAsyncReadHeader(std::error_code ec, std::size_t length, shared_ptr<Session<MsgType>> session)
    {
        if(!ec)
        {
            session->deserializeHeader();
            size_t bodySize = session->getTempHeader().calculateNeededSizeForThis();
            asio::async_read(session->getSocket(),
                             asio::buffer(session->getBodyInBuffer(), session->getTempHeader().getBodySize()),
                             std::bind(&BasicNetIOManager::onAsyncReadBody,
                                       this,
                                       std::placeholders::_1,
                                       std::placeholders::_2,
                                       session)
                             );
        }
        else
        {

        }
    }

    virtual void onAsyncReadBody(std::error_code ec, std::size_t length, shared_ptr<Session<MsgType>> session)
    {
        if(!ec)
        {
            auto netMessage = m_bodyDeserializer->deserializeBody(session->getTempHeader().getMessageType(), session->getBodyInBuffer());
            m_messageProcessor->processNetMessage(netMessage, session);
            asio::async_read(session->getSocket(),
                             asio::buffer(session->getHeaderInBuffer(), sizeof (NetMessageHeader<MsgType>)),
                             std::bind(&BasicNetIOManager::onAsyncReadHeader,
                                       this,
                                       std::placeholders::_1,
                                       std::placeholders::_2,
                                       session)
                             );
        }
    }

    virtual void onAsyncWrite(std::error_code ec, std::size_t length)
    {
        if(!ec)
        {
            // All good
        }
        else
        {

        }
    }

    virtual void writeMessage(shared_ptr<NetMessage<MsgType>> msg, shared_ptr<Session<MsgType>> session) override
    {
        size_t msgSize = msg->getHeader().getBodySize() + msg->getHeader().calculateNeededSizeForThis();
        char* msgBuffer = new char[msgSize];
        asio::async_write(session->getSocket(),
                          asio::buffer(msgBuffer, msgSize),
                          std::bind(&BasicNetIOManager::onAsyncWrite,
                          this,
                          std::placeholders::_1,
                          std::placeholders::_2)
                          );
    }

    // IService interface
public:
    virtual void start() override
    {
        m_acceptor.async_accept(std::bind(&BasicNetIOManager::onAsyncAccepted,
                                          this,
                                          std::placeholders::_1,
                                          std::placeholders::_2)
                                );
        for(size_t i = 0; i < m_ioThreads.capacity(); i++)
        {
            this->m_ioThreads.emplace_back([this](){
                this->m_ioContext.run();
            });
        }
    }
    virtual void stop() override
    {
        m_ioContext.stop();
        for (auto& thread : m_ioThreads)
            if(thread.joinable())
                thread.join();
    }
};
