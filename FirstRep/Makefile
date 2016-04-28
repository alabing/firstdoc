all:server client
server:server.o 
	gcc -o server server.o -lpthread
client:client.o 
	gcc -o client client.o -lpthread
server.o:server.c 
	gcc -c server.c -lpthread
client.o:client.c 
	gcc -c client.c -lpthread
clean:
	rm server.o client.o server client
