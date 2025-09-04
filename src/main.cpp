// main.cpp - Main compiler driver
#include "Lexer.h"
#include "Parser.h"
#include "CodeGen.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options] <input_file>\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help        Show this help message\n";
    std::cout << "  -t, --tokens      Print tokens and exit\n";
    std::cout << "  -a, --ast         Print AST (not implemented yet) and exit\n";
    std::cout << "  -i, --ir          Print LLVM IR and exit\n";
    std::cout << "  -o, --output      Specify output file for IR\n";
    std::cout << "  -r, --run         Compile and run with JIT\n";
}

int main(int argc, char* argv[]) {
    std::string inputFile;
    std::string outputFile;
    bool printTokens = false;
    bool printAST = false;
    bool printIR = false;
    bool runJIT = false;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-t" || arg == "--tokens") {
            printTokens = true;
        } else if (arg == "-a" || arg == "--ast") {
            printAST = true;
        } else if (arg == "-i" || arg == "--ir") {
            printIR = true;
        } else if (arg == "-r" || arg == "--run") {
            runJIT = true;
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                outputFile = argv[++i];
            } else {
                std::cerr << "Error: -o requires an output filename\n";
                return 1;
            }
        } else if (arg.front() != '-') {
            inputFile = arg;
        } else {
            std::cerr << "Error: Unknown option " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }
    
    if (inputFile.empty()) {
        std::cerr << "Error: No input file specified\n";
        printUsage(argv[0]);
        return 1;
    }
    
    try {
        // Read input file
        std::string sourceCode = readFile(inputFile);
        std::cout << "Compiling: " << inputFile << "\n\n";
        
        // Lexical analysis
        Lexer lexer(sourceCode);
        std::vector<Token> tokens = lexer.tokenize();
        
        if (printTokens) {
            std::cout << "=== TOKENS ===\n";
            for (const auto& token : tokens) {
                std::cout << "Line " << token.line << ", Col " << token.column 
                         << ": " << static_cast<int>(token.type) 
                         << " '" << token.value << "'\n";
            }
            return 0;
        }
        
        // Parsing
        Parser parser(tokens);
        std::unique_ptr<Program> ast = parser.parse();
        std::cout << "✓ Parsing completed successfully\n";
        
        if (printAST) {
            std::cout << "=== AST ===\n";
            std::cout << "AST pretty-printing not implemented yet\n";
            return 0;
        }
        
        // Code generation
        CodeGenerator codeGen;
        codeGen.generate(*ast);
        std::cout << "✓ Code generation completed successfully\n";
        
        if (printIR) {
            std::cout << "\n=== LLVM IR ===\n";
            codeGen.dumpIR();
            return 0;
        }
        
        if (!outputFile.empty()) {
            codeGen.writeIRToFile(outputFile);
            std::cout << "✓ IR written to " << outputFile << "\n";
        }
        
        if (runJIT) {
            std::cout << "\n=== EXECUTING WITH JIT ===\n";
            int result = codeGen.executeJIT();
            std::cout << "Program executed successfully\n";
            std::cout << "Return value: " << result << "\n";
        }
        
    } catch (const ParseError& e) {
        std::cerr << "Parse Error: " << e.what() << "\n";
        return 1;
    } catch (const CodeGenError& e) {
        std::cerr << "Code Generation Error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}