CC := gcc
NAME := spcecho

MK_PATH = $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

DIR := $(MK_PATH)src

OBJS := $(DIR)/main.o $(DIR)/addresses.o $(DIR)/readwrite.o

CFLAGS := -O2 -s -Wall -Wpedantic -Wextra
LDFLAGS := -O2 -s

%.o: $(DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	@rm -f $(MK_PATH)*~ $(DIR)/*.o $(MK_PATH)$(NAME)
