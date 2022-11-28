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

#include "rv32i_hart.h"

/**
 * the way to tell the simulator how to run an instruction
 *
 * @param hdr is printed to the left of all output lines
 ********************************************************************************/
void rv32i_hart::tick(const std::string &hdr)
{
	//return if halt
	if(halt)
		return;

	//increment instruction counter
	++insn_counter;

	//dump registers if wanted
	if(show_registers)
		dump(hdr);

	uint32_t insn = mem.memory::get32(pc);
	if(show_instructions)
	{
		//print header at beginning of line if it's set
		if(hdr != "")
			std::cout << hdr;

		std::cout << hex::to_hex32(pc) << ": " << hex::to_hex32(insn) << " "; //print pc register and the fetched instruction in hex
		exec(insn, &std::cout); //execute instruction
		std::cout << "\n";
	}
	else //execute the instruction without rendering anything
		exec(insn, nullptr);
}

/**
 * dumps the entire state of the hart
 *
 * @param hdr is printed to the left of all output lines
 ********************************************************************************/
void rv32i_hart::dump(const std::string &hdr) const
{
	//dump registers
	regs.dump(hdr);

	//print header if set
	if(hdr != "")
		std::cout << hdr;

	//print pc register
	std::cout << " pc " << hex::to_hex32(pc) << "\n";
}

/**
 * reset pc register, registers, instruction counter, halt, and halt reason
 ********************************************************************************/
void rv32i_hart::reset()
{
	//reset everything to 0/false/none
	pc = 0;
	regs.reset();
	insn_counter = 0;
	halt = false;
	halt_reason = "none";
}

/**
 * executes the given RV32I instruction using the get_xxx methods
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec(uint32_t insn, std::ostream* pos)
{
        //get funct3 and funct7
        uint32_t funct3 = get_funct3(insn);
        uint32_t funct7 = get_funct7(insn);

	switch(get_opcode(insn))
	{
		default: exec_illegal_insn(insn, pos); return;

                //U-Type instructions
		case opcode_lui: exec_lui(insn, pos); return; //lui instruction
		case opcode_auipc: exec_auipc(insn, pos); return; //auipc instruction

		//J-Type instructions
                case opcode_jal: exec_jal(insn, pos); return; //jal instruction

                //B-Type instructions
                case opcode_btype:
                        switch(funct3)
                        {
				default: exec_illegal_insn(insn, pos); return;
                                case funct3_beq: exec_beq(insn, pos); return; //beq instruction
                                case funct3_bne: exec_bne(insn, pos); return; //bne instruction
                                case funct3_blt: exec_blt(insn, pos); return; //blt instruction
                                case funct3_bge: exec_bge(insn, pos); return; //bge instruction
                                case funct3_bltu: exec_bltu(insn, pos); return; //bltu instruction
                                case funct3_bgeu: exec_bgeu(insn, pos); return; //bgeu instruction
                        }
                        assert(0 && "unrecognized funct3"); //impossible


		//S-Type instructions
                case opcode_stype:
                        switch(funct3)
                        {
				default: exec_illegal_insn(insn, pos); return;
                                case funct3_sb: exec_sb(insn, pos); return; //sb instruction
                                case funct3_sh: exec_sh(insn, pos); return; //sh instruction
                                case funct3_sw: exec_sw(insn, pos); return; //sw instruction
                        }
			assert(0 && "unrecognized funct3"); //impossible


                //R-Type instructions
                case opcode_rtype:
                        switch(funct3)
                        {
				default: exec_illegal_insn(insn, pos); return;
                                case funct3_sll: exec_sll(insn, pos); return; //sll instruction
                                case funct3_slt: exec_slt(insn, pos); return; //slt instruction
                                case funct3_sltu: exec_sltu(insn, pos); return; //sltu instruction
                                case funct3_xor: exec_xor(insn, pos); return; //xor instruction
                                case funct3_or: exec_or(insn, pos); return; //or instruction
                                case funct3_and: exec_and(insn, pos); return; //and instruction
                                case funct3_srx:
                                        switch(funct7)
                                        {
						default: exec_illegal_insn(insn, pos); return;
                                                case funct7_srl: exec_srl(insn, pos); return; //srl instruction
                                                case funct7_sra: exec_sra(insn, pos); return; //sra instruction
                                        }
                                        assert(0 && "unrecognized funct7"); //impossible
				case funct3_add:
                                        switch(funct7)
                                        {
						default: exec_illegal_insn(insn, pos); return;
                                                case funct7_add: exec_add(insn, pos); return; //add instruction
                                                case funct7_sub: exec_sub(insn, pos); return; //sub instruction
                                        }
                                        assert(0 && "unrecognized funct7"); //impossible
                        }
                        assert(0 && "unrecognized funct3"); //impossible

		//I-Type instructions
                case opcode_jalr: exec_jalr(insn, pos); return; //jalr instruction


		//Load
                case opcode_load_imm:
                        switch(funct3)
                        {
				default: exec_illegal_insn(insn, pos); return;
                                case funct3_lb: exec_lb(insn, pos); return; //lb instruction
                                case funct3_lh: exec_lh(insn, pos); return; //lh instruction
                                case funct3_lw: exec_lw(insn, pos); return; //lw instruction
                                case funct3_lbu: exec_lbu(insn, pos); return; //lbu instruction
                                case funct3_lhu: exec_lhu(insn, pos); return; //lhu instruction
                        }
                        assert(0 && "unrecognized funct3"); //impossible

		//ALU
                case opcode_alu_imm:
                        switch(funct3)
                        {
				default: exec_illegal_insn(insn, pos); return;
                                case funct3_sll: exec_slli(insn, pos); return; //slli instruction
                                case funct3_slt: exec_slti(insn, pos); return; //slti instruction
                                case funct3_sltu: exec_sltiu(insn, pos); return; //sltiu instruction
                                case funct3_xor: exec_xori(insn, pos); return; //xori instruction
                                case funct3_or: exec_ori(insn, pos); return; //ori instruction
                                case funct3_and: exec_andi(insn, pos); return; //andi instruction
                                case funct3_add: exec_addi(insn, pos); return; //addi instruction
                                case funct3_srx:
                                        switch(funct7)
                                        {
						default: exec_illegal_insn(insn, pos); return;
                                                case funct7_srl: exec_srli(insn, pos); return; //slri instruction
                                                case funct7_sra: exec_srai(insn, pos); return; //srai instruction
                                        }
                                        assert(0 && "unrecognized funct7"); //impossible
                        }
                        assert(0 && "unrecognized funct3"); //impossible

		case opcode_system:
			if(insn == insn_ebreak) //ebreak
				{exec_ebreak(insn, pos); return;}
			if(insn == insn_ecall) //ecall
                                {exec_ecall(insn, pos); return;}
                        switch(funct3)
                        {
				default: exec_illegal_insn(insn, pos); return;
                                case funct3_csrrs: exec_csrrs(insn, pos); return; //csrrs instruction
                        }
                        assert(0 && "unrecognized funct3"); //impossible
        }
        assert(0 && "unrecognized opcode"); // It should be impossible to ever get here!
}

/**
 * sets halt flag if illegal instruction occurs
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_illegal_insn(uint32_t insn, std::ostream *pos)
{
	//to prevent unused variable error
	(void)insn;

	//prints illegal instruction
	if(pos)
		*pos << render_illegal_insn(insn);

	//sets halt and halt reason
	halt = true;
	halt_reason = "Illegal instruction";
}

//*****exec_xxx helper functions*****

//***U-Type Instructions***

/**
 * simulates the execution of the lui instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_lui(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);

	if(pos)
	{
		std::string s = " " + render_lui(insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(imm_u);
	}

	regs.set(rd, imm_u); //set imm_u in rd register
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the auipc instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_auipc(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);

	if(pos)
	{
		std::string s = " " + render_auipc(insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(pc) << " + " << to_hex0x32(imm_u) << " = " << to_hex0x32(pc + imm_u);
	}

	regs.set(rd, (pc + imm_u)); //set pc+imm_u value in rd register
	pc += 4; //increment program counter
}

//***J-Type Instructions***
/**
 * simulates the execution of the jal instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_jal(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        uint32_t nextInsn = pc + 4;
        int32_t imm_j = get_imm_j(insn); //J-type
        int32_t pcrel_21 = imm_j + pc;

	if(pos)
	{
		std::string s = " " + render_jal(pc, insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(nextInsn) << ",  pc = " << to_hex0x32(pc);
		*pos << " + " << to_hex0x32(imm_j) << " = " << to_hex0x32(pcrel_21);
	}

	regs.set(rd, nextInsn);
	pc += imm_j; //increment program counter
}

//***B-Type Instructions***
/**
 * simulates the execution of the beq instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_beq(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        int32_t rs1 = regs.get(get_rs1(insn));
        int32_t rs2 = regs.get(get_rs2(insn));
	int32_t imm_b = get_imm_b(insn);
	int32_t branch = (rs1 == rs2 ? 0x00000008 : 4);

	if(pos)
	{
		std::string s = " " + render_btype(pc, insn, "beq");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // pc += (" << to_hex0x32(rs1) << " == " << to_hex0x32(rs2) << " ? " << to_hex0x32(imm_b);
		*pos << " : 4) = " << to_hex0x32(pc + branch);
	}
	pc += branch; //increment program counter
}

/**
 * simulates the execution of the bne instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_bne(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        int32_t rs1 = regs.get(get_rs1(insn));;
        int32_t rs2 = regs.get(get_rs2(insn));;
	int32_t imm_b = get_imm_b(insn);
	int32_t branch = (rs1 != rs2 ? imm_b : 4);

	if(pos)
	{
		std::string s = " " + render_btype(pc, insn, "bne");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // pc += (" << to_hex0x32(rs1) << " != " << to_hex0x32(rs2) << " ? " << to_hex0x32(imm_b);
		*pos << " : 4) = " << to_hex0x32(pc + branch);
	}
	pc += branch; //increment program counter
}

/**
 * simulates the execution of the blt instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_blt(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        int32_t rs1 = regs.get(get_rs1(insn));;
        int32_t rs2 = regs.get(get_rs2(insn));;
	int32_t imm_b = get_imm_b(insn);
	int32_t branch = (rs1 < rs2 ? imm_b : 4);

	if(pos)
	{
		std::string s = " " + render_btype(pc, insn, "blt");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // pc += (" << to_hex0x32(rs1) << " < " << to_hex0x32(rs2) << " ? " << to_hex0x32(imm_b);
		*pos << " : 4) = " << to_hex0x32(pc + branch);
	}
	pc += branch; //increment program counter
}

/**
 * simulates the execution of the bltu instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_bltu(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rs1 = regs.get(get_rs1(insn));;
        uint32_t rs2 = regs.get(get_rs2(insn));;
	int32_t imm_b = get_imm_b(insn);
	int32_t branch = (rs1 < rs2 ? imm_b : 4);

	if(pos)
	{
		std::string s = " " + render_btype(pc, insn, "bltu");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // pc += (" << to_hex0x32(rs1) << " <U " << to_hex0x32(rs2) << " ? " << to_hex0x32(imm_b);
		*pos << " : 4) = " << to_hex0x32(pc + branch);
	}
	pc += branch; //increment program counter
}

/**
 * simulates the execution of the bqe instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_bge(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        int32_t rs1 = regs.get(get_rs1(insn));;
        int32_t rs2 = regs.get(get_rs2(insn));;
	int32_t imm_b = get_imm_b(insn);
	int32_t branch = (rs1 >= rs2 ? imm_b : 4);

	if(pos)
	{
		std::string s = " " + render_btype(pc, insn, "bge");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // pc += (" << to_hex0x32(rs1) << " >= " << to_hex0x32(rs2) << " ? " << to_hex0x32(imm_b);
		*pos << " : 4) = " << to_hex0x32(pc + branch);
	}
	pc += branch; //increment program counter
}

/**
 * simulates the execution of the bgeu instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_bgeu(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rs1 = regs.get(get_rs1(insn));;
        uint32_t rs2 = regs.get(get_rs2(insn));;
	int32_t imm_b = get_imm_b(insn);
	int32_t branch = (rs1 >= rs2 ? imm_b : 4);

	if(pos)
	{
		std::string s = " " + render_btype(pc, insn, "bgeu");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // pc += (" << to_hex0x32(rs1) << " >=U " << to_hex0x32(rs2) << " ? " << to_hex0x32(imm_b);
		*pos << " : 4) = " << to_hex0x32(pc + branch);
	}
	pc += branch; //increment program counter
}

//***I-Type Instructions***
/**
 * simulates the execution of the jalr instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_jalr(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        uint32_t rs1 = regs.get(get_rs1(insn));
        int32_t imm_i = get_imm_i(insn); //I-type
        uint32_t nextInsn = pc + 4;
	pc = ((rs1 + imm_i) & 0xfffffffe); //increment program counter

	if(pos)
	{
		std::string s = " " + render_jalr(insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(nextInsn) << ",  pc = (" << to_hex0x32(imm_i);
		*pos << " + " << to_hex0x32(rs1) << ") & 0xfffffffe = " << to_hex0x32(pc);
	}

	regs.set(rd, nextInsn);
}


/**
 * simulates the execution of the lb instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_lb(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        uint32_t rs1 = regs.get(get_rs1(insn));
        int32_t imm_i = get_imm_i(insn); //I-type
        uint32_t addr = rs1 + imm_i;
	int32_t byte = mem.get8(addr); //sign-extended

	if(byte & 0x80)
                byte |= 0xffffff00; // sign-extend the left

	if(pos)
	{
		std::string s = " " + render_itype_load(insn, "lb");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = sx(m8(" << to_hex0x32(rs1) << " + " << to_hex0x32(imm_i);
		*pos << ")) = " << to_hex0x32(byte);
	}

	regs.set(rd, byte);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the lh instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_lh(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        uint32_t rs1 = regs.get(get_rs1(insn));
        int32_t imm_i = get_imm_i(insn); //I-type
        uint32_t addr = rs1 + imm_i;
	int32_t halfword = mem.get16(addr); //zero-extended

	//check signed bit
	if(halfword & 0x8000)
                halfword |= 0xffff0000; // sign-extend the left

	if(pos)
	{
		std::string s = " " + render_itype_load(insn, "lh");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = sx(m16(" << to_hex0x32(rs1) << " + " << to_hex0x32(imm_i);
		*pos << ")) = " << to_hex0x32(halfword);
	}

	regs.set(rd, halfword);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the lw instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_lw(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        uint32_t rs1 = regs.get(get_rs1(insn));
        int32_t imm_i = get_imm_i(insn); //I-type
        uint32_t addr = rs1 + imm_i;
	int32_t fullword = mem.get32(addr);

	if(pos)
	{
		std::string s = " " + render_itype_load(insn, "lw");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = sx(m32(" << to_hex0x32(rs1) << " + " << to_hex0x32(imm_i);
		*pos << ")) = " << to_hex0x32(fullword);
	}

	regs.set(rd, fullword);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the lbu instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_lbu(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        uint32_t rs1 = regs.get(get_rs1(insn));
        int32_t imm_i = get_imm_i(insn); //I-type
        uint32_t addr = rs1 + imm_i;
	int32_t byte = mem.get8(addr); //zero-extended

	if(pos)
	{
		std::string s = " " + render_itype_load(insn, "lbu");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = zx(m8(" << to_hex0x32(rs1) << " + " << to_hex0x32(imm_i);
		*pos << ")) = " << to_hex0x32(byte);
	}

	regs.set(rd, byte);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the lhu instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_lhu(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        uint32_t rs1 = regs.get(get_rs1(insn));
        int32_t imm_i = get_imm_i(insn); //I-type
        uint32_t addr = rs1 + imm_i;
	int32_t halfword = mem.get16(addr); //zero-extended

	if(pos)
	{
		std::string s = " " + render_itype_load(insn, "lhu");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = zx(m16(" << to_hex0x32(rs1) << " + " << to_hex0x32(imm_i);
		*pos << ")) = " << to_hex0x32(halfword);
	}

	regs.set(rd, halfword);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the addi instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_addi(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        uint32_t rs1 = regs.get(get_rs1(insn));
        int32_t imm_i = get_imm_i(insn); //I-type

	if(pos)
	{
		std::string s = " " + render_itype_alu(insn, "addi", imm_i);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(rs1) << " + " << to_hex0x32(imm_i);
		*pos << " = " << to_hex0x32(rs1 + imm_i);
	}

	regs.set(rd, (rs1 + imm_i));
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the slti instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_slti(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        int32_t rs1 = regs.get(get_rs1(insn));
        int32_t imm_i = get_imm_i(insn); //I-type

	if(pos)
	{
		std::string s = " " + render_itype_alu(insn, "slti", imm_i);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = (" << to_hex0x32(rs1) << " < " << imm_i;
		*pos << ") ? 1 : 0 = " << to_hex0x32(rs1 < imm_i);
	}

	regs.set(rd, (rs1 < imm_i) ? 1 : 0);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the sltiu instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_sltiu(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        uint32_t rs1 = regs.get(get_rs1(insn));
        uint32_t imm_i = get_imm_i(insn); //I-type

	uint32_t val = (rs1 < imm_i) ? 1 : 0;

	if(pos)
	{
		std::string s = " " + render_itype_alu(insn, "sltiu", imm_i);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = (" << to_hex0x32(rs1) << " <U " << imm_i;
		*pos << ") ? 1 : 0 = " << to_hex0x32(val);
	}

	regs.set(rd, (rs1 < imm_i) ? 1 : 0);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the xori instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_xori(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        int32_t rs1 = regs.get(get_rs1(insn));
        int32_t imm_i = get_imm_i(insn); //I-type
	int32_t val = rs1 ^ imm_i;

	if(pos)
	{
		std::string s = " " + render_itype_alu(insn, "xori", imm_i);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(rs1) << " ^ " << to_hex0x32(imm_i);
		*pos << " = " << to_hex0x32(val);
	}

	regs.set(rd, val);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the ori instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_ori(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        int32_t rs1 = regs.get(get_rs1(insn));
        int32_t imm_i = get_imm_i(insn); //I-type
	int32_t val = rs1 | imm_i;

	if(pos)
	{
		std::string s = " " + render_itype_alu(insn, "ori", imm_i);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(rs1) << " | " << to_hex0x32(imm_i);
		*pos << " = " << to_hex0x32(val);
	}

	regs.set(rd, val);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the andi instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_andi(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        int32_t rs1 = regs.get(get_rs1(insn));
        int32_t imm_i = get_imm_i(insn); //I-type
	int32_t val = rs1 & imm_i;

	if(pos)
	{
		std::string s = " " + render_itype_alu(insn, "andi", imm_i);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(rs1) << " & " << to_hex0x32(imm_i);
		*pos << " = " << to_hex0x32(val);
	}

	regs.set(rd, val);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the slli instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_slli(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        int32_t rs1 = regs.get(get_rs1(insn));
        int32_t imm_i = get_imm_i(insn); //I-type
	int32_t shamt_i = imm_i % XLEN; //last 5 of imm_i
	int32_t val = rs1 << shamt_i; //logical left shift

	if(pos)
	{
		std::string s = " " + render_itype_alu(insn, "slli", imm_i);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(rs1) << " << " << shamt_i;
		*pos << " = " << to_hex0x32(val);
	}

	regs.set(rd, val);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the srli instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_srli(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        uint32_t rs1 = regs.get(get_rs1(insn));
        int32_t imm_i = get_imm_i(insn); //I-type
	int32_t shamt_i = imm_i % XLEN; //last 5 of imm_i
	int32_t val = rs1 >> shamt_i; //logical right shift

	if(pos)
	{
		std::string s = " " + render_itype_alu(insn, "srli", imm_i);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(rs1) << " >> " << shamt_i;
		*pos << " = " << to_hex0x32(val);
	}

	regs.set(rd, val);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the srai instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_srai(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        int32_t rs1 = regs.get(get_rs1(insn));
        int32_t imm_i = get_imm_i(insn); //I-type
	int32_t shamt_i = imm_i % XLEN; //last 5 of imm_i
	int32_t val = rs1 >> shamt_i; //arithmetic right shift

	if(pos)
	{
		std::string s = " " + render_itype_alu(insn, "srai", shamt_i);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(rs1) << " >> " << shamt_i;
		*pos << " = " << to_hex0x32(val);
	}

	regs.set(rd, val);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the csrrs instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_csrrs(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        uint32_t rs1 = regs.get(get_rs1(insn));
	int32_t imm_i = get_imm_i(insn) & 0x00000fff;

	if(imm_i != 0xf14 && rs1 != 0)
	{
		halt = true;
		halt_reason = "Illegal CSR in CRRSS instructions";
	}

	if(pos)
	{
		std::string s = " " + render_csrrx(insn, "csrrs");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << mhartid;
	}

	if(!halt)
	{
		regs.set(rd, mhartid);
		pc += 4; //increment program counter
	}
}

//***S-Type Instructions***
/**
 * simulates the execution of the sb instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_sb(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        int32_t rs1 = regs.get(get_rs1(insn));
        uint8_t rs2 = regs.get(get_rs2(insn)); //byte
        int32_t imm_s = get_imm_s(insn); //S-type
        uint32_t addr = rs1 + imm_s;

	mem.set8(addr, rs2);

	if(pos)
	{
		std::string s = " " + render_stype(insn, "sb");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // m8(" << to_hex0x32(rs1) << " + " << to_hex0x32(imm_s) << ") = " << to_hex0x32(rs2);
	}

	pc += 4; //increment program counter
}

/**
 * simulates the execution of the sh instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_sh(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        int32_t rs1 = regs.get(get_rs1(insn));
        uint16_t rs2 = regs.get(get_rs2(insn)); //halfword
        int32_t imm_s = get_imm_s(insn); //S-type
        uint32_t addr = rs1 + imm_s;

	mem.set16(addr, rs2);

	if(pos)
	{
		std::string s = " " + render_stype(insn, "sh");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // m16(" << to_hex0x32(rs1) << " + " << to_hex0x32(imm_s) << ") = " << to_hex0x32(rs2);
	}

	pc += 4; //increment program counter
}

/**
 * simulates the execution of the sw instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_sw(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        int32_t rs1 = regs.get(get_rs1(insn));
        uint32_t rs2 = regs.get(get_rs2(insn)); //fullword
        int32_t imm_s = get_imm_s(insn); //S-type
        uint32_t addr = rs1 + imm_s;

	mem.set32(addr, rs2);

	if(pos)
	{
		std::string s = " " + render_stype(insn, "sw");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // m32(" << to_hex0x32(rs1) << " + " << to_hex0x32(imm_s) << ") = " << to_hex0x32(rs2);
	}

	pc += 4; //increment program counter
}

//***R-Type Instructions***
/**
 * simulates the execution of the add instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_add(uint32_t insn, std::ostream *pos)
{
	uint32_t rd = get_rd(insn);
        int32_t rs1 = regs.get(get_rs1(insn));
        int32_t rs2 = regs.get(get_rs2(insn));

	if(pos)
	{
		std::string s = " " + render_rtype(insn, "add");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(rs1) << " + " << to_hex0x32(rs2) << " = " << to_hex0x32(rs1 + rs2);
	}

	regs.set(rd, (rs1 + rs2));
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the sub instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_sub(uint32_t insn, std::ostream *pos)
{
	uint32_t rd = get_rd(insn);
        int32_t rs1 = regs.get(get_rs1(insn));
        int32_t rs2 = regs.get(get_rs2(insn));

	if(pos)
	{
		std::string s = " " + render_rtype(insn, "sub");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(rs1) << " - " << to_hex0x32(rs2) << " = " << to_hex0x32(rs1 - rs2);
	}

	regs.set(rd, (rs1 - rs2));
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the sll instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_sll(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        uint32_t rs1 = regs.get(get_rs1(insn));
        uint32_t rs2 = regs.get(get_rs2(insn));
	int32_t shamt_i = rs2 % XLEN; //last 5 of rs2
	int32_t val = rs1 << shamt_i; //left shift

	if(pos)
	{
		std::string s = " " + render_rtype(insn, "sll");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(rs1) << " << " << shamt_i;
		*pos << " = " << to_hex0x32(val);
	}

	regs.set(rd, val);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the slt instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_slt(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        uint32_t rs1 = get_rs1(insn);
        uint32_t rs2 = get_rs2(insn);

	int32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;

	if(pos)
	{
		std::string s = " " + render_rtype(insn, "slt");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = (" << to_hex0x32(regs.get(rs1)) << " < " << to_hex0x32(regs.get(rs2));
		*pos << ") ? 1 : 0 = " << to_hex0x32(val);
	}

	regs.set(rd, val);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the sltu instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_sltu(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        uint32_t rs1 = regs.get(get_rs1(insn));
        uint32_t rs2 = regs.get(get_rs2(insn));

	int32_t val = (rs1 < rs2) ? 1 : 0;

	if(pos)
	{
		std::string s = " " + render_rtype(insn, "sltu");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = (" << to_hex0x32(rs1) << " <U " << to_hex0x32(rs2);
		*pos << ") ? 1 : 0 = " << to_hex0x32(val);
	}

	regs.set(rd, val);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the xor instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_xor(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        int32_t rs1 = regs.get(get_rs1(insn));
        int32_t rs2 = regs.get(get_rs2(insn));

	int32_t val = rs1 ^ rs2;

	if(pos)
	{
		std::string s = " " + render_rtype(insn, "xor");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(rs1) << " ^ " << to_hex0x32(rs2);
		*pos << " = " << to_hex0x32(val);
	}

	regs.set(rd, val);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the srl instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_srl(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        uint32_t rs1 = regs.get(get_rs1(insn));
        uint32_t rs2 = regs.get(get_rs2(insn));
	int32_t shamt_i = rs2 % XLEN; //last 5 of rs2

	int32_t val = rs1 >> rs2; //logical right shift

	if(pos)
	{
		std::string s = " " + render_rtype(insn, "srl");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(rs1) << " >> " << shamt_i;
		*pos << " = " << to_hex0x32(val);
	}

	regs.set(rd, val);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the sra instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_sra(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        int32_t rs1 = regs.get(get_rs1(insn));
        int32_t rs2 = regs.get(get_rs2(insn));
	int32_t shamt_i = rs2 % XLEN; //last 5 of rs2
	int32_t val = rs1 >> shamt_i; //arithmetic right shift

	if(pos)
	{
		std::string s = " " + render_rtype(insn, "sra");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(rs1) << " >> " << shamt_i;
		*pos << " = " << to_hex0x32(val);
	}

	regs.set(rd, val);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the or instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_or(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        int32_t rs1 = regs.get(get_rs1(insn));
        int32_t rs2 = regs.get(get_rs2(insn));
	int32_t val = rs1 | rs2;

	if(pos)
	{
		std::string s = " " + render_rtype(insn, "or");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(rs1) << " | " << to_hex0x32(rs2);
		*pos << " = " << to_hex0x32(val);
	}

	regs.set(rd, val);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the and instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_and(uint32_t insn, std::ostream *pos)
{
	//get necessary variables
        uint32_t rd = get_rd(insn);
        int32_t rs1 = regs.get(get_rs1(insn));
        int32_t rs2 = regs.get(get_rs2(insn));
	int32_t val = rs1 & rs2;

	if(pos)
	{
		std::string s = " " + render_rtype(insn, "and");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // " << render_reg(rd) << " = " << to_hex0x32(rs1) << " & " << to_hex0x32(rs2);
		*pos << " = " << to_hex0x32(val);
	}

	regs.set(rd, val);
	pc += 4; //increment program counter
}

/**
 * simulates the execution of the ecall instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_ecall(uint32_t insn, std::ostream *pos)
{
	if(pos)
	{
		std::string s = " " + render_ecall(insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // HALT";
	}
	halt = true;
	halt_reason = "ECALL instruction";
}

/**
 * simulates the execution of the ebreak instruction
 *
 * @param insn holds the fetched instruction
 *
 * @param pos holds the stream to print to
 ********************************************************************************/
void rv32i_hart::exec_ebreak(uint32_t insn, std::ostream *pos)
{
	if(pos)
	{
		std::string s = " " + render_ebreak(insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << " // HALT";
	}
	halt = true;
	halt_reason = "EBREAK instruction";
}
