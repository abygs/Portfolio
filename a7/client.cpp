//***************************************************************************
//
//  Abigail Shulgan
//  z1893909
//  CSCI 463-1
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment.
//
//**************************************************************************/

//includes
#include <getopt.h>
#include <iostream>
#include <sstream>
#include <arpa/inet.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>
#include <cstring>

//usings
using std::cout;
using std::cerr;

/**
 * Prints how to use the program
 ********************************************************************************/
static void usage()
{
	cerr << "Usage: client [-s server-ip] server-port\n";
	cerr << "     -s server-ip Specify the server's IPv4 number in dotted-quad format.\n";
	cerr << "     -server-port The server port number to which the client must connect.\n";
	exit(1);
}

/**
 * Same as write() but includes a loop to complete any partials
 ********************************************************************************/
static ssize_t safe_write(int fd, const char *buf, size_t len)
{
	while(len > 0)
	{
		ssize_t wlen = write(fd, buf, len);
		if(wlen == -1)
			return -1; //unrecoverable error

		len -= wlen; //reduce remaining number of bytes to send
		buf += wlen; //advance the buffer pointer past the written dataf
	}
	return len; //full requestion length sent
}


/**
 * Read data from the given socket fd and print it until we reach EOF
 ********************************************************************************/
static int print_response(int fd)
{
	//define variables
	char buf[1024];
	int rval = 1; //prime the while loop

	while(rval > 0)
	{
		//read
		if((rval = read(fd, buf, sizeof(buf) - 1)) == -1)
		{
			perror("reading stream message");
			return -1; //let called know what happened
		}
		else if(rval > 0)
		{
			buf[rval] = '\0';
			cout << buf;
		}
	}
	return 0;
}


/**
 * simulates a client
 *
 * @param argc is the command line argument count
 *
 * @param argv is a vector that holds any arguments passed in the command line
 ********************************************************************************/
int main(int argc, char **argv)
{
	//declare and intialize variables
	int opt;
	int sock;
	int port;
	bool serverIpSet = false;
	const char* serverIP = "127.0.0.1"; //default server ip
	struct sockaddr_in server;
	struct hostent *hp;
	size_t len = 1;
	char buf[2048];

	//if no arguments passed - quit
	if(argc <= 1)
		usage();

	//getopt
        while((opt = getopt(argc, argv, "s:")) != -1)
        {
                switch(opt)
                {
                        case 's': //server-ip
				serverIP = optarg;
       				serverIpSet = true;
	                         break;
                        default:
				usage();
				break;
                }
        }

	//set port
	if(serverIpSet)
	{
		if (argc >= 4)
			port = atoi(argv[3]);
		else
			usage();
	}
	else
		if (argc >= 2)
			port = atoi(argv[1]);


	//parse serverIP's IPV4 address
	if(inet_pton(AF_INET, serverIP, &server.sin_addr)<=0)
	{
		cerr << serverIP << ": invalid address/format\n";
		exit(2);
	}


	//create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("opening stream socket");
		exit(1);
	}


	//connect socket
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	hp = gethostbyname(serverIP);

	if(hp == 0)
	{
		cerr << serverIP << ": unknown host\n";
		exit(2);
	}

	bcopy(hp->h_addr, &server.sin_addr, hp->h_length);
	server.sin_port = htons(port);

	//connect
	if(connect(sock, (sockaddr*)&server, sizeof(server)) < 0)
	{
		perror("connecting stream socket");
		exit(1);
	}


	while(len > 0)
	{
		//read  length
		len = read(fileno(stdin), buf, sizeof(buf) - 1);

		if(len < 0)
		{
			perror("reading input");
			exit(1);
		}

		//write
		if(safe_write(sock, buf, len) < 0)
		{
			perror("writing on stream socket");
			exit(1);
		}
	}

	//shutdown
	shutdown(sock, SHUT_WR);

	//print
	print_response(sock);


	//close socket
	close(sock);

	return 0;
}
