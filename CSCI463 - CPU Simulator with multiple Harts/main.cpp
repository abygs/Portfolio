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

#include "memory.h"
#include "rv32i_decode.h"
#include "registerfile.h"
#include "rv32i_hart.h"
#include "cpu_single_hart.h"
#include <getopt.h>

using std::cout;
using std::cerr;
using std::endl;

/**
 * prints how to use the program
 ********************************************************************************/
static void usage()
{
	cerr << "Usage: rv32i [-d] [-i] [-r] [-z] [-l exec-limit] [-m hex-mem-size] infile" << endl;
	cerr << "    -d show disassembly before program execution" << endl;
	cerr << "    -i show instruction printing during execution" << endl;
	cerr << "    -l maximum number of instructions to exec" << endl;
	cerr << "    -m specify memory size (default = 0x100)" << endl;
	cerr << "    -r show register printing during execution" << endl;
	cerr << "    -z show a dump of the regs & memory after simulation" << endl;
	exit(1);
}

/**
 * Prints the memory address, instruction hex value, and the instruction
 * mnemonic for each 32-bit word in simulated memory
 *
 * @param memory holds the simulated memory
 *
 * @note calls rv32i_decode::decode()
 ********************************************************************************/
static void disassemble(const memory &mem)
{
	//iterating through memory
	for(unsigned int i = 0; i < mem.get_size(); i+=4)
	{
		cout << hex::to_hex32(i) << ": ";
	        cout << hex::to_hex32(mem.get32(i)) << "  ";

		cout << rv32i_decode::decode(i, mem.get32(i)) << "\n"; //pass addr and insn
	}
}

/**
 * starts the program's process and calls various functions
 *
 * @param argc is the command line argument count
 *
 * @param argv is a vector that holds any arguments passed in the command line
 ********************************************************************************/
int main(int argc, char **argv)
{
	uint32_t memory_limit = 0x100;	// default memory size is 0x100 (256 bytes)
	uint64_t exec_limit = 0; //run forever
	bool show_disassembly = false;
	bool show_instructions = false;
	bool show_registers = false;
	bool show_dump = false;
	int opt;

	while ((opt = getopt(argc, argv, "dil:m:rz")) != -1)
	{
		switch(opt)
		{
			case 'd':
				show_disassembly = true;
				break;
			case 'i':
				show_instructions = true;
				break;
			case 'l':
				{
					std::istringstream iss(optarg);
					iss >> exec_limit;
				}
				break;
			case 'm':
				{
					std::istringstream iss(optarg);
					iss >> std::hex >> memory_limit;
				}
				break;
			case 'r':
				show_registers = true;
				break;
			case 'z':
				show_dump = true;
				break;
			default: /* '?' */
				usage();
		}
	}

	if(optind >= argc)
		usage();	// missing filename

	memory mem(memory_limit);

	if(!mem.load_file(argv[optind]))
		usage();

	//show disassembly if option was selected
	if(show_disassembly)
		disassemble(mem);

	cpu_single_hart cpu(mem);
	cpu.reset();


	//show instructions if option was selected
	cpu.set_show_instructions(show_instructions);

	//show registers if option was selected
	cpu.set_show_registers(show_registers);

	//execute instructions
	cpu.run(exec_limit);

	//show dumps if option was selected
	if(show_dump)
	{
		cpu.dump();
		mem.dump();
	}

	return 0;
}
