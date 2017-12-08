compile: control.o write.o main.o
	gcc -o test control.o write.o main.o

control.o: ./control/control.c ./control/control.h
	gcc -c control.c

write.o: ./write/write.c ./write/write.h
	gcc -c write.c

main.o: main.c main.h
	gcc -c main.c

clean:
	rm *.o