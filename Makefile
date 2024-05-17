# MCP Makefile
# Author: Joseph Erlinger (jerling2@uoregon.edu)
# Brief :
# Usuage: 

all : part1 part2 part3 part4 iobound cpubound

readproc.o : readproc.c
	gcc -o readproc.o -c -g -Wall readproc.c

cpubound : cpubound.c
	gcc -o cpubound cpubound.c

iobound : iobound.c
	gcc -o iobound iobound.c

test_queue : test_queue.o queue.o parser.o
	gcc -o test_queue -g test_queue.o queue.o parser.o

test_queue.o : Tests/test_queue.c
	gcc -o test_queue.o -c -g $^

part4 : part4.o parser.o queue.o terminal.o readproc.o
	gcc -o part4 -g part4.o parser.o queue.o terminal.o readproc.o

part3 : part3.o parser.o queue.o terminal.o
	gcc -o part3 -g part3.o parser.o queue.o terminal.o

part2 : part2.o parser.o queue.o terminal.o
	gcc -o part2 -g part2.o parser.o queue.o terminal.o

part1 : part1.o parser.o queue.o terminal.o
	gcc -o part1 -g part1.o parser.o queue.o terminal.o

part4.o : part4.c
	gcc -o part4.o -c -g -Wall part4.c

part3.o : part3.c
	gcc -o part3.o -c -g -Wall part3.c

part2.o : part2.c
	gcc -o part2.o -c -g -Wall part2.c

part1.o : part1.c
	gcc -o part1.o -c -g -Wall part1.c

parser.o : parser.c
	gcc -o parser.o -c -g -Wall parser.c

queue.o : queue.c
	gcc -o queue.o -c -g -Wall queue.c

terminal.o : terminal.c
	gcc -o terminal.o -c -g -Wall terminal.c

clean :
	rm part[0-9] *.o