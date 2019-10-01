all: main.c
	gcc -g main.c -o ./usr/local/bin/kerrat `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -lpthread

clean: ./usr/local/bin/kerrat
	rm ./usr/local/bin/kerrat
