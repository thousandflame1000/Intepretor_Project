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
struct nil {
    bool operator==(const nil&) const { return true; }  // 所有 nil 都相等
};







struct Var;


struct env {
    shared_ptr<env> parent = nullptr ;  
    unordered_map<string ,  Var> VARIABLE_TABLE  ;
    bool is_root = false ; 
   // env(bool root = false) : is_root(root) {}
}; 


struct function {
    int ptr; 
    shared_ptr<env> parent ; 
    string name ; 
} ;





using table = std::unordered_map<Var, Var>;
struct Var : public std::variant< bool ,int, float, double, std::string, std::shared_ptr<table>, nil, function> {
    using std::variant< bool , int, float, double, std::string,std::shared_ptr<table>, nil, function>::variant; // 繼承構造函數
 
    // 相等比較
    bool operator==(const Var& other) const {
        return std::visit([](auto&& a, auto&& b) -> bool {
            using A = std::decay_t<decltype(a)>;
            using B = std::decay_t<decltype(b)>;
            
            // 處理相同類型的比較
            if constexpr (std::is_same_v<A, B>) {
                if constexpr (std::is_same_v<A, std::shared_ptr<table>>)
                    return a == b;  // 比較 shared_ptr 指向的物件是否相等
                else if constexpr (std::is_same_v<A, function>)
                    return a.ptr == b.ptr;  // 比較 function 物件的 ptr 成員
                else if constexpr(std::is_same_v<A ,nil >)
                    return false ; 
                else
                    return a == b;  // 其他情況直接比較值
            } else {
                return false;  // 類型不匹配返回 false
            }
        }, *this, other);
    }
};

namespace std {
    template <>
    struct hash<Var> {
        std::size_t operator()(const Var& v) const {
            return std::visit([](auto&& value) -> std::size_t {
                using T = std::decay_t<decltype(value)>;
                if constexpr (std::is_arithmetic_v<T>)
                    return std::hash<double>{}(static_cast<double>(value));
                else if constexpr (std::is_same_v<T, std::string>)
                    return std::hash<std::string>{}(value);
                else if constexpr (std::is_same_v<T, std::shared_ptr<table>>)
                    return std::hash<void*>{}(value.get());
                else if constexpr (std::is_same_v<T, nil>)
                    return 0;
                else if constexpr (std::is_same_v<T, function>)
                    return std::hash<int>{}(value.ptr);
                else
                    return 0;
            }, v);
        }
    };
}
namespace std {
    template <>
    struct hash<std::weak_ptr<Var>> {
        std::size_t operator()(const std::weak_ptr<Var>& wp) const {
            auto sp = wp.lock();
            if (sp) {
                return std::hash<std::shared_ptr<Var>>{}(sp);
            }
            return 0;
        }
    };
    template <>
    struct equal_to<std::weak_ptr<Var>> {
        bool operator()(const std::weak_ptr<Var>& wp1, const std::weak_ptr<Var>& wp2) const {
            auto sp1 = wp1.lock();
            auto sp2 = wp2.lock();
            return sp1 == sp2;
        }
    };
}














// Bytecode 指令類型
enum Bytecode_type {
    POP_SCOPE,
    PUSH_SCOPE,
    PUSH_FUNCTION,
    POP_FUNCTION,
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
    LOAD_CONST , 
    LOAD_VAR,
    CALL_FUNCTION,
    DECLARE_FUNCTION , 
    RETURN,
    AND,
    OR,
    NOT,
    NEG,
    START_LOOP,
    END_LOOP,
    TO_SCOPE , 
//    GET_ENV ,
    ADD_ARGS_QUE,
    EQUAL,
    NOT_EQUAL,
    NOT_GREATER_THAN,
    NOT_LESS_THAN,
    SHIFT_LEFT,
    SHIFT_RIGHT,
    BITWISE_OR,
    BITWISE_AND,
    BITWISE_XOR,
    BITWISE_NOT,
    LESS_THAN,
    GREATER_THAN,
    CLEAR ,
    TABLE_ADDING ,
    MAKING_TABLE,
    CLEANING_TABLE , 
    PUSH_ARGUMENT , 
    POP_ARGUMENT , 
    CLEAN_ARGUMENT ,
    INDEX , 
    ADD_CONTINUE ,
    ADD_BREAK ,
//    ADD_RETURN, 
    PTR
};










// 類型分類（可選）
enum class DataType {
    NIL,
    INTEGER,
    FLOAT,
    BOOLEAN,
    STRING,
    DOUBLE
};

// ByteCode 類別
class ByteCode {
public:
    Bytecode_type type;
    Var data;
    bool hasOperand;
    int data2;
    int data3 ;
    int where  ; 
    shared_ptr<env> scope ; // 給函數用的
     
    ByteCode(Bytecode_type t)
        : type(t), hasOperand(false) {}

    ByteCode(Bytecode_type t, const Var& op)
        : type(t), data(op), hasOperand(true) {}
};

// 內建函數表（暫定）
//std::vector<std::vector<ByteCode>> Functions = { /* "print" 之類的可以這邊加 */ };

// 內建 print 函數（示意）


// 符號表清單（每層作用域一個 stack frame）


class Interpreter {
    private:
        std::string input;
        size_t index;
    public:
        vector<ByteCode> Instructions ; 
        void add_root_keywords ();
        void interupt_helper(int start , int end ) ; 
        void Code_Generation(shared_ptr<Node>) ; 
        void execute () ;         
        void Resolver () ; 
        void print_ins_type(Bytecode_type type);
        void printByteCode();
        Interpreter(const std::string input);
    };

#endif
