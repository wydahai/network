#include <cassert>
#include <cstring>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>

#include "base_client.h"


int CreateClient(const char *ip, int port) {
    assert(ip && port>=0 && port<=65535);

    int client = socket(AF_INET, SOCK_STREAM, 0);
    assert(client > 0);

    struct sockaddr_in addr;
    memset(&addr, 0x00, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &addr.sin_addr);
    addr.sin_port = htons(port);
    // connect向远程服务器发起连接请求，常见的errno是：
    // ETIMEDOUT：由于网络繁忙等原因连接失败，连接超时
    // 会尝试连接6次，超时时间分别1、2、4、8、16、32，
    // 共63秒
    // ECONNREFUSED：目标端口不存在，连接被拒绝
    int ret = connect(client, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1) {
        if (errno == ETIMEDOUT) {
            std::cout << "timeout" << std::endl;
        } else if (errno == ECONNREFUSED) {
            std::cout << "refused: remote port doesn't exist" << std::endl;
        }
        close(client);
        return -1;
    }
    return client;
}
