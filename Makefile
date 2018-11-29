main.o: functions.h functions.o
	gcc -o main.o main.c

proj2: functions.c functions.h
	gcc -o proj2 functions.c main.c

build: functions.h
	gcc -o proj2.out main.c

run:
	./proj2.out addresses.txt

clean:
	rm proj2.out main.o
