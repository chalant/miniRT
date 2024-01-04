INCLUDES_DIR = ./includes
SRC_DIR = ./src
OBJ_DIR = ./obj
LIBFT_DIR = ./libft
MLX_DIR = ./minilibx

INCLUDES_FILES = minirt.h\
		         minirt_bindings.h\
		         matrix.h

SRC_FILES =	minirt.c \
			minirt_key_hooks.c \
			matrix.c \
			matrix_interface.c \
			minirt_init.c \
			minirt_window_hooks.c


INCLUDES = $(addprefix $(INCLUDES_DIR)/, $(INCLUDES_FILES))
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

NAME = minirt

LIBFT = $(LIBFT_DIR)/libft.a
MINILIBX = $(MLX_DIR)/libmlx_Linux.a

C_FLAGS = -O3 -I$(MLX_DIR) -I$(INCLUDES_DIR) -I$(LIBFT_DIR) -I$(SRC_DIR) -Wall -Wextra -Werror -MMD -MP

all:
	mkdir -p $(OBJ_DIR)
	make $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	cc $(C_FLAGS) $(OBJ) $(LIBFT) -lmlx -framework OpenGL -framework AppKit -o $(NAME)

$(MINILIBX):
	make -C $(MLX_DIR)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDES)
	cc $(C_FLAGS) -c $< -o $@

linux:
	cc $(C_FLAGS) $(OBJ) $(MINILIBX) $(LIBFT) -lXext -lX11 -lm -o $(NAME)

clean:
	make -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

bonus: all

re: fclean all

.PHONY: all clean fclean bonus re
