#include "public.h"

int server_loop();

int client_loop(uint32_t dstip, uint32_t loop_count,
    uint32_t connections_count, bool reuse_port, bool linger);

int usage() {
    std::cout << "server\n";
    std::cout << "client dstip loop_count connections_count reuseport[0|1] linger[0|1]\n";
    return 1;
}
int main(int argc, char** argv)
{
    if (argc == 2 && !strcmp(argv[1], "server")) {
        return server_loop();
    } else if (argc == 7 && !strcmp(argv[1], "client")) {
        uint32_t dstip = ntohl(inet_addr(argv[2]));
        uint32_t loop_count = atoi(argv[3]);
        uint32_t connections = atoi(argv[4]);
        bool reuse_port = atoi(argv[5]) ? true : false;
        bool linger = atoi(argv[6]) ? true : false;
        return client_loop(dstip, loop_count, connections, reuse_port, linger);
    } else {
        return usage();
    }
}

