#
#  PROGRAM:   Assignment 4
#  AUTHOR:    Abigail Shulgan
#  LOGON ID:  z1893909
#  DUE DATE:  3/29/22
#

CCFLAGS = -Wall -g

z1893909_project4: z1893909_project4.o process.o
	g++ $(CCFLAGS) -o z1893909_project4 z1893909_project4.o process.o

z1893909_project4.o: z1893909_project4.cpp
	g++ $(CCFLAGS) -c z1893909_project4.cpp

process.o: process.cpp
	g++ $(CCFLAGS) -c process.cpp

clean:
	rm *.o z1893909_project4
