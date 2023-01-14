#pragma once
#include <unordered_map>
#include <queue>
#include <mutex>
#include "./NetIOManager.hpp"

class ChatServer : public IMainManager, public IService
{
    _NetIOManager_ptr m_netIOManager;

    std::unordered_map<_UserId_T, _Sesstion_ptr> m_onlineUsers;
    std::unordered_map<_UserId_T, std::string> m_usersNames;
    std::unordered_map<std::string, _UserId_T> m_usersIds;

    std::unordered_map<_UserId_T, std::queue<_BNetMsg_ptr>> m_usersPendingMessages;

    std::mutex m_idsLock, m_dataLock, m_onlineUsersLock;
    _UserId_T getNextId()
    {
        std::scoped_lock<std::mutex> scopedLock(m_idsLock);
        static _UserId_T latestId = 0;
        return ++latestId;
    }

public:
    ChatServer()
    {}

    // IMainManager interface
public:
    void onClientDisconnected(shared_ptr<Session<MessageTypes> > session) override
    {
        std::scoped_lock<std::mutex> scopedLock(m_onlineUsersLock);
        m_onlineUsers.erase(session->getId());
    }

    void processNetMessage(_BNetMsg_ptr msg, _Sesstion_ptr session) override
    {
        switch (msg->getMessageType()) {
        case MessageTypes::CONNECT:
            handleConnectMessage(msg, session);
            break;
        case MessageTypes::CONNACK:
            std::cerr << "Client sent a Connect ack message\n";
            break;
        case MessageTypes::LIST:
            handleListMessage(session);
            break;
        case MessageTypes::LISTREPLY:
            std::cerr << "Client sent a List Reply message\n";
            break;
        case MessageTypes::INFO:
            handleInfoMessage(msg, session);
            break;
        case MessageTypes::INFOREPLY:
            std::cerr << "Client sent a Info Reply message\n";
            break;
        case MessageTypes::SEND:
            handleSendMessage(msg, session);
            break;
        case MessageTypes::SENDREPLY:
            std::cerr << "Client sent a Send Reply message\n";
            break;
        case MessageTypes::RECEIVE:
            handleReceiveMessage(session);
            break;
        case MessageTypes::RECEIVEREPLY:
            std::cerr << "Client sent a Receive Reply message\n";
            break;
        }
    }
    inline void handleConnectMessage(_BNetMsg_ptr msg, _Sesstion_ptr session)
    {
        ConnectMessage* req = (ConnectMessage*) msg.get();
        {
            std::scoped_lock<std::mutex> lock(m_dataLock);
            if(!m_usersIds.contains(req ->getName()))
            {
                session->setId(getNextId());
                m_onlineUsers[session->getId()] = session;
                m_usersNames[session->getId()] = req ->getName();
                m_usersIds[req ->getName()] = session->getId();
            }
            else
            {
                session->setId(m_usersIds[req->getName()]);
            }
        }
        {
            std::scoped_lock<std::mutex> lock(m_onlineUsersLock);
            m_onlineUsers[session->getId()] = session;
        }
        shared_ptr<ConnAckMessage> response = make_shared<ConnAckMessage>();
        m_netIOManager->writeMessage(std::move(response), std::move(session));
    }

    inline void handleListMessage( _Sesstion_ptr session)
    {
        std::vector<_UserId_T> users;
        users.reserve(m_usersIds.size());
        for (auto& pair : m_usersIds)
            users.push_back(pair.second);

        shared_ptr<ListReplyMessage> response = make_shared<ListReplyMessage>(std::move(users));
        m_netIOManager->writeMessage(std::move(response), std::move(session));
    }

    inline void handleInfoMessage(_BNetMsg_ptr msg, _Sesstion_ptr session)
    {
        UserInfoMessage* req = (UserInfoMessage*)msg.get();
        std::string wantedUsername;
        {
            std::scoped_lock<std::mutex> lock(m_dataLock);
            wantedUsername = m_usersNames[req->getWantedUserId()];
        }
        shared_ptr<UserInfoReplyMessage> response = make_shared<UserInfoReplyMessage>(std::move(wantedUsername));
        m_netIOManager->writeMessage(std::move(response), std::move(session));
    }

    inline void handleSendMessage(_BNetMsg_ptr msg, _Sesstion_ptr session)
    {
        SendMessage* req = (SendMessage*) msg.get();

        auto anounceNewMsgMessage = make_shared<ReceiveReplyMessage>(session->getId(), req->getMessage());
        shared_ptr<SendReplyMessage> response;

        std::scoped_lock<std::mutex> scopedLock(m_onlineUsersLock);
        if(m_onlineUsers.contains(req->getUserId()))
        {
            response = make_shared<SendReplyMessage>(true);
            m_netIOManager->writeMessage(std::move(anounceNewMsgMessage), m_onlineUsers[req->getUserId()]);
        }
        else
        {
            m_usersPendingMessages[req->getUserId()].push(anounceNewMsgMessage);
            response = make_shared<SendReplyMessage>(false);
        }
        m_netIOManager->writeMessage(std::move(response), std::move(session));
    }

    inline void handleReceiveMessage(_Sesstion_ptr session)
    {
        std::scoped_lock<std::mutex> scopedLock(m_dataLock);
        auto messages = m_usersPendingMessages[session->getId()];
        m_usersPendingMessages.erase(session->getId());
        while (!messages.empty())
        {
            m_netIOManager->writeMessage(messages.front(), session);
            messages.pop();
        }
        m_netIOManager->writeMessage(make_shared<ReceiveReplyMessage>(0, ""), session);
    }



    void setNetIOManager(const _NetIOManager_ptr &newNetIOManager)
    {
        m_netIOManager = newNetIOManager;
    }

    // IService interface
public:
    void start() override
    {
        m_netIOManager->start();
    }
    void stop() override
    {
        m_netIOManager->start();
    }
};


