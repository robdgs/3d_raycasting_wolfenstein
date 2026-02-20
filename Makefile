NAME    := cub3D
CC      := cc
CFLAGS  := -Wall -Wextra -Werror -O2
INC     := -I include -I minilibx-linux
LDFLAGS := -L minilibx-linux -lmlx -lX11 -lXext -lm

SRC_DIR := src
OBJ_DIR := obj

SRCS := $(SRC_DIR)/main.c         \
        $(SRC_DIR)/error.c        \
        $(SRC_DIR)/parse_utils.c  \
        $(SRC_DIR)/parse_scene.c  \
        $(SRC_DIR)/parse_map.c    \
        $(SRC_DIR)/init.c         \
        $(SRC_DIR)/render.c       \
        $(SRC_DIR)/events.c       \
        $(SRC_DIR)/cleanup.c

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

MLX_DIR  := minilibx-linux
MLX_LIB  := $(MLX_DIR)/libmlx.a

all: $(MLX_LIB) $(NAME)

$(MLX_LIB):
	@$(MAKE) -C $(MLX_DIR) --quiet

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "[OK] $(NAME) built"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) clean -C $(MLX_DIR) --quiet
	@echo "[clean] objects removed"

fclean: clean
	@rm -f $(NAME)
	@echo "[fclean] $(NAME) removed"

re: fclean all

.PHONY: all clean fclean re
