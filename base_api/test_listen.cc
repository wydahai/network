#include <iostream>
#include <signal.h>
#include <libgen.h>
#include <cstdlib>
#include <unistd.h>

#include "base_server.h"

static bool g_stop = false;

static 
void HandleTerm(int signo) {
    std::cout << "receive signo: " << signo << std::endl;
    g_stop = true;
}

int main(int argc, char *argv[]) {
    signal(SIGTERM, HandleTerm);
    if (argc <= 3) {
        std::cout << "usage: " << basename(argv[0]) 
            << " ip port backlog" << std::endl;
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int backlog = atoi(argv[3]);

    int lis_sock = CreateServer(ip, port, backlog);
    while (!g_stop) {
        sleep(1);
    }

    close(lis_sock);

    return 0;
}
