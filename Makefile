all: final

debug: main_debug.o queue_debug.o arrays_debug.o
	gcc -fsanitize=address -fno-omit-frame-pointer -o out/branch out/main.o out/queue.o out/arrays.o -lraylib -lGL -lm -lpthread -ldl -lrt -lX11


final: main.o queue.o arrays.o
	gcc -o out/branch out/main.o out/queue.o out/arrays.o -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

out: 
	test ! -d out && mkdir out

main_debug.o: main.c out
	gcc -g -fsanitize=address -fno-omit-frame-pointer -c main.c -o out/main.o

queue_debug.o: queue.c out
	gcc -g -fsanitize=address -fno-omit-frame-pointer -c queue.c -o out/queue.o

arrays_debug.o: arrays.c out
	gcc -g -fsanitize=address -fno-omit-frame-pointer arrays.c -c -o out/arrays.o

main.o: main.c out
	gcc -c main.c -o out/main.o

queue.o: queue.c out
	gcc -c queue.c -o out/queue.o

arrays.o: arrays.c out
	gcc -c arrays.c -o out/arrays.o

clean:
	rm -rf out
