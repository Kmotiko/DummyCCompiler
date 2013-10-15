CC = g++
PROJECT_DIR = .
SRC_DIR = $(PROJECT_DIR)/src
INC_DIR = $(PROJECT_DIR)/inc
OBJ_DIR = $(PROJECT_DIR)/obj
BIN_DIR = $(PROJECT_DIR)/bin
LIB_DIR = $(PROJECT_DIR)/lib

SAMPLE_DIR = $(PROJECT_DIR)/sample

MAIN_SRC = dcc.cpp
LEXER_SRC = lexer.cpp
AST_SRC = AST.cpp
PARSER_SRC = parser.cpp
CODEGEN_SRC = codegen.cpp


MAIN_SRC_PATH = $(SRC_DIR)/$(MAIN_SRC)
LEXER_SRC_PATH = $(SRC_DIR)/$(LEXER_SRC)
AST_SRC_PATH = $(SRC_DIR)/$(AST_SRC)
PARSER_SRC_PATH = $(SRC_DIR)/$(PARSER_SRC)
CODEGEN_SRC_PATH = $(SRC_DIR)/$(CODEGEN_SRC)

MAIN_OBJ = $(OBJ_DIR)/$(MAIN_SRC:.cpp=.o)
LEXER_OBJ = $(OBJ_DIR)/$(LEXER_SRC:.cpp=.o)
AST_OBJ = $(OBJ_DIR)/$(AST_SRC:.cpp=.o)
PARSER_OBJ = $(OBJ_DIR)/$(PARSER_SRC:.cpp=.o)
CODEGEN_OBJ = $(OBJ_DIR)/$(CODEGEN_SRC:.cpp=.o)
FRONT_OBJ = $(MAIN_OBJ) $(LEXER_OBJ) $(AST_OBJ) $(PARSER_OBJ) $(CODEGEN_OBJ)

TOOL = $(BIN_DIR)/dcc
CONFIG = llvm-config
LLVM_FLAGS = --cxxflags --ldflags --libs
INC_FLAGS = -I$(INC_DIR)


all:$(FRONT_OBJ) 
	mkdir -p $(BIN_DIR)
	$(CC) -g $(FRONT_OBJ) $(INC_FLAGS) `$(CONFIG) $(LLVM_FLAGS)` -ldl -o $(TOOL)

$(MAIN_OBJ):$(MAIN_SRC_PATH)
	mkdir -p $(OBJ_DIR)
	$(CC) -g $(MAIN_SRC_PATH) $(INC_FLAGS) `$(CONFIG) $(LLVM_FLAGS)` -c -o $(MAIN_OBJ) 

$(LEXER_OBJ):$(LEXER_SRC_PATH)
	$(CC) -g $(LEXER_SRC_PATH) $(INC_FLAGS) `$(CONFIG) $(LLVM_FLAGS)` -c -o $(LEXER_OBJ) 

$(AST_OBJ):$(AST_SRC_PATH)
	$(CC) -g $(AST_SRC_PATH) $(INC_FLAGS) `$(CONFIG) $(LLVM_FLAGS)` -c -o $(AST_OBJ) 

$(PARSER_OBJ):$(PARSER_SRC_PATH)
	$(CC) -g $(PARSER_SRC_PATH) $(INC_FLAGS) `$(CONFIG) $(LLVM_FLAGS)` -c -o $(PARSER_OBJ) 

$(CODEGEN_OBJ):$(CODEGEN_SRC_PATH)
	$(CC) -g $(CODEGEN_SRC_PATH) $(INC_FLAGS) `$(CONFIG) $(LLVM_FLAGS)` -c -o $(CODEGEN_OBJ) 

clean:
	rm -rf $(FRONT_OBJ) $(TOOL)

run:
	$(TOOL) -o $(SAMPLE_DIR)/test.ll -l $(LIB_DIR)/printnum.ll $(SAMPLE_DIR)/test.dc -jit

link:
	llvm-link $(SAMPLE_DIR)/test.ll $(LIB_DIR)/printnum.ll -S -o  $(SAMPLE_DIR)/link_test.ll

lib:$(LIB_DIR)/printnum.c
	clang -emit-llvm -S -O -o $(LIB_DIR)/printnum.ll $(LIB_DIR)/printnum.c

do:
	lli $(SAMPLE_DIR)/link_test.ll
