#pragma once

#include "./IService.h"
#include "./io/BasicHTTPMessage.h"
#include "./INetWriter.h"

using std::shared_ptr;
using std::make_shared;


class AbstractNetIOManager : public IService,
                             public INetWriter,
                             public std::enable_shared_from_this<AbstractNetIOManager>
{
    using socket = asio::ip::tcp::socket;
protected:
    std::vector<std::thread> m_ioThreads;
    asio::io_context m_ioContext;

    asio::ip::tcp::acceptor m_acceptor;

public:
    AbstractNetIOManager(const string& ip, const uint16_t& port, const uint8_t& ioThreadsCount)
        : IService(),
          INetWriter(),
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
            shared_ptr<Session> newConnection = make_shared<Session>(std::move(socket));
            onNewConnectionAccepted(newConnection);
            asio::async_read_until(newConnection->getSocket(),
                                   newConnection->getInStreamBuff(),
                                   "\r\n\r\n",
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

    virtual void onAsyncReadHeader(std::error_code ec, std::size_t length, shared_ptr<Session> session)
    {
        if(!ec)
        {
            std::cout << "read something" << std::endl;
            session->deserializeHeader(length);
            if(!session->getTempRequest()->hasBody())
            {
                onNewMessageReadCompletely(session);
            }
            else // for requests with body (not included in the project [just GET]
            {

//                asio::async_read_until(session->getSocket(),
              //                                       session->getInStreamBuff(),
              //                                       "\r\n\r\n", // ????????
              //                                       std::bind(&AbstractNetIOManager::onAsyncReadBody,
              //                                               this,
              //                                               std::placeholders::_1,
              //                                               std::placeholders::_2,
              //                                               session)
              //                                       );
            }

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

    virtual void onAsyncReadBody(std::error_code ec, std::size_t length, shared_ptr<Session> session)
    {
        if(!ec)
        {
            onNewMessageReadCompletely(session);
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

    virtual void onAsyncWrite(std::error_code ec, std::size_t length)
    {
        if(!ec)
        {
            std::cout << "all sent: " << length << std::endl;
        }
        else
        {
            std::cout << "error, sent: " << length << std::endl;
        }
    }

    virtual void onNewConnectionAccepted(shared_ptr<Session> newConnection) = 0;

    virtual void onConnectionClosedByClient(std::error_code ec, shared_ptr<Session> session) = 0;

    virtual void onNewMessageReadCompletely(shared_ptr<Session> session) = 0;

public:
    virtual void writeMessage(shared_ptr<HTTPMessage> msg, shared_ptr<Session> session) override
    {
        std::stringstream stream;
        msg->serialize(stream);
        try {
            asio::write(session->getSocket(),
                        asio::buffer(stream.view())
                        );
        }  catch (std::exception e) {
            std::cout << e.what() << std::endl;
        }
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




