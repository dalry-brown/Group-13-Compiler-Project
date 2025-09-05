# README.md - Project Documentation
# SimpleLang Compiler

A simple compiler for a basic programming language built with LLVM, developed as a educational project for a compilers course.

## Features

- **Lexical Analysis**: Tokenizes source code with proper error reporting
- **Syntax Analysis**: Recursive descent parser with error recovery
- **Semantic Analysis**: Basic type checking and symbol resolution
- **Code Generation**: LLVM IR generation with optimization support
- **JIT Execution**: Run programs directly with LLVM's JIT compiler

## Language Features

### Data Types
- `int`: 32-bit signed integers
- `bool`: Boolean values (true/false)

### Language Constructs
- Variable declarations: `var x = 10;`
- Function definitions: `function name(params) { ... }`
- Control flow: `if/else`, `while` loops
- Expressions: Arithmetic, logical, comparison operators
- Function calls with parameters and return values

### Sample Program
```
function factorial(n) {
    if (n <= 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

function main() {
    return factorial(5);
}
```

## Building the Compiler

### Prerequisites
- CMake 3.16 or higher
- LLVM 15 or higher development packages
- C++17 compatible compiler (GCC 7+, Clang 7+, MSVC 2019+)

### Build Instructions

1. **Install LLVM** (Ubuntu/Debian):
   ```bash
   sudo apt-get install llvm-15-dev llvm-15-tools clang-15
   ```

2. **Clone and build**:
   ```bash
   git clone https://github.com/dalry-brown/Group-13-Compiler-Project.git
   cd SimpleLangCompiler
   mkdir build && cd build
   cmake ..
   make
   ```

### Usage

```bash
# Compile and show tokens
./simplelang -t test.sl

# Compile and show LLVM IR
./simplelang -i test.sl

# Compile and run with JIT
./simplelang -r test.sl

# Compile and save IR to file
./simplelang -o output.ll test.sl

# Show help
./simplelang --help
```

## Project Structure

```
SimpleLangCompiler/
├── demos/
│   ├── factorial.sl
│   ├── fibonacci.sl
│   ├── loops.sl
│   ├── mathematics.sl
│   └── simple_interest.sl
├── src/
│   ├── Token.h           # Token definitions
│   ├── Lexer.h/cpp       # Lexical analyzer
│   ├── AST.h/cpp         # Abstract syntax tree
│   ├── Parser.h/cpp      # Recursive descent parser
│   ├── CodeGen.h/cpp     # LLVM code generator
│   └── main.cpp          # Main driver
├── tests/
│   └── run_tests.sh
├── CMakeLists.txt
└── README.md
```

## Team Contributions

### Member 1: Lexical Analysis
- Implemented tokenizer with comprehensive token types
- Added line/column tracking for error reporting
- Created lexer test suite and error handling
- **Files**: `Token.h`, `Lexer.h/cpp`

### Member 2: Syntax Analysis  
- Designed and implemented recursive descent parser
- Created expression parsing with operator precedence
- Implemented statement parsing (declarations, control flow)
- **Files**: `Parser.h/cpp`

### Member 3: Abstract Syntax Tree
- Designed AST node hierarchy with visitor pattern
- Implemented semantic analysis and type checking
- Created AST pretty-printer for debugging
- **Files**: `AST.h/cpp`

### Member 4: Code Generation
- Implemented LLVM IR generation for all language constructs
- Added JIT compilation and execution support
- Created optimization passes and IR verification
- **Files**: `CodeGen.h/cpp`

### Member 5: Testing and Integration
- Designed comprehensive test suite
- Created integration testing framework
- Implemented build system and CI/CD
- **Files**: `CMakeLists.txt`, test programs, documentation

### Team Leader: Project Coordination
- Overall architecture design and integration
- Main compiler driver implementation
- Documentation and presentation preparation
- **Files**: `main.cpp`, `README.md`

## Testing

Run the test script:

```bash
# Run test scripts
cd tests
./run_tests.sh

```

Expected outputs:<br>

- === SimpleLang Compiler Test Suite ===<br>

- Test 1: Factorial (5! = 120)<br>
- ✅ PASS<br>
- Test 2: Fibonacci (fib(10) = 55)<br>
- ✅ PASS<br>
- Test 3: Simple Interest (SI = 150)<br>
- ✅ PASS<br>
- Test 4: Loops - Sum 1 to 10 (sum = 55)<br>
- ✅ PASS<br>
- Test 5: Mathematics - Power (2^8 = 256)<br>
- ✅ PASS<br>

- === Test Summary ===<br>
- Total tests: 5<br>
- All tests completed!<br>


## Demo Presentation Structure

### 1. Team Introductions (2 minutes)
Each member introduces themselves and their specific contribution to the project.

### 2. Language Overview (1 minute)
Brief explanation of SimpleLang features and syntax.

### 3. Live Compilation Demo (3-4 minutes)
- Show source code for factorial program
- Compile with `./simplelang -i test3_factorial.sl` to show LLVM IR
- Execute with `./simplelang -r test3_factorial.sl` to show result
- Demonstrate error handling with intentionally broken code

### 4. Architecture Walkthrough (2-3 minutes)
- Explain compilation pipeline: Lexer → Parser → AST → CodeGen
- Show how components interact
- Highlight key design decisions

### 5. Q&A and Wrap-up (1 minute)
Address any questions and summarize achievements.

## Learning Outcomes

This project demonstrates:
- **Compiler Design**: Understanding of lexical analysis, parsing, and code generation
- **LLVM Integration**: Practical experience with LLVM IR and JIT compilation
- **Software Architecture**: Modular design with clear separation of concerns
- **Team Collaboration**: Coordinated development across multiple components
- **Testing Strategy**: Comprehensive testing from unit to integration levels

## Future Enhancements

Potential improvements for future iterations:
- String data type support
- Arrays and pointers
- More advanced control flow (for loops, break/continue)
- Standard library functions
- Optimization passes
- Better error messages and recovery
- IDE integration with syntax highlighting

## References

- LLVM Language Reference: https://llvm.org/docs/LangRef.html
- LLVM Tutorial: https://llvm.org/docs/tutorial/
- Crafting Interpreters by Robert Nystrom
- Compilers: Principles, Techniques, and Tools (Dragon Book)