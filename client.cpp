#include "public.h"

uint32_t finished_count = 0;
uint32_t failed_count = 0;

class Client : public std::enable_shared_from_this<Client> {
public:
    Client(asio::io_service& io_service, uint32_t dstip, bool reuseport,
        bool linger)
        : io_service_(io_service)
        , socket_(io_service)
        , dstip_(dstip)
        , reuseport_(reuseport)
        , linger_(linger) {
    }
    ~Client() {
        if (linger_ && socket_.is_open()) {
            asio::socket_base::linger option(true, 0);
            socket_.set_option(option);
            socket_.close();
        }
    }
    void start() {
        auto self(shared_from_this());
        auto ep = asio::ip::tcp::endpoint(asio::ip::address_v4(dstip_), kPort);
        socket_.open(ep.protocol());
        if (reuseport_) {
            socket_.set_option(asio::socket_base::reuse_address(true));
            int one = 1;
            int r = setsockopt(socket_.native_handle(), SOL_SOCKET,
                SO_REUSEPORT, (char*)&one, sizeof(one));
            if (r < 0) {
                error(1, errno, "SO_REUSEPORT");
            }
        }

        socket_.async_connect(ep, [self, this](auto const& error) {
            if (error) {
                //std::cout << "connect failed\n";
                ++failed_count;
            } else {
                //std::cout << "connected\n";
                asio::async_write(socket_, asio::buffer(data_),
                    [this, self](auto const& error, size_t /*cb*/) {
                    if (error) {
                        //std::cout << "send failed\n";
                        ++failed_count;
                    } else {
                        //std::cout << "send\n";
                        asio::async_read(socket_, asio::buffer(data_),
                            [this, self](auto const& error, size_t) {
                            if (error) {
                                //std::cout << "recv failed\n";
                                ++failed_count;
                            }
                            //std::cout << "done\n";
                            ++finished_count;
                        });
                    }
                });
            }
        });
    }
private:
    asio::io_service& io_service_;
    asio::ip::tcp::socket socket_;
    const uint32_t dstip_;    
    const bool reuseport_;
    const bool linger_;
    char data_[100];
};

int client_loop(uint32_t dstip, uint32_t loop_count,
    uint32_t connections_count, bool reuse_port, bool linger) {
    for (uint32_t loop = 0; loop < loop_count; ++loop) {
        auto begin = time(nullptr);
        finished_count = 0;
        failed_count = 0;
        asio::io_service io_service;
        for (uint32_t i = 0; i < connections_count; ++i) {
            auto client = std::make_shared<Client>(io_service, dstip,
                reuse_port, linger);
            client->start();
        }
        io_service.run();
        auto end = time(nullptr);
        auto period = end - begin;
        std::cout << "loop: " << loop << ", finished_count: " <<
            finished_count << ", failed_count: " << failed_count <<
            ", time(s): " << period << "\n";
    }
    return 0;
}