main: main.c peripheral.o
	gcc -O2 -o main main.c peripheral.o -lm -static -Wno-unused-result

main_debug: main.c peripheral.o
	gcc -O2 -g -o main main.c peripheral.o -lm -static -Wno-unused-result

peripheral.o: peripheral.c
	gcc -O2 -c peripheral.c -Wno-unused-result

clean:
	rm main peripheral.o
	/home/nullptr/gem5-project/tests/test-progs/simplecnn
