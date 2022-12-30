#pragma once
#include <iostream>
#include <exception>
#include "./IResponseResolver.h"
#include "./NetMessages/AllNetMessages.hpp"
class ResponseHandler : IResponseResolver
{
    ResponseHandler() = default;

    // IResponseResolver interface
public:
    void resolveResponse(shared_ptr<BasicNetMessage<FTPMessageType> > msg) override
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
                msg.reset(static_cast<BasicNetMessage<FTPMessageType>*>(new FileContentMessage()));
                break;
            }
            case FTPMessageType::HELP_CONTENT:
            {
                msg.reset(static_cast<BasicNetMessage<FTPMessageType>*>(new HelpContentMessage()));
                break;
            }
        }
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
        case 223:
            cout << responseNumber << ": Need account for login" << endl;
            break;
        case 551:
            cout << responseNumber << ": Syntax error in parameters or arguments" << endl;
            break;
        case 425:
            cout << responseNumber << ": Can't open data connectioin" << endl;
            break;
        case 500:
            cout << responseNumber << ": Error" << endl;
        default:
            throw runtime_error(std::string("Unknown response number: ") + to_string(responseNumber));

        }
    }
};
