MIPS Single Cycle Processor Emulator Project
===============================================


# Description

* Create a simple MIPS simulator
* Simulator will read a binary file containing a MIPS program and execute that program

* First the program will genrate the assembly code for the given MIPS program (disassembler)
* Second the program will create an instruction-by-instruction simulation of the MIPS program


# Implementation

* May implement this project in any programming language of choosing
* MUST include instruction in a README file that indicate how to compile (if necessary) and run program
* MUST be able to run in linux home

* JADA REWATCH VIDEOS


Details
==========

* Will be given an input file containing a sequence of 32 bit instruction words

* Assume that the first instruction is at memory address 96
* The final instruction in an instruction sequence is ALWAYS a "BREAK" instruction

* Following the break instruction is a sequence of 32 bit 2's compliment signed integers for the program data

* Simulator/disassembler must support the following MIPS instructions.
* Check the MIPS manual for Details on instruction representation and operation for each instruction

```ruby
J, JR, BEQ, BLTZ
ADD, ADDI, SUB
SW, LW
SLL, SRL
MUL,
AND, OR,
MOVZ
NOP
```


Input
========

* Program must accept command line arguments for execution
* The following arguments must be supported (Executable named "mipssim"):

```ruby
mipssim -i INPUTFILENAME -o OUTPUTFILENAME
```

* Program will produce 2 ouput files
    * OUTPUTFILENAME_sim.txt, which contains the simulation ouput
    * OUTPUTFILENAME_dis.txt, which contains the disassembled program code for the input MIPS program


Output
=========

* Program will produce 2 ouput files
    * OUTPUTFILENAME_sim.txt, which contains the simulation ouput
    * OUTPUTFILENAME_dis.txt, which contains the disassembled program code for the input MIPS program

* The disassembled ouput file should contain one line per word in teh input file
    * Should be separated into 4 columns, each separated by tab character

* The columns contain the following information
    1. The binary representation of the instruction word. If the word is an instruction (as opposed to memory data after the BREAK instruction), the instruction should be split into six groups of digits: the opcode bits, four groups of 5 bits, and a final group of 6 bits.
    2. The address of the memory location (in decimal)
    3. The disassembled instruction opcode
    4. If it is an instruction, print the operation, followed by a tab character, then print each argument separated by a comman and a space (", ")

* The simulation file must have the following format:
    * 20 equal signs and a newline

    * cycle: [cycle number] [tab] [instruction address] [tab] [instruction string (same as step 4 above)]

    * [blank lane]

* registers:
    * r00: [tab] [integer value of R00] [tab] [integer value of R01] [tab] ... [integer value of R07]