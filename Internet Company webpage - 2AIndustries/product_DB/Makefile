invenory: inventory.o
	g++ -o inventory inventory.o -lmariadb

inventory.o: inventory.cpp
	g++ -c -I/usr/include/mariadb inventory.cpp

clean:
	rm *.o inventory
