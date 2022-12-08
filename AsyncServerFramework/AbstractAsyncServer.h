#pragma once
#include <memory>
#include <vector>
#include "IService.h"
#include "./BasicNetIOManager.h"

using std::shared_ptr;

template <typename MsgType>
class AbstractAsyncServer : IService
{
protected:
    std::vector<shared_ptr<IService>> m_services;
    shared_ptr<BasicNetIOManager<MsgType>> m_netIOManager;
public:
    AbstractAsyncServer(const int& netIOThreadsCount);
    ~AbstractAsyncServer();

    // IService interface
public:
    void start() override;
    void stop() override;
};
