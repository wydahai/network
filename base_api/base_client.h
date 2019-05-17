#ifndef __SOCKET_BASE_CLIENT_H__
#define __SOCKET_BASE_CLIENT_H__

#include <sys/types.h>
#include <sys/socket.h>

int CreateClient(const char *ip, int port);

#endif  // __SOCKET_BASE_CLIENT_H__
