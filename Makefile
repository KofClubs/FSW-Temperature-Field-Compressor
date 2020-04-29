CC:=g++
CCFLAGS:=
LDFLAGS:=

TARGETS_DIR:=./bin
OBJ_DIR=./obj
SRC_DIR=./src

TARGETS:=$(TARGETS_DIR)/ftfc
MAINS:=$(OBJ_DIR)/main.o
OBJ:=$(OBJ_DIR)/user_defined.o $(OBJ_DIR)/vector3.o $(OBJ_DIR)/temperature.o $(OBJ_DIR)/bitmap.o $(OBJ_DIR)/compress.o $(OBJ_DIR)/decompress.o $(OBJ_DIR)/test.o $(OBJ_DIR)/select.o

.PHONY:all clean

all:$(TARGETS)

clean:
	rm -f $(MAINS) $(OBJ) $(TARGETS)

$(TARGETS):$(MAINS) $(OBJ)
	$(CC) -o $(TARGETS) $(MAINS) $(OBJ)

$(MAINS):$(SRC_DIR)/main.cpp $(SRC_DIR)/compress.h $(SRC_DIR)/decompress.h $(SRC_DIR)/exit_status.h $(SRC_DIR)/options.hpp $(SRC_DIR)/select.h $(SRC_DIR)/test.h
	$(CC) -c -o $(MAINS) $(SRC_DIR)/main.cpp

$(OBJ_DIR)/user_defined.o:$(SRC_DIR)/user_defined.cpp $(SRC_DIR)/user_defined.h
	$(CC) -c -o $(OBJ_DIR)/user_defined.o $(SRC_DIR)/user_defined.cpp

$(OBJ_DIR)/vector3.o:$(SRC_DIR)/vector3.cpp $(SRC_DIR)/vector3.h
	$(CC) -c -o $(OBJ_DIR)/vector3.o $(SRC_DIR)/vector3.cpp

$(OBJ_DIR)/temperature.o:$(SRC_DIR)/temperature.cpp $(SRC_DIR)/temperature.h $(SRC_DIR)/exit_status.h
	$(CC) -c -o $(OBJ_DIR)/temperature.o $(SRC_DIR)/temperature.cpp

$(OBJ_DIR)/bitmap.o:$(SRC_DIR)/bitmap.cpp $(SRC_DIR)/bitmap.h
	$(CC) -c -o $(OBJ_DIR)/bitmap.o $(SRC_DIR)/bitmap.cpp

$(OBJ_DIR)/compress.o:$(SRC_DIR)/compress.cpp $(SRC_DIR)/compress.h $(SRC_DIR)/bitmap.h $(SRC_DIR)/exit_status.h $(SRC_DIR)/temperature.h $(SRC_DIR)/user_defined.h $(SRC_DIR)/vector3.h
	$(CC) -c -o $(OBJ_DIR)/compress.o $(SRC_DIR)/compress.cpp

$(OBJ_DIR)/decompress.o:$(SRC_DIR)/decompress.cpp $(SRC_DIR)/decompress.h $(SRC_DIR)/bitmap.h $(SRC_DIR)/temperature.h $(SRC_DIR)/user_defined.h $(SRC_DIR)/vector3.h
	$(CC) -c -o $(OBJ_DIR)/decompress.o $(SRC_DIR)/decompress.cpp

$(OBJ_DIR)/test.o:$(SRC_DIR)/test.cpp $(SRC_DIR)/test.h $(SRC_DIR)/temperature.h $(SRC_DIR)/user_defined.h
	$(CC) -c -o $(OBJ_DIR)/test.o $(SRC_DIR)/test.cpp

$(OBJ_DIR)/select.o:$(SRC_DIR)/select.cpp $(SRC_DIR)/select.h $(SRC_DIR)/exit_status.h $(SRC_DIR)/user_defined.h $(SRC_DIR)/vector3.h
	$(CC) -c -o $(OBJ_DIR)/select.o $(SRC_DIR)/select.cpp
