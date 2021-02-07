CFLAGS = -g
space: space.cpp space.h
	g++ $(CFLAGS) -c space.cpp space.h
gameserver: gameserver.cpp space.o
	g++ $(CFLAGS) gameserver.cpp space.o  -o server.out
gameclient: gameclient.cpp Parser.h Scanner.h
	g++ $(CFLAGS) gameclient.cpp Parser.h Scanner.h -o client.out
runserver:
	make space
	make gameserver
	./server.out
runclient:
	make gameclient
	./client.out
clean:
	rm server.out
	rm client.out
	rm space.o
	rm space.h.gch

