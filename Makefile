CC = g++
CFLAGS = $$(pkg-config --libs --cflags cairo)

default: tile

tile: main.o make_image.o lozenge.o
	$(CC) -o tile main.o make_image.o lozenge.o $(CFLAGS)

main.o: main.cpp make_image.h lozenge.h
	$(CC) -c main.cpp

make_image.o: make_image.cpp make_image.h
	$(CC) -c make_image.cpp $(CFLAGS)

lozenge.o: lozenge.cpp lozenge.h
	$(CC) -c -O2 lozenge.cpp

clean:
	$(RM) tile *.o *~
