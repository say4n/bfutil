/*
bf2c - A minimal Brainf**k to C translator
author - Sayan Goswami
usage - ./bfi <bf-program.bf> <c-code.c>
*/

#include <iostream>
#include <string>
#include <fstream>

#define MAXMEM 65536

void man() {
    printf("\n");
    printf("**********************************************\n");
    printf("* bf2c - A minimal Brainf**k to C translator *\n");
    printf("**********************************************\n");
    printf("Usage: [-d] <bf-program.bf> <c-code.c>\n");
    printf("Flags:\n");
    printf("\t-d\tEnable debug mode\n");
    printf("Example:\n\tbfi -d hello_world.bf\n");
    printf("\n");
    exit(-1);
}

std::string tabs(int tabLevel) {
    std::string tab;
    for(int i=0; i<tabLevel; i++) tab.push_back('\t');
    return tab;
}

int main(int argc, char** argv) {

    if(argc < 2) {
        man();
    }
    else {
        int idx = 1;
        std::string bf_source_path = argv[idx++];
        std::string c_source_path = argv[idx++];

        std::ifstream inFile(bf_source_path, std::ios::in);
        std::ofstream outFile(c_source_path, std::ios::out);

        if (!inFile.is_open() || !outFile.is_open()) {
            std::cerr << "Error opening file(s) .. exiting" << std::endl;
            exit(-1);
        }
        else {
            // write init code to outFile
            outFile << "#include<stdio.h>\n"
                    << "#include<stdlib.h>\n\n"
                    << "/* \n"
                    << "Generated automatically with bf2c\n"
                    << "Compile : gcc " << c_source_path << " -o out/main.out\n"
                    << "Execute : out/main.out \n"
                    << "*/\n\n"
                    << "int main() {\n"
                    << "\t// initialize cells\n"
                    << "\tunsigned char *cell = calloc(" << MAXMEM << ", sizeof(char));\n"
                    << "\tunsigned char *cells = cell;\n"
                    << "\n\tif (!cell) {\n"
                    << "\t\tfprintf(stderr, \"Error allocating memory.\\n\");\n"
                    << "\t\texit(-1);\n"
                    << "\t}\n"
                    << "\n\t// BF begin\n\n";

            // process bf input
            char curr;
            int tabLevel = 1;
            while (true) {
                curr = inFile.get();

                if (inFile.eof()) break;

                outFile << tabs(tabLevel);

                switch (curr) {
                    case '>':
                        outFile << "++cell;\n";
                        break;
                    case '<':
                        outFile << "--cell;\n";
                        break;
                    case '+':
                        outFile << "++*cell;\n";
                        break;
                    case '-':
                        outFile << "--*cell;\n";
                        break;
                    case '[':
                        tabLevel++;
                        outFile << "while (*cell) {\n";
                        break;
                    case ']':
                        tabLevel--;
                        outFile << "}\n";
                        break;
                    case ',':
                        outFile << "*cell=getchar();\n";
                        break;
                    case '.':
                        outFile << "putchar(*cell);\n";
                        break;
                    default:
                        std::cerr << "Illegal character " << int(curr) << std::endl;
                        break; 
                } 
            }

            // write final lines
            outFile << "\n\t// BF end\n"
                    << "\n\t// cleanup\n"
                    << "\tfree(cells);\n\n"
                    << "\treturn 0;\n"
                    << "}\n";
        }
    }

    return 0;
}
