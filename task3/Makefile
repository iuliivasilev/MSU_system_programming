CFLAGS = -g
customer: customer.h customer.cpp
	g++ $(CFLAGS) -c customer.cpp customer.h
developer: developer.h developer.cpp
	g++ $(CFLAGS) -c developer.cpp developer.h
admin: admin.h admin.cpp
	g++ $(CFLAGS) -c admin.cpp admin.h
compile:
	make customer
	make developer
	make admin
run: test.cpp user.h customer.o developer.o admin.o
	g++ $(CFLAGS) test.cpp user.h customer.o developer.o admin.o -o run.out
	./run.out
clean:	
	rm customer.o
	rm developer.o
	rm admin.o
	rm customer.h.gch
	rm developer.h.gch
	rm admin.h.gch
	rm run.out
	rm user.h.gch
