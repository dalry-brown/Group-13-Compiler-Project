// Parser.h - Recursive descent parser
#pragma once
#include "AST.h"
#include "Token.h"
#include "Lexer.h"
#include <stdexcept>

class ParseError : public std::runtime_error {
public:
    ParseError(const std::string& msg) : std::runtime_error(msg) {}
};

class Parser {
private:
    std::vector<Token> tokens;
    size_t current;
    
    bool isAtEnd();
    Token peek();
    Token previous();
    Token advance();
    bool check(TokenType type);
    bool match(std::initializer_list<TokenType> types);
    Token consume(TokenType type, const std::string& message);
    
    // Expression parsing (precedence climbing)
    std::unique_ptr<Expression> expression();
    std::unique_ptr<Expression> logicalOr();
    std::unique_ptr<Expression> logicalAnd();
    std::unique_ptr<Expression> equality();
    std::unique_ptr<Expression> comparison();
    std::unique_ptr<Expression> term();
    std::unique_ptr<Expression> factor();
    std::unique_ptr<Expression> unary();
    std::unique_ptr<Expression> call();
    std::unique_ptr<Expression> primary();
    
    // Statement parsing
    std::unique_ptr<Statement> statement();
    std::unique_ptr<Statement> varDeclaration();
    std::unique_ptr<Statement> assignment();
    std::unique_ptr<Statement> ifStatement();
    std::unique_ptr<Statement> whileStatement();
    std::unique_ptr<Statement> functionDeclaration();
    std::unique_ptr<Statement> returnStatement();
    std::unique_ptr<Statement> expressionStatement();
    std::unique_ptr<Block> block();
    
public:
    Parser(std::vector<Token> tokens);
    std::unique_ptr<Program> parse();
};