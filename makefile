LIBS=-lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system

all:
	g++ -c main.cc
	g++ main.o -o thegame $(LIBS)

clean:
	rm -f thegame

install:
	cp thegame /usr/bin

uninstall:
	rm thegame