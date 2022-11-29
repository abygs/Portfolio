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

#include "hex.h"

/**
 * formats 8-bit integer into a 2-character hex string with a leading 0
 *
 * @param i holds an unsigned 8-bit integer
 *
 * @return string with exactly 2 hex digits representing the
 * 	8 bits of the i argument
 ********************************************************************************/
std::string hex::to_hex8(uint8_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i); //formatting the hex value
	return os.str();
}


/**
 * formats 32-bit integer into a 8-character hex string
 *
 * @param i holds an unsigned 32-bit integer
 *
 * @return string with exactly 8 hex digits representing the
 * 	32 bits of the i argument
 ********************************************************************************/
std::string hex::to_hex32(uint32_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(8) << i; //formatting the hex value
	return os.str();
}


/**
 * formats 32-bit integer into a 8-character hex string with leading 0x
 *
 * @param i holds an unsigned 32-bit integer
 *
 * @return string beginning with 0x, followed by exactly 8 hex digits
 *	representing the 32 bits of the i argument
 *
 * @note concatenates 0x to to_hex32()'s output
 ********************************************************************************/
std::string hex::to_hex0x32(uint32_t i)
{
	return std::string("0x")+to_hex32(i); //formatting the hex value
}

/**
 * formats 32-bit integer into a 5 digit hex string with leading 0x
 *
 * @param i holds an unsigned 32-bit integer
 *
 * @return string beginning with 0x, followed by exactly 5 hex digits
 *	representing the 20 bits of the i argument
 ********************************************************************************/
std::string hex::to_hex0x20(uint32_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(5) << (i); //formatting the hex value
	return "0x" + os.str();
}


/**
 * formats 32-bit integer into a 3 digit hex string with leading 0x
 *
 * @param i holds an unsigned 32-bit integer
 *
 * @return string beginning with 0x, followed by exactly 3 hex digits
 *	representing the 12 bits of the i argument
 ********************************************************************************/
std::string hex::to_hex0x12(uint32_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(3) << (i); //formatting the hex value
	return "0x" + os.str();
}
