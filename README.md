# SimpleLang Compiler

A complete compiler for a basic programming language built with LLVM, developed as an educational project for a compilers course.

## 📋 Table of Contents
- [Features](#features)
- [Quick Start](#quick-start)
- [Installation](#installation)
- [Usage](#usage)
- [Language Guide](#language-guide)
- [Demo Programs](#demo-programs)
- [Testing](#testing)
- [Architecture](#architecture)
- [Team Contributions](#team-contributions)
- [Demo Presentation](#demo-presentation)
- [Troubleshooting](#troubleshooting)

## 🚀 Features

- **Lexical Analysis**: Comprehensive tokenization with error reporting
- **Syntax Analysis**: Recursive descent parser with error recovery
- **Semantic Analysis**: Type checking and symbol resolution
- **Code Generation**: LLVM IR generation with optimization support
- **JIT Execution**: Run programs directly with LLVM's JIT compiler

## ⚡ Quick Start

### 5-Minute Setup
```bash
# 1. Install dependencies (Ubuntu/Debian)
sudo apt-get install llvm-15-dev llvm-15-tools clang-15 cmake build-essential

# 2. Clone and build
git clone https://github.com/dalry-brown/Group-13-Compiler-Project.git
cd SimpleLangCompiler
mkdir build && cd build
cmake ..
make

# 3. Test with a simple program
echo 'function main() { return 42; }' > test.sl
./simplelang -r test.sl
# Expected output: Return value: 42
🔧 Installation
Prerequisites

CMake 3.16+
LLVM 15+ development libraries
C++17 compatible compiler (GCC 7+, Clang 7+)

Platform-Specific Installation
Ubuntu/Debian
bashsudo apt-get update
sudo apt-get install llvm-15-dev llvm-15-tools clang-15 cmake build-essential git
macOS
bashbrew install llvm cmake git
export PATH="/opt/homebrew/opt/llvm/bin:$PATH"
Build Instructions
bashgit clone https://github.com/dalry-brown/Group-13-Compiler-Project.git
cd SimpleLangCompiler
mkdir build && cd build
cmake ..
make

# Verify installation
./simplelang --help
📖 Usage
Command Line Options
bash./simplelang [options] <input_file.sl>

Options:
  -h, --help        Show help message
  -r, --run         Compile and execute with JIT
  -i, --ir          Print LLVM IR to stdout
  -t, --tokens      Print token stream for debugging
  -o, --output      Save LLVM IR to file
Examples
bash# Compile and run a program
./simplelang -r demos/factorial.sl

# View generated LLVM IR
./simplelang -i demos/factorial.sl

# Debug tokenization
./simplelang -t demos/factorial.sl

# Save IR to file
./simplelang -o program.ll demos/factorial.sl
📝 Language Guide
Data Types

int: 32-bit signed integers (-2,147,483,648 to 2,147,483,647)
bool: Boolean values (true, false)

Variables
cppvar x = 10;           // Declaration with initialization
var flag = true;      // Boolean variable
x = x + 5;           // Assignment
Functions
cppfunction functionName(param1, param2) {
    // Function body
    return expression;
}

// Example
function add(a, b) {
    return a + b;
}
Control Flow
cpp// If statements
if (condition) {
    // code
} else {
    // code
}

// While loops
while (condition) {
    // code
}
Operators
CategoryOperatorsExampleArithmetic+, -, *, /x + y * zComparison==, !=, <, <=, >, >=x > 5Logical&&, ||, !(x > 0) && (y < 10)
Complete Example
cppfunction factorial(n) {
    if (n <= 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

function main() {
    return factorial(5);  // Returns 120
}
🎯 Demo Programs
Ready-to-run examples in the demos/ directory:
1. Factorial (Recursion)
bash./simplelang -r demos/factorial.sl
# Expected: Return value: 120
2. Fibonacci Sequence
bash./simplelang -r demos/fibonacci.sl
# Expected: Return value: 55 (10th Fibonacci number)
3. Simple Interest Calculator
bash./simplelang -r demos/simple_interest.sl
# Expected: Return value: 150 ($1000 at 5% for 3 years)
4. Loops and Variables
bash./simplelang -r demos/loops.sl
# Expected: Return value: 55 (sum of 1 to 10)
5. Mathematical Operations
bash./simplelang -r demos/mathematics.sl
# Expected: Return value: 256 (2^8)
🧪 Testing
Automated Test Suite
bash# Run all tests
./tests/run_tests.sh

# Expected output:
# ✅ PASS - Factorial (5! = 120)
# ✅ PASS - Fibonacci (fib(10) = 55)
# ✅ PASS - Simple Interest (SI = 150)
# ✅ PASS - Loops (sum = 55)
# ✅ PASS - Mathematics (2^8 = 256)
Manual Testing
bash# Test individual features
./simplelang -r demos/factorial.sl   # Test recursion
./simplelang -r demos/loops.sl       # Test loops
./simplelang -i demos/factorial.sl   # View generated IR
🏗️ Architecture
Compilation Pipeline
Source Code → Lexer → Parser → AST → CodeGen → LLVM IR → Executable
Component Overview

Lexer (src/Lexer.cpp): Tokenizes source code
Parser (src/Parser.cpp): Builds Abstract Syntax Tree
AST (src/AST.cpp): Represents program structure
CodeGen (src/CodeGen.cpp): Generates LLVM IR
Main (src/main.cpp): Compiler driver

Project Structure
SimpleLangCompiler/
├── src/                    # Source code
│   ├── Token.h            # Token definitions
│   ├── Lexer.h/cpp        # Lexical analyzer
│   ├── AST.h/cpp          # Abstract syntax tree
│   ├── Parser.h/cpp       # Recursive descent parser
│   ├── CodeGen.h/cpp      # LLVM code generator
│   └── main.cpp           # Main driver
├── demos/                 # Example programs
│   ├── factorial.sl       # Recursion example
│   ├── fibonacci.sl       # Complex recursion
│   ├── loops.sl           # While loop demo
│   ├── mathematics.sl     # Mathematical operations
│   └── simple_interest.sl # Real-world calculation
├── tests/                 # Test suite
│   └── run_tests.sh       # Automated tests
├── CMakeLists.txt         # Build configuration
└── README.md              # This file
👥 Team Contributions
Team Leader - Project Coordination
Responsibilities: Overall architecture, integration, main driver
Files: src/main.cpp, CMakeLists.txt, README.md
Key Achievements: Modular design, successful integration, project delivery
Member 1 - Lexical Analysis Expert
Responsibilities: Tokenization, error reporting, lexical analysis
Files: src/Token.h, src/Lexer.h, src/Lexer.cpp
Key Achievements: Comprehensive tokenization, precise error locations
Member 2 - Parser Expert
Responsibilities: Syntax analysis, recursive descent parsing
Files: src/Parser.h, src/Parser.cpp
Key Achievements: Complete grammar implementation, operator precedence
Member 3 - AST & Semantics Expert
Responsibilities: AST design, visitor pattern, semantic analysis
Files: src/AST.h, src/AST.cpp
Key Achievements: Clean AST hierarchy, type checking infrastructure
Member 4 - Code Generation Expert
Responsibilities: LLVM IR generation, JIT compilation
Files: src/CodeGen.h, src/CodeGen.cpp
Key Achievements: Complete IR generation, JIT execution support
Member 5 - Testing & Integration Expert
Responsibilities: Testing framework, integration, demo programs
Files: Test scripts, demo programs, CI/CD setup
Key Achievements: Comprehensive testing, demo program creation
🎬 Demo Presentation
Presentation Structure (5-10 minutes)
1. Team Introductions (2 minutes)
Each member introduces themselves and states their contribution (cameras ON).
2. Language Overview (1 minute)
Brief explanation of SimpleLang features and capabilities.
3. Live Demo (3-4 minutes)
bash# Show source code
cat demos/factorial.sl

# Show LLVM IR generation
./simplelang -i demos/factorial.sl

# Execute program
./simplelang -r demos/factorial.sl

# Demonstrate multiple examples
./simplelang -r demos/fibonacci.sl
./simplelang -r demos/simple_interest.sl
4. Architecture Walkthrough (2-3 minutes)
Explain compilation pipeline and component integration.
5. Q&A (1 minute)
Address questions about design decisions and implementation.
Demo Programs Results

factorial(5) → 120
fibonacci(10) → 55
simple_interest(1000, 5, 3) → 150
sum_to_n(10) → 55
power(2, 8) → 256

🚨 Troubleshooting
Common Build Issues
LLVM not found:
bash# Check LLVM installation
llvm-config --version
llvm-config --prefix

# Update CMakeLists.txt if using different version
Compilation errors:
bash# Clean rebuild
cd build && make clean
cmake ..
make VERBOSE=1  # See detailed output
Common Runtime Errors
Parse Error: Expected ';'
cpp// ❌ Missing semicolon
var x = 10

// ✅ Correct
var x = 10;
Code Generation Error: Unknown variable
cpp// ❌ Variable not declared
function main() { return x; }

// ✅ Declare before use
function main() { var x = 10; return x; }
Function verification failed:
This indicates an internal compiler bug. Check for:

Missing return statements in functions
Mismatched braces in control flow

Getting Help

Check that your program follows the language syntax
Test with simple examples first
Use ./simplelang -t to debug tokenization
Use ./simplelang -i to check parsing and IR generation

🎓 Learning Outcomes
This project demonstrates mastery of:

Compiler Design: Lexical analysis, parsing, code generation
LLVM Framework: IR generation and JIT compilation
Software Architecture: Modular design with clean interfaces
Team Collaboration: Coordinated development across components
Testing Strategy: Comprehensive validation and integration testing

🔮 Future Enhancements
Potential Improvements

String data type and string operations
Arrays and basic data structures
For loops and enhanced control flow
Standard library functions (print, input)
Optimization passes and better code generation
Enhanced error messages and recovery
IDE integration with syntax highlighting

📚 References

LLVM Language Reference
LLVM Tutorial
Crafting Interpreters by Robert Nystrom
Compilers: Principles, Techniques, and Tools (Dragon Book)


Built with ❤️ by Group 13 for Compilers Course
This project showcases a complete compiler implementation from source code to executable, demonstrating practical application of compiler design principles and modern development practices.
