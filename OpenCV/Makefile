CC = g++
PROJECT = pen_detect
SRC = PenDetect.cpp

LIBS = `pkg-config --cflags --libs	opencv4 ` -lserial -I/usr/include/libserial

$(PROJECT) : $(SRC)
	$(CC) $(SRC) -o $(PROJECT) $(LIBS)