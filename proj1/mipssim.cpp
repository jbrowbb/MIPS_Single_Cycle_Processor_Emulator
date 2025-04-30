#include <map>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <bitset>
#include <iomanip>
#include <fstream>
using namespace std;

struct items{
    int i, rs, rt, rd, imm, opcode, valid, instr_index, funct, hint, offset, sa, opp1, opp2, dest;
    unsigned int asUint;
    string instStr, binstr, binStrSpace;
};

int preIssue[4] = {0};
int preMem[2] = {0};
int preALU[2] = {0};
int postMem = {0};
int postALU = {0};
int aluValue = 0;
int memValue = 0; 


bool XBW(int rNum, int index, items MEM[]) {
    for(int i = 0; i < index; i++) {
        if(preIssue[i] !=0 && MEM[preIssue[i]].dest == rNum) {
            return true;
        }
    }
    
    for(int i = 0; i < 2; i++) {
        if(preMem[i] != 0 &&MEM[preMem[i]].dest == rNum) {
            return true;
        }
    }

    for(int i = 0; i < 2; i++) {
        if(preALU[i] != 0 && MEM[preALU[i]].dest == rNum) {
            return true;
        }
    }

    if(postALU != 0 && MEM[postALU].dest == rNum) {
        return true;
    }

    if(postMem !=0 && MEM[postMem].dest == rNum) {
        return true;
    }
    
    return false;
}


bool WBR(int rNum, int index, items MEM[], int postalu, int postMem, int preIssue[]) {
    for(int i = 0; i < index; i++) {
        if(preIssue[i] != 0 && MEM[preIssue[i]].rs == rNum) {
            return true;
        }

        if(preIssue[i] != 0 && MEM[preIssue[i]].rt == rNum) {
            return true;
        }
    }
    
    return false;
}


int main(int argc, char* argv[]) {
    char buffer[4];
    int i;
    char * iPtr;
    iPtr = (char*)(void*) &i;
    bool breakVal = true;

    int FD = open(argv[2], O_RDONLY);
    ofstream disout(string (argv[4]) + "_dis.txt");
    ofstream simout(string (argv[4]) + "_sim.txt");

    
    bool didBreak = false;
    int addr = 96;
    int amt = 4;
    int dataStart, dataEnd;
    items MEM[500];
    while (amt != 0)
    {
        
        amt = read(FD, buffer, 4);
        if(amt == 4)
        {
            iPtr[0] = buffer[3];
            iPtr[1] = buffer[2];
            iPtr[2] = buffer[1];
            iPtr[3] = buffer[0];
            items I;
            
            
            unsigned int asUint = (unsigned int) i;
            I.asUint = asUint;

            bitset<32> b(i);
            I.binstr = b.to_string();
            I.binStrSpace = I.binstr;
            I.binStrSpace.insert(26, " "), I.binStrSpace.insert(21, " "), I.binStrSpace.insert(16, " "),
            I.binStrSpace.insert(11, " "), I.binStrSpace.insert(6, " "), I.binStrSpace.insert(1, " ");
            
            I.valid = asUint >> 31;
            I.opcode = asUint >> 26;
            I.funct = (asUint << 26) >> 26;
            
            I.rs = (asUint << 6)>>27;
            I.rt = (asUint << 11)>>27;
            I.imm = (asUint << 16) >> 16;
            I.rd = (asUint << 16) >> 27;
            I.sa = (asUint << 21) >> 27;

            I.instr_index = (asUint << 26) >> 24;
            I.offset = asUint << 20 >> 18;


                if(didBreak) 
                {
                    I.instStr = to_string(i);
                    I.binStrSpace = I.binstr;

                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if(I.valid == 0) {
                I.instStr = "Invalid Instruction";
                disout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
           
                else if(I.opcode == 34) {
                    I.instStr = "J\t#" + to_string(I.instr_index);
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 32 && I.funct == 8) {
                    I.instStr = "JR\tR" + to_string(I.rs);
                    disout << I.binstr << " " << addr << "\t" <<I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 33 && I.rt == 0) {
                    I.instStr = "BLTZ\tR" + to_string(I.rs) + ", #" + to_string(I.offset);
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if(I.opcode ==  36) {
                    I.instStr = "BEQ\tR" + to_string(I.rt) + ", R" + to_string(I.rs) + ", #" + to_string(I.offset);
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 32 && I.funct == 32) {
                    I.instStr = "ADD\tR" + to_string(I.rt) + ", R" + to_string(I.rs) + ", R" + to_string(I.rd);
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 40) {
                    if(I.imm > 300) {
                        I.imm = ((asUint << 28) >> 28) * -1;
                    }
                    I.instStr = "ADDI\tR" + to_string(I.rt) + ", R" + to_string(I.rs) + ", #" + to_string(I.imm);
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 60 && I.funct == 2) {
                    I.instStr = "MUL\tR" + to_string(I.rd) + ", R" + to_string(I.rs) + ", R" + to_string(I.rt);
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 32 && I.funct == 36) {
                    I.instStr = "And\tR" + to_string(I.rd) + ", R" + to_string(I.rs) + ", R" + to_string(I.rt);
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 32 && I.funct ==37) {
                    I.instStr = "Or\tR" + to_string(I.rd) + ", R" + to_string(I.rs) + ", R" + to_string(I.rt);
                    disout << I.binstr << "\t" << I.instStr << endl;
                    cout << I.binstr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 32 && I.funct == 5) {
                    I.instStr = "MOVZ\tR" + to_string(I.rd) + ", R" + to_string(I.rs)+ ", R" + to_string(I.rt);
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 32 && I.sa == 0 && I.funct == 0) {
                    I.instStr = "NOP";
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }
                else if(I.opcode == 32 && I.funct == 34) {
                    I.instStr = "SUB\tR" + to_string(I.rd) + ", R" + to_string(I.rs) + ", R" + to_string(I.rt); 
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }

                else if(I.opcode == 43) {
                    I.instStr = "SW\tR" + to_string(I.rt) + ", " + to_string(I.imm) + "(R" + to_string(I.rs) + ')';
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }

                else if(I.opcode == 35) {
                    I.instStr = "LW\tR" + to_string(I.rt) + ", " + to_string(I.imm) + "(R" + to_string(I.rs) + ')';
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }

                else if(I.opcode == 32 && I.sa > 0 && I.funct == 0) {
                    I.instStr = "SLL\tR" + to_string(I.rd) + ", R" + to_string(I.rt) + ", #" + to_string(I.sa);
                    // it shifts left
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }

                else if(I.opcode == 32 && I.funct == 2) {
                    I.instStr = "SRL\tR" + to_string(I.rd) + ", R" + to_string(I.rt) + ", #" + to_string(I.sa);
                    //it shifts right
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                    }
                else if(I.opcode == 32 && I.funct == 13) {
                    I.instStr = "BREAK";
                    didBreak = true;
                    dataStart = addr + 4;
                    disout << I.binstr << " " << addr << "\t" << I.instStr << endl; 
                    cout << I.binstr << " " << addr << "\t" << I.instStr << endl;
                }

            MEM[addr] = I;
            dataEnd = addr;
            addr+=4;
        }

    }
    
    
    
    // end of decode

    int PC = 96;
    int R[32] = {0};
    int cycle = 1;
    didBreak = false;


 struct fetch{
    void run(int preIssue[], bool didBreak, items MEM[], int PC, int R[]) {
        for (int i = 0; i < 2; i++) {
            //checks if there is room at pre-issue
            if(preIssue[3] != 0) break;
            //checks if a break instruction was fetched which stalls fetch by breaking
            if(didBreak) break;
            items I = MEM[PC];

            // if I is branch or jump, try to do it then break
            //BLTZ opcode 000001(1)
            if((I.opcode == 33 && R[I.rt] == 0)) {
                //want to find target address and break and check if it is taken
                if(R[I.rs] > 0) {
                    PC = I.offset; 
                }
                break;
            }
            // J opcode 000010(2)
            else if(I.opcode == 34) {
                //goes here
                PC = I.instr_index;
                break;
            }
            //JR opcode 001000(8)
            else if(I.opcode == 32 && I.funct == 8) {
                PC = R[I.rs];
                break;
            }
            //BREAK opcode 001101(13)
            else if(I.opcode == 32 && I.funct == 13) {
                didBreak = true;
                break;
            }
            // else move I to next open spot in preIssue
            else {
                for (int j = 0; j < 4; j++) {
                    if(preIssue[j] == 0) {
                        preIssue[j] = PC;
                        PC += 4;
                        break;
                    }

                }
            }
        

        }
    }
 };
 
    
 
 // Depending on the conditions, it either continues or updates the `preIssue` array.
//- It rearranges elements in the `preIssue` array based on certain conditions.
    struct issue{
        void run(int preIssue[], int preALU[], int preMem[], items MEM[]) {
            for(int i = 0; i < 4; i++) {
                if(preIssue[i] == 0) continue;
                items I = MEM[preIssue[i]];
                if(XBW(I.rs, i, MEM)) continue;
                if(XBW(I.rt, i, MEM)) continue;
                if(XBW(I.rd, i, MEM)) continue;
                //WBR Check
                if(WBR(I.rs, i, MEM, postALU, postMem, preIssue)) continue;                           
                if(WBR(I.rt, i, MEM, postALU, postMem, preIssue)) continue;                           
                if(WBR(I.rd, i, MEM, postALU, postMem, preIssue)) continue;                             
                if(I.opcode == 35 || I.opcode == 43) {
                    if(preMem[1] != 0) continue;
                    //LW SW checcks
                    //issue
                    for(int j = 0; j < 2; j++) {
                        if(preMem[j] == 0) {
                            preMem[j] = preIssue[i];
                            preIssue[i] = 0;
                        }
                    }
                }
                else {
                    if(preALU[1] != 0) continue;
                    for(int k = 0; k < 2; k++) {
                        if(preALU[k] == 0) {
                            preALU[k] = preIssue[i];
                            preIssue[i] = 0;
                        }
                    }
                }
            }
            for (int k = 0; k < 4; k++) {
                for (int i = 3; i > 0; i--) {
                    if(preIssue[i-1] == 0) {
                        preIssue[i-1] = preIssue[i];
                        preIssue[i] = 0;
                    }
                }
            }
        }
    };

    
 
/*
 Defines a struct `alu` with a method `run()`.
- The `run()` method iterates through a loop of 2 elements.
- It handles moving elements from `preALU` to `postALU` based on certain conditions.
*/
   struct alu{
        void run(int preALU[], items MEM[], int PC, int R[], int postalu) {
            if(preALU[0] != 0) {
                for(int i = 0; i < 2; i++) {
                    if(postALU != 0) break;
                    items I = MEM[preALU[i]];
                    //if there is nothing in the preALU, do nothing
                    // if there is something in the preALU-move it to post, unless post is full
                        //ADDI R[I.rt] = R[I.rs] + I.imm;
                        if(I.opcode == 40) {
                            postALU = I.rt; // destination
                            aluValue = R[I.rs] + I.imm;
                        }
                        //ADD R[I.rd] = R[I.rs] + R[I.rt];
                        if(I.opcode == 32 && I.funct == 32) {
                            postALU = I.rd; //destination
                            aluValue = R[I.rs] + R[I.rt];
                        }
                }
                //need to clear out the instruction executed and move the next down
                preALU[0] = preALU[1];
                preALU[1] = 0;
            }
        }

    };

    

    struct mem{
        void run(int preMem[], items MEM[], int PC, int R[], int postMem) {
            if(preMem[0] != 0) {
                for(int i = 0; i < 2; i++) {
                    if(postMem != 0) break;
                    items I = MEM[preMem[i]];
                    //if SW
                    if(I.opcode == 43) {
                        MEM[I.rs + I.imm].imm = R[I.rt];
                        preMem[i] = 0;
                    }
                    //LW
                    if(I.opcode == 35) {
                        postMem = I.rt; //destination
                        memValue = MEM[I.rs + I.imm].imm;
                        preMem[i] = 0;
                    }
                }
            }
        }
    };

    struct writeback {
        void run(items MEM[], int R[], int postalu, int postMem, int aluValue, int memValue) {
            if(postMem != 0) {
                R[postMem] = memValue;
            }
            else if(postalu != 0) {
                R[postalu] = aluValue;

            }
        }
    };
    
 fetch FETCH;
 issue ISSUE;
 alu ALU;
 mem MEMO;
 writeback WB;

 while (!didBreak) {
    WB.run(MEM, R, postALU, postMem, aluValue, memValue);
    MEMO.run(preMem, MEM, PC, R, postMem);
    ALU.run(preALU, MEM, PC, R, postALU);
    ISSUE.run(preIssue, preALU, preMem, MEM);
    FETCH.run(preIssue, didBreak, MEM, PC, R);

    // Print state
    simout << "====================\n";
    simout << "cycle:" << cycle << "\t" << PC - 4 << "\t" << MEM[PC - 4].instStr << "\n\n";

    simout << "registers:\n";
    simout << "r00:\t" << R[0] << "\t" << R[1] << "\t" << R[2] << "\t" << R[3] << "\t"
           << R[4] << "\t" << R[5] << "\t" << R[6] << "\t" << R[7] << "\n";
    simout << "r08:\t" << R[8] << "\t" << R[9] << "\t" << R[10] << "\t" << R[11] << "\t"
           << R[12] << "\t" << R[13] << "\t" << R[14] << "\t" << R[15] << "\n";
    simout << "r16:\t" << R[16] << "\t" << R[17] << "\t" << R[18] << "\t" << R[19] << "\t"
           << R[20] << "\t" << R[21] << "\t" << R[22] << "\t" << R[23] << "\n";
    simout << "r24:\t" << R[24] << "\t" << R[25] << "\t" << R[26] << "\t" << R[27] << "\t"
           << R[28] << "\t" << R[29] << "\t" << R[30] << "\t" << R[31] << "\n\n";

    simout << "data:\n";
    for (int i = dataStart; i <= dataEnd; i += 4) {
        simout << i << ":\t" << MEM[i].asUint;
        for (int j = 1; j < 4 && i + j * 4 <= dataEnd; ++j) {
            simout << "\t" << MEM[i + j * 4].asUint;
        }
        simout << "\n";
    }
    simout << "\n";

    cycle += 1;
}

 }