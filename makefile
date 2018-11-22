server: server.o
	gcc -o server server.o
server.o: server.c
	gcc -pedantic-errors -Wall -c server.c
clean:
	rm server.o server
