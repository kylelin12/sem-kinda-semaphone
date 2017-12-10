all: control write
	clear

control:
	gcc -o control control.c

write:
	gcc -o write write.c

clean:
	rm *~
	clear

cleanall:
	rm control
	rm write
	clear
	rm *~
	clear
