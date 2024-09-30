CC = gcc
CXX = g++     
CFLAGS = -Wall -Wextra -O2
CXXFLAGS = -Wall -Wextra -O2
TARGET = DataDoctor
LIBNAME = hexfile
SRC = main.c read.c fat32.c partition.c block.c ascii.c
CPP_SRC = hexfile.cpp
OBJ = $(SRC:.c=.o) $(CPP_SRC:.cpp=.o)
LIB = lib$(LIBNAME).a

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c precomp.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) 

distclean: clean
	rm -f $(TARGET) *~
