#pragma once
#include <filesystem>
#include <fstream>
#include <fmt/format.h>
#include "./NetIOManager.hpp"
#include "./INetMessageProcessor.hpp"
#include "./AsyncServerFramework/io/HTTP-Response.hpp"
class HTTPServer : public IService,
                   public INetMessageProcessor,
                   public std::enable_shared_from_this<HTTPServer>
{
    NetIOManager m_netIoManager;
    std::unordered_map<std::string, std::string> m_contentTypeValues;
public:
    HTTPServer(std::string&& ip, uint16_t&& port, uint8_t&& ioThreadsCount)
        : IService(),
          INetMessageProcessor(),
          std::enable_shared_from_this<HTTPServer>(),
          m_netIoManager(std::move(ip), std::move(port), std::move(ioThreadsCount))
    {
        m_netIoManager.setMessageProcessor(shared_ptr<INetMessageProcessor>(this));

        m_contentTypeValues[".pdf"] = "application/pdf";
        m_contentTypeValues[".html"] = "text/html";
        m_contentTypeValues[".gif"] = "image/gif";
        m_contentTypeValues[".jpeg"] = "image/jpeg";
        m_contentTypeValues[".jpg"] = "image/jpeg";
        m_contentTypeValues[".png"] = "image/png";
        m_contentTypeValues[".mp3"] = "audio/mpeg";
    }

    // INetMessageProcessor interface
public:
    void processNetMessage(shared_ptr<HTTPRequest> netMsg, shared_ptr<Session> session) override
    {
        if(!netMsg->hasBody())
        {
            switch (netMsg->getRequestType())
            {
                case HTTPRequest::GET:
                    handleGET(netMsg, session);
                case HTTPRequest::POST:
                    break;
                case HTTPRequest::PUT:
                    break;
                case HTTPRequest::PATCH:
                    break;
                case HTTPRequest::DELETE:
                    break;
            }
        }
        else
        {
            // not included
        }
    }

    // IService interface
public:
    void start() override
    {
        m_netIoManager.start();
    }
    void stop() override
    {
        m_netIoManager.stop();
    }
private:
    void handleGET(shared_ptr<HTTPRequest> netMsg, shared_ptr<Session> session)
    {
        std::filesystem::path filepath = toFilePath(netMsg->getUri());
        if(!std::filesystem::exists(filepath) || filepath.string().find(".") == std::string::npos)
           filepath = "./404.html";

        auto filecontent = readFileContent(filepath);
        std::unordered_map<std::string, std::string> headers;
        headers["Content-Type"] = m_contentTypeValues[filepath.extension().string()];
        headers["Content-Length"] = std::to_string(filecontent.size());

        m_netIoManager.writeMessage(make_shared<HTTPResponse>(200, std::move(headers), std::move(filecontent)), std::move(session));
    }

    std::string toFilePath(const std::string& uri)
    {
        std::string filepath;
        return uri == "/" ? "./index.html" : std::string(".") + uri;
    }

    std::vector<char> readFileContent(const std::string& filepath)
    {
        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        if(!file)
            return std::vector<char>();
        std::vector<char> content(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(content.data(), content.size());
        file.close();
        return content;
    }
};









