#ifndef EVALUATE_H
#define EVALUATE_H

#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <variant>
#include <unordered_map>
#include <vector>
#include <stack>
#include <memory>

// 定義變數型別
using namespace std ; 
struct Var;
using VarMap = std::unordered_map<std::string, Var>;

struct Var : public std::variant<int, float,double  , std::string, std::shared_ptr<VarMap>> {
    using std::variant<int, float, double ,  std::string, std::shared_ptr<VarMap>>::variant; // 繼承構造函數
};




// Bytecode 指令類型
enum Bytecode_type {
    POP_SCOPE,
    PUSH_SCOPE,
    PUSH_FUNCTION,
    POP_FUNCTION,
    PUSH_FLAG,
    POP_FLAG,
    ADD,
    SUB,
    ASSIGN,
    DECLARE,
    MUL,
    DIV,
    MOD,
    JUMP,
    JUMP_IF_FALSE,
    JUMP_IF_TRUE,
    LOAD_CONST_L,
    LOAD_CONST_R,
    LOAD_CONST , 
    LOAD_VAR,
    STORE_VAR,
    CALL_FUNCTION,
    RETURN,
    PRINT,
    LOAD_MODULE,
    IMPORT,
    AND,
    OR,
    NOT,
    NEG,
    
    EQUAL,
    NOT_EQUAL,NOT_GREATER_THAN,NOT_LESS_THAN,SHIFT_LEFT,SHIFT_RIGHT,
    BITWISE_OR,
    BITWISE_AND,
    BITWISE_XOR,
    BITWISE_NOT,
    LESS_THAN,
    GREATER_THAN,
    PUSH_LOCAL,
    POP_LOCAL,
    CLEAR ,
    PUSH_ARGUMENT , 
    POP_ARGUMENT , 
};

// 類型分類（可選）
enum class DataType {
    NIL,
    INTEGER,
    FLOAT,
    BOOLEAN,
    STRING,
    DOUBLE,
    CHAR
};

// ByteCode 類別
class ByteCode {
public:
    Bytecode_type type;
    Var data;
    bool hasOperand;

    ByteCode(Bytecode_type t)
        : type(t), hasOperand(false) {}

    ByteCode(Bytecode_type t, const Var& op)
        : type(t), data(op), hasOperand(true) {}
};

// 內建函數表（暫定）
std::vector<std::vector<ByteCode>> Functions = { /* "print" 之類的可以這邊加 */ };

// 內建 print 函數（示意）


// 符號表清單（每層作用域一個 stack frame）


class Intepretor {
    private:
        std::string input;
        size_t index;
    public:
    
        vector<ByteCode> Instructions ; 
        void Code_Generation(shared_ptr<Node>) ; 
        

        Intepretor(const std::string& input);
    };

#endif
