INCLUDES_DIR = ./includes
SRC_DIR = ./src
OBJ_DIR = ./obj
LIBFT_DIR = ./libft
MLX_DIR_LINUX = ./minilibx
MLX_DIR_MAC = ./minilibx_mac

INCLUDES_FILES = minirt.h \
		         minirt_bindings.h \
		         linear_algebra.h \
				 minirt_objects.h \
				 minirt_colors.h

SRC_FILES =	minirt.c \
			map.c \
			map_read.c \
			map_process.c \
			map_process_utils.c \
			map_atof.c \
			map_error.c \
			minirt_key_hooks.c \
			matrix.c \
			matrix_operations.c \
			matrix_utils.c \
			matrix_inversion.c \
			minirt_vector_operations.c \
			minirt_vector_products.c \
			minirt_vector_utils.c \
			minirt_basis_utils.c \
			minirt_transforms.c \
			minirt_bump_mapping.c \
			minirt_mapping_load.c \
			minirt_init.c \
			minirt_cleanup.c \
			minirt_rotations.c \
			minirt_rotations_setup.c \
			minirt_materials.c \
			minirt_material_edit.c \
			minirt_window_hooks.c \
			minirt_plane.c \
			minirt_sphere.c \
			minirt_cylinder.c \
			minirt_cone.c \
			minirt_rendering.c \
			minirt_ray.c \
			minirt_shading.c \
			minirt_shading_texture.c \
			minirt_lights.c \
			minirt_shadows.c \
			minirt_spaces.c \
			minirt_camera.c \
			minirt_camera_control.c \
			minirt_projection.c \
			minirt_colors.c \
			minirt_mouse.c \
			minirt_textures.c \
			minirt_interaction.c


INCLUDES = $(addprefix $(INCLUDES_DIR)/, $(INCLUDES_FILES))
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

NAME = minirt
LINUX = lin

LIBFT = $(LIBFT_DIR)/libft.a
MINILIBX_LINUX = $(MLX_DIR_LINUX)/libmlx_Linux.a
MLX_MAC = $(MLX_DIR_MAC)/libmlx.a

# C_FLAGS = -O3 -I$(MLX_DIR_LINUX) -I$(INCLUDES_DIR) -I$(LIBFT_DIR) -I$(SRC_DIR) -Wall -Wextra -Werror -MMD -MP
C_FLAGS = -g -O3 -I$(MLX_DIR_LINUX) -I$(INCLUDES_DIR) -I$(LIBFT_DIR) -I$(SRC_DIR) -Wall -Wextra -Werror -Wno-unused-result
C_FLAGS_MAC = -O3 -I$(MLX_DIR_MAC) -I$(INCLUDES_DIR) -I$(LIBFT_DIR) -I$(SRC_DIR) -Wall -Wextra -Werror

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
