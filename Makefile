# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elbenkri <elbenkri@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/06/30 00:51:30 by elbenkri          #+#    #+#              #
#    Updated: 2020/06/30 00:51:36 by elbenkri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	Matt_daemon

CC	=	clang++

CFLAGS	=	-Wall -Wextra -Werror

SRCS	=	daemon.cpp \
		main.cpp \
		Tintin_Reporter.cpp \
		persiste.cpp \

OBJ	=	$(SRCS:.cpp=.o)

all: 		$(NAME)

$(NAME): 	$(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "\033[H\033[2J\033[32mMatt_daemon [\033[32;5mCreated\033[0m\033[32m]"

clean:
	@/bin/rm -rf $(OBJ)
	@echo "\033[H\033[2J\033[32mall .o [\033[32;5mCleared\033[0m\033[32m]"

fclean:	clean
	@/bin/rm -rf $(NAME)
	@echo "\033[H\033[2J\033[32mBinary Matt_daemon [\033[32;5mCleared\033[0m\033[32m]"

re: fclean all
