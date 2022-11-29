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

//header guards
#ifndef REGISTERFILE_H
#define REGISTERFILE_H

//includes
#include "hex.h"
#include <vector>
#include <iostream>

class registerfile : public hex
{
	private:
		std::vector<int32_t> regs;

        public:
		registerfile();
		void reset();
		void set(uint32_t r, int32_t val);
		int32_t get(uint32_t r) const;
		void dump(const std::string &hdr) const;
};

#endif
