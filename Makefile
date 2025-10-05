CC = cc

CFLAGS = -Wall -Werror -Wextra

NAME = philo

SRCS = src/activity.c src/init_and_parse.c src/main.c \
	   src/threads_utils.c src/threads.c \
	   src/utils_lib.c src/utils.c

OBJS = $(SRCS:.c=.o)

all: ${NAME}

${NAME}: ${OBJS}
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c include/philosophers.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: clean fclean all bonus