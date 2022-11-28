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

#include "rv32i_decode.h"

/**
 * determines how to handle the given instruction through a series of switches
 *
 * @param addr holds the given address in simulated memory
 *
 * @param insn holds the given instruction
 *
 * @return string containing the disassembled instruction text
 *
 * @note passes insn to get_opcode()
 ********************************************************************************/
std::string rv32i_decode::decode(uint32_t addr, uint32_t insn)
{
	//get funct3 and funct7
	uint32_t funct3 = get_funct3(insn);
	uint32_t funct7 = get_funct7(insn);

	//use a switch state with the value of the opcode field by extracting it from insn by calling get_opcode(insn)
	switch(get_opcode(insn))
	{
		default: return render_illegal_insn(insn); //default for any unimplemented instructions

		//U-Type instructions
		case opcode_lui: return render_lui(insn); //lui instruction
		case opcode_auipc: return render_auipc(insn); //auipc instruction

		//J-Type instructions
		case opcode_jal: return render_jal(addr, insn); //jal instruction

		//B-Type instructions
		case opcode_btype:
			switch(funct3)
			{
				default: return render_illegal_insn(insn); //default for any unimplemented instructions
				case funct3_beq: return render_btype(addr, insn, "beq"); //beq instruction
				case funct3_bne: return render_btype(addr, insn, "bne"); //bne instruction
				case funct3_blt: return render_btype(addr, insn, "blt"); //blt instruction
				case funct3_bge: return render_btype(addr, insn, "bge"); //bge instruction
				case funct3_bltu: return render_btype(addr, insn, "bltu"); //bltu instruction
				case funct3_bgeu: return render_btype(addr, insn, "bgeu"); //bgeu instruction
			}
			assert(0 && "unrecognized funct3"); //impossible

		//S-Type instructions
		case opcode_stype:
			switch(funct3)
			{
				default: return render_illegal_insn(insn); //default for any unimplemented instructions
				case funct3_sb: return render_stype(insn, "sb"); //sb instruction
				case funct3_sh: return render_stype(insn, "sh"); //sh instruction
				case funct3_sw: return render_stype(insn, "sw"); //sw instruction
			}
			assert(0 && "unrecognized funct3"); //impossible


		//R-Type instructions
		case opcode_rtype:
			switch(funct3)
			{
				default: return render_illegal_insn(insn); //default for any unimplemented instructions
				case funct3_sll: return  render_rtype(insn, "sll"); //sll instruction
				case funct3_slt: return  render_rtype(insn, "slt"); //slt instruction
				case funct3_sltu: return  render_rtype(insn, "sltu"); //sltu instruction
				case funct3_xor: return  render_rtype(insn, "xor"); //xor instruction
				case funct3_or: return  render_rtype(insn, "or"); //or instruction
				case funct3_and: return  render_rtype(insn, "and"); //and instruction
				case funct3_srx:
					switch(funct7)
					{
						default: return render_illegal_insn(insn); //default for any unimplemented instructions
						case funct7_srl: return render_rtype(insn, "srl"); //srl instruction
						case funct7_sra: return render_rtype(insn, "sra"); //sra instruction
					}
					assert(0 && "unrecognized funct7"); //impossible
				case funct3_add:
					switch(funct7)
					{
						default: return render_illegal_insn(insn); //default for any unimplemented instructions
						case funct7_add: return render_rtype(insn, "add"); //add instruction
						case funct7_sub: return render_rtype(insn, "sub"); //sub instruction
					}
					assert(0 && "unrecognized funct7"); //impossible

			}
			assert(0 && "unrecognized funct3"); //impossible

		//I-Type instructions
		case opcode_jalr: return render_jalr(insn); //jalr instruction

		//Load
		case opcode_load_imm:
			switch(funct3)
			{
				default: return render_illegal_insn(insn); //default for any unimplemented instructions
				case funct3_lb: return render_itype_load(insn, "lb"); //lb instruction
				case funct3_lh: return render_itype_load(insn, "lh"); //lh instruction
				case funct3_lw: return render_itype_load(insn, "lw"); //lw instruction
				case funct3_lbu: return render_itype_load(insn, "lbu"); //lbu instruction
				case funct3_lhu: return render_itype_load(insn, "lhu"); //lhu instruction
			}
			assert(0 && "unrecognized funct3"); //impossible

		//ALU
		case opcode_alu_imm:
			switch(funct3)
			{
				default: return render_illegal_insn(insn); //default for any unimplemented instructions
				case funct3_sll: return render_itype_alu(insn, "slli", get_imm_i(insn) % XLEN); //slli instruction
				case funct3_slt: return render_itype_alu(insn, "slti", get_imm_i(insn)); //slti instruction
				case funct3_sltu: return render_itype_alu(insn, "sltiu", get_imm_i(insn)); //sltiu instruction
				case funct3_xor: return render_itype_alu(insn, "xori", get_imm_i(insn)); //xori instruction
				case funct3_or: return render_itype_alu(insn, "ori", get_imm_i(insn)); //ori instruction
				case funct3_and: return render_itype_alu(insn, "andi", get_imm_i(insn)); //andi instruction
				case funct3_add: return render_itype_alu(insn, "addi", get_imm_i(insn)); //addi instruction
				case funct3_srx:
					switch(funct7)
					{
						default: return render_illegal_insn(insn); //default for any unimplemented instructions
						case funct7_srl: return render_itype_alu(insn, "srli", get_imm_i(insn) % XLEN); //slri instruction
						case funct7_sra: return render_itype_alu(insn, "srai", get_imm_i(insn) % XLEN); //srai instruction
					}
					assert(0 && "unrecognized funct7"); //impossible
			}
			assert(0 && "unrecognized funct3"); //impossible

		case opcode_system:
			if(insn == insn_ebreak)
				return render_ebreak(insn);
			if(insn == insn_ecall)
				return render_ecall(insn);
			switch(funct3)
			{
				default: return render_illegal_insn(insn); //default for any unimplemented instructions
				case funct3_csrrw: return render_csrrx(insn, "csrrw"); //csrrw instruction
				case funct3_csrrs: return render_csrrx(insn, "csrrs"); //csrrs instruction
				case funct3_csrrc: return render_csrrx(insn, "csrrc"); //csrrc instruction
				case funct3_csrrwi: return render_csrrxi(insn, "csrrwi"); //csrrwi instruction
				case funct3_csrrsi: return render_csrrxi(insn, "csrrsi"); //csrrsi instruction
				case funct3_csrrci: return render_csrrxi(insn, "csrrci"); //csrrci instruction
			}
			assert(0 && "unrecognized funct3"); //impossible

	}
	assert(0 && "unrecognized opcode"); // It should be impossible to ever get here!
}


//*****Render_x functions*****

/**
 * Renders illegal instructions
 *
 * @param insn holds the given instruction
 *
 * @return string about the error
 ********************************************************************************/
std::string rv32i_decode::render_illegal_insn(uint32_t insn)
{
	(void)insn; //prevent an unused variable error
	return "ERROR: UNIMPLEMENTED INSTRUCTION";
}

/**
 * render the name of the register with a given number
 *
 * @param r holds the given number of the register
 *
 * @return string holding the formatted register name
 *
 * @note this is the only code that will format the name of a register
 ********************************************************************************/
std::string rv32i_decode::render_reg(int r)
{
	//concatenate an x to the register
	return "x" + std::to_string(r);
}

/**
 * render the operands of the form imm(register)
 *
 * @param reg holds the base register
 *
 * @param imm holds the displacement
 *
 * @return string holding the formatted imm(reg)
 *
 * @note this is the only code that will format an imm(reg) operand
 ********************************************************************************/
std::string rv32i_decode::render_base_disp(uint32_t reg, int32_t imm)
{
	//formatting
	return std::to_string(imm) + "(" + render_reg(reg) + ")";
}

/**
 * render the mnemonic
 *
 * @param mnemonic holds the instruction's mnemonic
 *
 * @return string holding the formatted mnemonic
 ********************************************************************************/
std::string rv32i_decode::render_mnemonic(const std::string &mnemonic)
{
	//preventing any space padding for ecall and ebreak
	if(mnemonic == "ecall" || mnemonic == "ebreak")
		return mnemonic;

	//format the mnemonic
	std::ostringstream os;
	os << std:: left << std::setw(mnemonic_width) << mnemonic;
	return os.str();
}

/**
 * render the lui instruction
 *
 * @param insn holds the given instruction
 *
 * @return string holding the formatted instruction and its operands
 ********************************************************************************/
std::string rv32i_decode::render_lui(uint32_t insn)
{
	//call functions to get necessary information to format the function
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);

	//format instruction: lui  rd,imm
	std::ostringstream os;
	os << render_mnemonic("lui") << render_reg(rd) << "," << hex::to_hex0x20((imm_u >> 12)&0x0fffff);
	return os.str();
}

/**
 * render the auipc instruction
 *
 * @param insn holds the given instruction
 *
 * @return string holding the formatted instruction and its operands
 ********************************************************************************/
std::string rv32i_decode::render_auipc(uint32_t insn)
{
	//call functions to get necessary information to format the function
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn); //U-Type
	std::ostringstream os;

	//format instruction: auipc  rd,imm
	os << render_mnemonic("auipc") << render_reg(rd) << "," << hex::to_hex0x20((imm_u >> 12)&0x0fffff);
	return os.str();

}

/**
 * render the jal instruction
 *
 * @param addr holds the given address in simulated memory
 *
 * @param insn holds the given instruction
 *
 * @return string holding the formatted instruction and its operands
 ********************************************************************************/
std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn)
{
        //call functions to get necessary information to format the function
        uint32_t rd = get_rd(insn);
        int32_t imm_j = get_imm_j(insn); //J-type
	int32_t pcrel_21 = imm_j + addr;
        std::ostringstream os;

	//format instruction: jal  rd,pcrel_21
        os << render_mnemonic("jal") << render_reg(rd) << "," << hex::to_hex0x32(pcrel_21);
        return os.str();
}

/**
 * render the jalr instruction
 *
 * @param insn holds the given instruction
 *
 * @return string holding the formatted instruction and its operands
 ********************************************************************************/
std::string rv32i_decode::render_jalr(uint32_t insn)
{
        //call functions to get necessary information to format the function
        uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
        int32_t imm_i = get_imm_i(insn); //I-type
        std::ostringstream os;

	//format instruction: jalr  rd,imm(rs1)
        os << render_mnemonic("jalr") << render_reg(rd) << "," << render_base_disp(rs1, imm_i);
        return os.str();
}


/**
 * render J-Type instructions
 *
 * @param insn holds the given instruction
 *
 * @param mnemonic holds the instruction mnemonic
 *
 * @return string holding the formatted instruction and its operands
 ********************************************************************************/
std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic)
{
        //call functions to get necessary information to format the function
	uint32_t rs1 = get_rs1(insn);
        uint32_t rs2 = get_rs2(insn);
        int32_t imm_s = get_imm_s(insn); //S-type
        std::ostringstream os;

	//format instruction: rs2,imm(rs1)
        os << render_mnemonic(mnemonic) << render_reg(rs2) << "," << render_base_disp(rs1, imm_s);
        return os.str();
}

/**
 * render B-Type instructions
 *
 * @param insn holds the given instruction
 *
 * @param mnemonic holds the instruction mnemonic
 *
 * @return string holding the formatted instruction and its operands
 ********************************************************************************/
std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char *mnemonic)
{
        //call functions to get necessary information to format the function
	uint32_t rs1 = get_rs1(insn);
        uint32_t rs2 = get_rs2(insn);
        int32_t pcrel_13 = get_imm_b(insn) + addr;
        std::ostringstream os;

	//format instruction: rs1,rs2,pcrel_13
        os << render_mnemonic(mnemonic) << render_reg(rs1) <<  "," << render_reg(rs2) << "," << hex::to_hex0x32(pcrel_13);
        return os.str();
}


/**
 * render R-Type instructions
 *
 * @param insn holds the given instruction
 *
 * @param mnemonic holds the instruction mnemonic
 *
 * @return string holding the formatted instruction and its operands
 ********************************************************************************/
std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic)
{
        //call functions to get necessary information to format the function
        uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
        std::ostringstream os;

	//format instruction: rd,rs1,rs2
        os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << render_reg(rs2);
        return os.str();

}


/**
 * render I-Type load instructions: lb, lh, lw, lbu, lhu
 *
 * @param insn holds the given instruction
 *
 * @param mnemonic holds the instruction mnemonic
 *
 * @return string holding the formatted instruction and its operands
 ********************************************************************************/
std::string rv32i_decode::render_itype_load(uint32_t insn, const char *mnemonic)
{
        //call functions to get necessary information to format the function
        uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
        int32_t imm_i = get_imm_i(insn); //I-type
        std::ostringstream os;

	//format instruction: rd,imm(rs1)
        os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_base_disp(rs1, imm_i);
        return os.str();
}


/**
 * render I-Type alu instructions: addi, slti, sltiu, xori, ori, andi, slli, srli, srai
 *
 * @param insn holds the given instruction
 *
 * @param mnemonic holds the instruction mnemonic
 *
 * @return string holding the formatted instruction and its operands
 ********************************************************************************/
std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i)
{
        //call functions to get necessary information to format the function
        uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
        std::ostringstream os;

	//format instruction: rd,rs1,imm OR rs,rs1,shamt
        os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << imm_i;
        return os.str();
}

/**
 * render ebreak
 *
 * @param insn holds the given instruction
 *
 * @return string holding the formatted instruction and its operands
 ********************************************************************************/
std::string rv32i_decode::render_ebreak(uint32_t insn)
{
	(void)insn; //prevent unused variable error

        std::ostringstream os;
        os << render_mnemonic("ebreak");
        return os.str();
}

/**
 * render ecall
 *
 * @param insn holds the given instruction
 *
 * @return string holding the formatted instruction and its operands
 ********************************************************************************/
std::string rv32i_decode::render_ecall(uint32_t insn)
{
	(void)insn; //prevent unused variable error

        std::ostringstream os;
        os << render_mnemonic("ecall");
        return os.str();
}

/**
 * render I-type csrrx instrucitons
 *
 * @param insn holds the given instruction
 *
 * @return string holding the formatted instruction and its operands
 ********************************************************************************/
std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic)
{
        uint32_t rd = get_rd(insn);
        uint32_t csr = get_imm_i(insn) & 0x00000fff; //I-type
	uint32_t rs1 = get_rs1(insn);
        std::ostringstream os;

	//format instruction: rd,csr,rs1
        os << render_mnemonic(mnemonic) << render_reg(rd) << "," << hex::to_hex0x12(csr) << "," << render_reg(rs1);
        return os.str();
}

/**
 * render I-type csrrxi instructions
 *
 * @param insn holds the given instruction
 *
 * @return string holding the formatted instruction and its operands
 ********************************************************************************/
std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic)
{
        uint32_t rd = get_rd(insn);
        uint32_t csr = get_imm_i(insn) & 0x00000fff; //I-type
	int32_t zimm = get_rs1(insn);
        std::ostringstream os;

	//format instruction: rd,csr,zimm
        os << render_mnemonic(mnemonic) <<  render_reg(rd) << "," << hex::to_hex0x12(csr) << "," << zimm;
        return os.str();
}


//*****Get_x functions*****

/**
 * extracts the opcode from the given instruction
 *
 * @param insn holds the given instruction
 *
 * @return uint32_t which is the extracted opcode
 *
 * @note
 ********************************************************************************/
uint32_t rv32i_decode::get_opcode(uint32_t insn)
{
	return (insn & 0x0000007f);
}

/**
 * extracts the rd field from the given instruction
 *
 * @param insn holds the given instruction
 *
 * @return uint32_t which is the extracted rd value
 *
 * @note the rd field is an integer value from 0 to 31
 ********************************************************************************/
uint32_t rv32i_decode::get_rd(uint32_t insn)
{
	return(insn & 0x00000ff8) >> 7;
}

/**
 * extracts the funct3 field from the given instruction
 *
 * @param insn holds the given instruction
 *
 * @return uint32_t which is the extracted funct3 value
 *
 * @note the funct3 field is an integer value from 0 to 7
 ********************************************************************************/
uint32_t rv32i_decode::get_funct3(uint32_t insn)
{
	return(insn & 0x00007000) >> 12;
}

/**
 * extracts the rs1 field from the given instruction
 *
 * @param insn holds the given instruction
 *
 * @return uint32_t which is the extracted rs1 value
 *
 * @note the rs1 field is an integer value from 0 to 31
 ********************************************************************************/
uint32_t rv32i_decode::get_rs1(uint32_t insn)
{
	return(insn >> 15) & 0x0000001f;
}

/**
 * extracts the rs2 field from the given instruction
 *
 * @param insn holds the given instruction
 *
 * @return uint32_t which is the extracted rs2 value
 *
 * @note the rs2 field is an integer value from 0 to 31
 ********************************************************************************/
uint32_t rv32i_decode::get_rs2(uint32_t insn)
{
	return(insn & 0x01f00000) >> 20;
}

/**
 * extracts the funct7 field from the given instruction
 *
 * @param insn holds the given instruction
 *
 * @return uint32_t which is the extracted funct7 value
 *
 * @note the funct7 field is an integer value from 0x00 to 0x7f
 ********************************************************************************/
uint32_t rv32i_decode::get_funct7(uint32_t insn)
{
	return(insn & 0x40000000) >> 25;
}


//*****Get imm_x functions*****

/**
 * extracts the imm_i field from the given instruction
 *
 * @param insn holds the given instruction
 *
 * @return int32_t which is the extracted imm_i value
 ********************************************************************************/
int32_t rv32i_decode::get_imm_i(uint32_t insn)
{
	int32_t imm_i = insn >> 20;
//	int32_t imm_i = (insn & 0xffff00000) >> 20;
	//check if negative
	if (insn & 0x80000000)
		imm_i |= 0xfffff000; // sign-extend the left

	return imm_i;
}


/**
 * extracts the imm_u field from the given instruction
 *
 * @param insn holds the given instruction
 *
 * @return int32_t which is the extracted imm_u value
 ********************************************************************************/
int32_t rv32i_decode::get_imm_u(uint32_t insn)
{
	//add 3 zeros to end of instruction
	return(insn & 0xfffff000);
}

/**
 * extracts the imm_b field from the given instruction
 *
 * @param insn holds the given instruction
 *
 * @return int32_t which is the extracted imm_b value
 ********************************************************************************/
int32_t rv32i_decode::get_imm_b(uint32_t insn)
{
	//format: bit 32 bit7 bits 30-25 bits 11-8 then last one is always 0
	int32_t imm_b = (insn & 0x80000000) >> (31-12); //get bit 32 and shift it right 19 times
	imm_b |= (insn & 0x7e000000) >> (30-10); //get bits 30-25 and shift them right 20 times
	imm_b |= (insn & 0x00000f00) >> (11-4); //get bits 11-8 and shift to the right 7 times
	imm_b |= (insn & 0x00000080) << (11-7); //get bit 7 and shift it left 4 times

	if (insn & 0x80000000)
		imm_b |= 0xfffff000; // sign-extend the left

	return imm_b;
}

/**
 * extracts the imm_s field from the given instruction
 *
 * @param insn holds the given instruction
 *
 * @return int32_t which is the extracted imm_s value
 ********************************************************************************/
int32_t rv32i_decode::get_imm_s(uint32_t insn)
{
	int32_t imm_s = (insn & 0xfe000000) >> (25-5); //get bits 32-25 and shift right 20 times
	imm_s |= (insn & 0x00000f80) >> (7-0);	//get bits 11-7 and shift right 7 times

	//check is instruction is negative
	if (insn & 0x80000000)
		imm_s |= 0xfffff000; // sign-extend the left

	return imm_s;
}

/**
 * extracts the imm_j field from the given instruction
 *
 * @param insn holds the given instruction
 *
 * @return int32_t which is the extracted imm_j value
 ********************************************************************************/
int32_t rv32i_decode::get_imm_j(uint32_t insn)
{
	//format: bit 32 bits 19-12 bit 20 bits 31-21 then last one is always 0
	int32_t imm_j = (insn & 0x80000000) >> (32-21); //get bit 32 and shift it right 11 times
	imm_j |= (insn & 0x7fe00000) >> (30-10); //get bits 31-21 and shit them right 20 times
	imm_j |= (insn & 0x00100000) >> (20-11); //get bit 20 and shift it  right 9 times
	imm_j |= (insn & 0x000ff000); //get bits 19-12

	if (insn & 0x80000000)
		imm_j |= 0xffe00000; // sign-extend the left

	return imm_j;
}
