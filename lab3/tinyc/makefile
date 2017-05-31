#
# Lexer for tiny c
# Mathon Luo 2017年 4月12日 星期三 23时50分01秒 CST
#
CXX = g++

CFLAGS = -Wall -g

OBJS = util.o scan.o parser.o main.o

parser: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o parser

main.o: main.cpp globals.h parser.h 
	$(CXX) $(CFLAGS) -c main.cpp

scan.o: scan.cpp globals.h scan.h
	$(CXX) $(CFLAGS) -c scan.cpp

util.o: util.cpp util.h globals.h
	$(CXX) $(CFLAGS) -c util.cpp

parser.o: util.cpp util.h globals.h parser.cpp parser.h
	$(CXX) $(CFLAGS) -c parser.cpp

test: parser
	-./parser test/example.tiny

clean:
	-rm parser
	-rm $(OBJS)

