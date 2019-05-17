#include <cassert>
#include <cstring>
#include <netinet/in.h>  // for INADDR_ANY
#include <arpa/inet.h>  // for inet_pton

#include "base_server.h"

int CreateServer(const char *ip, int port, int backlog) {
    // 创建一个socket套接字（起始就是ip-port地址对），第一个
    // 参数domain是地址家族；第二个参数是服务类型：
    // SOCK_STREAM（流服务）和SOCK_DGRAM（数据报服务），第三
    // 个参数一般默认，因为第一个和第二个参数就唯一确定了第三
    // 个参数：协议类型
    // Linux2.6.17之后，第二个参数type可以是和下面两个重要参
    // 数与的值：SOCK_NONBLOCK（非阻塞套接字）和SOCK_CLOEXEC
    // （调用fork创建子进程时在子进程中关闭此socket），之前只
    // 能通过调用类似fcntl函数来实现
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock > 0);

    // ipv4 address
    struct sockaddr_in addr;
    memset(&addr, 0x00, sizeof(addr));
    addr.sin_family = AF_INET;
    if (!ip) {
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        // NOTE：成功时返回1，失败返回0
        inet_pton(AF_INET, ip, &addr.sin_addr);
    }
    addr.sin_port = htons(port);

    // 设置SO_REUSEADDR选项：地址重用，这样就可以重复使用已处于
    // TIME_WAIT状态的连接
    int flag = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    // 命名（绑定）socket，常见的错误：
    // EACCES：被绑定的端口是受保护的地址，仅超级用户能够访问，
    // 如绑定到0 ~ 1023端口
    // EADDRINUSE：被绑定的端口正在使用中，如将socket绑定到一个
    // 出于TIME_WAIT状态的socket地址
    int ret = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    assert(ret == 0);

    // 根据实际测试：未被accept的最大连接数为：backlog + 1
    // 显示为：ESTABLISHED状态，对于其他的连接显示为SYN_RECV
    // 状态，且客户端的状态仍然显示为ESTABLISHED
    // listen的作用是内核建立一个监听队列，backlog指定监听队列
    // 的大小（实际测试为backlog+1），在Linux2.2之前，此大小为
    // SYN_RECV状态+ESTABLISHED状态之和，Linux2.2之后只是
    // ESTABLISHED状态（完全连接状态）的队列大小
    // 出于半连接状态（SYN_RECV）的大小由内核参数定义：
    // /proc/sys/net/ipv4/tcp_max_syn_backlog，默认值为128（64bit）
    ret = listen(sock, backlog);
    assert(ret == 0);

    return sock;
}
