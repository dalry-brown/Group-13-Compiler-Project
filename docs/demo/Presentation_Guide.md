Demo Presentation Guide
5-10 Minute Demo Structure
1. Team Introductions (2 minutes)
Each member introduces themselves with cameras ON and states their contribution:
Member 1 (Lexer): "I implemented the lexical analyzer that tokenizes source code..."
Member 2 (Parser): "I built the recursive descent parser that creates the AST..."
Member 3 (AST): "I designed the abstract syntax tree and semantic analysis..."
Member 4 (CodeGen): "I implemented LLVM code generation and JIT execution..."
Member 5 (Testing): "I created the testing framework and integration pipeline..."
Team Leader: "I coordinated the project and implemented the main driver..."
2. Language Overview (1 minute)
Brief explanation of SimpleLang features:

Integer and boolean data types
Functions with parameters and returns
Control flow (if/else, while loops)
Arithmetic and logical operations

3. Live Demo (3-4 minutes)
Show actual compilation and execution:
bash# Show source code
cat demos/factorial.sl

# Show LLVM IR generation
./build/simplelang -i demos/factorial.sl

# Execute program
./build/simplelang -r demos/factorial.sl
Demonstrate multiple examples:

Factorial (recursion)
Loops (while loop)
Simple interest (real-world application)

4. Architecture Walkthrough (2-3 minutes)
Explain compilation pipeline:

Source → Lexer → Parser → AST → CodeGen → Execution
Highlight key design decisions
Show component integration

5. Q&A (1 minute)
Be prepared for questions about:

LLVM integration
Error handling
Language design choices
Team collaboration

Backup Plans

Pre-recorded terminal session if live demo fails
Static slides showing code and IR output
Individual component explanations if integration issues

Demo Programs
All demo programs are in demos/ directory:

factorial.sl - Recursive functions
fibonacci.sl - Complex recursion
simple_interest.sl - Real-world calculation
loops.sl - While loop demonstration
mathematics.sl - Mathematical operations

Expected Results

factorial(5) → 120
fibonacci(10) → 55
simple_interest → 150
sum_to_n(10) → 55
power(2, 8) → 256
EOF