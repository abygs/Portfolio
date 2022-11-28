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
/**
 * Allocate bytes in the mem vector and initialize every byte/element to 0xa5
 *
 * @param siz holds size to allocate mem vector
 ********************************************************************************/
memory::memory(uint32_t siz)
{
	//round the length up, mod-16
	siz = (siz+15)&0xfffffff0;

	//set vector size and initialize every element
	mem.resize(siz, 0xa5);
}

/**
 * destructor for clean up
 ********************************************************************************/
memory::~memory()
{
	//clear mem vector
	mem.clear();
}

/**
 * checks if given address is out of bounds
 *
 * @param addr holds the address
 *
 * @return true if the given address is out of bounds
 *
 * @note uses hex::to_hex0x32() function
 ********************************************************************************/
bool memory::check_illegal(uint32_t addr) const
{
	//check is given address is out of bounds
	if(addr >= mem.size())
	{
		std::cout << "WARNING: Address out of range: " << hex::to_hex0x32(addr) << "\n";
		return true; //illegal
	}
	return false; //legal
}

/**
 * rounds up a number of bytes
 *
 * @return the rounded up number of bytes in the simulated memory
 ********************************************************************************/
uint32_t memory::get_size() const
{
	return mem.size();
}

/**
 * gets 8-bit value from memory at a given address
 *
 * @param addr holds the given address
 *
 * @return the value of the byte from simulated memory at the given address if
 *      it is valid, else returns 0
 *
 * @note reads from mem vector and calls check_illegal()
 ********************************************************************************/
uint8_t memory::get8(uint32_t addr) const
{
	//check if addr is valid
	if(check_illegal(addr))
		return 0;
	else
		return mem.at(addr); //get bytes from memory at given address
}

/**
 * gets 16-bit value from memory at a given address
 *
 * @param addr holds the given address
 *
 * @return the unsigned 16-bit value
 *
 * @note calls get8() twice
 ********************************************************************************/
uint16_t memory::get16(uint32_t addr) const
{
	return (get8(addr) & 0xff) | (get8(addr+1) << 8);
}

/**
 * gets 32-bit value from memory at a given address
 *
 * @param addr holds the given address
 *
 * @return the unsigned 32-bit value
 *
 * @note calls get16() twice
 ********************************************************************************/
uint32_t memory::get32(uint32_t addr) const
{
	return (get16(addr) & 0xffff) | (get16(addr+2) << 16);
}

/**
 * gets the sign-extended value of 8-bits
 *
 * @param addr holds the given address
 *
 * @return the sign-extended value of the byte as a 32-bit signed integer
 *
 * @note calls get8()
 ********************************************************************************/
int32_t memory::get8_sx(uint32_t addr) const
{
	//get unsigned 8-bits
	int32_t tempAddr = get8(addr);

	//check if signed bit is negative, if yes, flip bits
	return tempAddr | (tempAddr & 0x00000080 ? 0xffffff00 : 0);
}

/**
 * gets the sign-exteneded value of 16 bits
 *
 * @param addr holds the given address
 *
 * @return the sign-extended value of the bytes as a 32-bit signed integer
 *
 * @note calls get16()
 ********************************************************************************/
int32_t memory::get16_sx(uint32_t addr)const
{
	//get unsigned 16-bits
	int32_t tempAddr = get16(addr);

	//check if signed bit is negative, if yes, flip bits
	return tempAddr | (tempAddr & 0x00008000 ? 0xffff0000 : 0);
}

/**
 * gets the sign-extended value of 32 bits
 *
 * @param addr holds the given address
 *
 * @return the sign-extended value of the bytes as a 32-bit signed integer
 *
 * @note calls get32()
 ********************************************************************************/
int32_t memory::get32_sx(uint32_t addr)const
{
	//gets the unsigned 32-bit and returns it in a signed data type
	return get32(addr);
}

/**
 * sets the given 8-bit value at the given address in memory
 *
 * @param addr holds the given address
 *
 * @param val holds the value to be added into memory
 *
 * @note writes to mem vector and calls check_illegal()
 ********************************************************************************/
void memory::set8(uint32_t addr, uint8_t val)
{
	//check is the address is valid
	if(check_illegal(addr))
		return;
	else
		mem.at(addr) = val; //write value to simulated memory at given address
}

/**
 * sets the given 16-bit value at the given address in memory
 *
 * @param addr holds the given address
 *
 * @param val holds the value to be added into memory
 *
 * @note calls set8() twice
 ********************************************************************************/
void memory::set16(uint32_t addr, uint16_t val)
{
	// sets first byte by incrementing the address location
	// and shifting off one byte from the value
	set8(addr + 1,val >> 8);
	set8(addr,val); //set the second bit
}

/**
 * sets the given 32-bit value at the given address in memory
 *
 * @param addr holds the given address
 *
 * @param val holds the value to be added into memory
 *
 * @note calls set16() twice
 ********************************************************************************/
void memory::set32(uint32_t addr, uint32_t val)
{
	// sets first two bytes by incrementing the address location
	// and shifting off two bytes from the value
	set16(addr+2,val >> 16);
	set16(addr,val); //set the second two bytes
}

/**
 * dumps the entire contents of the simulated memory in hex with ASCII characters
 *
 * @note calls hex class functions to_hex32() and to_hex8()
 ********************************************************************************/
void memory::dump() const
{
	//declare variables
	int counter = 0;
	std::string asciiValues = "";

	//loop through simulated memory
	for(uint32_t val : mem)
	{
		//print range of memory
		if(counter % 16 == 0)
			std::cout << hex::to_hex32(counter) << ": ";

		//print byte of memory
		std::cout << hex::to_hex8(val) << " ";

		//use counter to determine when to print spaces
		counter++;
		if(counter % 8 == 0 && counter % 16 != 0)
			std::cout << " ";

		//get ascii values and concatenate string
		uint8_t ch = val;
		ch = isprint(ch) ? ch : '.';
		asciiValues += ch;

		//use counter to determine when to print spaces
		if(counter % 16 == 0)
		{
			std::cout << "*" << asciiValues  << "*\n";
			asciiValues.clear();
		}
	}
}


/**
 * opens file in binary mode and reads the content into the simulated memory
 *
 * @param fname is the file to be read in
 *
 * @return false if the file could not be opened
 ********************************************************************************/
bool memory::load_file(const std::string &fname)
{
	//opening file in binary mode
	std::ifstream infile(fname, std::ios::in|std::ios::binary);

	if(infile) //check if file opened successfully
	{
		uint8_t i;

		//read file with no whitespace
		infile >> std::noskipws;

		//go through file byte by bytes
		for (uint32_t addr = 0; infile >> i; ++addr)
		{
			//check is byte is legal
			if(check_illegal(addr))
			{
				std::cerr << "Program too big.\n";
				infile.close();
				return false;
			}
			else //add to simulated memory
				mem.at(addr) = i;
		}
		infile.close();
		return true;
	}
	else //could not open file
	{
		std::cerr << "Can’t open file ’" << fname << "’ for reading.\n";
		return false;
	}
	return false;
}
