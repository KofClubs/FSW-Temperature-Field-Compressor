CC:=g++
CCFLAGS:=
LDFLAGS:=

TARGETS_DIR:=./bin
OBJ_DIR=./obj
SRC_DIR=./src

TARGETS:=$(TARGETS_DIR)/ftfc
MAINS:=$(OBJ_DIR)/main.o
OBJ:=$(OBJ_DIR)/options.o $(OBJ_DIR)/compress.o $(OBJ_DIR)/decompress.o
DEPS:=$(SRC_DIR)/user_defined.hpp $(SRC_DIR)/options.h $(SRC_DIR)/compress.h $(SRC_DIR)/decompress.h $(SRC_DIR)/vector3.hpp $(SRC_DIR)/exit_status.h

.PHONY:all clean

all:$(TARGETS)

clean:
	rm -f $(MAINS) $(OBJ) $(TARGETS)

$(TARGETS):$(MAINS) $(OBJ)
	$(CC) -o $(TARGETS) $(MAINS) $(OBJ)

$(MAINS):$(SRC_DIR)/main.cpp $(DEPS)
	$(CC) -c -o $(MAINS) $(SRC_DIR)/main.cpp

$(OBJ_DIR)/options.o:$(SRC_DIR)/options.cpp $(DEPS)
	$(CC) -c -o $(OBJ_DIR)/options.o $(SRC_DIR)/options.cpp

$(OBJ_DIR)/compress.o:$(SRC_DIR)/compress.cpp $(DEPS)
	$(CC) -c -o $(OBJ_DIR)/compress.o $(SRC_DIR)/compress.cpp

$(OBJ_DIR)/decompress.o:$(SRC_DIR)/decompress.cpp $(DEPS)
	$(CC) -c -o $(OBJ_DIR)/decompress.o $(SRC_DIR)/decompress.cpp
