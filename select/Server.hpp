#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <iostream>
# include <unistd.h>
# include <stdio.h>
# include <sys/socket.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <limits.h>
# include <cstring>
# include <fstream>

//
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void ft_error(std::string error);

# define DEFAULT_PORT 8042
# define DEFAULT_HOST "127.0.0.1"
# define DEFAULT_NBCLIENTMAX 3

class Server
{
private:
    //from parsing
    short               _port;
    const char *        _host;
    int                 _nbClientMax;
    //.../
    int                 _serverSocket;
    fd_set              _currentSockets;
    fd_set              _readySockets;
    sockaddr_in         _addr;

    int     accept_new_connection(int server_socket);
    void    handle_connection(int client_socket);
public:
    Server(void);
    Server(short port, const char *host, int nbClientMax);
    virtual ~Server(void);
    void start(void);
};

#endif
