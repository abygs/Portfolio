#
#  PROGRAM:   Assignment 5
#  AUTHOR:    Abigail Shulgan
#  LOGON ID:  z1893909
#  DUE DATE:  4/16/22
#

CCFLAGS = -Wall -g

all: z1893909_project5 z1893909_project5_p2

z1893909_project5: z1893909_project5.o
	g++ $(CCFLAGS) -o z1893909_project5 z1893909_project5.o -lpthread

z1893909_project5.o: z1893909_project5.cpp
	g++ $(CCFLAGS) -c z1893909_project5.cpp -lpthread

z1893909_project5_p2: z1893909_project5_p2.o
	g++ $(CCFLAGS) -o z1893909_project5_p2 z1893909_project5_p2.o -lpthread

z1893909_project5_p2.o: z1893909_project5_p2.cpp
	g++ $(CCFLAGS) -c z1893909_project5_p2.cpp -lpthread

clean:
	rm *.o z1893909_project5 z1893909_project5_p2
