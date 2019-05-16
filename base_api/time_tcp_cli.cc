// 一个简单的时间获取客户端程序

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <libgen.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <cstdio>

constexpr int MAX_LINE = 1024;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "usage: " << basename(argv[0]) 
            << " ip port" << std::endl;
        return 1;
    }
    const char *remote_ip = argv[1];
    int remote_port = atoi(argv[2]);

    int clifd = socket(AF_INET, SOCK_STREAM, 0);
    if (clifd < 0) {
        std::cerr << "socket error: " << errno << std::endl;
        return 1;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0x00, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, remote_ip, &serv_addr.sin_addr) != 1) {
        std::cerr << "inet_pton error: " << errno << std::endl;
        close(clifd);
        return 1;
    }
    serv_addr.sin_port = htons(remote_port);
    if (connect(clifd, (struct sockaddr*)&serv_addr, 
                sizeof(serv_addr)) < 0) {
        std::cerr << "connect error: " << errno << std::endl;
        close(clifd);
        return 1;
    }

    int len = 0;
    char recv_buf[MAX_LINE+1];
    while ((len=read(clifd, recv_buf, MAX_LINE)) > 0) {
        recv_buf[len] = '\0';
        // 此处使用C函数来进行输出，使用文件描述符来操作
        if (fputs(recv_buf, stdout) == EOF) {
            std::cerr << "fputs error: " << errno << std::endl;
        }
    }
    if (len == 0) {
        std::cout << "connection had been closed by server" << std::endl;
    } else {
        std::cout << "read error: " << errno << std::endl;
    }

    close(clifd);

    return 0;
}
