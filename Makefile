INCLUDES_DIR = ./includes
SRC_DIR = ./src
OBJ_DIR = ./obj
LIBFT_DIR = ./libft
MLX_DIR_LINUX = ./minilibx
MLX_DIR_MAC = ./minilibx_mac

INCLUDES_FILES = minirt.h \
		         minirt_bindings.h \
		         matrix.h \
				 minirt_objects.h \
				 minirt_colors.h

SRC_FILES =	minirt.c \
			map.c \
			map_read.c \
			map_process.c \
			map_atof.c \
			minirt_key_hooks.c \
			matrix.c \
			matrix_operations.c \
			matrix_interface.c \
			matrix_inversion.c \
			vector_operations.c \
			rotations.c \
			minirt_transforms.c \
			transforms.c \
			minirt_init.c \
			minirt_setup.c \
			minirt_window_hooks.c \
			minirt_objects.c \
			minirt_rendering.c \
			minirt_camera.c \
			minirt_projection.c \
			minirt_colors.c \
			procedural_textures.c


INCLUDES = $(addprefix $(INCLUDES_DIR)/, $(INCLUDES_FILES))
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

NAME = minirt
LINUX = lin

LIBFT = $(LIBFT_DIR)/libft.a
MINILIBX_LINUX = $(MLX_DIR_LINUX)/libmlx_Linux.a
MLX_MAC = $(MLX_DIR_MAC)/libmlx.a

# C_FLAGS = -O3 -I$(MLX_DIR_LINUX) -I$(INCLUDES_DIR) -I$(LIBFT_DIR) -I$(SRC_DIR) -Wall -Wextra -Werror -MMD -MP
C_FLAGS = -g -O3 -I$(MLX_DIR_LINUX) -I$(INCLUDES_DIR) -I$(LIBFT_DIR) -I$(SRC_DIR) -Wall -Wextra -Werror
C_FLAGS_MAC = -g -O3 -I$(MLX_DIR_MAC) -I$(INCLUDES_DIR) -I$(LIBFT_DIR) -I$(SRC_DIR) -Wall -Wextra -Werror -g -fsanitize=address

all:
	mkdir -p $(OBJ_DIR)
	make $(NAME)

$(NAME): $(OBJ) $(LIBFT) $(MLX_MAC)
	cc $(C_FLAGS_MAC) $(OBJ) $(LIBFT) $(MLX_MAC) -framework OpenGL -framework AppKit -o $(NAME)

$(LINUX): $(OBJ) $(LIBFT) $(MINILIBX_LINUX)
	cc $(C_FLAGS) $(OBJ) $(LIBFT) $(MINILIBX_LINUX) -lXext -lX11 -lm -o $(NAME)

$(MINILIBX_LINUX):
	make -C $(MLX_DIR_LINUX)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(MLX_MAC):
	make -C $(MLX_DIR_MAC)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDES)
	cc $(C_FLAGS) -c $< -o $@

linux:
	mkdir -p $(OBJ_DIR)
	make $(LINUX)

clean:
	make -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

bonus: all

re: fclean all

.PHONY: all clean fclean bonus re
