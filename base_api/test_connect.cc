#include <iostream>
#include <libgen.h>
#include <cstdlib>
#include <unistd.h>

#include "base_client.h"

int main(int argc, char *argv[]) {
    if (argc <= 2) {
        std::cout << "usage: " << basename(argv[0]) 
            << " remote_ip remote_port" << std::endl;
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    int client = CreateClient(ip, port);
    if (client < 0) {
        return -1;
    }
    std::cout << "success to connect" << std::endl;

    close(client);

    return 0;
}
