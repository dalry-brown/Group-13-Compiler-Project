// CodeGen.h - LLVM Code Generator
#pragma once
#include "AST.h"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <unordered_map>
#include <memory>

class CodeGenError : public std::runtime_error {
public:
    CodeGenError(const std::string& msg) : std::runtime_error(msg) {}
};

class CodeGenerator : public ASTVisitor {
private:
    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::Module> module;
    std::unique_ptr<llvm::IRBuilder<>> builder;
    
    // Symbol table for variables
    std::unordered_map<std::string, llvm::AllocaInst*> namedValues;
    
    // Function symbol table
    std::unordered_map<std::string, llvm::Function*> functions;
    
    // Current function being compiled
    llvm::Function* currentFunction;
    
    // Value from last expression evaluation
    llvm::Value* lastValue;
    
    // Helper methods
    llvm::AllocaInst* createEntryBlockAlloca(llvm::Function* function, const std::string& varName);
    llvm::Type* getType(const std::string& typeName);
    
public:
    CodeGenerator();
    ~CodeGenerator() = default;
    
    void generate(Program& program);
    void dumpIR();
    void writeIRToFile(const std::string& filename);
    int executeJIT();
    
    // Visitor methods
    void visit(NumberLiteral& node) override;
    void visit(BooleanLiteral& node) override;
    void visit(Variable& node) override;
    void visit(BinaryOperation& node) override;
    void visit(UnaryOperation& node) override;
    void visit(FunctionCall& node) override;
    void visit(VariableDeclaration& node) override;
    void visit(Assignment& node) override;
    void visit(IfStatement& node) override;
    void visit(WhileStatement& node) override;
    void visit(Block& node) override;
    void visit(FunctionDeclaration& node) override;
    void visit(ReturnStatement& node) override;
    void visit(ExpressionStatement& node) override;
    void visit(Program& node) override;
};