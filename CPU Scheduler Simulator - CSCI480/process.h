/****************************************************************
   PROGRAM:   Assignment 4
   AUTHOR:    Abigail Shulgan
   ID:  z1893909
   DUE DATE:  3/30/22
****************************************************************/

//header guards
#ifndef PROCESS_H
#define PROCESS_H

//includes
#include <string>
#include <vector>
#include <utility>

//Process structure 
struct Process
{
	Process();
	std::string processName;
	int processID;
	std::vector< std::pair<char, int> > history;
	int sub;
	int CPUTimer;
	int ITimer;
	int OTimer;
	int CPUTotal;
	int ITotal;
	int OTotal;
	int CPUCount;
	int ICount;
	int OCount;
	int arrivalTime;
	int startTime;
	int waitTime;
	bool hasTicked;
	bool terminated;
};

#endif
