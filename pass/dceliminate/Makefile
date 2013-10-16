CC = g++
DCC = ../../bin/dcc
PROJECT_DIR = .
SRC_DIR = $(PROJECT_DIR)/src
INC_DIR = $(PROJECT_DIR)/inc
OBJ_DIR = $(PROJECT_DIR)/obj
BIN_DIR = $(PROJECT_DIR)/bin

SAMPLE_DIR = $(PROJECT_DIR)/sample

PASS_SRC = dceliminate.cpp

PASS_SRC_PATH = $(SRC_DIR)/$(PASS_SRC)

PASS_OBJ = $(OBJ_DIR)/$(PASS_SRC:.cpp=.o)

LIB = $(BIN_DIR)/dceliminate.so
CONFIG = llvm-config
LLVM_FLAGS = --cxxflags
LLVM_FLAGS2 = --ldflags --libs
LLVM_FLAGS3 = --cxxflags --ldflags --libs
INC_FLAGS = -I$(INC_DIR)


all:$(PASS_OBJ) 
	mkdir -p $(BIN_DIR)
	$(CC) -g -shared -o $(LIB) $(PASS_OBJ) 

$(PASS_OBJ):$(PASS_SRC_PATH)
	mkdir -p $(OBJ_DIR)
	$(CC) -g $(PASS_SRC_PATH) $(INC_FLAGS) `$(CONFIG) $(LLVM_FLAGS)` -c -o $(PASS_OBJ) 

clean:
	rm -rf $(PASS_OBJ) $(LIB)

sample:$(SAMPLE_DIR)/sample.c 
	clang -emit-llvm -S $(SAMPLE_DIR)/sample.c -o $(SAMPLE_DIR)/sample.ll
	opt -mem2reg -S $(SAMPLE_DIR)/sample.ll -o $(SAMPLE_DIR)/sample.ll

run:
	opt -load $(LIB) -dceliminate -debug-pass=Structure  $(SAMPLE_DIR)/sample.ll -S 
