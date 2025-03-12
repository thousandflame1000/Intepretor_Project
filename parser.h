#ifndef PARSER_H
#define PARSER_H


#include<string> 
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <stack>


using namespace std ; 

typedef class AST {
public:
    virtual ~AST() = default;
};





class OperatorNode : public AST {
public:
    string op;  
    unique_ptr<AST> left;  
    unique_ptr<AST> right; 

    OperatorNode(const string& op, unique_ptr<AST> left, unique_ptr<AST> right)
        : op(op), left(move(left)), right(move(right)) {}
};

class NumberNode : public AST {
public:
    int value;

    NumberNode(int value) : value(value) {}
};


class BoolNode : public AST {
    public:
        bool value;
    
        BoolNode(int value) : value(value) {}
};

class ConditionNode : public AST {
public:
    string op ;
    unique_ptr <AST> left ;
    unique_ptr <AST> right ; 

    ConditionNode(const string op , unique_ptr<AST> left , unique_ptr<AST> right ) 
    :op(op) , left(move(left)) , right(move(right)) {}
    


};





class IdentifierNode : public AST {
public:
    string name;

    IdentifierNode(const string& name) : name(name) {}
};


class ConditionNode : public AST {
    public:
        unique_ptr<AST> condition;
        explicit ConditionNode(unique_ptr<AST> condition) : condition(move(condition)) {}
    };
    
    // if 语句
class IfNode : public AST {
    public:
        unique_ptr<AST> condition;
        vector<unique_ptr<AST>> body;
        vector<unique_ptr<AST>> elseBody;
    
        IfNode(unique_ptr<AST> condition, vector<unique_ptr<AST>> body, vector<unique_ptr<AST>> elseBody = {})
            : condition(move(condition)), body(move(body)), elseBody(move(elseBody)) {}
    };
    
    // while 语句
class WhileNode : public AST {
    public:
        unique_ptr<AST> condition;
        vector<unique_ptr<AST>> body;
    
        WhileNode(unique_ptr<AST> condition, vector<unique_ptr<AST>> body)
            : condition(move(condition)), body(move(body)) {}
    };
    
    // for 语句
class ForNode : public AST {
    public:
        unique_ptr<AST> init;
        unique_ptr<AST> condition;
        unique_ptr<AST> update;
        vector<unique_ptr<AST>> body;
    
        ForNode(unique_ptr<AST> init, unique_ptr<AST> condition, unique_ptr<AST> update, vector<unique_ptr<AST>> body)
            : init(move(init)), condition(move(condition)), update(move(update)), body(move(body)) {}
    };

class BlockNode : public AST {
public:
    vector<unique_ptr<AST>> statements;

    void addStatement(unique_ptr<AST> stmt) {
        statements.push_back(move(stmt));
    }
};




#endif PARSER_H