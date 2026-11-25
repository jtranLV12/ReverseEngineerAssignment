#include <iostream>
#include <fstream>

using namespace std;

struct Instructions {
    string inst;
    int code;
    int functionCode;
};
struct RegTable {
    string reg;
    int address;
};

string getInstruction (int opcode, int funcCode, Instructions instructions[], int size) {
    for (int i = 0; i < size; i++) {
        if (instructions[i].code == opcode && instructions[i].functionCode == funcCode) {
            return instructions[i].inst;
        }
    }
    return "Error";
};

string getRegister (int address, RegTable registers[], int size) {
    for (int i = 0; i < size; i++) {
        if (registers[i].address == address) {
            return registers[i].reg;
        }
    }
    return "Error";
};

int main(int argc, char** argv) {
	if (argc < 2) {
		cout << "ERROR - missing file name\n";
		return 1;
	}
   Instructions instr[5] {
        {"add", 0, 32},
        {"addi", 8, 0},
        {"sub", 0, 34},
        {"lw", 35, 0}, 
        {"sw", 43, 0}
   };
   RegTable registers[6] {  // There are 6 registers listed not 5
        {"$t0", 8},
        {"$t1", 9},
        {"$t2", 10},
        {"$0", 0}, 
        {"$s0", 16}, 
        {"$s1", 17}
   };
   ifstream file(argv[1]);
   if (!file.is_open()) {
        cout << "Error: Cannot open file." << endl;
        return 1;
   }
   while (!file.eof()) {
        string line;
        while(getline(file, line)) {
            unsigned int num = stoul(line, nullptr, 16);
            unsigned int opcode = (num >> 26) & 0x3F;
            if (opcode == 0) {
                unsigned int rs = (num >> 21) & 0x1F;
                unsigned int rt = (num >> 16) & 0x1F;
                unsigned int rd = (num >> 11) & 0x1F;
                unsigned int funcCode = num & 0x3F;

                string opNum = getInstruction(opcode, funcCode, instr, 5);
                string rsNum = getRegister(rs, registers, 6);
                string rtNum = getRegister(rt, registers, 6);
                string rdNum = getRegister(rd, registers, 6);
                cout << opNum << " " << rdNum << ", " << rsNum << ", " << rtNum;
                cout << endl;
            }
            else if (opcode == 8 || opcode == 23 || opcode == 43) {
                unsigned int rs = (num >> 21) & 0x1F;
                unsigned int rt = (num >> 16) & 0x1F;
                unsigned int immediate = num & 0xFFFF;

                string opNum = getInstruction(opcode, 0, instr, 5);
                string rsNum = getRegister(rs, registers, 6);
                string rtNum = getRegister(rt, registers, 6);
                if (opcode == 8) {
                    cout << opNum << " " << rtNum << ", " << rsNum << ", " << immediate;
                }
                else {
                    cout << opNum << " " << rtNum << ", " << immediate << "(" << rsNum << ")";
                }
                cout << endl;
            }
        }
   }

   return 0;
}