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

//surement inutile
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void ftError(std::string error);

# define DEFAULT_PORT 8042
# define DEFAULT_HOST "127.0.0.1"
# define DEFAULT_NB_CLIENT_MAX 3

class Server
{
private:
    //from parsing
    short               port;
    const char *        host;
    int                 nb_client_max;
    //.../
    int                 server_socket;
    fd_set              current_sockets;
    fd_set              ready_sockets;
    sockaddr_in         addr;

    int     acceptNewConnection(int server_socket);
    void    handleConnection(int client_socket);
public:
    Server(void);
    Server(short port, const char *host, int nb_client_max);

    virtual ~Server(void);
    
    void start(void);
};

#endif
