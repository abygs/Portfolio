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

#include "registerfile.h"
#include <string>

/**
 * constructor
 *
 * @note uses registerfile::reset to initialize the registers
 ********************************************************************************/
registerfile::registerfile()
{
	reset();
}


/**
 * Initializes register 0x to 0 and the rest of the registers to 0xf0f0f0f0
 ********************************************************************************/
void registerfile::reset()
{
	//set vector length to 32 and initialize each element to 0xf0f0f0f0
        regs.resize(32, 0xf0f0f0f0);

	//set the 0x register to 0
	regs.at(0) = 0;
}

/**
 * sets the given register to the given value
 *
 * @param r is the given register
 *
 * @param val is the given value to put in the register
 *
 * @note if the given register is 0x, do nothing
 ********************************************************************************/
void registerfile::set(uint32_t r, int32_t val)
{
	if(r > 0 && r < 32)
		regs.at(r) = val;
}

/**
 * gets the value of the given register
 *
 * @param r is the given register
 *
 * @return int32_t which is the value in the given register
 *
 * @note is the given register is 0x then return 0
 ********************************************************************************/
int32_t registerfile::get(uint32_t r) const
{
	if(r > 0 && r < 32)
		return regs.at(r);
	else if(r == 0)
		return 0;

	return -1; //if the register is invalid
}

/**
 * dumps the registers
 *
 * @param hdr is a string printed at the beginning of the each output line
 *
 * @note uses hex::to_hex32() to print the register values
 ********************************************************************************/
void registerfile::dump(const std::string &hdr) const
{
	int counter = 0;
	std::string xCounter;

	for(int32_t reg : regs)
	{
		//print the line header if needed
		if(counter % 8 == 0)
		{
			if(hdr != "")
				std::cout << hdr;

			xCounter = "x" + std::to_string(counter);
			std::cout << std::right << std::setw(3) << std::setfill(' ') << xCounter;
		}
                //print a register
                std::cout << " " << hex::to_hex32(reg);

                //use counter to determine when to print spaces
                counter++;
                if(counter % 4 == 0 && counter % 8 != 0)
                        std::cout << " ";

                //use counter to determine when to print newline
                if(counter % 8 == 0)
                        std::cout << "\n";
	}
}
