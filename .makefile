
CFLAGS="-m32"
LIBS=-L/usr/lib32 -lfl 

OBJS=

main: ${OBJS}
	clang++ -o main  -m32 ${LIBS}




clean:
	rm -f *.o main

