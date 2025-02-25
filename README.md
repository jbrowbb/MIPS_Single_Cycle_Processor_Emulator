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