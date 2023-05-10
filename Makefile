# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: albgonza <albgonza@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/19 12:00:19 by albgonza          #+#    #+#              #
#    Updated: 2023/01/19 12:00:19 by albgonza         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS			= philo.c \
					utils.c \
					loop_split.c \

OBJS			= $(SRCS:.c=.o)
NAME			= philo
CC				= gcc
CFLAGS			= -Wall -Wextra -Werror -g
RM				= rm -f
COLOR_RED = \033[91m
BOLD = \033[1m
COLOR_NONE = \033[0m

all: $(LFLAGS) ascii $(NAME) info

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

ascii:
	@echo "$(COLOR_RED)"
	@echo "$(BOLD)"
	@echo "        _     _ _        "
	@echo "       | |   (_) |       "
	@echo " _  __ | |__  _| | ___   "
	@echo " | '_ \| '_ \| | |/ _ \  " 
	@echo " | |_) | | | | | | (_) | "
	@echo " | .__/|_| |_|_|_|\___/  "
	@echo " | |                     " 
	@echo " |_|                     "
	@echo "$(COLOR_NONE)"

info:
	@echo "$(BOLD)"
	@echo "method ./philo number_of_philos time_to_die time_to_eat time_to_sleep [times_each_philo_must_eat]"
clean:
	@echo "$(BOLD)$(COLOR_RED)REMOVING OBJECTS FROM PROJECT$(COLOR_NONE)"
	$(RM) $(OBJS)

fclean: clean
	@echo "$(BOLD)$(COLOR_RED)REMOVING EXEC AND DEATHLY SPAGUETTI FROM PROJECT$(COLOR_NONE)"
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re ascii info info