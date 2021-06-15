# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: erlajoua <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/09 12:10:40 by erlajoua          #+#    #+#              #
#    Updated: 2021/06/15 15:23:29 by user42           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRCS =	srcs/main.cpp \
		srcs/Program.cpp \
		srcs/Route.cpp \
		srcs/Server.cpp

OBJS = $(SRCS:.cpp=.o)

FLAGS = -Wall -Wextra -std=c++98 #-Werror -std=c++98

COMPILER = clang++

.cpp.o:
	$(COMPILER) ${FLAGS} -c $< -o ${<:.cpp=.o}

$(NAME): $(OBJS)
	${COMPILER} $(FLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re


