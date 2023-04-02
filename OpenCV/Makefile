CC = g++
PROJECT = open_project
SRC = main.cpp 

LIBS = `pkg-config --cflags --libs	opencv4 `

$(PROJECT) : $(SRC)
	$(CC) $(SRC) -o $(PROJECT) $(LIBS)