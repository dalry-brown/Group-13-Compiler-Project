// Lexer.h - Lexical analyzer
#pragma once
#include "Token.h"
#include <string>
#include <vector>
#include <unordered_map>

class Lexer {
private:
    std::string input;
    size_t current;
    int line;
    int column;
    
    // Keywords map
    std::unordered_map<std::string, TokenType> keywords;
    
    void initKeywords();
    char peek(int offset = 0);
    char advance();
    void skipWhitespace();
    void skipComment();
    Token makeNumber();
    Token makeIdentifier();
    Token makeString();
    
public:
    Lexer(const std::string& input);
    Token nextToken();
    std::vector<Token> tokenize();
    bool isAtEnd();
};