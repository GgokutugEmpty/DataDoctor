CC = gcc
CXX = g++ 
CFLAGS = -Wall -Wextra -O2
CXXFLAGS = -Wall -Wextra -O2
TARGET = DataDoctor
SRC = main.c read.c fat32.c partition.c block.c hexfile.cpp
OBJ = $(SRC:.c=.o) $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c precomp.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

distclean: clean
	rm -f *~

.PHONY: all clean distclean
