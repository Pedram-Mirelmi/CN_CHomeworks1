#pragma once

#include "./IService.h"
#include "./io/BasicNetMessage.h"
#include "./INetWriter.h"
#include "./io/INetMessageBodyDeserializer.h"

using std::shared_ptr;
using std::make_shared;


template<typename MsgType>
class AbstractNetIOManager : public IService, public INetWriter<MsgType>, public std::enable_shared_from_this<AbstractNetIOManager<MsgType>>
{
    using socket = asio::ip::tcp::socket;
protected:
    std::vector<std::thread> m_ioThreads;
    asio::io_context m_ioContext;

    asio::ip::tcp::acceptor m_acceptor;

public:
    AbstractNetIOManager(const string& ip, const uint16_t port, const int& ioThreadsCount)
        : IService(),
          INetWriter<MsgType>(),
          m_acceptor(m_ioContext, asio::ip::tcp::endpoint(asio::ip::make_address(ip), port))
    {
        m_ioThreads.reserve(ioThreadsCount);
    }
    ~AbstractNetIOManager()
    {
        this->stop();
    }

protected:

    virtual void onAsyncAccepted(std::error_code ec, socket socket)
    {
        m_acceptor.async_accept(std::bind(&AbstractNetIOManager::onAsyncAccepted,
                                          this,
                                          std::placeholders::_1,
                                          std::placeholders::_2)
                                );
        if(!ec)
        {
            shared_ptr<Session<MsgType>> newConnection = make_shared<Session<MsgType>>(std::move(socket));
            onNewConnectionAccepted(newConnection);
            asio::async_read(newConnection->getSocket(),
                             asio::buffer(newConnection->getHeaderInBuffer(), NetMessageHeader<MsgType>::getHeaderSize()),
                             std::bind(&AbstractNetIOManager::onAsyncReadHeader,
                                       this,
                                       std::placeholders::_1,
                                       std::placeholders::_2,
                                       newConnection)
                             );



        }
        else
        {
            // TODO log that a new connection could not be established
        }
    }

    virtual void onAsyncReadHeader(std::error_code ec, std::size_t length, shared_ptr<Session<MsgType>> session)
    {
        if(!ec)
        {
            session->deserializeHeader();
            asio::async_read(session->getSocket(),
                             asio::buffer(session->getBodyInBuffer(), session->getTempHeader().getBodySize()),
                             std::bind(&AbstractNetIOManager::onAsyncReadBody,
                                       this,
                                       std::placeholders::_1,
                                       std::placeholders::_2,
                                       session)
                             );
        }
        else
        {
            if(length == 0)
            {
                onConnectionClosedByClient(ec, session);
            }
            else // ?
            {

            }
        }
    }

    virtual void onAsyncReadBody(std::error_code ec, std::size_t length, shared_ptr<Session<MsgType>> session)
    {
        if(!ec)
        {
            onNewMessageReadCompletely(session);
            asio::async_read(session->getSocket(),
                             asio::buffer(session->getHeaderInBuffer(), NetMessageHeader<MsgType>::getHeaderSize()),
                             std::bind(&AbstractNetIOManager::onAsyncReadHeader,
                                       this,
                                       std::placeholders::_1,
                                       std::placeholders::_2,
                                       session)
                             );
        }
        else
        {
            if(length == 0)
            {
                session->getSocket().close();
                // TODO log that there was an error
            }
            else // ?
            {

            }
        }
    }

    virtual void onAsyncWrite(std::error_code ec, std::size_t length, char* outBuffer)
    {
        if(!ec)
        {
            // All good
            delete [] outBuffer;
        }
        else
        {

        }
    }

    virtual void onNewConnectionAccepted(shared_ptr<Session<MsgType>> newConnection) = 0;

    virtual void onConnectionClosedByClient(std::error_code ec, shared_ptr<Session<MsgType>> session) = 0;

    virtual void onNewMessageReadCompletely(shared_ptr<Session<MsgType>> session) = 0;

public:
    virtual void writeMessage(shared_ptr<NetMessage<MsgType>> msg, shared_ptr<Session<MsgType>> session) override
    {
        uint32_t msgSize = msg->getHeader().getBodySize() + msg->getHeader().calculateNeededSizeForThis();
        char* msgBuffer = new char[msgSize];
        msg->serialize(msgBuffer);
        asio::async_write(session->getSocket(),
                          asio::buffer(msgBuffer, msgSize),
                          std::bind(&AbstractNetIOManager::onAsyncWrite,
                          this,
                          std::placeholders::_1,
                          std::placeholders::_2,
                          msgBuffer)
                          );
    }

    // IService interface
public:
    virtual void start() override
    {
        m_acceptor.async_accept(std::bind(&AbstractNetIOManager::onAsyncAccepted,
                                          this,
                                          std::placeholders::_1,
                                          std::placeholders::_2)
                                );
        for(uint32_t i = 0; i < m_ioThreads.capacity(); i++)
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




