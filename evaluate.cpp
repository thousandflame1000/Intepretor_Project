#include "parser.h"
#include "lexer.h"
#include "evaluate.h"
#include <stack>
#include <unordered_map>
#include <variant> 


using namespace std ; 






std::unordered_map<string , int> Identifier_GET_number  = 
{{"print" , 0} , {"int" , 1} , {"float" , 2 },
 {"float" , 3} , {"string" , 4 }  , {"co",5} , 

} ; 

unordered_map< int , std::stack<VarMap>> Identifiers_list;


stack<unordered_map< int ,  bool>> Scope;  


Intepretor::Intepretor(const std::string& input) : input(input), index(0) {}







void Intepretor::Code_Generation(shared_ptr<Node> node) {

    if (node->type == NodeType::BLOCK)
    {

        Instructions.push_back(ByteCode(Bytecode_type::PUSH_SCOPE)) ; 
        for (int i = 0  ; i < node->children.size() ; i++){
            Code_Generation(node->children[i]) ; 
        }
        Instructions.push_back(ByteCode(Bytecode_type::PUSH_SCOPE)) ; 

    }else if (node->type == NodeType::BINARY)
    {
        
        Code_Generation(node->children[0]) ; 
        Code_Generation(node->children[1]) ; 
        if (node->value == "ADD") {
            Instructions.push_back(ByteCode(Bytecode_type::ADD));
        } else if (node->value == "SUB") {
            Instructions.push_back(ByteCode(Bytecode_type::SUB));
        } else if (node->value == "MUL") {
            Instructions.push_back(ByteCode(Bytecode_type::MUL));
        } else if (node->value == "DIV") {
            Instructions.push_back(ByteCode(Bytecode_type::DIV));
        } else if (node->value == "MOD") {
            Instructions.push_back(ByteCode(Bytecode_type::MOD));
        } else if (node->value == "EQUAL") {
            Instructions.push_back(ByteCode(Bytecode_type::EQUAL));
        } else if (node->value == "NOT_EQUAL") {
            Instructions.push_back(ByteCode(Bytecode_type::NOT_EQUAL));
        } else if (node->value == "LESS_THAN") {
            Instructions.push_back(ByteCode(Bytecode_type::LESS_THAN));
        } else if (node->value == "GREATER_THAN") {
            Instructions.push_back(ByteCode(Bytecode_type::GREATER_THAN));
        } else if (node->value == "NOT_GREATER_THAN") {
            Instructions.push_back(ByteCode(Bytecode_type::NOT_GREATER_THAN));
        } else if (node->value == "NOT_LESS_THAN") {
            Instructions.push_back(ByteCode(Bytecode_type::NOT_LESS_THAN));
        } else if (node->value == "AND") {
            Instructions.push_back(ByteCode(Bytecode_type::AND));
        } else if (node->value == "OR") {
            Instructions.push_back(ByteCode(Bytecode_type::OR));
        } else if (node->value == "NOT") {
            Instructions.push_back(ByteCode(Bytecode_type::NOT));
        } else if (node->value == "BITWISE_AND") {
            Instructions.push_back(ByteCode(Bytecode_type::BITWISE_AND));
        } else if (node->value == "BITWISE_OR") {
            Instructions.push_back(ByteCode(Bytecode_type::BITWISE_OR));
        } else if (node->value == "BITWISE_XOR") {
            Instructions.push_back(ByteCode(Bytecode_type::BITWISE_XOR));
        } else if (node->value == "BITWISE_NOT") {
            Instructions.push_back(ByteCode(Bytecode_type::BITWISE_NOT));
        } else if (node->value == "SHIFT_LEFT") {
            Instructions.push_back(ByteCode(Bytecode_type::SHIFT_LEFT));
        } else if (node->value == "SHIFT_RIGHT") {
            Instructions.push_back(ByteCode(Bytecode_type::SHIFT_RIGHT));
        } else {
            std::cerr << "Unknown binary operator: " << node->value << std::endl;
        }


    }else if (node->type ==NodeType::UNARY)
    {
        Code_Generation(node->children[0]) ; 
        if (node->value == "NEG")
        {
            Instructions.push_back(ByteCode(Bytecode_type::NEG)) ; 
        }else if (node->value == "NOT")
        {
            Instructions.push_back(ByteCode(Bytecode_type::NOT)) ; 
        }else if (node->value == "BITWISE_NOT")
        {
            Instructions.push_back(ByteCode(Bytecode_type::BITWISE_NOT)) ; 
        }

    }
    else if (node->type == NodeType::INT)
    {
        Instructions.push_back(ByteCode(Bytecode_type::LOAD_CONST  , stoi(node->value))) ; 
    }else if (node->type == NodeType::BOOL)
    {
        Instructions.push_back(ByteCode(Bytecode_type::LOAD_CONST  , node->value=="TRUE"?true:false)) ; 
    }else if (node ->type == NodeType::FLOAT)
    {
        Instructions.push_back(ByteCode(Bytecode_type::LOAD_CONST , stof(node->value))) ; 
    }else if (node ->type == NodeType::DOUBLE)
    {  
        Instructions.push_back(ByteCode(Bytecode_type::LOAD_CONST , stod(node->value))) ;     
    }else if (node ->type == NodeType::STRING)
    {
        Instructions.push_back(ByteCode(Bytecode_type::LOAD_CONST , node->value)) ;    
    }
    else if (node ->type == NodeType::IF){
    
        for (int i = 0 ;i < node ->children.size() ; i++)
        {
            Code_Generation(node->children[0]) ;     
            Instructions.push_back(ByteCode(Bytecode_type::POP_FLAG)) ; 
        }    

    }else if (node ->type == NodeType::WHILE)
    {
        int Flagged_place = Instructions.size();
        Code_Generation(node->children[0]) ; 
        Instructions.push_back(ByteCode(Bytecode_type::POP_FLAG)) ; 

    }else if (node ->type == NodeType::FOR) 
    {
        Instructions.push_back(ByteCode(Bytecode_type::PUSH_SCOPE)) ;  // there is double layer of scope inside the for loop 
        Code_Generation(node->children[0]->children[0]) ; 
        // INITIALIZE
        int Flagged_place = Instructions.size() ; 
        Code_Generation(node->children[1]) ; // CONDITION_DO BLOCK HERE 
        
        Code_Generation(node->children[2]->children[0]) ; //  UPDATE (POST PROCESSING ) 
        Instructions.push_back(ByteCode(Bytecode_type::JUMP_IF_TRUE ,Flagged_place)) ; 
        Instructions.push_back(ByteCode(Bytecode_type::POP_FLAG)) ; 
        Instructions.push_back(ByteCode(Bytecode_type::POP_SCOPE)) ; 
    }
    else if ( node-> type == NodeType::CONDITION_DO) 
    {
        
        Code_Generation(node->children[0]) ; 
        Instructions.push_back(ByteCode(Bytecode_type::PUSH_FLAG)) ; 
        Instructions.push_back(ByteCode(Bytecode_type::JUMP_IF_FALSE)) ; 
        int Flagged_place = Instructions.size()-1; 
        Code_Generation(node->children[1]) ;   
        int JUMP_TO = Instructions.size() ; 
        Instructions[Flagged_place].data = JUMP_TO ; 
        // 這裡有上下文問題 flag 必須依靠外部pop
    }else if (node->type == NodeType::VARIABLE )
    {
        Instructions.push_back(ByteCode(Bytecode_type::LOAD_VAR , node->value)) ; 
    }else if (node ->type == NodeType::CALL_FUNCTION )
    {
        int count = 0  ;
        Instructions.push_back(ByteCode(Bytecode_type::CALL_FUNCTION , node->children[0]->value));
        
    }else if (node ->type == NodeType::DECLARATION)
    {
        
    }


    
}