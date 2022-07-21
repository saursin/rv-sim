SRC_DIR = src

BIN_DIR = build/bin
$(shell mkdir -p build/bin)

OBJ_DIR = build/obj
$(shell mkdir -p build/obj)


SRCS_ := main.cpp mem.cpp core.cpp
OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS_))
SRCS = $(patsubst %, $(SRC_DIR)/%, $(SRCS_))

CC := g++
CFLAGS := -Wall -g -DLOGGER_ALL -DLOG_REGVIEW
LFLAGS := 


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "compiling... [$< -> $@]"
	$(CC) -c $(CFLAGS) -o $@ $<


$(BIN_DIR)/rvsim: $(OBJS)
	@echo "linking...   [{$^} -> $@]"
	$(CC) $(LFLAGS) -o $@ $^


.PHONY: clean
clean:
	rm -rf build/

