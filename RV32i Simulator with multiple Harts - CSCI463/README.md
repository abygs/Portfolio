# Project for CSCI463

This project simulates a RISC-V disassembler and simulates the execution of RV32I instructions. This program is capable of executing real programs compiled with gcc.

Usage: rv32i [-d] [-i] [-r] [-z] [-l exec-limit] [-m hex-mem-size] infile <br />
    -d show disassembly before program execution <br />
    -i show instruction printing during execution<br />
    -l maximum number of instructions to exec<br />
    -m specify memory size (default = 0x100)<br />
    -r show register printing during execution<br />
    -z show a dump of the regs & memory after simulation<br />
