#include "server.hpp"

Server::Server(int port, const std::string &rOrigin) : m_port(port), m_origin(rOrigin) {}

void Server::start() {
    httplib::Server svr;

    svr.Get("/testing", [&](const httplib::Request &, httplib::Response &res) {
        res.set_redirect(m_origin + "/products");
    });

    svr.set_error_handler([&](const httplib::Request &, httplib::Response &res) {
        res.set_redirect(m_origin);
    });

    auto th = std::thread([&]() {svr.listen("localhost", m_port); });

    auto se = httplib::detail::scope_exit([&] {
        svr.stop();
        th.join();
    });

}
