#ifndef SERVERS_HPP
# define SERVERS_HPP

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

void ft_error(std::string error);

# define DEFAULT_PORT 8080
# define DEFAULT_HOST "127.0.0.1"
# define DEFAULT_NBCLIENTMAX 3

class Servers
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
        void    *handle_connection(int client_socket);
    public:
        Servers(void);
        Servers(short port, const char *host, int nbClientMax);
        virtual ~Servers(void);
        void start(void);
};

#endif