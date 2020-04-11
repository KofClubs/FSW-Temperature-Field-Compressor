CC:=g++
CCFLAGS:=
LDFLAGS:=

TARGETS_DIR:=./bin
OBJ_DIR=./obj
SRC_DIR=./src

TARGETS:=$(TARGETS_DIR)/ftfc
MAINS:=$(OBJ_DIR)/main.o
OBJ:=$(OBJ_DIR)/user_defined.o $(OBJ_DIR)/vector3.o $(OBJ_DIR)/compress.o $(OBJ_DIR)/decompress.o

.PHONY:all clean

all:$(TARGETS)

clean:
	rm -f $(MAINS) $(OBJ) $(TARGETS)

$(TARGETS):$(MAINS) $(OBJ)
	$(CC) -o $(TARGETS) $(MAINS) $(OBJ)

$(MAINS):$(SRC_DIR)/main.cpp $(SRC_DIR)/compress.h $(SRC_DIR)/decompress.h $(SRC_DIR)/exit_status.h $(SRC_DIR)/options.hpp
	$(CC) -c -o $(MAINS) $(SRC_DIR)/main.cpp

$(OBJ_DIR)/user_defined.o:$(SRC_DIR)/user_defined.cpp $(SRC_DIR)/user_defined.h
	$(CC) -c -o $(OBJ_DIR)/user_defined.o $(SRC_DIR)/user_defined.cpp

$(OBJ_DIR)/vector3.o:$(SRC_DIR)/vector3.cpp $(SRC_DIR)/vector3.h
	$(CC) -c -o $(OBJ_DIR)/vector3.o $(SRC_DIR)/vector3.cpp

$(OBJ_DIR)/compress.o:$(SRC_DIR)/compress.cpp $(SRC_DIR)/compress.h $(SRC_DIR)/exit_status.h $(SRC_DIR)/user_defined.h $(SRC_DIR)/vector3.h
	$(CC) -c -o $(OBJ_DIR)/compress.o $(SRC_DIR)/compress.cpp

$(OBJ_DIR)/decompress.o:$(SRC_DIR)/decompress.cpp $(SRC_DIR)/decompress.h
	$(CC) -c -o $(OBJ_DIR)/decompress.o $(SRC_DIR)/decompress.cpp
