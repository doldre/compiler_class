#
# Lexer for tiny c
# Mathon Luo 2017年 4月12日 星期三 23时50分01秒 CST
#
CXX = g++

CFLAGS = -Wall -g

OBJS = util.o scan.o main.o

lexer: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o lexer

main.o: main.cpp globals.h scan.h
	$(CXX) $(CFLAGS) -c main.cpp

scan.o: scan.cpp globals.h scan.h
	$(CXX) $(CFLAGS) -c scan.cpp

util.o: util.cpp util.h globals.h
	$(CXX) $(CFLAGS) -c util.cpp
all: lexer

test: lexer
	-./lexer test/example.tiny
clean:
	-rm lexer
	-rm $(OBJS)

