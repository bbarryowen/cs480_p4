#825730146
# CXX Make variable for compiler
CC = g++

# Compiler flags
CCFLAGS = -std=c++11 -Wall -g3 -c

# Object files
OBJS = tradecrypto.o threadargs.o requestsQueue.o report.o producers.o consumer.o

# Target executable
PROGRAM = tradecrypto

# The program depends upon its object files
$(PROGRAM) : $(OBJS)
	$(CC) -pthread -o $(PROGRAM) $(OBJS)

tradecrypto.o : tradecrypto.cpp
	$(CC) $(CCFLAGS) tradecrypto.cpp

threadargs.o : threadargs.cpp threadargs.h
	$(CC) $(CCFLAGS) threadargs.cpp

requestQueue.o : requestsQueue.cpp requestsQueue.h
	$(CC) $(CCFLAGS) requestQueue.cpp

report.o : report.c report.h
	$(CC) $(CCFLAGS) report.c

producers.o : producers.cpp producers.h
	$(CC) $(CCFLAGS) producers.cpp

consumer.o : consumer.cpp consumer.h
	$(CC) $(CCFLAGS) consumer.cpp

# Add rules for other object files if needed

# Once things work, people frequently delete their object files.
# If you use "make clean", this will do it for you.
clean :
	rm -f *.o *~ $(PROGRAM)