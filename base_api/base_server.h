#ifndef __SOCKET_BASE_SERVER_H__
#define __SOCKET_BASE_SERVER_H__

#include <sys/types.h>
#include <sys/socket.h>

int CreateServer(const char *ip, int port, int backlog);

#endif  // __SOCKET_BASE_SERVER_H__
