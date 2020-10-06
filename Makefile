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

NAME		=	Matt_daemon

NAME_CLIENT	=	BEN_AFK

CC		=	clang++

CFLAGS		=	-Wall -Wextra -Werror

SRCS		=	server/daemon.cpp \
			server/main.cpp \
			server/Tintin_Reporter.cpp \
			client/encrypt.cpp \

SRCS_CLIENT	=	client/main.cpp \
			client/encrypt.cpp \

OBJ_CLIENT	=	$(SRCS_CLIENT:.cpp=.o)

OBJ		=	$(SRCS:.cpp=.o)

all: 		$(NAME)

$(NAME): 	$(OBJ) $(OBJ_CLIENT)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@$(CC) $(CFLAGS) $(OBJ_CLIENT) -o $(NAME_CLIENT)
	@echo "\033[H\033[2J\033[32mMatt_daemon and BEN_AFK [\033[32;5mCreated\033[0m\033[32m]"

clean:
	@/bin/rm -rf $(OBJ)
	@/bin/rm -rf $(OBJ_CLIENT)
	@echo "\033[H\033[2J\033[32mall .o [\033[32;5mCleared\033[0m\033[32m]"

fclean:	clean
	@/bin/rm -rf $(NAME)
	@/bin/rm -rf $(NAME_CLIENT)
	@echo "\033[H\033[2J\033[32mBinary Matt_daemon and BEN_AFK [\033[32;5mCleared\033[0m\033[32m]"

re: fclean all
