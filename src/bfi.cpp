/*
bfinterp - Brainf**k Interpreter
author - Sayan Goswami
usage - ./bfi [-d] <bf-program.bf>
*/

#include <iostream>
#include <string>
#include <fstream>

#define MAXMEM 65536

bool DEBUG;

class bfi {
    private:
        std::string code;
        int memPtr, codePtr, memory[MAXMEM], maxMemPtr = 0;

        void printMemoryDump() {
            std::cerr << "\nmaxMemPtr: " << this->maxMemPtr;

            if (this->maxMemPtr > 0) {
                std::cerr << "\nMemory dump: ";
                for (int i=0; i < this->maxMemPtr; i++) {
                    std::cerr << i << " : " << this->memory[i] << "\t";
                    if (i % 10 == 0) std::cout << std::endl;
                }
            }
        }

    public:
        bfi(std::string program) {
            if (DEBUG) std::cout << "Initializing ... ";
            
            // assign code
            this->code = program;

            // init memory
            for(int i=0; i<MAXMEM; i++) this->memory[i] = 0;

            this->memPtr = 0;
            this->codePtr = -1;
            if (DEBUG) std::cout << "DONE\n";
        }

        void printCode() {
            std::cout << "Program: " << this->code << std::endl;
        }

        void skipToEndOfLoop() {
            int parenCount = 0;
            while(true) {
                switch(this->code[this->codePtr]) {
                    case '[':
                        parenCount++;
                        break;
                    case ']':
                        parenCount--;
                        if(parenCount == 0)
                            return;
                        break;
                }
                this->codePtr++;
            }
        }

        void goToBeginningOfLoop() {
            int parenCount = 0;
            while(true) {
                switch(this->code[this->codePtr]) {
                    case ']':
                        parenCount++;
                        break;
                    case '[':
                        parenCount--;
                        if(parenCount == 0)
                            return;
                        break;
                }
                this->codePtr--;
            }
        }

        void run() {

            if (DEBUG) std::cout << "Running ... ";

            char curr;

            while (true) {
                curr = this->code[++this->codePtr];

                if (this->memPtr < 0) {
                    std::cerr << "Underflow Error!" << std::endl;
                    exit(-4);
                }

                switch(curr) {
                    case '>':
                        this->memPtr++;
                        this->maxMemPtr = std::max(this->memPtr, this->maxMemPtr);
                        break;
                    case '<':
                        this->memPtr--;
                        break;
                    case '+':
                        this->memory[this->memPtr]++;
                        break;
                    case '-':
                        this->memory[this->memPtr]--;
                        break;
                    case '[':
                        if (this->memory[this->memPtr] == 0)  skipToEndOfLoop();
                        break;
                    case ']':
                        if (this->memory[this->memPtr] != 0) goToBeginningOfLoop();
                        break;
                    case ',':
                        std::cin >> this->memory[this->memPtr];
                        break;
                    case '.':
                        std::cout << char(this->memory[this->memPtr]);
                        break;
                    case '#':
                        // reached End Of Code
                        return;
                    default:
                        std::cerr << "Error parsing " << curr << " at " << this->codePtr << std::endl; 
                        exit(-3);
                }

                // if (DEBUG) printMemoryDump();
            }

            if (DEBUG) std::cout << "DONE\n";
        }
};

std::string readFile(std::string fileName) {
    
    std::ifstream inFile(fileName, std::ios::in);
    
    std::string code;
    int lineCount = 0, letterCount = 0;
    char c;

    if (!inFile.is_open()) {
        std::cerr << "Error opening " << fileName << std::endl;
        exit(-1);
    }
    else {
        while (true) {
            inFile >> c;

            if(inFile.eof()) {
                // mark End of Code
                code.push_back('#');
                break;
            }

            switch (c) {
                case '>':
                case '<':
                case '+':
                case '-':
                case '[':
                case ']':
                case ',':
                case '.':
                    // store any of the above  
                    letterCount++;
                    code.push_back(c);
                    break;
                case '\n':
                case '\r':
                    letterCount = 0;
                    lineCount++;
                case ' ':
                case '\t':
                    // ignore characters not in spec
                    break;
                default:
                    if (DEBUG) std::cerr << "Illegal character " << c
                                    << " at " << lineCount << ":" << letterCount
                                    << " in " << fileName << std::endl;
            }
        }

        inFile.close();

        return code;
    }
}


int main(int argc, char **argv) {
    DEBUG = false;

    if(argc == 1) {
        printf("\n");
        printf("*******************************************\n");
        printf("* bfinterp - A tiny Brainf**k Interpreter *\n");
        printf("*******************************************\n");
        printf("Usage: bfi [-d] <bf-program>\n");
        printf("Flags:\n");
        printf("\t-d\tEnable debug mode\n");
        printf("Example:\n\tbfi -d hello_world.bf\n");
        printf("\n");
        return -1;
    }
    
    else {
        int idx = 1;
        if (strcmp(argv[idx], "-d") == 0) {
            DEBUG = true;
            idx++;
        }

        std::string program = readFile(argv[idx]);

        bfi interpreter = bfi(program);

        if (DEBUG) interpreter.printCode();

        interpreter.run();
    }

    return 0;
}