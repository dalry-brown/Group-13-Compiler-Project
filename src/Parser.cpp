// Parser.cpp - Parser implementation
#include "Parser.h"

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)), current(0) {}

bool Parser::isAtEnd() {
    return peek().type == TokenType::END_OF_FILE;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(std::initializer_list<TokenType> types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    
    Token current_token = peek();
    throw ParseError("Line " + std::to_string(current_token.line) + 
                    ", Column " + std::to_string(current_token.column) + 
                    ": " + message + ". Got '" + current_token.value + "'");
}

std::unique_ptr<Program> Parser::parse() {
    std::vector<std::unique_ptr<Statement>> statements;
    
    while (!isAtEnd()) {
        try {
            statements.push_back(statement());
        } catch (const ParseError& error) {
            // Error recovery: skip to next statement
            while (!isAtEnd() && peek().type != TokenType::SEMICOLON) {
                advance();
            }
            if (!isAtEnd()) advance(); // skip semicolon
            throw; // Re-throw for now, but could continue parsing
        }
    }
    
    return std::make_unique<Program>(std::move(statements));
}

std::unique_ptr<Statement> Parser::statement() {
    if (match({TokenType::VAR})) {
        return varDeclaration();
    }
    if (match({TokenType::IF})) {
        return ifStatement();
    }
    if (match({TokenType::WHILE})) {
        return whileStatement();
    }
    if (match({TokenType::FUNCTION})) {
        return functionDeclaration();
    }
    if (match({TokenType::RETURN})) {
        return returnStatement();
    }
    if (match({TokenType::LEFT_BRACE})) {
        return block();
    }
    
    // Check for assignment vs expression statement
    if (check(TokenType::IDENTIFIER)) {
        size_t saved_current = current;
        advance(); // consume identifier
        if (check(TokenType::ASSIGN)) {
            current = saved_current; // backtrack
            return assignment();
        }
        current = saved_current; // backtrack
    }
    
    return expressionStatement();
}

std::unique_ptr<Statement> Parser::varDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expected variable name");
    
    std::unique_ptr<Expression> initializer = nullptr;
    if (match({TokenType::ASSIGN})) {
        initializer = expression();
    }
    
    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration");
    return std::make_unique<VariableDeclaration>(name.value, std::move(initializer));
}

std::unique_ptr<Statement> Parser::assignment() {
    Token name = consume(TokenType::IDENTIFIER, "Expected variable name");
    consume(TokenType::ASSIGN, "Expected '='");
    std::unique_ptr<Expression> value = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after assignment");
    
    return std::make_unique<Assignment>(name.value, std::move(value));
}

std::unique_ptr<Statement> Parser::ifStatement() {
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'if'");
    std::unique_ptr<Expression> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after if condition");
    
    std::unique_ptr<Statement> thenBranch = statement();
    std::unique_ptr<Statement> elseBranch = nullptr;
    
    if (match({TokenType::ELSE})) {
        elseBranch = statement();
    }
    
    return std::make_unique<IfStatement>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<Statement> Parser::whileStatement() {
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'while'");
    std::unique_ptr<Expression> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after while condition");
    std::unique_ptr<Statement> body = statement();
    
    return std::make_unique<WhileStatement>(std::move(condition), std::move(body));
}

std::unique_ptr<Statement> Parser::functionDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expected function name");
    
    consume(TokenType::LEFT_PAREN, "Expected '(' after function name");
    std::vector<std::string> parameters;
    
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            Token param = consume(TokenType::IDENTIFIER, "Expected parameter name");
            parameters.push_back(param.value);
        } while (match({TokenType::COMMA}));
    }
    
    consume(TokenType::RIGHT_PAREN, "Expected ')' after parameters");
    
    consume(TokenType::LEFT_BRACE, "Expected '{' before function body");
    std::unique_ptr<Block> body = block();
    
    return std::make_unique<FunctionDeclaration>(name.value, std::move(parameters), std::move(body));
}

std::unique_ptr<Statement> Parser::returnStatement() {
    std::unique_ptr<Expression> value = nullptr;
    
    if (!check(TokenType::SEMICOLON)) {
        value = expression();
    }
    
    consume(TokenType::SEMICOLON, "Expected ';' after return value");
    return std::make_unique<ReturnStatement>(std::move(value));
}

std::unique_ptr<Statement> Parser::expressionStatement() {
    std::unique_ptr<Expression> expr = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression");
    return std::make_unique<ExpressionStatement>(std::move(expr));
}

std::unique_ptr<Block> Parser::block() {
    std::vector<std::unique_ptr<Statement>> statements;
    
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(statement());
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' after block");
    return std::make_unique<Block>(std::move(statements));
}

// Expression parsing with precedence climbing
std::unique_ptr<Expression> Parser::expression() {
    return logicalOr();
}

std::unique_ptr<Expression> Parser::logicalOr() {
    std::unique_ptr<Expression> expr = logicalAnd();
    
    while (match({TokenType::LOGICAL_OR})) {
        std::string operator_ = previous().value;
        std::unique_ptr<Expression> right = logicalAnd();
        expr = std::make_unique<BinaryOperation>(std::move(expr), operator_, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::logicalAnd() {
    std::unique_ptr<Expression> expr = equality();
    
    while (match({TokenType::LOGICAL_AND})) {
        std::string operator_ = previous().value;
        std::unique_ptr<Expression> right = equality();
        expr = std::make_unique<BinaryOperation>(std::move(expr), operator_, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::equality() {
    std::unique_ptr<Expression> expr = comparison();
    
    while (match({TokenType::NOT_EQUAL, TokenType::EQUAL})) {
        std::string operator_ = previous().value;
        std::unique_ptr<Expression> right = comparison();
        expr = std::make_unique<BinaryOperation>(std::move(expr), operator_, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::comparison() {
    std::unique_ptr<Expression> expr = term();
    
    while (match({TokenType::GREATER_THAN, TokenType::GREATER_EQUAL, 
                  TokenType::LESS_THAN, TokenType::LESS_EQUAL})) {
        std::string operator_ = previous().value;
        std::unique_ptr<Expression> right = term();
        expr = std::make_unique<BinaryOperation>(std::move(expr), operator_, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::term() {
    std::unique_ptr<Expression> expr = factor();
    
    while (match({TokenType::MINUS, TokenType::PLUS})) {
        std::string operator_ = previous().value;
        std::unique_ptr<Expression> right = factor();
        expr = std::make_unique<BinaryOperation>(std::move(expr), operator_, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::factor() {
    std::unique_ptr<Expression> expr = unary();
    
    while (match({TokenType::DIVIDE, TokenType::MULTIPLY})) {
        std::string operator_ = previous().value;
        std::unique_ptr<Expression> right = unary();
        expr = std::make_unique<BinaryOperation>(std::move(expr), operator_, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::unary() {
    if (match({TokenType::LOGICAL_NOT, TokenType::MINUS})) {
        std::string operator_ = previous().value;
        std::unique_ptr<Expression> right = unary();
        return std::make_unique<UnaryOperation>(operator_, std::move(right));
    }
    
    return call();
}

std::unique_ptr<Expression> Parser::call() {
    std::unique_ptr<Expression> expr = primary();
    
    while (true) {
        if (match({TokenType::LEFT_PAREN})) {
            // Function call
            if (auto var = dynamic_cast<Variable*>(expr.get())) {
                std::string function_name = var->name;
                std::vector<std::unique_ptr<Expression>> arguments;
                
                if (!check(TokenType::RIGHT_PAREN)) {
                    do {
                        arguments.push_back(expression());
                    } while (match({TokenType::COMMA}));
                }
                
                consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments");
                expr = std::make_unique<FunctionCall>(function_name, std::move(arguments));
            } else {
                throw ParseError("Only identifiers can be called as functions");
            }
        } else {
            break;
        }
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::primary() {
    if (match({TokenType::TRUE})) {
        return std::make_unique<BooleanLiteral>(true);
    }
    
    if (match({TokenType::FALSE})) {
        return std::make_unique<BooleanLiteral>(false);
    }
    
    if (match({TokenType::NUMBER})) {
        int value = std::stoi(previous().value);
        return std::make_unique<NumberLiteral>(value);
    }
    
    if (match({TokenType::IDENTIFIER})) {
        return std::make_unique<Variable>(previous().value);
    }
    
    if (match({TokenType::LEFT_PAREN})) {
        std::unique_ptr<Expression> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after expression");
        return expr;
    }
    
    Token current_token = peek();
    throw ParseError("Line " + std::to_string(current_token.line) + 
                    ", Column " + std::to_string(current_token.column) + 
                    ": Unexpected token '" + current_token.value + "'");
}

