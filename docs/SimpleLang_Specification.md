# SimpleLang Language Specification

## Overview
SimpleLang is a statically-typed imperative programming language designed for educational purposes. It features basic data types, control flow, and function definitions.

## Grammar (BNF)
program         → statement_list
statement_list  → statement*
statement       → var_decl | assignment | if_stmt | while_stmt | function_decl | return_stmt | expression_stmt | block
var_decl        → 'var' IDENTIFIER '=' expression ';'
assignment      → IDENTIFIER '=' expression ';'
if_stmt         → 'if' '(' expression ')' statement ('else' statement)?
while_stmt      → 'while' '(' expression ')' statement
function_decl   → 'function' IDENTIFIER '(' param_list? ')' block
return_stmt     → 'return' expression? ';'
expression_stmt → expression ';'
block           → '{' statement_list '}'
expression      → logical_or
logical_or      → logical_and ('||' logical_and)*
logical_and     → equality ('&&' equality)*
equality        → comparison (('==' | '!=') comparison)*
comparison      → term (('<' | '<=' | '>' | '>=') term)*
term            → factor (('+' | '-') factor)*
factor          → unary (('' | '/') unary)
unary           → ('-' | '!') unary | call
call            → primary ('(' arg_list? ')')*
primary         → NUMBER | IDENTIFIER | '(' expression ')' | 'true' | 'false'

## Data Types
- **int**: 32-bit signed integers (-2,147,483,648 to 2,147,483,647)
- **bool**: Boolean values (true/false)

## Keywords
- `var`: Variable declaration
- `function`: Function declaration
- `if`, `else`: Conditional statements
- `while`: Loop statement
- `return`: Return from function
- `true`, `false`: Boolean literals

## Operators
### Arithmetic
- `+`: Addition
- `-`: Subtraction
- `*`: Multiplication
- `/`: Division (integer division)

### Comparison
- `==`: Equal to
- `!=`: Not equal to
- `<`: Less than
- `<=`: Less than or equal to
- `>`: Greater than
- `>=`: Greater than or equal to

### Logical
- `&&`: Logical AND
- `||`: Logical OR
- `!`: Logical NOT

## Example Programs
See `demos/` directory for complete examples.
EOF

# 2. User Manual
cat > docs/User_Manual.md << 'EOF'
# SimpleLang Compiler User Manual

## Installation

### Prerequisites
- LLVM 15+ development libraries
- CMake 3.16+
- C++17 compatible compiler

### Ubuntu/Debian Installation
```bash
sudo apt-get install llvm-15-dev llvm-15-tools clang-15 cmake build-essential
Building the Compiler
bashgit clone <repository-url>
cd SimpleLangCompiler
mkdir build && cd build
cmake ..
make
Usage
Command Line Options
bash./simplelang [options] <input_file>

Options:
  -h, --help        Show help message
  -t, --tokens      Print tokens and exit
  -a, --ast         Print AST and exit  
  -i, --ir          Print LLVM IR and exit
  -o, --output      Specify output file for IR
  -r, --run         Compile and run with JIT
Examples
bash# Compile and run a program
./simplelang -r program.sl

# View generated LLVM IR
./simplelang -i program.sl

# Save IR to file
./simplelang -o output.ll program.sl

# Debug tokenization
./simplelang -t program.sl
Writing SimpleLang Programs
Basic Program Structure
cppfunction main() {
    return 0;
}
Variables
cppvar x = 10;
var y = x + 5;
Functions
cppfunction add(a, b) {
    return a + b;
}

function main() {
    var result = add(5, 3);
    return result;
}
Control Flow
cppfunction max(a, b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

function factorial(n) {
    var result = 1;
    var i = 1;
    while (i <= n) {
        result = result * i;
        i = i + 1;
    }
    return result;
}
Error Handling
The compiler provides detailed error messages with line and column information:
Parse Error: Line 3, Column 5: Expected ';' after variable declaration. Got 'x'
Limitations

No string data type
No arrays or pointers
No standard library functions
No floating-point arithmetic
Integer division only
EOF