all: final

final: main.o queue.o
	gcc -o out/branch out/main.o out/queue.o -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

out: 
	test ! -d out && mkdir out

main.o: main.c out
	gcc -c main.c -o out/main.o

queue.o: queue.c out
	gcc -c queue.c -o out/queue.o

clean:
	rm -rf out
