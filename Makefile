# Binary output directory
BIN = ./bin

# Library includes directory
LIB_INCLUDES = ./lib/include

# Library source code directory
LIB_SOURCE = ./lib/source

# Main source code directory
SOURCE = ./src

# Build the target executable
shell: $(BIN)/command_table.o $(BIN)/parser.o $(BIN)/executor.o $(BIN)/prompt.o $(BIN)/jobs.o $(BIN)/builtin.o $(BIN)/main.o $(BIN)
	cc -o ./shell $(BIN)/command_table.o $(BIN)/parser.o $(BIN)/executor.o $(BIN)/prompt.o $(BIN)/jobs.o $(BIN)/builtin.o $(BIN)/main.o

$(BIN)/main.o: $(LIB_INCLUDES)/command_table.h $(LIB_INCLUDES)/parser.h $(LIB_INCLUDES)/executor.h $(LIB_INCLUDES)/prompt.h $(LIB_INCLUDES)/jobs.h $(LIB_INCLUDES)/builtin.h $(SOURCE)/main.c $(BIN)
	cc -c $(SOURCE)/main.c -o $(BIN)/main.o -I$(LIB_INCLUDES)

$(BIN)/executor.o: $(LIB_INCLUDES)/command_table.h $(LIB_INCLUDES)/jobs.h $(LIB_INCLUDES)/executor.h $(LIB_SOURCE)/executor.c $(BIN)
	cc -c $(LIB_SOURCE)/executor.c -o $(BIN)/executor.o -I$(LIB_INCLUDES)

$(BIN)/parser.o: $(LIB_INCLUDES)/str_util.h $(LIB_INCLUDES)/command_table.h $(LIB_INCLUDES)/parser.h $(LIB_SOURCE)/parser.c $(BIN)
	cc -c $(LIB_SOURCE)/parser.c -o $(BIN)/parser.o -I$(LIB_INCLUDES)

$(BIN)/command_table.o: $(LIB_INCLUDES)/command_table.h $(LIB_SOURCE)/command_table.c $(BIN)
	cc -c $(LIB_SOURCE)/command_table.c -o $(BIN)/command_table.o -I$(LIB_INCLUDES)

$(BIN)/prompt.o: $(LIB_INCLUDES)/prompt.h $(LIB_SOURCE)/prompt.c $(BIN)
	cc -c $(LIB_SOURCE)/prompt.c -o $(BIN)/prompt.o -I$(LIB_INCLUDES)

$(BIN)/jobs.o: $(LIB_INCLUDES)/command_table.h $(LIB_INCLUDES)/prompt.h $(LIB_INCLUDES)/jobs.h $(LIB_SOURCE)/jobs.c $(BIN)
	cc -c $(LIB_SOURCE)/jobs.c -o $(BIN)/jobs.o -I$(LIB_INCLUDES)

$(BIN)/builtin.o: $(LIB_INCLUDES)/command_table.h $(LIB_INCLUDES)/jobs.h $(LIB_INCLUDES)/builtin.h $(LIB_SOURCE)/builtin.c $(BIN)
	cc -c $(LIB_SOURCE)/builtin.c -o $(BIN)/builtin.o -I$(LIB_INCLUDES)

$(BIN):
	mkdir -p $(BIN)

# Clean any previous build
clean:
	rm -rf ./bin/
	rm -rf ./shell
