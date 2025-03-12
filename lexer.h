#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <memory>
#include <vector>
#include <cctype>
#include <iostream>

enum class TokenType {
    NUMBER,     // 数字，如 123, 456
    BOOL,
    STRING,
    OPERATOR,   // 运算符，如 +, -, *, /
    IDENTIFIER, // 变量名或函数名，如 x, sum
    FOR,        // 关键字 for
    WHILE,      // 关键字 while
    IF,         // 关键字 if
    ELSE,       // 关键字 else
    ELIF,       // 关键字 elif (Python)
    BRACE_OPEN , 
    BRACE_CLOSE , 
    BRACKET_OPEN , 
    BRACKET_CLOSE,
    PAREN_OPEN, // 左括号 (
    PAREN_CLOSE,// 右括号 )
    END,        // 结束标记
    SYMBOL,
    UNKNOWN     // 无法识别的字符
};




enum class Operator {
    ADD ,
    SUB , 
    MUL , 
    DIV , 
    MOD , 
    POW ,
    AND ,
    XOR , 
    NOT ,
    EQUAL ,  
    NOT_EQUAL , 
    LESS_THAN , 
    GREATER_THAN , 
    NOT_LESS_THAN, // 大於等於
    NOT_GREATER_THAN ,  // 小於等於
    SHIFT_LEFT , 
    SHIFT_RIGHT
};


class Token {
    public:
        virtual ~Token() = default;  // Virtual destructor for proper cleanup
        virtual void print() const = 0;  // Pure virtual function to print the token
};
  

class NumberToken : public Token {
    public:
        TokenType token_type = TokenType::NUMBER;
        int value; 
    
        NumberToken(int value) : value(value) {}
};


class BoolToken : public Token {
    public:
        TokenType token_type = TokenType::BOOL; 
        bool value; 
    
        BoolToken(bool value) : value(value) {}
};

class StringToken : public Token {
    public:
        TokenType token_type = TokenType::STRING; 
        string value; 
    
        StringToken(string value) : value(value) {}
};

class OperatorToken : public Token {
    public:
        TokenType token_type = TokenType::BOOL; 
        Operator value; 
    
        OperatorToken(Operator value) : value(value) {}
};



class IdentifierToken : public Token {
    public:
        TokenType token_type = TokenType::IDENTIFIER;  
        string identifier;  // 变量名
        int value;  // 变量的值（假设是整数）
    
        IdentifierToken(const string& name, int val) 
            : identifier(name), value(val) {}
};

class ForToken : public Token {
    public:
        
        TokenType token_type = TokenType::FOR;  // 设置 token_type 为 FOR
        
    };
    
    // while 关键字类型的 Token
    class WhileToken : public Token {
    public:
        TokenType token_type = TokenType::WHILE;  // 设置 token_type 为 WHILE
        
    };
    
    // if 关键字类型的 Token
    class IfToken : public Token {
    public:
        TokenType token_type = TokenType::IF;  // 设置 token_type 为 IF
        
    };
    
    // else 关键字类型的 Token
    class ElseToken : public Token {
    public:
        TokenType token_type = TokenType::ELSE;  // 设置 token_type 为 ELSE
        
    };
    
    // elif 关键字类型的 Token
    class ElifToken : public Token {
    public:
        TokenType token_type = TokenType::ELIF;  // 设置 token_type 为 ELIF
        
    };
    
    // 括号类型的 Token
    class BraceOpenToken : public Token {
    public:
        TokenType token_type = TokenType::BRACE_OPEN;  // 设置 token_type 为 BRACE_OPEN
        
    };
    
    class BraceCloseToken : public Token {
    public:
        TokenType token_type = TokenType::BRACE_CLOSE;  // 设置 token_type 为 BRACE_CLOSE
        
    };
    
    class BracketOpenToken : public Token {
    public:
        TokenType token_type = TokenType::BRACKET_OPEN;  // 设置 token_type 为 BRACKET_OPEN
        
    };
    
    class BracketCloseToken : public Token {
    public:
        TokenType token_type = TokenType::BRACKET_CLOSE;  // 设置 token_type 为 BRACKET_CLOSE
        
    };
    
    class ParenOpenToken : public Token {
    public:
        TokenType token_type = TokenType::PAREN_OPEN;  // 设置 token_type 为 PAREN_OPEN
        
    };
    
    class ParenCloseToken : public Token {
    public:
        TokenType token_type = TokenType::PAREN_CLOSE;  // 设置 token_type 为 PAREN_CLOSE
        
    };
    
    // 结束标记类型的 Token
    class EndToken : public Token {
    public:
        TokenType token_type = TokenType::END;  // 设置 token_type 为 END
        
    };
    
    // 无法识别的字符类型的 Token
    class UnknownToken : public Token {
    public:

        TokenType token_type = TokenType::UNKNOWN;  // 设置 token_type 为 UNKNOWN

    };

    class Lexer {
        public:
            explicit Lexer(const std::string& input);
            std::vector<std::unique_ptr<Token>> tokenize();  // Tokenize the input string
            void printTokensToStream(std::stringstream& ss); // Function to print tokens to stringstream
        
        private:
            std::string input;  // Input source code
            size_t index;       // Current index in input
            char currentChar(); // Get the current character
            void advance();     // Move to the next character
            bool isEnd();       // Check if the input is finished
        };
#endif // LEXER_H
