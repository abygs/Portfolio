/****************************************************************
   PROGRAM:   Assignment 4
   AUTHOR:    Abigail Shulgan
   ID:  z1893909
   DUE DATE:  3/30/22
****************************************************************/

//header guards
#ifndef Z1893909_PROJECT4_H
#define Z1893909_PROJECT4_H

//includes
#include "process.h"
#include <iostream>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <deque>
#include <fstream>
#include <vector>

//constants
#define MAX_TIME 500 //length of the whole run
#define IN_USE 5 //maximum number of processes that can be in play at a time
#define HOW_OFTEN 25 //how often to reprint the state of the system

//global
int timer;
int idleTime;
int TotalProcesses;
bool idle;
int quantum;
int quantumTime;
bool isRoundRobin;


//function prototypes
void createProcesses(std::string);
bool isEmpty();
int totalProcesses();
void printStatus();
void terminate(Process* p);
void printSummary();
void loadEntryToReady();
void activeProcess();
void inputProcess();
void outputProcess();
void setHasTickedToFalse();

//deques
std::deque<Process*> Entry;
std::deque<Process*> Ready;
std::deque<Process*> Input;
std::deque<Process*> Output;
std::deque<Process*> Terminated;

//vector to hold processes that need hasTicked to be set to false
std::vector<Process*> setToFalse;

//pointers
Process* Active = NULL;
Process* IActive = NULL;
Process* OActive = NULL;

#endif
