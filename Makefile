INCLUDES_DIR = ./includes
SRC_DIR = ./src
OBJ_DIR = ./obj
LIBFT_DIR = ./libft

INCLUDES_FILES = minirt.h \
		         minirt_bindings.h \
		         linear_algebra.h \
				 minirt_objects.h \
				 minirt_colors.h

SRC_FILES =	minirt.c \
			map.c \
			map_read.c \
			map_process1.c \
			map_process2.c \
			map_process3.c \
			map_process_utils1.c \
			map_process_utils2.c \
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

LIBFT = $(LIBFT_DIR)/libft.a

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	MLX_DIR = ./minilibx
    LFLAGS = -lXext -lX11 -lm
	MINILIBX = $(MLX_DIR)/libmlx_Linux.a
	C_FLAGS = -Wno-unused-result
endif
ifeq ($(UNAME_S),Darwin)
	MLX_DIR = ./minilibx_mac
    LFLAGS = -framework OpenGL -framework AppKit
	MINILIBX = $(MLX_DIR)/libmlx.a
endif

C_FLAGS += -O3 -I$(MLX_DIR) -I$(INCLUDES_DIR) -I$(LIBFT_DIR) -I$(SRC_DIR) -Wall -Wextra -Werror

all:
	mkdir -p $(OBJ_DIR)
	make $(NAME)

$(NAME): $(OBJ) $(LIBFT) $(MINILIBX)
	cc $(C_FLAGS) $(OBJ) $(LIBFT) $(MINILIBX) $(LFLAGS) -o $(NAME)

$(MINILIBX):
	make -C $(MLX_DIR)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDES)
	cc $(C_FLAGS) -c $< -o $@

clean:
	make -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)
	make -C $(MLX_DIR) clean

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

bonus: all

re: fclean all

.PHONY: all clean fclean bonus re
