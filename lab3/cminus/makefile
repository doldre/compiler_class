#
# Lexer for tiny c
# Mathon Luo 2017年 4月12日 星期三 23时50分01秒 CST
#
CXX = g++

CFLAGS = -Wall -g

OBJS = util.o scan.o main.o parser.o

parser: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o parser

main.o: main.cpp globals.h scan.h parser.h
	$(CXX) $(CFLAGS) -c main.cpp

scan.o: scan.cpp globals.h scan.h
	$(CXX) $(CFLAGS) -c scan.cpp

util.o: util.cpp util.h globals.h
	$(CXX) $(CFLAGS) -c util.cpp

parser.o: scan.h parser.h globals.h parser.cpp
	$(CXX) $(CFLAGS) -c parser.cpp

all: parser

test: parser test/example.c
	-./parser test/example.c
clean:
	-rm parser
	-rm $(OBJS)

