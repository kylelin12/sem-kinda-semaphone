all: control write
	clear

control:
	gcc -o control control.c

write:
	gcc -o write write.c

cleanall:
	rm -f control write a.out *~ storytext.txt
	clear
