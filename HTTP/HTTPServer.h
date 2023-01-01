#pragma once
#include "./NetIOManager.hpp"
#include "./INetMessageProcessor.hpp"
class HTTPServer : public IService,
                   public INetMessageProcessor,
                   public std::enable_shared_from_this<HTTPServer>
{
    NetIOManager netIoManager;
public:
    HTTPServer(std::string&& ip, uint16_t&& port, uint8_t&& ioThreadsCount)
        : IService(),
          INetMessageProcessor(),
          std::enable_shared_from_this<HTTPServer>(),
          netIoManager(std::move(ip), std::move(port), std::move(ioThreadsCount))
    {
        netIoManager.setMessageProcessor(shared_ptr<INetMessageProcessor>(this));
    }

    // INetMessageProcessor interface
public:
    void processNetMessage(shared_ptr<HTTPMessage> netMsg, shared_ptr<Session> session) override
    {
//        if(netMsg->)
    }

    // IService interface
public:
    void start() override
    {
        netIoManager.start();
    }
    void stop() override
    {
        netIoManager.stop();
    }
};
