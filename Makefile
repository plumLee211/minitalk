# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jiyoulee <jiyoulee@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/16 00:28:49 by jiyoulee          #+#    #+#              #
#    Updated: 2022/03/16 00:28:50 by jiyoulee         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minitalk
SERVER_NAME = server
CLIENT_NAME = client
CC = gcc
CFLAGS = -Werror -Wall -Wextra
SERVER_SRCS = server.c
CLIENT_SRCS = client.c
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)
SERVER_OBJS = $(SERVER_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(SERVER_NAME) $(CLIENT_NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER_NAME): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -fsanitize=address -o $(SERVER_NAME) $(SERVER_OBJS)

$(CLIENT_NAME): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $(CLIENT_NAME) $(CLIENT_OBJS)

clean:
	${RM} ${SERVER_OBJS}
	${RM} ${CLIENT_OBJS}

fclean: clean
	${RM} ${SERVER_NAME}
	${RM} ${CLIENT_NAME}

re: fclean all

.PHONY: all clean fclean re