CC = gcc
FLAGS = -Wall -Wextra -pedantic -std=c11 -Iinclude -lm
DEBUG_FLAG = -g
OPTIMIZE_FLAG = -O3

NAME = ipv4h

SRC_DIR = src
BUILD_DIR = build

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst ${SRC_DIR}/%.c, ${BUILD_DIR}/%.o, $(SRC))

ifeq ($(debug),1)
	FLAGS += $(DEBUG_FLAG)
endif

ifeq ($(optimize),1)
	FLAGS += $(OPTIMIZE_FLAG)
endif

.PHONY: all clean

all: dir ${NAME}

${NAME}: $(OBJ)
	$(CC) $(FLAGS) $^ -o $@

${BUILD_DIR}/%.o: $(SRC_DIR)/%.c
	$(CC) $(FLAGS) -c $< -o $@

dir:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(NAME) 
