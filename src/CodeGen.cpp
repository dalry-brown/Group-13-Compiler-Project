// CodeGen.cpp - Complete LLVM Code Generator with bug fixes
#include "CodeGen.h"
#include <iostream>

CodeGenerator::CodeGenerator() {
    // Initialize LLVM
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
    
    context = std::make_unique<llvm::LLVMContext>();
    module = std::make_unique<llvm::Module>("SimpleLang", *context);
    builder = std::make_unique<llvm::IRBuilder<>>(*context);
    
    currentFunction = nullptr;
    lastValue = nullptr;
}

llvm::AllocaInst* CodeGenerator::createEntryBlockAlloca(llvm::Function* function, const std::string& varName) {
    llvm::IRBuilder<> tmpBuilder(&function->getEntryBlock(), function->getEntryBlock().begin());
    return tmpBuilder.CreateAlloca(llvm::Type::getInt32Ty(*context), nullptr, varName);
}

llvm::Type* CodeGenerator::getType(const std::string& typeName) {
    if (typeName == "int") {
        return llvm::Type::getInt32Ty(*context);
    } else if (typeName == "bool") {
        return llvm::Type::getInt1Ty(*context);
    }
    return llvm::Type::getVoidTy(*context);
}

void CodeGenerator::generate(Program& program) {
    program.accept(*this);
}

void CodeGenerator::dumpIR() {
    module->print(llvm::outs(), nullptr);
}

void CodeGenerator::writeIRToFile(const std::string& filename) {
    std::error_code EC;
    llvm::raw_fd_ostream dest(filename, EC, llvm::sys::fs::OF_None);
    
    if (EC) {
        throw CodeGenError("Could not open file: " + EC.message());
    }
    
    module->print(dest, nullptr);
}

int CodeGenerator::executeJIT() {
    // Verify the module
    std::string errorStr;
    if (llvm::verifyModule(*module, &llvm::errs())) {
        throw CodeGenError("Module verification failed");
    }
    
    // Create execution engine
    llvm::ExecutionEngine* executionEngine = llvm::EngineBuilder(std::unique_ptr<llvm::Module>(module.release()))
        .setErrorStr(&errorStr)
        .create();
    
    if (!executionEngine) {
        throw CodeGenError("Failed to create execution engine: " + errorStr);
    }
    
    // Look for main function
    llvm::Function* mainFunction = executionEngine->FindFunctionNamed("main");
    if (!mainFunction) {
        throw CodeGenError("Main function not found");
    }
    
    // Execute main function
    std::vector<llvm::GenericValue> args;
    llvm::GenericValue result = executionEngine->runFunction(mainFunction, args);
    
    delete executionEngine;
    return result.IntVal.getSExtValue();
}

void CodeGenerator::visit(NumberLiteral& node) {
    lastValue = llvm::ConstantInt::get(*context, llvm::APInt(32, node.value, true));
}

void CodeGenerator::visit(BooleanLiteral& node) {
    lastValue = llvm::ConstantInt::get(*context, llvm::APInt(1, node.value ? 1 : 0, false));
}

void CodeGenerator::visit(Variable& node) {
    llvm::AllocaInst* alloca = namedValues[node.name];
    if (!alloca) {
        throw CodeGenError("Unknown variable name: " + node.name);
    }
    
    // Load the value
    lastValue = builder->CreateLoad(alloca->getAllocatedType(), alloca, node.name.c_str());
}

void CodeGenerator::visit(BinaryOperation& node) {
    node.left->accept(*this);
    llvm::Value* left = lastValue;
    
    node.right->accept(*this);
    llvm::Value* right = lastValue;
    
    if (!left || !right) {
        throw CodeGenError("Invalid operands for binary operation");
    }
    
    if (node.operator_ == "+") {
        lastValue = builder->CreateAdd(left, right, "addtmp");
    } else if (node.operator_ == "-") {
        lastValue = builder->CreateSub(left, right, "subtmp");
    } else if (node.operator_ == "*") {
        lastValue = builder->CreateMul(left, right, "multmp");
    } else if (node.operator_ == "/") {
        lastValue = builder->CreateSDiv(left, right, "divtmp");
    } else if (node.operator_ == "<") {
        lastValue = builder->CreateICmpSLT(left, right, "cmptmp");
    } else if (node.operator_ == "<=") {
        lastValue = builder->CreateICmpSLE(left, right, "cmptmp");
    } else if (node.operator_ == ">") {
        lastValue = builder->CreateICmpSGT(left, right, "cmptmp");
    } else if (node.operator_ == ">=") {
        lastValue = builder->CreateICmpSGE(left, right, "cmptmp");
    } else if (node.operator_ == "==") {
        lastValue = builder->CreateICmpEQ(left, right, "cmptmp");
    } else if (node.operator_ == "!=") {
        lastValue = builder->CreateICmpNE(left, right, "cmptmp");
    } else if (node.operator_ == "&&") {
        lastValue = builder->CreateAnd(left, right, "andtmp");
    } else if (node.operator_ == "||") {
        lastValue = builder->CreateOr(left, right, "ortmp");
    } else {
        throw CodeGenError("Unknown binary operator: " + node.operator_);
    }
}

void CodeGenerator::visit(UnaryOperation& node) {
    node.operand->accept(*this);
    llvm::Value* operand = lastValue;
    
    if (!operand) {
        throw CodeGenError("Invalid operand for unary operation");
    }
    
    if (node.operator_ == "-") {
        lastValue = builder->CreateNeg(operand, "negtmp");
    } else if (node.operator_ == "!") {
        lastValue = builder->CreateNot(operand, "nottmp");
    } else {
        throw CodeGenError("Unknown unary operator: " + node.operator_);
    }
}

void CodeGenerator::visit(FunctionCall& node) {
    llvm::Function* calleeFunction = functions[node.name];
    if (!calleeFunction) {
        throw CodeGenError("Unknown function referenced: " + node.name);
    }
    
    // Check argument count
    if (calleeFunction->arg_size() != node.arguments.size()) {
        throw CodeGenError("Incorrect number of arguments passed to function: " + node.name);
    }
    
    std::vector<llvm::Value*> args;
    for (auto& arg : node.arguments) {
        arg->accept(*this);
        args.push_back(lastValue);
        if (!lastValue) {
            throw CodeGenError("Invalid argument in function call");
        }
    }
    
    lastValue = builder->CreateCall(calleeFunction, args, "calltmp");
}

void CodeGenerator::visit(VariableDeclaration& node) {
    llvm::Function* function = builder->GetInsertBlock()->getParent();
    
    // Create alloca in entry block
    llvm::AllocaInst* alloca = createEntryBlockAlloca(function, node.name);
    
    // Generate initializer if present
    llvm::Value* initValue = nullptr;
    if (node.initializer) {
        node.initializer->accept(*this);
        initValue = lastValue;
    } else {
        // Default initialize to 0
        initValue = llvm::ConstantInt::get(*context, llvm::APInt(32, 0, true));
    }
    
    builder->CreateStore(initValue, alloca);
    namedValues[node.name] = alloca;
    lastValue = nullptr; // Variable declarations don't return values
}

void CodeGenerator::visit(Assignment& node) {
    llvm::AllocaInst* variable = namedValues[node.name];
    if (!variable) {
        throw CodeGenError("Unknown variable name: " + node.name);
    }
    
    node.value->accept(*this);
    if (!lastValue) {
        throw CodeGenError("Invalid assignment value");
    }
    
    builder->CreateStore(lastValue, variable);
}

void CodeGenerator::visit(IfStatement& node) {
    node.condition->accept(*this);
    llvm::Value* conditionValue = lastValue;
    
    if (!conditionValue) {
        throw CodeGenError("Invalid if condition");
    }
    
    // Convert condition to boolean if necessary
    if (conditionValue->getType() != llvm::Type::getInt1Ty(*context)) {
        conditionValue = builder->CreateICmpNE(conditionValue,
                                               llvm::ConstantInt::get(*context, llvm::APInt(32, 0, true)),
                                               "ifcond");
    }
    
    llvm::Function* function = builder->GetInsertBlock()->getParent();
    
    // Create blocks
    llvm::BasicBlock* thenBlock = llvm::BasicBlock::Create(*context, "then", function);
    llvm::BasicBlock* elseBlock = node.elseBranch ? 
        llvm::BasicBlock::Create(*context, "else", function) : nullptr;
    llvm::BasicBlock* mergeBlock = llvm::BasicBlock::Create(*context, "ifcont", function);
    
    // Create conditional branch
    if (node.elseBranch) {
        builder->CreateCondBr(conditionValue, thenBlock, elseBlock);
    } else {
        builder->CreateCondBr(conditionValue, thenBlock, mergeBlock);
    }
    
    // Generate then block
    builder->SetInsertPoint(thenBlock);
    node.thenBranch->accept(*this);
    
    // Only add branch if block doesn't already have a terminator
    if (!builder->GetInsertBlock()->getTerminator()) {
        builder->CreateBr(mergeBlock);
    }
    
    // Generate else block if present
    if (node.elseBranch) {
        builder->SetInsertPoint(elseBlock);
        node.elseBranch->accept(*this);
        
        // Only add branch if block doesn't already have a terminator
        if (!builder->GetInsertBlock()->getTerminator()) {
            builder->CreateBr(mergeBlock);
        }
    }
    
    // Continue with merge block
    builder->SetInsertPoint(mergeBlock);
    lastValue = nullptr; // If statements don't return values
}

void CodeGenerator::visit(WhileStatement& node) {
    llvm::Function* function = builder->GetInsertBlock()->getParent();
    
    llvm::BasicBlock* condBlock = llvm::BasicBlock::Create(*context, "whilecond", function);
    llvm::BasicBlock* bodyBlock = llvm::BasicBlock::Create(*context, "whilebody", function);
    llvm::BasicBlock* afterBlock = llvm::BasicBlock::Create(*context, "afterwhile", function);
    
    builder->CreateBr(condBlock);
    
    // Generate condition block
    builder->SetInsertPoint(condBlock);
    node.condition->accept(*this);
    llvm::Value* conditionValue = lastValue;
    
    if (!conditionValue) {
        throw CodeGenError("Invalid while condition");
    }
    
    // Convert condition to boolean if necessary
    if (conditionValue->getType() != llvm::Type::getInt1Ty(*context)) {
        conditionValue = builder->CreateICmpNE(conditionValue,
                                               llvm::ConstantInt::get(*context, llvm::APInt(32, 0, true)),
                                               "whilecond");
    }
    
    builder->CreateCondBr(conditionValue, bodyBlock, afterBlock);
    
    // Generate body block
    builder->SetInsertPoint(bodyBlock);
    node.body->accept(*this);
    
    // Only add branch if block doesn't already have a terminator
    if (!builder->GetInsertBlock()->getTerminator()) {
        builder->CreateBr(condBlock);
    }
    
    // Continue with after block
    builder->SetInsertPoint(afterBlock);
    lastValue = nullptr; // While statements don't return values
}

void CodeGenerator::visit(Block& node) {
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}

void CodeGenerator::visit(FunctionDeclaration& node) {
    // Create function type
    std::vector<llvm::Type*> paramTypes(node.parameters.size(), llvm::Type::getInt32Ty(*context));
    llvm::FunctionType* functionType = llvm::FunctionType::get(llvm::Type::getInt32Ty(*context), paramTypes, false);
    
    llvm::Function* function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, node.name, module.get());
    
    // Set parameter names
    unsigned idx = 0;
    for (auto& arg : function->args()) {
        arg.setName(node.parameters[idx++]);
    }
    
    functions[node.name] = function;
    
    // Create entry block
    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(*context, "entry", function);
    builder->SetInsertPoint(entryBlock);
    
    // Save current state
    std::unordered_map<std::string, llvm::AllocaInst*> oldNamedValues = namedValues;
    llvm::Function* oldCurrentFunction = currentFunction;
    currentFunction = function;
    
    // Create allocas for parameters
    namedValues.clear();
    for (auto& arg : function->args()) {
        llvm::AllocaInst* alloca = createEntryBlockAlloca(function, std::string(arg.getName()));
        builder->CreateStore(&arg, alloca);
        namedValues[std::string(arg.getName())] = alloca;
    }
    
    // Generate function body
    node.body->accept(*this);
    
    // If no explicit return, add return 0
    if (!builder->GetInsertBlock()->getTerminator()) {
        builder->CreateRet(llvm::ConstantInt::get(*context, llvm::APInt(32, 0, true)));
    }
    
    // Verify function
    if (llvm::verifyFunction(*function, &llvm::errs())) {
        function->eraseFromParent();
        throw CodeGenError("Function verification failed for: " + node.name);
    }
    
    // Restore state
    namedValues = oldNamedValues;
    currentFunction = oldCurrentFunction;
    lastValue = nullptr;
}

void CodeGenerator::visit(ReturnStatement& node) {
    if (node.value) {
        node.value->accept(*this);
        builder->CreateRet(lastValue);
    } else {
        builder->CreateRet(llvm::ConstantInt::get(*context, llvm::APInt(32, 0, true)));
    }
}

void CodeGenerator::visit(ExpressionStatement& node) {
    node.expression->accept(*this);
    // Expression statements evaluate but don't use the result
}

void CodeGenerator::visit(Program& node) {
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}