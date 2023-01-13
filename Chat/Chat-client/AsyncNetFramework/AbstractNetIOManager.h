#pragma once

#include <asio.hpp>
#include <string>
#include <iostream>
#include "./IService.h"
#include "./io/BasicNetMessage.h"
#include "./INetWriter.h"


using std::shared_ptr;
using std::make_shared;


template<typename MsgType>
class AbstractNetIOManager : public IService,
                             public INetWriter<MsgType>,
                             public std::enable_shared_from_this<AbstractNetIOManager<MsgType>>
{
    using socket = asio::ip::tcp::socket;
    using end_point = asio::ip::tcp::resolver::results_type;
    using string = std::string;
protected:
    asio::io_context m_ioContext;
    socket m_socket;
    end_point m_endPoints;

    std::thread m_asioThread;

    bool m_isConnected = false;

    std::vector<char> m_headerInBuffer, m_messageInBuff;
    NetMessageHeader<MsgType> m_tempHeader;

public:
    AbstractNetIOManager(const string& ip, uint16_t port)
        : IService(),
          INetWriter<MsgType> (),
          std::enable_shared_from_this<AbstractNetIOManager>(),
          m_socket(m_ioContext)
    {
        m_headerInBuffer.resize(NetMessageHeader<MsgType>::getHeaderSize());
        asio::ip::tcp::resolver resolver(m_ioContext);
        m_endPoints = resolver.resolve(ip, std::to_string(port));
    }
    ~AbstractNetIOManager()
    {
        this->stop();
    }
protected:
    virtual void onAsyncConnected(std::error_code ec, asio::ip::tcp::endpoint endPoint)
    {
        if(!ec)
        {
            std::cout << "Connected to " << endPoint << std::endl;
            m_isConnected = true;
            asio::async_read(m_socket,
                             asio::buffer(m_headerInBuffer.data(), NetMessageHeader<MsgType>::getHeaderSize()),
                             std::bind(&AbstractNetIOManager::onAsyncReadHeader,
                                       this,
                                       std::placeholders::_1,
                                       std::placeholders::_2)
                             );



        }
    }

    virtual void onAsyncReadHeader(std::error_code ec, std::size_t length)
    {
        if(!ec)
        {
            deserializeHeader();
            m_messageInBuff.resize(m_tempHeader.getBodySize() + NetMessageHeader<MsgType>::getHeaderSize());
            asio::async_read(m_socket,
                             asio::buffer(m_messageInBuff.data() + NetMessageHeader<MsgType>::getHeaderSize(), m_tempHeader.getBodySize()),
                             std::bind(&AbstractNetIOManager::onAsyncReadBody,
                                       this,
                                       std::placeholders::_1,
                                       std::placeholders::_2)
                             );
        }
        else
        {
            if(length == 0)
                onDisconnected();
        }
    }

    virtual void onAsyncReadBody(std::error_code ec, std::size_t length)
    {
        if(!ec)
        {
            onNewMessageReadCompletely();
            asio::async_read(m_socket,
                             asio::buffer(m_headerInBuffer, NetMessageHeader<MsgType>::getHeaderSize()),
                             std::bind(&AbstractNetIOManager::onAsyncReadHeader,
                                       this,
                                       std::placeholders::_1,
                                       std::placeholders::_2)
                             );
        }
        else
        {
            if(length == 0)
                onDisconnected();
        }
    }

    virtual void onAsyncWrite(std::error_code ec, std::size_t length, const char* msgBuffer)
    {
        if(!ec)
        {
            delete [] msgBuffer;
        }
        else
        {
            if(length == 0)
                onDisconnected();
        }
    }

    void deserializeHeader()
    {
        m_tempHeader.deserialize(m_headerInBuffer.data());
    }

    virtual void onNewMessageReadCompletely() = 0;

    virtual void onDisconnected()
    {
        m_isConnected = false;
    };

public:
    virtual void writeMessage(shared_ptr<NetMessage<MsgType>> msg) override
    {
        uint32_t msgSize = msg->getHeader().getBodySize() + msg->getHeader().calculateNeededSizeForThis();
        char* msgBuffer = new char[msgSize];
        msg->serialize(msgBuffer);
        asio::async_write(m_socket,
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
    virtual void connectToServer()
    {
        asio::connect(m_socket,
                        m_endPoints);
    }

    virtual void start() override
    {
        connectToServer();

        m_asioThread = std::thread([=](){m_ioContext.run();});

    }
    virtual void stop() override
    {
        m_socket.close();
        m_ioContext.stop();
        if(m_asioThread.joinable())
            m_asioThread.join();
    }

};




