NAME = webserv

SRCS = main.cpp Program.cpp Server.cpp Route.cpp

PATH_SRCS = ./srcs/

_SRCS = ${addprefix ${PATH_SRCS}, ${SRCS}}

OBJS = ${_SRCS:.cpp=.o}

CC = clang++

FLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -rf

PATH_HEADER = ./includes/

.c.o:
		${CC} ${FLAGS} -I${PATH_HEADER} -c $< -o ${<:.cpp=.o}

all:			${NAME}

${NAME}:		${OBJS}
				${CC} ${FLAGS} ${OBJS} -o ${NAME}

clean:
				${RM} ${OBJS}

fclean:			clean
				${RM} ${NAME}

re:				fclean all

test:			re

.PHONY:			all clean fclean re
