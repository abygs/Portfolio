/****************************************************************
   PROGRAM:   Assignment 4
   AUTHOR:    Abigail Shulgan
   ID:  z1893909
   DUE DATE:  3/30/22
****************************************************************/

#include "process.h"

//constructor
Process::Process()
{
	processName = "";
	processID = 0;
	sub = 0;
	CPUTimer = 0;
	ITimer = 0;
	OTimer = 0;
	CPUTotal = 0;
	ITotal = 0;
	OTotal = 0;
	CPUCount = 0;
	ICount = 0;
	OCount = 0;
	startTime = 0;
	waitTime = 0;
	hasTicked = false;
	terminated = false;
};
