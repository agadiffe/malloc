# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/05/14 20:31:10 by agadiffe          #+#    #+#              #
#    Updated: 2018/04/13 16:16:23 by agadiffe         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ----------------------------------------------------------------------------
# VARIABLES ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
# ----------------------------------------------------------------------------
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so

CC = clang
CFLAGS += -Wall -Werror -Wextra -fPIC -fvisibility=hidden


SRC_PATH = ./srcs/
SRC_NAME = malloc.c	\
		   calloc.c	\
		   free.c	\
		   realloc.c	\
		   reallocf.c	\
		   chunk_create.c	\
		   chunk_find.c	\
		   lib_print_nbr.c	\
		   lib_print_str.c	\
		   ft_memlock.c	\
		   print.c

OBJ_PATH = ./obj/
OBJ_NAME = $(SRC_NAME:.c=.o)
OBJ = $(addprefix $(OBJ_PATH),$(OBJ_NAME))


INC_PATH = ./includes/
INC = $(addprefix -I,$(INC_PATH))
INC_FILE = $(INC_PATH)malloc.h

# ----------------------------------------------------------------------------
# MISC |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
# ----------------------------------------------------------------------------
$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(INC_FILE)
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

# ----------------------------------------------------------------------------
# RULES ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
# ----------------------------------------------------------------------------
all: $(NAME)

test:
	$(CC) $(CFLAGS) test.c $(INC) -L. -lft_malloc

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -shared -lpthread
	@$(shell ln -s $@ libft_malloc.so)

clean:
	@rm -rf $(OBJ)
	@rm -rf $(OBJ_PATH)

fclean: clean
	@rm -rf $(NAME)
	@rm -rf libft_malloc.so

re: fclean all

.PHONY: all clean fclean re
