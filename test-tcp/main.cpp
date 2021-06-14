#include <iostream>
#include <cstdlib>
#include <string.h>
#include <sys/socket.h> //socket functions
#include <netinet/in.h> //sockaddr_in struct
#include <unistd.h> //read

void ft_error(std::string error)
{
	std::cout << "Failure at " << error << "\n";
	exit(EXIT_FAILURE);
}

//

int main(int ac, char **av)
{
	short port = (short)atoi(av[ac - 1]);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0); //domain, type, protocol
	if (sockfd == -1)
		ft_error("Socket");
	
	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET; //même domaine que la socket
	sockaddr.sin_addr.s_addr = INADDR_ANY; //symbolise n'importe quel interface (ethernet ou wifi)
	sockaddr.sin_port = htons(port); //le port sur lequel la socket va être utilisée

	if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
		ft_error("Bind");
	if (listen(sockfd, 3) < 0)
		ft_error("Listen");

	socklen_t addrlen = sizeof(sockaddr);
	int connection = accept(sockfd, (struct sockaddr *)&sockaddr,&addrlen);
	while (true)
	{
		char buffer[1024];
		bzero(buffer, 1024);
		if (connection < 0)
			ft_error("Accept");
		read(connection, buffer, 1024);
		std::cout << "The message was: " << buffer;
		
		if (!strncmp(buffer, "Salut", 5))
		{
			std::string response = "Good talking to you!!\n";
			send(connection, response.c_str(), response.size(), 0);
		}
	}	
	close(connection);
	close(sockfd);
	
	return 0;
}
