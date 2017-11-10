# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/05/14 20:31:10 by agadiffe          #+#    #+#              #
#    Updated: 2017/11/02 18:00:55 by agadiffe         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ----------------------------------------------------------------------------
# VARIABLES ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
# ----------------------------------------------------------------------------
ifeq ($(HOSTTYPE),)
	HOSTTYPE :=$(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so

$(shell ln -s $(NAME) libft_malloc.so 2> /dev/null)

CC = clang
CFLAGS += -Wall -Werror -Wextra -fPIC

SRC_PATH = ./srcs/
SRC_NAME = malloc.c						\

OBJ_PATH = ./obj/
OBJ_NAME = $(SRC_NAME:.c=.o)
OBJ = $(addprefix $(OBJ_PATH),$(OBJ_NAME))


INC_PATH = ./includes/
INC = $(addprefix -I,$(INC_PATH))
INC_FILE = $(INC_PATH)malloc.h


LIBFT_PATH = ./libft/
LIBFT_NAME = libft.a
LIBFT_INC_PATH = ./libft/includes/
LIBFT = $(addprefix -L,$(LIBFT_PATH))
LIBFT_INC = $(addprefix -I,$(LIBFT_INC_PATH))

# ----------------------------------------------------------------------------
# MISC |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
# ----------------------------------------------------------------------------
$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(INC_FILE)
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) $(LIBFT_INC) $(INC) -o $@ -c $<

# ----------------------------------------------------------------------------
# RULES ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
# ----------------------------------------------------------------------------
all: $(NAME)

test:
	$(CC) $(CFLAGS) main.c -L. -$(NAME:.so=) $(LIBFT) -lft

$(NAME): $(LIBFT_PATH)$(LIBFT_NAME) $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) -shared $(OBJ) $(LIBFT) -lft

$(LIBFT_PATH)$(LIBFT_NAME):
	@$(MAKE) -C $(LIBFT_PATH)

clean:
	@$(MAKE) -C $(LIBFT_PATH) clean
	@rm -rf $(OBJ)
	@rm -rf $(OBJ_PATH)

fclean: clean
	@$(MAKE) -C $(LIBFT_PATH) fclean
	@rm -rf $(NAME)
	@rm -rf libft_malloc.so

re: fclean all

.PHONY: all clean fclean re
