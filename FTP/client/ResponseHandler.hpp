#pragma once
#include <iostream>
#include <exception>
#include <fstream>
#include <queue>
#include <mutex>
#include "./IResponseResolver.h"
#include "./NetMessages/AllNetMessages.hpp"
class ResponseHandler : public IResponseResolver
{
    std::mutex m_queuesLock;
    std::queue<std::string> m_pendingDownloadFiles, m_pendingUploadFiles;
    std::shared_ptr<INetWriter<FTPMessageType>> m_netWriter;

public:
    ResponseHandler() = default;

    // IResponseResolver interface
public:
    void resolveResponse(shared_ptr<BasicNetMessage<FTPMessageType>> msg) override
    {
        switch (msg->getMessageType())
        {
            case FTPMessageType::USERNAME:
            {
                throw std::runtime_error("Server sent a Username message!!");
                break;
            }
            case FTPMessageType::PASSWORD:
            {
                throw std::runtime_error("Server sent a Password message!!");
                break;
            }

            case FTPMessageType::DOWNLOAD_FILE:
            {
                throw std::runtime_error("Server sent a Download file message!!");
                break;
            }
            case FTPMessageType::UPLOAD_FILE:
            {
                throw std::runtime_error("Server sent a Upload file message!!");
                break;
            }
            case FTPMessageType::GET_HELP:
            {
                throw std::runtime_error("Server sent a Get help message!!");
                break;
            }
            case FTPMessageType::QUIT:
            {
                std::cout << "server closed the connection" << std::endl;
                exit(0);
                break;
            }
            case FTPMessageType::SHORT_RESPONSE:
            {
                handleShortResponse(((ShortResponseMessage*)msg.get())->getResponseNumber());
                break;
            }
            case FTPMessageType::FILE_CONTENT:
            {
                handleFileContent((FileContentMessage*)msg.get());
                break;
            }
            case FTPMessageType::HELP_CONTENT:
            {
                auto helpContentMsg = (HelpContentMessage*) msg.get();
                std::cout << helpContentMsg->getHelpContent() << std::endl;
                break;
            }
        }
    }
public:
    void addPendingdDownloadFile(std::string&& filename) override
    {
        std::scoped_lock<std::mutex> scopedLock(m_queuesLock);
        m_pendingDownloadFiles.push(std::move(filename));
    }
    void addPendingUploadFile(std::string&& filename) override
    {
        std::scoped_lock<std::mutex> scopedLock(m_queuesLock);
        m_pendingUploadFiles.push(filename);
    }
    void setNetWriter(std::shared_ptr<INetWriter<FTPMessageType>> &&newNetWriter)
    {
        m_netWriter = std::move(newNetWriter);
    }

private:
    void handleShortResponse(uint16_t responseNumber)
    {
        using namespace std;
        switch (responseNumber) {
            case 331: // username ok, need password
                cout << responseNumber << ": Username Okay, need password." << endl;
                break;
            case 503:
                cout << responseNumber << ": Bad sequence of commands" << endl;
                break;
            case 230:
                cout << responseNumber << ": User logged in, proceed. Logged out if appropriate" << endl;
                break;
            case 430:
                cout << responseNumber << ": Invalid username of password" << endl;
                break;
            case 550:
                cout << responseNumber << ": file unavailable" << endl;
                break;
            case 226:
                cout << responseNumber << ": Successful Download" << endl;
                break;
            case 221:
                cout << responseNumber << ": Successful Quit." << endl;
                break;
            case 224: // additional short response. server states that file is available and will send it right away
                cout << responseNumber << ": File found!" << endl;
                break;
            case 225: // additional short response. server states that the client the permission to upload file
                cout << responseNumber << ": Server allowed uploading..." << endl;
                break;
            case 223:
                cout << responseNumber << ": Need account for login" << endl;
                break;
            case 551:
                cout << responseNumber << ": Syntax error in parameters or arguments" << endl;
                break;
            case 425:
                cout << responseNumber << ": Can't open data connectioin" << endl;
                break;
            case 403: // addiotional: when a user tries to upload but doesn't have permission to do so
                cout << responseNumber << ": Permissoin denied!" << endl;
                break;
            case 500:
                cout << responseNumber << ": Error" << endl;
                break;
            default:
                throw runtime_error(std::string("Unknown response number: ") + to_string(responseNumber));

        }
    }
    void handleFileContent(FileContentMessage* msg)
    {
        std::cout << "File(" << m_pendingDownloadFiles.front() << ") downloaded completely, what would you like to name it?(press enter to use the default name) ";
        std::string filename;
        getline(std::cin, filename);
        filename = filename == "" ? m_pendingDownloadFiles.front() : filename;
        std::ofstream file(std::string("./") + filename, std::ios::binary);
        file.write(msg->getFileContent().data(), msg->getFileContent().size());
        file.close();
        m_pendingDownloadFiles.pop();
    }

    void uploadFile()
    {
        auto filename = m_pendingUploadFiles.front();
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        std::vector<char> content(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(content.data(), content.size());
        file.close();

        m_netWriter->writeMessage(make_shared<FileContentMessage>(std::move(content)));
    }
};


