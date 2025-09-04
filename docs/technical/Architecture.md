SimpleLang Compiler Architecture
Overview
The SimpleLang compiler follows a traditional multi-phase design:
Source Code → Lexer → Parser → AST → CodeGen → LLVM IR → Executable
Component Details
1. Lexical Analyzer (Lexer)
Files: src/Token.h, src/Lexer.h/cpp
Responsibility: Convert source code into tokens
Key Features:

Multi-character operator recognition
Line/column tracking for error reporting
Comment handling
Keyword recognition

Token Types:

Literals (NUMBER, IDENTIFIER)
Keywords (VAR, FUNCTION, IF, etc.)
Operators (+, -, *, /, ==, etc.)
Delimiters ((, ), {, }, ;, etc.)

2. Parser
Files: src/Parser.h/cpp
Responsibility: Convert tokens into Abstract Syntax Tree
Key Features:

Recursive descent parsing
Operator precedence handling
Error recovery
Syntax validation

Grammar Implementation:

Expression parsing with precedence climbing
Statement parsing for all language constructs
Function declaration and call parsing

3. Abstract Syntax Tree (AST)
Files: src/AST.h/cpp
Responsibility: Represent program structure
Key Features:

Visitor pattern for traversal
Type-safe node hierarchy
Memory management with smart pointers

Node Types:

Expression nodes (literals, variables, operations)
Statement nodes (declarations, assignments, control flow)
Program structure (functions, blocks)

4. Code Generator
Files: src/CodeGen.h/cpp
Responsibility: Convert AST to LLVM IR
Key Features:

LLVM IR generation
Symbol table management
Function compilation
JIT execution support

LLVM Integration:

Uses LLVM IRBuilder for code generation
Handles basic blocks and control flow
Supports function calls and recursion
Provides JIT compilation

Build System
File: CMakeLists.txt

CMake-based build configuration
LLVM library detection and linking
C++17 standard enforcement

Testing Framework
Files: tests/run_tests.sh

Automated test execution
Integration testing
Regression prevention
EOF

4. API Documentation
cat > docs/technical/API_Reference.md << 'EOF'
SimpleLang Compiler API Reference
Lexer API
Token Class
cppstruct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
};
Lexer Class
cppclass Lexer {
public:
    Lexer(const std::string& input);
    Token nextToken();
    std::vector<Token> tokenize();
    bool isAtEnd();
};
Parser API
Parser Class
cppclass Parser {
public:
    Parser(std::vector<Token> tokens);
    std::unique_ptr<Program> parse();
};
AST Visitor Pattern
cppclass ASTVisitor {
public:
    virtual void visit(NumberLiteral& node) = 0;
    virtual void visit(BinaryOperation& node) = 0;
    // ... other visit methods
};
Code Generator API
CodeGenerator Class
cppclass CodeGenerator : public ASTVisitor {
public:
    CodeGenerator();
    void generate(Program& program);
    void dumpIR();
    void writeIRToFile(const std::string& filename);
    int executeJIT();
};
Usage Examples
Basic Compilation
cpp#include "Lexer.h"
#include "Parser.h"
#include "CodeGen.h"

std::string source = "function main() { return 42; }";

// Lexical analysis
Lexer lexer(source);
auto tokens = lexer.tokenize();

// Parsing
Parser parser(tokens);
auto ast = parser.parse();

// Code generation
CodeGenerator codegen;
codegen.generate(*ast);
int result = codegen.executeJIT();
Error Handling
cpptry {
    // Compilation steps...
} catch (const ParseError& e) {
    std::cerr << "Parse Error: " << e.what() << std::endl;
} catch (const CodeGenError& e) {
    std::cerr << "Code Generation Error: " << e.what() << std::endl;
}