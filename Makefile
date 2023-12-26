INCLUDES_DIR = ./includes
SRC_DIR = ./src
OBJ_DIR = ./obj
LIBFT_DIR = ./libft

INCLUDES_FILES = minirt.h\
		   		 get_next_line.h\
		         minirt_bindings.h\
		         matrix.h\
				 fdf_rendering.h\

SRC_FILES = minirt.c \

INCLUDES = $(addprefix $(INCLUDES_DIR)/, $(INCLUDES_FILES))
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

NAME = minirt

LIBFT = $(LIBFT_DIR)/libft.a

C_FLAGS = -O3 -I$(INCLUDES_DIR) -I$(LIBFT_DIR) -I$(SRC_DIR) -Wall -Wextra -Werror

LFLAGS = -lmlx -framework OpenGL -framework AppKit

all:
	mkdir -p $(OBJ_DIR)
	make $(NAME)

$(NAME): $(OBJ) $(LIBFTPRINTF)
	cc $(C_FLAGS) $(OBJ) $(LIBFTPRINTF) $(LFLAGS) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c $(INCLUDES)
	cc $(C_FLAGS) -c $< -o $@

clean:
	make -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

bonus: all

re: fclean all

.PHONY: all clean fclean bonus re
