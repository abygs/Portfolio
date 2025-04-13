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
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

//usings
using std::cout;
using std::cerr;

/**
 * Prints how to use the program
 ********************************************************************************/
static void usage()
{
        cerr << "Usage: server [-l listener-port]\n";
        cerr << "     -listener-port The port number to which the server must listen.\n";
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
 * simulates a server
 *
 * @param argc is the command line argument count
 *
 * @param argv is a vector that holds any arguments passed in the command line
 ********************************************************************************/
int main(int argc, char **argv)
{
	//ignore broken pipes
	signal(SIGPIPE, SIG_IGN);

	//delcare and initialize variables
	int sock;
	int msgsock;
	int rval = 1; //prime the while loop
	int opt;
	uint16_t port = 0; //0 by default
	socklen_t length;
	struct sockaddr_in server;
	char buf[1024];

        //getopt
        while((opt = getopt(argc, argv, "l:")) != -1)
        {
                switch(opt)
                {
                        case 'l': //listener post
				port = atoi(optarg);
                                break;
                        default:
                                usage();
                                break;
                }
        }


	//create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("opening stream socket");
		exit(1);
	}

	//name socket using wildcards
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	//bind to socket
	if(bind(sock, (sockaddr*)&server, sizeof(server)))
	{
		perror("binding stream socket");
		exit(1);
	}

	//find out assigned port number and print it
	length = sizeof(server);
	if(getsockname(sock, (sockaddr*)&server, &length))
	{
		perror("getting socket name");
		exit(1);
	}

	cout << "Socket has port #" << ntohs(server.sin_port) << "\n";

	//start accepting connections
	listen(sock, 5);
	do
	{
		//variables
		uint16_t sumBytes = 0;
		uint32_t countBytes = 0;
		struct sockaddr_in from;
		socklen_t from_len = sizeof(from);

		//attempt to accept client connection
		msgsock = accept(sock, (struct sockaddr*)&from, &from_len);

		if(msgsock == -1)
		{
			perror("accept");
			exit(1);
		}
		else
		{
			//convert ip4v
			inet_ntop(from.sin_family, &from.sin_addr, buf, sizeof(buf));

			cout << "Accepted connection from '" << buf << "', port " << ntohs(from.sin_port) << "\n";
		}

		do
		{
			//read
			if((rval = read(msgsock, buf, sizeof(buf) - 1)) < 0)
				perror("reading stream message");

			if(rval == 0)
				cout << "Ending connection\n";
			else
			{
				buf[rval] = '\0';

				//sum of each byte
				for(int i = 0; i < rval; i++)
					sumBytes += (uint8_t)buf[i];

				countBytes += rval; //count bytes
			}
		} while(rval != 0);

		//print sum and count of bytes
		std::ostringstream os;
		os << "Sum: " << sumBytes << " Len: " << countBytes << "\n";
		std::string str = os.str();
		const char *ch = str.c_str();

		//write to client
		if(safe_write(msgsock, ch, str.size()) < 0)
		{
			perror("Writing to client");
			exit(1);
		}

		close(msgsock); //close socket
	} while(true);

	return 0;
}
