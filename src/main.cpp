#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <utility>

#include "server.hpp"

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

    Server svr{port, argv[4]};

    svr.start();

    return 0;
}
