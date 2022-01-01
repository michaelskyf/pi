PROJECT_NAME := pi

ifneq ("$(origin CC)", "commandline")
  CC := gcc
endif

files := pi.c
CFLAGS := -std=c99 -Wall -Wextra -Wpedantic -g -Ofast
LDFLAGS := -lm -pthread

all: $(PROJECT_NAME)

$(PROJECT_NAME): FORCE
	$(CC) $(files) -o $(PROJECT_NAME) $(CFLAGS) $(LDFLAGS)

run: $(PROJECT_NAME)
	./$(PROJECT_NAME)
clean:
	rm -f $(PROJECT_NAME)

FORCE: ;
