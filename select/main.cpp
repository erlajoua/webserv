#include "Server.hpp"

void ftError(std::string error)
{
	std::cout << "Error at " << error << "\n";
	exit(EXIT_FAILURE);
}

int main(int ac, char **av)
{
	if (ac > 2)
	{
		ftError("Please enter a port as argument");
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
		int   nb_client_max = 3;
		
		Server serv2(port, host, nb_client_max);
		serv2.start();
	}

	return 0;
}
