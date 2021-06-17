# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: erlajoua <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/09 12:10:40 by erlajoua          #+#    #+#              #
#    Updated: 2021/06/17 12:28:37 by nessayan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRCS =	srcs/main.cpp \
		srcs/Program.cpp \
		srcs/Route.cpp \
		srcs/Server.cpp

INCLUDES =	-I ./includes

OBJS = $(SRCS:.cpp=.o)

FLAGS = -Wall -Wextra -Werror -std=c++98

COMPILER = clang++

.cpp.o:
	$(COMPILER) ${FLAGS} ${INCLUDES} -c $< -o ${<:.cpp=.o}

$(NAME): $(OBJS)
	${COMPILER} $(FLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re


