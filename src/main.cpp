#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <iostream>
#include <string>
#include <unordered_map>

std::string strip_url(const std::string &url);

struct CacheEntry {
    std::string body;
    httplib::Headers headers;
    std::string content_type;
};

int main(int argc, char *argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: ./caching-proxy --port <port_number> --origin <url>\n";
        return 1;
    }

    int port{};

    try {
        port = std::stoi(argv[2]);
    } catch (std::invalid_argument const &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (std::out_of_range const &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    std::string origin_url = argv[4];
    std::unordered_map<std::string, CacheEntry> cache;

    httplib::Server srv;
    httplib::SSLClient cli(strip_url(argv[4]));
    cli.enable_server_certificate_verification(false);

    srv.Get(".*", [&](const httplib::Request &req, httplib::Response &res) {
        std::string path = req.path.empty() ? "/" : req.path;

        auto iter = cache.find(path);
        if (iter != cache.end()) {
            const auto &entry = iter->second;
            res.set_content(entry.body, entry.content_type);
            res.set_header("X-Cache", "HIT");
            std::cout << path << " X-Cache: HIT\n";
            return;
        }

        auto origin_res = cli.Get(path.c_str());
        if (!origin_res) {
            res.status = 502;
            res.set_content("Bad Gateway (origin request failed)", "text/plain");
            return;
        }

        cache[path] = {
            origin_res->body,
            origin_res->headers,
            origin_res->get_header_value("Content-Type")
        };

        res.set_content(origin_res->body, origin_res->get_header_value("Content-Type"));
        res.set_header("X-Cache", "MISS");

        for (const auto &h : origin_res->headers) {
            std::string key = h.first;
            if (key != "Content-Length" && key != "Transfer-Encoding" && key != "Content-Encoding" && key != "Content-Type") {
                res.set_header(h.first.c_str(), h.second.c_str());
            }
        }

        std::cout << path << " X-Cache: MISS\n";
    });

    srv.listen("0.0.0.0", port);
}

std::string strip_url(const std::string &url) {
    std::string host = url;

    if (host.rfind("http://", 0) == 0) {
        host = host.substr(7);
    } else if (host.rfind("https://", 0) == 0) {
        host = host.substr(8);
    }

    return host;
}
