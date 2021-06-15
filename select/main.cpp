#include "Server.hpp"

void ft_error(std::string error)
{
	std::cout << "Error at " << error << "\n";
	exit(EXIT_FAILURE);
}

int main(int ac, char **av)
{
	if (ac > 2)
	{
		ft_error("Please enter a port as argument");
	}

	if (ac == 1)
	{
		Server serv1;
		serv1.start();
	}

	else if (ac == 2)
	{
		const char *host = DEFAULT_HOST;
		short port = (short)atoi(av[1]);
		int   nbClientMax = 3;
		
		Server serv2(port, host, nbClientMax);
		serv2.start();
	}

	return 0;
}
