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

#include "cpu_single_hart.h"

/**
 * starts the process to execute the instructions
 *
 * @param exec_limit holds the limit on how many instruction can execute
 *
 * @note calls tick
 ********************************************************************************/
void cpu_single_hart::run(uint64_t exec_limit)
{
	//set register x2 to memory size
	regs.set(2, mem.get_size());

	//call tick in a loop until halted
	while(exec_limit == 0 && !is_halted())
		tick();

	//call tick in a look until halted or execution limit is equal to instructions executed
	while(exec_limit != 0 && (!is_halted() && exec_limit != get_insn_counter()))
		tick();

	//if program is halted
	if(is_halted())
		std::cout << "Execution terminated. Reason: " << get_halt_reason() << "\n";

	//print how many instruction executed
	std::cout << get_insn_counter() << " instructions executed\n";
}
