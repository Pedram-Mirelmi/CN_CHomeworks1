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

    std::vector<char> m_messageInBuff;
    NetMessageHeader<MsgType> m_tempHeader;

public:
    AbstractNetIOManager(const string& ip, uint16_t port)
        : IService(),
          std::enable_shared_from_this<AbstractNetIOManager>(),
          m_socket(m_ioContext)
    {
        m_messageInBuff.resize(NetMessageHeader<MsgType>::getHeaderSize());
        asio::ip::tcp::resolver resolver(m_ioContext);
        m_endPoints = resolver.resolve(ip, std::to_string(port));
    }
    ~AbstractNetIOManager()
    {
        this->stop();
    }
protected:

    void asyncRead() {
        asio::async_read(m_socket,
                             asio::buffer(m_messageInBuff.data(), NetMessageHeader<MsgType>::getHeaderSize()),
                             std::bind(&AbstractNetIOManager::onAsyncReadHeader,
                                       this,
                                       std::placeholders::_1,
                                       std::placeholders::_2)
                             );
    }

    virtual void onAsyncConnected(std::error_code ec, asio::ip::tcp::endpoint endPoint)
    {

        if(!ec)
        {
            onConnected();
            m_isConnected = true;
            asio::async_read(m_socket,
                             asio::buffer(m_messageInBuff.data(), NetMessageHeader<MsgType>::getHeaderSize()),
                             std::bind(&AbstractNetIOManager::onAsyncReadHeader,
                                       this,
                                       std::placeholders::_1,
                                       std::placeholders::_2)
                             );
        }
        else {
            std::cout << "[ERROR]  " << ec.message() << " trying again\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            connectToServer();
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
            m_messageInBuff.resize(NetMessageHeader<MsgType>::getHeaderSize());
            asio::async_read(m_socket,
                             asio::buffer(m_messageInBuff.data(), NetMessageHeader<MsgType>::getHeaderSize()),
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
        m_tempHeader.deserialize(m_messageInBuff.data());
    }

    virtual void onNewMessageReadCompletely() = 0;

    virtual void onConnected() = 0;

    virtual void onDisconnected()
    {
        m_isConnected = false;
    };

protected:

    virtual void writeMessage(shared_ptr<NetMessage<MsgType>> msg) override
    {
        uint32_t msgSize = msg->calculateNeededSizeForThis();
        char* msgBuffer = new char[msgSize];
        msg->serialize(msgBuffer);
        asio::write(m_socket, asio::buffer(msgBuffer, msgSize));
    }

    virtual void writeSyncMessage(shared_ptr<NetMessage<MsgType>> msg)
    {
        uint32_t msgSize = msg->calculateNeededSizeForThis();
        char* msgBuffer = new char[msgSize];
        msg->serialize(msgBuffer);
        asio::write(m_socket, asio::buffer(msgBuffer, msgSize));
    }

    virtual void readSyncMessage() {
        asio::read(m_socket,
                    asio::buffer(m_messageInBuff.data(),
                                NetMessageHeader<MsgType>::getHeaderSize()));
        deserializeHeader();
        asio::read(m_socket,
                    asio::buffer(m_messageInBuff.data() + NetMessageHeader<MsgType>::getHeaderSize(),
                                m_tempHeader.getBodySize()));
    }

    // IService interface
public:
    virtual void connectToServer()
    {
        asio::async_connect(m_socket,
                            m_endPoints,
                            std::bind(&AbstractNetIOManager::onAsyncConnected,
                                        this,
                                        std::placeholders::_1,
                                        std::placeholders::_2)
                            );
    }

    virtual void start() override
    {
        std::cout << "[INFO] Connecting to server ...\n";
        connectToServer();
        m_asioThread = std::thread([=, this](){m_ioContext.run();});
    }
    virtual void stop() override
    {
        m_socket.close();
        m_ioContext.stop();
        if(m_asioThread.joinable())
            m_asioThread.join();
    }

    bool is_connected() {
        return this->m_isConnected;
    }

};




