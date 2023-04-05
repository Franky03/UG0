CC = g++
PROJECT = pen_detect
SRC = manoelgomes.cpp

LIBS = `pkg-config --cflags --libs	opencv4 `

$(PROJECT) : $(SRC)
	$(CC) $(SRC) -o $(PROJECT) $(LIBS)