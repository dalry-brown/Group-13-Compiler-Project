// AST.h - Abstract Syntax Tree node definitions
#pragma once
#include <memory>
#include <vector>
#include <string>

// Forward declarations for visitor pattern
class ASTVisitor;

// Base AST Node
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
};

// Expression base class
class Expression : public ASTNode {
public:
    virtual ~Expression() = default;
};

// Statement base class  
class Statement : public ASTNode {
public:
    virtual ~Statement() = default;
};

// Expression nodes
class NumberLiteral : public Expression {
public:
    int value;
    NumberLiteral(int val) : value(val) {}
    void accept(ASTVisitor& visitor) override;
};

class BooleanLiteral : public Expression {
public:
    bool value;
    BooleanLiteral(bool val) : value(val) {}
    void accept(ASTVisitor& visitor) override;
};

class Variable : public Expression {
public:
    std::string name;
    Variable(const std::string& n) : name(n) {}
    void accept(ASTVisitor& visitor) override;
};

class BinaryOperation : public Expression {
public:
    std::unique_ptr<Expression> left;
    std::string operator_;
    std::unique_ptr<Expression> right;
    
    BinaryOperation(std::unique_ptr<Expression> l, const std::string& op, std::unique_ptr<Expression> r)
        : left(std::move(l)), operator_(op), right(std::move(r)) {}
    void accept(ASTVisitor& visitor) override;
};

class UnaryOperation : public Expression {
public:
    std::string operator_;
    std::unique_ptr<Expression> operand;
    
    UnaryOperation(const std::string& op, std::unique_ptr<Expression> expr)
        : operator_(op), operand(std::move(expr)) {}
    void accept(ASTVisitor& visitor) override;
};

class FunctionCall : public Expression {
public:
    std::string name;
    std::vector<std::unique_ptr<Expression>> arguments;
    
    FunctionCall(const std::string& n, std::vector<std::unique_ptr<Expression>> args)
        : name(n), arguments(std::move(args)) {}
    void accept(ASTVisitor& visitor) override;
};

// Statement nodes
class VariableDeclaration : public Statement {
public:
    std::string name;
    std::unique_ptr<Expression> initializer;
    
    VariableDeclaration(const std::string& n, std::unique_ptr<Expression> init)
        : name(n), initializer(std::move(init)) {}
    void accept(ASTVisitor& visitor) override;
};

class Assignment : public Statement {
public:
    std::string name;
    std::unique_ptr<Expression> value;
    
    Assignment(const std::string& n, std::unique_ptr<Expression> val)
        : name(n), value(std::move(val)) {}
    void accept(ASTVisitor& visitor) override;
};

class IfStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> thenBranch;
    std::unique_ptr<Statement> elseBranch;
    
    IfStatement(std::unique_ptr<Expression> cond, std::unique_ptr<Statement> then, 
                std::unique_ptr<Statement> else_ = nullptr)
        : condition(std::move(cond)), thenBranch(std::move(then)), elseBranch(std::move(else_)) {}
    void accept(ASTVisitor& visitor) override;
};

class WhileStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> body;
    
    WhileStatement(std::unique_ptr<Expression> cond, std::unique_ptr<Statement> b)
        : condition(std::move(cond)), body(std::move(b)) {}
    void accept(ASTVisitor& visitor) override;
};

class Block : public Statement {
public:
    std::vector<std::unique_ptr<Statement>> statements;
    
    Block(std::vector<std::unique_ptr<Statement>> stmts)
        : statements(std::move(stmts)) {}
    void accept(ASTVisitor& visitor) override;
};

class FunctionDeclaration : public Statement {
public:
    std::string name;
    std::vector<std::string> parameters;
    std::unique_ptr<Block> body;
    
    FunctionDeclaration(const std::string& n, std::vector<std::string> params, std::unique_ptr<Block> b)
        : name(n), parameters(std::move(params)), body(std::move(b)) {}
    void accept(ASTVisitor& visitor) override;
};

class ReturnStatement : public Statement {
public:
    std::unique_ptr<Expression> value;
    
    ReturnStatement(std::unique_ptr<Expression> val = nullptr)
        : value(std::move(val)) {}
    void accept(ASTVisitor& visitor) override;
};

class ExpressionStatement : public Statement {
public:
    std::unique_ptr<Expression> expression;
    
    ExpressionStatement(std::unique_ptr<Expression> expr)
        : expression(std::move(expr)) {}
    void accept(ASTVisitor& visitor) override;
};

class Program : public ASTNode {
public:
    std::vector<std::unique_ptr<Statement>> statements;
    
    Program(std::vector<std::unique_ptr<Statement>> stmts)
        : statements(std::move(stmts)) {}
    void accept(ASTVisitor& visitor) override;
};

// Visitor interface
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    virtual void visit(NumberLiteral& node) = 0;
    virtual void visit(BooleanLiteral& node) = 0;
    virtual void visit(Variable& node) = 0;
    virtual void visit(BinaryOperation& node) = 0;
    virtual void visit(UnaryOperation& node) = 0;
    virtual void visit(FunctionCall& node) = 0;
    virtual void visit(VariableDeclaration& node) = 0;
    virtual void visit(Assignment& node) = 0;
    virtual void visit(IfStatement& node) = 0;
    virtual void visit(WhileStatement& node) = 0;
    virtual void visit(Block& node) = 0;
    virtual void visit(FunctionDeclaration& node) = 0;
    virtual void visit(ReturnStatement& node) = 0;
    virtual void visit(ExpressionStatement& node) = 0;
    virtual void visit(Program& node) = 0;
};