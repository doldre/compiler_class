CXX = g++

CFLAGS = -Wall -g

OBJS = scan.o main.o

lexer: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o lexer

main.o: main.cpp globals.h scan.h
	$(CXX) $(CFLAGS) -c main.cpp

scan.o: scan.cpp globals.h scan.h
	$(CXX) $(CFLAGS) -c scan.cpp

all: lexer

clean:
	-rm lexer
	-rm $(OBJS)

