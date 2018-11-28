main.o: functions.h functions.c
	gcc -c -Wall main.c

build: functions.h functions.c
	gcc -o proj2.out main.c -Wall

run:
	./proj2.out adresses.txt

clean:
	rm proj2.out main.o
