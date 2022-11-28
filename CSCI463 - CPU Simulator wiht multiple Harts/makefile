#
# PROGRAM: Assignment 5
# AUTHOR:  Abigail Shulgan
#

CXXFLAGS = -ansi -pedantic -Wall -Werror -g -std=c++14

rv32i: main.o rv32i_decode.o memory.o hex.o registerfile.o rv32i_hart.o cpu_single_hart.o
	g++ $(CXXFLAGS) -o rv32i main.o rv32i_decode.o memory.o hex.o registerfile.o rv32i_hart.o cpu_single_hart.o

main.o: main.cpp
	g++ $(CXXFLAGS) -c -o main.o main.cpp

rv32i_decode.o: rv32i_decode.cpp
	g++ $(CXXFLAGS) -c -o rv32i_decode.o rv32i_decode.cpp

memory.o: memory.cpp
	g++ $(CXXFLAGS) -c -o memory.o memory.cpp

hex.o: hex.cpp
	g++ $(CXXFLAGS) -c -o hex.o hex.cpp

registerfile.o: registerfile.cpp
	g++ $(CXXFLAGS) -c -o registerfile.o registerfile.cpp

rv32i_hart.o: rv32i_hart.cpp
	g++ $(CXXFLAGS) -c -o rv32i_hart.o rv32i_hart.cpp

cpu_single_hart.o: cpu_single_hart.cpp
	g++ $(CXXFLAGS) -c -o cpu_single_hart.o cpu_single_hart.cpp

clean:
	rm -f *.o rv32i
