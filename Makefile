mymake:main.o Node.o Edge.o
	gcc -Wall main.o Node.o Edge.o -o mymake
%.o : %.c %.h
	gcc -Wall -c $*.c -o $@
.Phony:clean
clean:
	rm -f *.o mymake
