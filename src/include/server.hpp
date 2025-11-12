#pragma once

#include <string>
#include <httplib.h>

class Server {
    public:
    Server(int port, const std::string &rOrigin);
    ~Server() = default;

    void start();

    private:
    std::string m_origin{};
    int m_port{};
};
