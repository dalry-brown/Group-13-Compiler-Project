// AST.cpp - Implementation of AST visitor pattern methods
#include "AST.h"

// Expression node visitor implementations
void NumberLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void BooleanLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void Variable::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void BinaryOperation::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void UnaryOperation::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void FunctionCall::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// Statement node visitor implementations
void VariableDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void Assignment::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void IfStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void WhileStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void Block::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void FunctionDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void ReturnStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void ExpressionStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void Program::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}