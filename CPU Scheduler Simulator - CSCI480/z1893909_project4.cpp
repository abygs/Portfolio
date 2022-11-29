/****************************************************************
   PROGRAM:   Assignment 4
   AUTHOR:    Abigail Shulgan
   ID:  z1893909
   DUE DATE:  3/30/22

   PURPOSE: simulate CPU scheduler for FCFS and Round Robin

****************************************************************/

#include "z1893909_project4.h"

/****************************************************************
   FUNCTION: Calls necessary functions to create processes from
			 input file then schedule the processes.
****************************************************************/
int main(int argc, char **argv)
{
	//get input file name
	if(argv[1] == NULL)
	{
		std::cerr << "Input file name missing\n";
		exit(1);
	}

	//initalize variables~
	timer = 0;
	idle = false;
	quantum = 5;

	//get quantum if it is set
	if(argv[2] != NULL)
		quantum = atoi(argv[2]);

	//check if this is going to be a round robin run
	if(quantum < 100)
		isRoundRobin = true;
	else
		isRoundRobin = false;

	//call function to create processes and insert them into the entry deque
	createProcesses(argv[1]);

	std::cout << "Simulation of CPU Scheduling\n\n\n";

	//while timer is less than max time and there are still processes in the queues
	while(timer < MAX_TIME && !isEmpty())
	{
		//load entry to ready
		if(Active == NULL && Ready.empty() && totalProcesses() < IN_USE && !Entry.empty())
		{
			//look for active in ready queue
			loadEntryToReady();
		}

		//determine if a status needs printing
		if(timer % HOW_OFTEN == 0)
		{
			std::cout << "Status at time " << timer << "\n";
			printStatus();
		}

		//check on active CPU burst processes
		activeProcess();

		//check on input processess
		inputProcess();

		//check on output processess
		outputProcess();

		//reset boolean to false
		setHasTickedToFalse();		

		//increment timer
		timer++;

		//terminate any processes that finished in the last tick
		for(size_t i = 0; i < Terminated.size(); i++)
			if(Terminated.at(i)->terminated == false)
				terminate(Terminated.at(i));
	}

	//print summary data
	printSummary();

	return 0;
}

/****************************************************************
   FUNCTION: reads input file, creates all processes, and loads
			 them into the Entry queue
****************************************************************/
void createProcesses(std::string fileName)
{
	//variables
	int processID = 101;
	int value;
	char letter;
	std::string processName;

	//read file input
	std::ifstream infile(fileName);

	//if file opens successfully
	if(infile)
	{
		while(!infile.eof()) //while not end of file
		{
			//get process name
			infile >> processName;

			//check if stop
			if(processName == "STOPHERE")
				break;

			//create process
			Process *p = new Process();

			//continue reading and setting process variables
			infile >> p->arrivalTime;
			p->processName = processName;
			p->processID = processID;
			processID++;

			//set history pairs
			for(int i = 0; i < 10; i++)
			{
				infile >> letter >> value;
				p->history.push_back(std::make_pair(letter, value));
			}

			//insert process into entry queue
			Entry.push_back(p);
		}
		//close file
		infile.close();
	}
	else
	{
		//error
		std::cerr << "Failed to open file: " << fileName << "\n";
		exit(1);
	}
}

/****************************************************************
   FUNCTION: returns true is all queues are empty and all
			 pointers are null
****************************************************************/
bool isEmpty()
{
	//check if all queues are empty and all process pointers are NULL
	if(Entry.empty() && Ready.empty() && Input.empty() && Output.empty() && Active == NULL && IActive == NULL && OActive == NULL)
		return true;

	return false;
}


/****************************************************************
   FUNCTION: counts up all processes in queues and pointers
****************************************************************/
int totalProcesses()
{
	int total = 0; //variable to hold total processes

	//check if any processes are being pointed at
	if(Active != NULL)
		total++;
	if(IActive != NULL)
		total++;
	if(OActive != NULL)
		total++;

	//get any processes in queues
	total += Ready.size();
	total += Input.size();
	total += Output.size();

	//return total processes in play
	return total;
}

/****************************************************************
   FUNCTION: prints necessary information for a status update
****************************************************************/
void printStatus()
{
	//print process pointers
	std::cout << "Active is " << (Active == NULL ? 0 : Active->processID) << "\n";
	std::cout << "IActive is " << (IActive == NULL ? 0 : IActive->processID) << "\n";
	std::cout << "OActive is " << (OActive == NULL ? 0 : OActive->processID) << "\n";

	//print entry queue
	std::cout << "Contents of the Entry Queue:\n";
	if(!Entry.empty())
	{
		for(size_t i = 0; i < Entry.size(); i++)
			std::cout << Entry.at(i)->processID << "    ";
		std::cout << "\n";
	}
	else
		std::cout << "(Empty)\n";

	//print ready queue
	std::cout << "Contents of the Ready Queue:\n";
	if(!Ready.empty())
	{
		for(size_t i = 0; i < Ready.size(); i++)
			std::cout << Ready.at(i)->processID << "    ";
		std::cout << "\n";
	}
	else
		std::cout << "(Empty)\n";

	//print input queue
	std::cout << "Contents of the Input Queue:\n";
	if(!Input.empty())
	{
		for(size_t i = 0; i < Input.size(); i++)
			std::cout << Input.at(i)->processID << "    ";
		std::cout << "\n";
	}
	else
		std::cout << "(Empty)\n";

	//print output queue
	std::cout << "Contents of the Output Queue:\n";
	if(!Output.empty())
	{
		for(size_t i = 0; i < Output.size(); i++)
			std::cout << Output.at(i)->processID << "    ";
		std::cout << "\n";
	}
	else
		std::cout << "(Empty)\n";

	std::cout << "\n\n";
}

/****************************************************************
   FUNCTION: prints terminating information about a process
			 when it terminates
****************************************************************/
void terminate(Process* termed)
{
	termed->waitTime = timer - termed->startTime - termed->CPUTotal - termed->ITotal - termed->OTotal; //add + 1 because ticks start when timer = 0

	std::cout << "Process " << termed->processID << " has ended.\n";
	std::cout << "Name: " << termed->processName << "\n";
	std::cout << "Started at time  " << termed->startTime << " and ended at time " << timer << "\n";
	std::cout << "Total CPU time = " << termed->CPUTotal << " in " << termed->CPUCount << " CPU burst" << (termed->CPUCount > 1 ? "s": "") << "\n";
	std::cout << "Total Input time = " << termed->ITotal << " in " << termed->ICount << " Input burst" << (termed->ICount > 1 ? "s": "") << "\n";
	std::cout << "Total Output time = " << termed->OTotal << " in " << termed->OCount << " Output burst" << (termed->OCount > 1 ? "s": "") << "\n";
	std::cout << "Time spent in waiting: " << termed->waitTime << "\n\n\n";

	termed->terminated = true;
}

/****************************************************************
   FUNCTION: prints summary at end of entire run
****************************************************************/
void printSummary()
{
	//get average waiting time
	int totalWait = 0;
	
	for(size_t i = 0; i < Terminated.size(); i++)
	{
		totalWait += Terminated.at(i)->waitTime;
	}

	std::cout << "The run has ended.\n";
	std::cout << "The final value of the timer was: " << timer << "\n";
	std::cout << "The amount of time spent idle was: " << idleTime << "\n";
	std::cout << "Number of terminated processes = " << Terminated.size() << "\n";
	std::cout << "Average waiting time for all terminated processes = " << totalWait/Terminated.size() << "\n";

	printStatus();
	exit(0);
}

/****************************************************************
   FUNCTION: loads processes from entry queue into ready queue
****************************************************************/
void loadEntryToReady()
{
	for(size_t i = 0; i < Entry.size();)
	{
		if(timer >= Entry.at(i)->arrivalTime && totalProcesses() < IN_USE)
		{
			//Add to ready queue and pop out of entry queue
			std::cout << "Process " << Entry.at(i)->processID << " moved from the Entry Queue into the Ready Queue at time " << timer << "\n\n\n";

			Entry.front()->startTime = timer;
			Ready.push_back(Entry.front());
			Entry.pop_front();
		} else
			i++;
	}
}

/****************************************************************
   FUNCTION: lets all hasTicked variables to false
****************************************************************/
void setHasTickedToFalse()
{
	for(size_t i = 0; i < setToFalse.size(); i++)
	{
		setToFalse.at(i)->hasTicked = false;
	}
	setToFalse.clear();

	if(Active != NULL)
		Active->hasTicked = false;
	if(IActive != NULL)
		IActive->hasTicked = false;
	if(OActive != NULL)
		OActive->hasTicked = false;
}

/****************************************************************
   FUNCTION: checks if active process is null and runs
			 necessary commands to set a new active process if
			 needed. Then continues processing if there is an
			 active process already.
****************************************************************/
void activeProcess()
{
	//check active
	if(Active == NULL) //no active process
	{
		//Look for next one to make active in ready queue
		if(!Ready.empty())
		{
			Active = Ready.front();
			Ready.pop_front();
			Active->CPUTimer = Active->history.at(Active->sub).second;
			idle = false;
		}
		else
		{
			//idle time
			if(idle == false)
				std::cout << "At time " << timer << ", Active is 0, so we have idle time for a while\n\n\n";

			//increment idle counter
			idleTime++;
			idle = true; //begin idle time
		}
	}

	//handle if there is an active process being pointed at
	if(Active != NULL)
	{
		if(isRoundRobin)
		{
			//if the active process isn't done yet
			if(Active->history.at(Active->sub).second > 0 && Active->hasTicked == false && quantumTime < quantum) //round robin
			{
				Active->CPUTotal++;
				Active->history.at(Active->sub).second--;
				quantumTime++;
				Active->hasTicked = true;				
			}

			//if process is done
			if(Active->history.at(Active->sub).second == 0)
			{
				//reset quantum time
				quantumTime = 0;

				//finished
				Active->sub++; //increment subscript for history
				Active->CPUCount++; //increment cpu count

				//get the type of the next burst (I, O, N)
				char type = Active->history.at(Active->sub).first;

				//put into necessary queue
				if(type == 'N') //terminate process
					Terminated.push_back(Active);
				else if(type == 'I') //add to input queue
					Input.push_back(Active);
				else if(type == 'O') //add to output queue
					Output.push_back(Active);

				//to reset hasTicked variable
				if(Active->hasTicked == true)
					setToFalse.push_back(Active);

				//remove active pointer
				Active = NULL;
			}
			else if(quantumTime == quantum)
			{
				//reset timer countdown
				quantumTime = 0;

				//put process back into ready even if its unfinished
				Ready.push_back(Active);

				//for resetting HasTicked boolean
				if(Active->hasTicked == true)
					setToFalse.push_back(Active);

				//remove active pointer
				Active = NULL;
			}
		}
		else //FCFS
		{
			if(Active->CPUTimer > 0 && Active->hasTicked == false) //FCFS
			{
				Active->CPUTotal++;
				Active->CPUTimer--;
				Active->hasTicked = true;				
			}

			//if process is done
			if(Active->CPUTimer == 0)
			{
				//finished
				Active->sub++; //increment subscript for history
				Active->CPUCount++; //increment cpu count

				//get the type of the next burst (I, O, N)
				char type = Active->history.at(Active->sub).first;

				//put into necessary queue
				if(type == 'N') //terminate process
					Terminated.push_back(Active);
				else if(type == 'I') //add to input queue
					Input.push_back(Active);
				else if(type == 'O') //add to output queue
					Output.push_back(Active);

				//to reset hasTicked variable
				if(Active->hasTicked == true)
					setToFalse.push_back(Active);

				//remove active pointer
				Active = NULL;
			}
		}
	}
}

/****************************************************************
   FUNCTION: checks if input process is null and runs
			 necessary commands to set a new input process if
			 needed. Then continues processing if there is an
			 input process already.
****************************************************************/
void inputProcess()
{
	//check IActive
	if(IActive == NULL) //no IActive process
	{
		//Look for next one to make IActive in Input queue
		if(!Input.empty())
		{
			IActive = Input.front();
			Input.pop_front();
			IActive->ITimer = IActive->history.at(IActive->sub).second;
		}
	}

	//handle IActive
	if(IActive != NULL)
	{
		//continue processing
		if(IActive->ITimer > 0 && IActive->hasTicked == false)
		{
			IActive->ITotal++; //increment input total
			IActive->ITimer--; //decrement burst
			IActive->hasTicked = true;
		}

		//finished
		if(IActive->ITimer == 0) 
		{
			IActive->sub++; //increment subscript for history
			IActive->ICount++;

			//add to ready queue
			Ready.push_back(IActive);

			//to reset hasTicked variable
			if(IActive->hasTicked == true)
				setToFalse.push_back(IActive);

			//remove IActive pointer
			IActive = NULL;
		}
	}	
}

/****************************************************************
   FUNCTION: checks if output process is null and runs
			 necessary commands to set a new output process if
			 needed. Then continues processing if there is an
			 output process already.
****************************************************************/
void outputProcess()
{
	//check OActive
	if(OActive == NULL) //no OActive process
	{
		//Look for next one to make OActive in Output queue
		if(!Output.empty())
		{	
			OActive = Output.front();
			Output.pop_front();
			OActive->OTimer = OActive->history.at(OActive->sub).second;
		}
	}

	//handle OActive
	if(OActive != NULL)
	{
		//continue process
		if(OActive->OTimer > 0 && OActive->hasTicked == false)
		{
			OActive->OTotal++; //increment output total
			OActive->OTimer--; //decrement burst
			OActive->hasTicked = true;
		}

		//finished
		if(OActive->OTimer == 0)
		{
			OActive->sub++; //increment subscript for history
			OActive->OCount++;

			//add to ready queue
			Ready.push_back(OActive);

			//to reset hasTicked variable
			if(OActive->hasTicked == true)
				setToFalse.push_back(OActive);

			//set Oactive pointer to NULL
			OActive = NULL;
		}
	}
}