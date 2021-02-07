CFLAGS = -g
create: polynomial.cpp polynomial.h
	g++ $(CFLAGS) -c polynomial.cpp polynomial.h
compile: main.cpp polynomial.o
	g++ $(CFLAGS) main.cpp polynomial.o -o run.out
run:
	./run.out
clean:
	rm polynomial.o
	rm run.out
