#include <cstdio>
#include <iostream>
#include <string>

#include <httplib.h>

std::string dump_headers(const httplib::Headers &headers);
std::string dump_multipart_formdata(const httplib::MultipartFormData &form);
std::string logger(const httplib::Request &req, const httplib::Response &res);

int main(int argc, char *argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: ./caching-proxy --port <port_number> --origin <url>";
        return 1;
    }

    int port;

    try {
        port = std::stoi(argv[2]);
    } catch (std::invalid_argument const &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (std::out_of_range const &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    httplib::Server srv;
    srv.set_logger([](const httplib::Request &req, const httplib::Response &res) { std::cout << logger(req, res); });

    while(true) {

    srv.Get("/products", [&](const httplib::Request &, httplib::Response &res) {
        res.set_redirect("http://dummyjson.com/products");
    });

    srv.set_error_handler([](const httplib::Request &, httplib::Response &res) {
        //const char *fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
        //char buf[BUFSIZ];
        //snprintf(buf, sizeof(buf), fmt, res.status);
        //res.set_content(buf, "text/html");
        res.set_redirect("http://dummyjson.com/");
    });

    auto th = std::thread([&]() { srv.listen("localhost", port); });

    auto se = httplib::detail::scope_exit([&] {
        srv.stop();
        th.join();
    });

    srv.wait_until_ready();
    }

    return 0;
}

std::string dump_headers(const httplib::Headers &headers) {
    std::string str;
    char buf[BUFSIZ];

    for (auto iter = headers.begin(); iter != headers.end(); iter++) {
        const auto &x = *iter;
        snprintf(buf, sizeof(buf), "%s: %s\n", x.first.c_str(), x.second.c_str());
        str += buf;
    }

    return str;
}

std::string dump_multipart_formdata(const httplib::MultipartFormData &form) {
    std::string str;
    char buf[BUFSIZ];

    str += "================================\n";

    for (const auto &x : form.fields) {
        const auto &name = x.first;
        const auto &field = x.second;

        snprintf(buf, sizeof(buf), "name: %s\n", name.c_str());
        str += buf;

        snprintf(buf, sizeof(buf), "text length: %zu\n", field.content.size());
        str += buf;

        str += "================================\n";
    }

    for (const auto &x : form.files) {
        const auto &name = x.first;
        const auto &file = x.second;

        snprintf(buf, sizeof(buf), "name: %s\n", name.c_str());
        str += buf;

        snprintf(buf, sizeof(buf), "filename: %s\n", file.filename.c_str());
        str += buf;

        snprintf(buf, sizeof(buf), "content type: %s\n", file.content_type.c_str());
        str += buf;

        snprintf(buf, sizeof(buf), "text length: %zu\n", file.content.size());
        str += buf;

        str += "================================\n";
    }

    return str;
}

std::string logger(const httplib::Request &req, const httplib::Response &res) {
    std::string str;
    char buf[BUFSIZ];

    str += "================================\n";

    snprintf(buf, sizeof(buf), "%s %s %s", req.method.c_str(),
             req.version.c_str(), req.path.c_str());
    str += buf;

    std::string query;
    for (auto it = req.params.begin(); it != req.params.end(); ++it) {
      const auto &x = *it;
      snprintf(buf, sizeof(buf), "%c%s=%s",
               (it == req.params.begin()) ? '?' : '&', x.first.c_str(),
               x.second.c_str());
      query += buf;
    }
    snprintf(buf, sizeof(buf), "%s\n", query.c_str());
    str += buf;

    str += dump_headers(req.headers);
    str += dump_multipart_formdata(req.form);

    str += "================================\n";

    snprintf(buf, sizeof(buf), "%d\n", res.status);
    str += buf;
    str += dump_headers(res.headers);

    return str;
}
