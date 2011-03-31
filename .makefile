
CFLAGS="-m32"
LIBS=-lfl -L/usr/lib32

OBJS=symbol.o cell.o scanner.o main.o boolean.o int32.o

main: ${OBJS}
	clang++ -o main symbol.o cell.o scanner.o main.o boolean.o int32.o -m32 ${LIBS}



symbol.o: symbol.cpp symbol.h atom.h
	clang++ -c symbol.cpp -o symbol.o ${CFLAGS}

 
cell.o: cell.cpp cell.h
	clang++ -c cell.cpp -o cell.o ${CFLAGS}

 
scanner.o: scanner.cpp scanner.h
	clang++ -c scanner.cpp -o scanner.o ${CFLAGS}

 
main.o: main.cpp cell.h int32.h boolean.h symbol.h
	clang++ -c main.cpp -o main.o ${CFLAGS}

 
boolean.o: boolean.cpp boolean.h
	clang++ -c boolean.cpp -o boolean.o ${CFLAGS}

 
int32.o: int32.cpp int32.h
	clang++ -c int32.cpp -o int32.o ${CFLAGS}



clean:
	rm -f *.o main

