// 测试接受一个socket连接：所谓接受一个socket连接是指从
// listen创建的监听队列中获取一个已经建立的连接（即处于
// ESTABLISHED状态）

#include <iostream>
#include <libgen.h>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

#include "base_server.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "usage: " << basename(argv[0]) 
            << " port_number" << std::endl;
        return 1;
    }

    int port_num = atoi(argv[1]);
    int lis_sock = CreateServer(nullptr, port_num, 5);

    // 测试若一个出于ESTABLISHED状态的连接对应的客户端出现
    // 网络异常（如掉线或客户端退出）时，accept是否会成功
    // 1.如果是掉线了（在accept之前），则accept返回成功，不
    // 会感知到网络已掉线
    // 2.如果客户端提前退出（在accept之前）则显示状态为：
    // CLOSE_WAIT（客户端处于FIN_WAIT2状态：因为客户端连接已
    // 经断开，发送了FIN，但是服务端还没有发送对FIN的ACK报文，
    // 所以是正常的
    // 经过测试发现：accept只是从监听队列中取出连接，而不论该
    // 连接出于何种状态，如ESTABLISHED或CLOSE_WAIT状态，更不关
    // 心任何网络状况
    sleep(30);  // 在sleep过程中去操作异常情况
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int conn = accept(lis_sock, (struct sockaddr*)&client_addr, 
                      &client_addr_len);
    if (conn < 0) {
        std::cout << "errno: " << errno << std::endl;
    } else {
        char remote[16];
        const char *premote = inet_ntop(AF_INET, 
                                        &client_addr.sin_addr, 
                                        remote, 
                                        sizeof(remote));
        std::cout << "client_ip: " << premote 
            << " and client_port: " << ntohs(client_addr.sin_port) 
            << std::endl;
        close(conn);
    }

    close(lis_sock);

    return 0;
}
