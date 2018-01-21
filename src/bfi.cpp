/*
bfinterp - Brainf**k Interpreter
author - Sayan Goswami
usage - ./bfi [-d] <bf-program.bf>
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#define MAX_MEMORY 65536
#define END_OF_CODE '$'
#define START_OF_CODE '#'

bool DEBUG;

class bfi {
    private:
        std::string code;
        std::vector<char> memory;
        
        std::string::iterator codePtr;
        std::vector<char>::iterator memPtr;

        void printMemoryDump() {
            if (this->memory.size() > 0) {
                std::cerr << "\nMemory dump: ";
                for (int i=0; i < this->memory.size(); i++) {
                    std::cerr << i << " : " << this->memory[i] << "\t";
                    if (i % 4 == 0) std::cout << std::endl;
                }
            }
        }

    public:
        bfi(std::string program) {
            // assign code
            this->code = program;

            // reserve memory
            this->memory.reserve(MAX_MEMORY);

            // init iterators
            this->memPtr = this->memory.begin();
            this->codePtr = this->code.begin();
        }

        void printCode() {
            // std::cout << "\nDist: \t" << std::distance(this->memory.begin(), this->memPtr) << "\n\n";
            std::cout << "Program: " << this->code << std::endl;
        }

        void skipToEndOfLoop() {
            int parenCount = 0;
            while(true) {
                switch(*this->codePtr) {
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
                switch(*this->codePtr) {
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

            if (DEBUG) std::cout << "Running ... \n";

            while (true) {

                if (DEBUG) std::cout << "Parsing " << *this->codePtr 
                                     << " at " << std::distance(this->code.begin(), this->codePtr)
                                     << std::endl;

                if (this->memPtr < this->memory.begin()) {
                    std::cerr << "Memory underflow: "
                              << "tried to access position " << std::distance(this->memory.begin(), this->memPtr)
                              << " out of " << this->memory.size() << " elements."
                              << std::endl;
                    exit(-1);
                }

                switch(*this->codePtr) {
                    case START_OF_CODE:
                        // beginning of code
                        this->memory.push_back(0);
                        break;
                    case '>':
                        this->memory.push_back(0);
                        ++this->memPtr;
                        break;
                    case '<':
                        --this->memPtr;
                        break;
                    case '+':
                        ++*this->memPtr;
                        break;
                    case '-':
                        --*this->memPtr;
                        break;
                    case '[':
                        if (*this->memPtr == 0) skipToEndOfLoop();
                        break;
                    case ']':
                        if (*this->memPtr != 0) goToBeginningOfLoop();
                        break;
                    case ',':
                        std::cin >> *this->memPtr;
                        break;
                    case '.':
                        std::cout << *this->memPtr;
                        break;
                    case END_OF_CODE:
                        // reached End Of Code
                        return;
                    default:
                        std::cerr << "Error parsing " << *this->codePtr 
                                  << " at " << std::distance(this->codePtr, this->code.begin())
                                  << std::endl; 
                        exit(-3);
                }

                ++this->codePtr;

                // if (DEBUG) printMemoryDump();
            }

            // reset iterators
            this->memPtr = this->memory.begin();
            this->codePtr = this->code.begin();

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

        // start of code
        code.push_back(START_OF_CODE);

        while (true) {
            inFile >> c;

            if(inFile.eof()) {
                // mark End of Code
                code.push_back(END_OF_CODE);
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