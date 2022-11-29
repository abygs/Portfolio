/****************************************************************
   PROGRAM:   Assignment 5 - Part 1
   AUTHOR:    Abigail Shulgan
   ID:  z1893909
   DUE DATE:  4/16/22

   PURPOSE: Use pthreads to solve reader-writer problem

****************************************************************/

//includes
#include <iostream>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <cstdint>

//global variables
std::string sharedString = "All work and no play makes Jack a dull boy.";
int read_count;
sem_t rw_sem; //used by both readers and writers
sem_t cs_sem; //used for protecting critical sections of readers

//function prototypes
void *reader(void *threadId);
void *writer(void *threadId);


/****************************************************************
   FUNCTION: Creates reader/writer threads and calls necessary
			 functions.
****************************************************************/
int main(int argc, char **argv)
{
	//variables
	int numOfReaders;
	int numOfWriters;

	read_count = 0; //initalize to 0

	//get command line arguments into variables
	//get number of  reader threads
	if(argv[1] == NULL)
	{
		std::cerr << "Number of reader threads is missing\n";
		exit(1);
	}
	else
	{
		numOfReaders = atoi(argv[1]); //get value

		if(numOfReaders < 0) //check if value is not valid
		{
			std::cerr << "Number of reader threads must be greater than or equal to 0\n";
			exit(1);
		}
	}

	//get number of writer threads
	if(argv[2] == NULL)
	{
		std::cerr << "Number of writer threads is missing\n";
		exit(1);
	}
	else
	{
		numOfWriters = atoi(argv[2]); //get value

		if(numOfWriters < 0) //check if value is not valid
		{
			std::cerr << "Number of writer threads must be greater than or equal to 0\n";
			exit(1);
		}
	}

	//initalize semaphores
	if(sem_init(&rw_sem, 0, 1) != 0 || sem_init(&cs_sem, 0, 1) != 0) //initalize and check for failures
	{
		std::cerr << "Failed to initalize semaphore(s)\n";
		exit(1);
	}
	

	//print info
	printf("*** Reader-Writer Problem Simulation ***\n");
	printf("Number of reader threads: %i\n", numOfReaders);
	printf("Number of writer threads: %i\n", numOfWriters);

	//define threads
	pthread_t readers[numOfReaders];
	pthread_t writers[numOfWriters];

	//create reader threads
	for(int i = 0; i < numOfReaders; i++)
	{
		if(pthread_create(&readers[i], NULL, reader, (void *)(intptr_t) i) != 0) //create and check for failure
		{
			std::cerr << "Failed to create reader thread\n";
			exit(1);
		}
	}

	//create writer threads
	for(int i = 0; i < numOfWriters; i++)
	{
		if(pthread_create(&writers[i], NULL, writer, (void *)(intptr_t) i) != 0) //create and check for failure
		{
			std::cerr << "Failed to create writer thread\n";
			exit(1);
		}
	}

	//join reader threads
	for(int i = 0; i < numOfReaders; i++)
	{
		if(pthread_join(readers[i], NULL) != 0) //join and check for failure
		{
			std::cerr << "Failed to join reader threads\n";
			exit(1);
		}
	}

	//join writer threads
	for(int i = 0; i < numOfWriters; i++)
	{
		if(pthread_join(writers[i], NULL) != 0) //join and check for failure
		{
			std::cerr << "Failed to join writer threads\n";
			exit(1);
		}
	}


	printf("All threads are done.\n"); //finished

	//clean up time
	//destory semaphores
	if(sem_destroy(&rw_sem) != 0 || sem_destroy(&cs_sem))
	{
		std::cerr << "Failed to destory semaphore(s)\n";
		exit(1);
	}
	
	printf("Resources cleaned up.\n");

	return 0;
}

/****************************************************************
   FUNCTION: Reads from shared string
****************************************************************/
void *reader(void *threadId)
{
	//thread id
	long tid = (intptr_t)threadId;

	//loop until the string is empty
	while(sharedString != "")
	{
		//critical section wait and increment read_count
		sem_wait(&cs_sem);
		read_count++;

		//print and flush
		printf("read_count increments to: %i.\n", read_count);
		fflush(stdout);

		//wait if read count is one
		if(read_count == 1)
			sem_wait(&rw_sem);

		sem_post(&cs_sem); //signal critical section
		
		//read, print, and flush
		if(sharedString != "") //only if the string is not empty otherwise might cause segmentation fault
		{
			printf("reader %li is reading ... content : %s\n", tid, sharedString.c_str());
			fflush(stdout);
		}

		//critical section wait and decrement read_count
		sem_wait(&cs_sem);
		read_count--;

		//flush and print
		printf("read_count decrements to: %i.\n", read_count);
		fflush(stdout);

		//signal if read_count is zero
		if(read_count == 0)
			sem_post(&rw_sem);

		sem_post(&cs_sem); //signal critical seciton

		sleep(1); //sleep for 1 second
	}

	//print and exit
	printf("reader %li is exiting ...\n", tid);
	pthread_exit(NULL);

	return NULL;
}

/****************************************************************
   FUNCTION: writes to shared string
****************************************************************/
void *writer(void *threadId)
{
	//thread id
	long tid = (intptr_t)threadId;

	//loop until string is empty
	while(sharedString != "")
	{
		sem_wait(&rw_sem); //wait

		//write, print, and flush
		if(sharedString != "") //only if the string is not empty otherwise might cause segmentation fault
		{
			sharedString.pop_back(); //write (chop off last character)

			printf("writer %li is writing ...\n", tid);
			//fflush(stdout);
		}

		sem_post(&rw_sem); //signal
		
		//sleep for 1 second
		sleep(1);
	}

	//print and exit
	printf("writer %li is exiting ...\n", tid);
	pthread_exit(NULL);

	return NULL;
}

