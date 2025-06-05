#include "parser.h"
#include "lexer.h"
#include "evaluate.h"
#include <stack>
#include <unordered_map>
#include <variant> 
#include <queue>
#include<unordered_set>
#include<cmath>
#include <chrono>
#include <cstdlib>  // for srand, rand
#include <ctime>    // for time
using namespace std ; 





unordered_map< int , std::stack<table>> Identifiers_list;
stack<unordered_map< int ,  bool>> Scope;  
Interpreter::Interpreter(const std::string input) : input(input), index(0) {} 
void Interpreter::interupt_helper(int start , int end )
{
    for (int i = start ; i <end ;i++ )
    {
        if (Instructions[i].type == Bytecode_type::ADD_BREAK)
        {
            Instructions[i].type = Bytecode_type::JUMP; //jump to end where for or while at the end  
            Instructions[i].data = end ; 
            Instructions[i].data2 = end ; 
        }
    }
}

void Interpreter::Code_Generation(shared_ptr<Node> node) {
    if (node->type == NodeType::BLOCK)
    {
        Instructions.push_back(ByteCode(Bytecode_type::PUSH_SCOPE)) ; 
        for (int i = 0  ; i < node->children.size() ; i++){
            Code_Generation(node->children[i]) ; 
            NodeType child_type = node->children[i]->type;
            if (child_type != NodeType::RETURN && child_type != NodeType::CONTINUE && child_type != NodeType::BREAK  && child_type != NodeType::FUNCTION
                && child_type != NodeType::IF  && child_type != NodeType::WHILE  && child_type != NodeType::FOR ) // here the fucking issue ; 
            {
                Instructions.push_back(ByteCode(Bytecode_type::CLEAR));//statment are independent each 要把結果清出來
            }  
        }
        Instructions.push_back(ByteCode(Bytecode_type::POP_SCOPE)) ; 
    }else if (node->type == NodeType::BINARY)
    {

        if (node->value != "ASSIGN")
        {

            Code_Generation(node->children[0]) ; 
        } else
        {
            if (node->children[0]->type == NodeType::INDEX)
            {
                Code_Generation(node->children[0]->children[0]) ;
                Code_Generation(node->children[0]->children[1]) ;
                Instructions.push_back(ByteCode(Bytecode_type::PTR)) ;  
            }else if (node->children[0]->type == NodeType::VARIABLE)
            {

                
                Instructions.push_back(ByteCode(Bytecode_type::LOAD_CONST  , node->children[0]->value))  ; 
            }else
            {
                Instructions.push_back(ByteCode(Bytecode_type::LOAD_CONST  , nil{}))  ; 

            }
        }
        
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
        } else if (node->value == "ASSIGN"){
            Instructions.push_back(ByteCode(Bytecode_type::ASSIGN));
        } 
        else {
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
        Instructions.push_back(ByteCode(Bytecode_type::LOAD_CONST  , node->value =="true")) ; 
    }else if (node ->type == NodeType::FLOAT)
    {
        Instructions.push_back(ByteCode(Bytecode_type::LOAD_CONST , stof(node->value))) ; 
    }else if (node ->type == NodeType::DOUBLE)
    {  
        Instructions.push_back(ByteCode(Bytecode_type::LOAD_CONST , stod(node->value))) ;     
    }else if (node ->type == NodeType::STRING)
    {
        Instructions.push_back(ByteCode(Bytecode_type::LOAD_CONST , node->value)) ;    
    }else if (node-> type == NodeType::NIL )
    {
        Instructions.push_back(ByteCode(Bytecode_type::LOAD_CONST , nil{}) ); 
    }
    else if (node ->type == NodeType::IF){
    

        bool is_first = true ; 
        int JUMP_FROM ; 
        int JUMP_TO;
        bool one_state= false ; 
        vector<int> JUMP_TO_IF_END ;
        for (int i = 0 ;i < node ->children.size() ; i++)
        {

            JUMP_TO = Instructions.size();
            if (!is_first)
            {
                Instructions[JUMP_FROM].data = JUMP_TO ; 
            }else
            {
                if (node->children.size() == 1 )
                {
                    one_state = true ; 
                }
                is_first = false  ; 
            }
            if ((node->children.size()-i) == 1 )
            {
                one_state = true ; 
            }
             Code_Generation(node->children[i]->children[0]) ; 
            
             JUMP_FROM = Instructions.size();

             if (one_state == true )
             {
                JUMP_TO_IF_END.push_back(JUMP_FROM);
             }
             Instructions.push_back(ByteCode(Bytecode_type::JUMP_IF_FALSE)) ;  
              
             Code_Generation(node->children[i]->children[1]) ; 
             JUMP_TO_IF_END.push_back(Instructions.size());
             Instructions.push_back(ByteCode(Bytecode_type::JUMP))  ; 
        }    

        int END_POINT = Instructions.size() ; 
        for (int i = 0 ; i < JUMP_TO_IF_END.size() ; i++)
        {
            Instructions[JUMP_TO_IF_END[i]].data  = END_POINT; 

        }


    }else if (node->type == NodeType::WHILE)
    {
        int loop_start = Instructions.size(); // loop 起點
        Instructions.push_back(ByteCode(Bytecode_type::START_LOOP));
    
        Code_Generation(node->children[0]->children[0]); // condition
    
        Instructions.push_back(ByteCode(Bytecode_type::JUMP_IF_FALSE));
        int jump_false_index = Instructions.size() - 1; // 記住 JUMP_IF_FALSE 的位置
    
        Code_Generation(node->children[0]->children[1]); // loop block
    
        Instructions.push_back(ByteCode(Bytecode_type::JUMP, loop_start)); // 跳回 loop_start
    
        int loop_end = Instructions.size(); // loop 結束處
        Instructions.push_back(ByteCode(Bytecode_type::END_LOOP));
        Instructions[jump_false_index].data = loop_end; // 補上條件為假的跳出點
        interupt_helper(loop_start,  loop_end) ; 


    }else if (node->type == NodeType::FOR)
    {
        // 1. 建立新作用域s
        Instructions.push_back(ByteCode(Bytecode_type::PUSH_SCOPE));  
    
        // 2. 執行初始化語句
        for (int i = 0; i < node->children[0]->children.size(); i++) {
            Code_Generation(node->children[0]->children[i]);
            Instructions.push_back(ByteCode(Bytecode_type::CLEAR));
        }
        Instructions.push_back(ByteCode(Bytecode_type::START_LOOP));
            // 3. 標記 loop 開始位置
        int loop_start = Instructions.size();
        // 4. 產生條件判斷程式碼
        Code_Generation(node->children[1]->children[0]);  // condition
    
        // 5. 插入條件為 false 時跳出的指令
        Instructions.push_back(ByteCode(Bytecode_type::JUMP_IF_FALSE));
        int jump_false_index = Instructions.size() - 1;
    
        // 6. 產生主體區塊程式碼
        Code_Generation(node->children[1]->children[1]);  // block
    
        // 7. 產生 update 區塊
        for (int i = 0; i < node->children[2]->children.size(); i++) {
            Code_Generation(node->children[2]->children[i]);
            Instructions.push_back(ByteCode(Bytecode_type::CLEAR));
        }
    
        // 8. 跳回條件判斷處
        Instructions.push_back(ByteCode(Bytecode_type::JUMP, loop_start));
    
        // 9. 迴圈結尾位置
        int loop_end = Instructions.size();
        Instructions.push_back(ByteCode(Bytecode_type::END_LOOP));
    
        // 10. 離開作用域
        Instructions.push_back(ByteCode(Bytecode_type::POP_SCOPE));
    
        // 11. 補上條件跳出的目標
        Instructions[jump_false_index].data = loop_end;
    }
    
    else if (node->type == NodeType::VARIABLE )
    {
        Instructions.push_back(ByteCode(Bytecode_type::LOAD_VAR , node->value )) ; 
        /// 這邊要判斷 
    }else if (node ->type == NodeType::CALL_FUNCTION )
    {
        Instructions.push_back(ByteCode(Bytecode_type::ADD_ARGS_QUE));
        Code_Generation(node->children[1]) ; 
        Instructions.push_back(ByteCode(Bytecode_type::CALL_FUNCTION , node->children[0]->value)); 
    }else if (node ->type == NodeType::DECLARATION)
    {

        if (node->children.size() > 1 )
        {
            Code_Generation(node->children[1]); 
        }else
        {
            Instructions.push_back(ByteCode(Bytecode_type::LOAD_CONST , nil{})) ; 
        }

        Instructions.push_back(ByteCode(Bytecode_type::DECLARE , node ->children[0]->value)) ;         
    }else if (node->type == NodeType::FUNCTION)
    {
        int TO_FUNCTION = Instructions.size() + 2 ; 
        int DECLARE_FUNCTION_POINT  = Instructions.size() ; 
        Instructions.push_back(ByteCode(Bytecode_type::DECLARE_FUNCTION ,node->value)) ;
        Instructions[DECLARE_FUNCTION_POINT].data2 = TO_FUNCTION ; 
        int JUMP_OVER_DECLARATION  = Instructions.size() ; 
        Instructions.push_back(ByteCode(Bytecode_type::JUMP)) ; // 越過代碼段
        Instructions.push_back(ByteCode(Bytecode_type::TO_SCOPE )) ;  // 進入的scop
        Instructions.push_back(ByteCode(Bytecode_type::PUSH_SCOPE)) ; // 推入function的scope 
        for (int i = 0 ; i < node->children[0]->children.size() ; i++)
        { 
            Instructions.push_back(ByteCode(Bytecode_type::POP_ARGUMENT)) ; // 取參數
            Instructions.push_back(ByteCode(Bytecode_type::DECLARE , node->children[0]->children[i]->value)) ; //宣告  
            Instructions.push_back(ByteCode(Bytecode_type::CLEAR));
        }
        Instructions.push_back(ByteCode(Bytecode_type::CLEAN_ARGUMENT));
        Code_Generation(node->children[1]) ;               
        Instructions.push_back(ByteCode(Bytecode_type::POP_SCOPE)) ;
        Instructions.push_back(ByteCode(Bytecode_type::LOAD_CONST , nil{}))  ;  
        Instructions.push_back(ByteCode(Bytecode_type::RETURN)) ;
        int over =  Instructions.size() ;  
        Instructions[JUMP_OVER_DECLARATION].data = over ;         
    }else if (node->type == NodeType::ARGUMENT )
    {
        for (int i = 0 ; i <node->children.size() ; i++)
        {
            Code_Generation(node->children[i]);
            Instructions.push_back(ByteCode(Bytecode_type::PUSH_ARGUMENT)) ;                        
        }
    }else if (node->type == NodeType::TABLE)
    {
        Instructions.push_back(ByteCode(Bytecode_type::MAKING_TABLE));
        int INDEXER_COUNTER = 0;  // Lua table 預設從 index 1 開始
    
        for (int i = 0; i < node->children.size(); i++)
        {
            if (node->children[i]->type == NodeType::ASSIGN_KEY)
            {
                // key
                // value
                Code_Generation(node->children[i]->children[1]);
                Code_Generation(node->children[i]->children[0]);

                Instructions.push_back(ByteCode(Bytecode_type::TABLE_ADDING));  // [key] = value
            }
            else
            {
                // auto key = INDEXER_COUNTER
                // key
                Instructions.push_back(ByteCode(Bytecode_type::LOAD_CONST, INDEXER_COUNTER));
                // value
                Code_Generation(node->children[i]);
                Instructions.push_back(ByteCode(Bytecode_type::TABLE_ADDING));  // [index] = value
                INDEXER_COUNTER++;
            }
        }
    
        Instructions.push_back(ByteCode(Bytecode_type::CLEANING_TABLE));
    }else if (node ->type  == NodeType::INDEX)
    {
        Code_Generation(node->children[0] );
        Code_Generation(node->children[1] ); 
        Instructions.push_back(ByteCode(Bytecode_type::INDEX)) ;      
    }else if (node->type == NodeType::CHILDREN) // assume this is equal to indexing 
    {
        Code_Generation(node->children[0]);
        Code_Generation(node->children[1])  ;
        Instructions.push_back(ByteCode(Bytecode_type::INDEX)) ; 
    }else if (node->type == NodeType::BREAK)
    {
        Instructions.push_back(ByteCode(Bytecode_type::ADD_BREAK)) ; // this is the tag for inside the while or for loop soon operation 
    }else if (node ->type == NodeType::CONTINUE)
    {
        Instructions.push_back(ByteCode(Bytecode_type::POP_SCOPE));//sus 
        Instructions.push_back(ByteCode(Bytecode_type::ADD_CONTINUE)) ; 
    }else if (node ->type == NodeType::RETURN) // ok
    {
        if (node->children.size()>=1)
        {
            Code_Generation(node->children[0]) ;
        }else
        {
           Instructions.push_back( ByteCode(Bytecode_type::LOAD_CONST , nil{}));
        }
        Instructions.push_back(ByteCode(Bytecode_type::RETURN)) ; 
    }

}

int var_to_int(const Var& v) {
    return std::visit([](auto&& arg) -> int {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int>) {
            return arg;
        } else {
            // 如果需要可以 throw 錯誤
            return 0;
        }
    }, v);
}

double var_to_double(const Var& v) {
    return std::visit([](auto&& arg) -> double {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, double>) {
            return arg;
        } else {
            // 如果需要可以 throw 錯誤
            return 0;
        }
    }, v);
}

float var_to_float(const Var& v) {
    return std::visit([](auto&& arg) -> float {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, float>) {
            return arg;
        } else {

            return 0;
        }
    }, v);
}

function var_to_function(const Var& v )
{
    return std::visit([](auto&& arg) -> function {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, function>) {
            return arg;
        } else {
            return  function { 0};
        }
    }, v);

}
string var_to_string(const Var& v) {
    return std::visit([](auto&& arg) -> string {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, string>) {
            return arg;
        } else {
            // 如果需要可以 throw 錯誤
            return 0;
        }
    }, v);
}







std::shared_ptr<table> var_to_table(const Var& v) {
    if (auto p = std::get_if<std::shared_ptr<table>>(&v)) {
        return *p;
    } else {
        throw std::runtime_error("Var is not a table");
    }
}








bool is_falsy(const Var& v) {
    return std::visit([](auto&& arg) -> bool {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, bool>) {
            return arg == false;
        } else if constexpr (std::is_same_v<T, nil>) {
            return true;
        } else {
            return false;
        }
    }, v);
}

bool is_int(const Var& v) {
    return std::holds_alternative<int>(v);
}

bool is_float(const Var& v) {
    return std::holds_alternative<float>(v);
}
bool is_bool(const Var& v) {
    return std::holds_alternative<bool>(v);
}

bool is_double(const Var& v) {
    return std::holds_alternative<double>(v);
}

bool is_string(const Var& v) {
    return std::holds_alternative<std::string>(v);
}

bool is_nil(const Var& v) {
    return std::holds_alternative<nil>(v);
}


bool is_function_(const Var& v )
{
    return std::holds_alternative<function>(v) ; 
}

bool is_table (const Var& v )
{
    return std::holds_alternative<std::shared_ptr<table>>(v) ; 
}

enum class VarType {
    NIL,
    BOOL , 
    INT,
    FLOAT,
    DOUBLE,
    STRING,
    FUNCTION , 
    TABLE
};



VarType get_var_type(const Var& v) {
    if (is_int(v)) return VarType::INT;
    if (is_bool(v)) return VarType::BOOL ; 
    if (is_float(v)) return VarType::FLOAT;
    if (is_double(v)) return VarType::DOUBLE;
    if (is_string(v)) return VarType::STRING;
    if (is_nil(v)) return VarType::NIL;
    if (is_function_(v)) return VarType::FUNCTION;
    if (is_table(v)) return VarType::TABLE ; 
}
stack<Var> OPERATION_STACK  ; 
/*Var Find_Var(const std::string& key) {
    Var key_var = key;
    // 從後往前查找（後面的作用域優先）
    for (auto it = VARIABLE_LIST.rbegin(); it != VARIABLE_LIST.rend(); ++it) {
        auto found = it->find(key);
        if (found != it->end()) {
            return found->second;
        }
    }
    return nil{};
}
void Declare_Var(const std::string& key, Var v) {
    Var key_var = key;
    // 宣告到最內層作用域（即最後一個 map）
    if (VARIABLE_LIST.empty()) {
        VARIABLE_LIST.push_back({});
    }
    VARIABLE_LIST.back()[key] = v;
}
int Find_Where(std::string& key)
{
    for (int i = VARIABLE_LIST.size() - 1; i >= 0; --i)
    {
        if (VARIABLE_LIST[i].find(key) != VARIABLE_LIST[i].end())
        {
            return i;  // 找到了 key，返回 index
        }
    }
    return -1;  // 沒找到 assume global  
}
 

f

vector<unordered_map<string , Var>> VARIABLELIST  ; */



shared_ptr<env> CURRENT_ENV = make_shared<env>();  // 全局的當前環境指標
shared_ptr<env> GLOBAL_ENV = CURRENT_ENV;
Var Find_Var(const std::string& key) {
    shared_ptr<env> current = CURRENT_ENV;
    while (current) {
        auto it = current->VARIABLE_TABLE.find(key);
        if (it != current->VARIABLE_TABLE.end()) {
            return it->second;
        }
        current = current->parent;
    }
    return nil{};  // 找不到就回傳 nil
}

void Declare_Var(const std::string& key, Var v) {
    if (!CURRENT_ENV) {
        CURRENT_ENV = make_shared<env>();
    }
    CURRENT_ENV->VARIABLE_TABLE[key] = v;  // 宣告在當前 scope
   
}

shared_ptr<env> Find_Where(const std::string key) {
    shared_ptr<env> current = CURRENT_ENV;
    while (current) {
        if (current->VARIABLE_TABLE.find(key) != current->VARIABLE_TABLE.end()) {
            return current;  // 找到了，回傳該 env
        }
        current = current->parent;
    }
 //   cout << "it is fucking null" ; 
    return nullptr;  // 沒找到
}
// 進入新作用域
void Push_Scope() {
    CURRENT_ENV = make_shared<env>(env{CURRENT_ENV, {}});
}

void Pop_Scope() {
    if (CURRENT_ENV) {
        CURRENT_ENV->VARIABLE_TABLE.clear();  // 手動清除 map
        CURRENT_ENV = CURRENT_ENV->parent;
    }
}



vector<unordered_map<string, Var>> SCOPES; 
stack<unordered_map<string , Var>> SCOPES_STACK ; 
stack<shared_ptr<table>> TABLE_MAKER ;
stack<queue<Var>> ARGS_QUE;

unordered_map<int, int> resolvedDepth;
stack<int> STACK_TRACE;
stack<shared_ptr<env>> SCOPE_TRACER ; 
stack<shared_ptr<env>> CONTINUE_TRACER_SCOPE;
stack<int> CONTINUE_TRACER_STEP;
stack<shared_ptr<env>> BREAK_TRACER_SCOPE;
stack<int> BREAK_TRACER_STEP;



stack<bool> PTR_TAG ; 
unordered_set<weak_ptr<Var>> ROOTS;          // GC 根集合
unordered_set<weak_ptr<Var>> ALL_TABLE_SET;  // 所有可能的对象（table）
unordered_set<shared_ptr<Var>> visited; // 存储已标记对象

void dfs_GC(const shared_ptr<Var>& node) {
    if (!node || visited.count(node)) return;
    visited.insert(node);
    if (auto table_ptr = get_if<shared_ptr<table>>(node.get())) {
        for (const auto& [k, v] : **table_ptr) {
            if (holds_alternative<shared_ptr<table>>(v)) {
                dfs_GC(make_shared<Var>(v));
            }
        }
    }
}
void GC() {
    visited.clear();

    // 清除無效 ROOT 並 DFS 有效的
    for (auto it = ROOTS.begin(); it != ROOTS.end(); ) {
        if (auto root = it->lock()) {
            dfs_GC(root);
            ++it;
        } else {
            it = ROOTS.erase(it);  // 刪除已失效的 weak_ptr
        }
    }

    // 過濾保留 visited 中的有效物件
    unordered_set<weak_ptr<Var>> new_all;
    for (auto it = ALL_TABLE_SET.begin(); it != ALL_TABLE_SET.end(); ) {
        if (auto obj = it->lock()) {
            if (visited.count(obj)) {
                new_all.insert(obj);
            }
            ++it;
        } else {
            it = ALL_TABLE_SET.erase(it);  // 移除已經失效的
        }
    }
    visited.clear(); 
    ALL_TABLE_SET = std::move(new_all);
}




int CREATE_COUNTER ; 



enum class build_in {
    PRINT,
    LEN,
    TYPE,
    INPUT,
    RANDOM,
    FOREACH , 
    ERROR  , 
    SUB_STR , 
    TO_INT , 
    TO_STRING,
    TO_DOUBLE , 
    TO_FLOAT  , 
    
};



unordered_map<string, build_in> BUILD_IN_FUNCTION = {
    {"print", build_in::PRINT},
    {"len", build_in::LEN},
    {"type", build_in::TYPE},
    {"input", build_in::INPUT},
    {"random", build_in::RANDOM} , 
    {"foreach" , build_in::FOREACH},
    {"error" ,build_in::ERROR } , 
    {"sub_str" , build_in::SUB_STR},
    {"to_string",build_in::TO_STRING},
    {"to_int" , build_in::TO_INT},
    {"to_float", build_in::TO_FLOAT},
    {"to_double",build_in::TO_DOUBLE}
};
bool is_build_in(const string& str)
{
    return BUILD_IN_FUNCTION.find(str) != BUILD_IN_FUNCTION.end();
}
Var do_build_in(const string& str)
{
    if (!is_build_in(str)) {
        throw std::runtime_error("Unknown built-in function: " + str);
    }

    build_in func = BUILD_IN_FUNCTION[str];

    switch (func)
    {
    case build_in::TO_STRING: {
    if (ARGS_QUE.empty()) return nil{};
    if (ARGS_QUE.top().empty()) return nil{};

    Var arg = ARGS_QUE.top().front();
    ARGS_QUE.pop();

    std::string result = std::visit([](auto&& val) -> std::string {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, nil>) {
            return "$nil";
        } else if constexpr (std::is_same_v<T, bool>) {
            return val ? "$true" : "$false";
        } else if constexpr (std::is_same_v<T, std::string>) {
            return val;
        } else if constexpr (std::is_same_v<T, function>) {
            return "$function";
        } else if constexpr (std::is_same_v<T, std::shared_ptr<table>>) {
            char buffer[20];
            std::sprintf(buffer, "0x%p", static_cast<void*>(val.get()));
            return std::string("$table: ") + buffer;
        } else {
            return std::to_string(val);
        }
    }, arg);

    return result;
    }

    case build_in::TO_INT:
    {
        if (ARGS_QUE.empty()) return nil{};
        if (ARGS_QUE.top().empty()) return nil{};

        Var arg = ARGS_QUE.top().front(); ARGS_QUE.pop();

        Var result = std::visit([](auto&& val) -> Var {
            using T = std::decay_t<decltype(val)>;

            if constexpr (std::is_same_v<T, nil>) {
                return nil{};
            } else if constexpr (std::is_same_v<T, bool>) {
                return val ? 1 : 0;
            } else if constexpr (std::is_same_v<T, int>) {
                return val;
            } else if constexpr (std::is_same_v<T, float>) {
                return static_cast<int>(val);
            } else if constexpr (std::is_same_v<T, double>) {
                return static_cast<int>(val);
            } else if constexpr (std::is_same_v<T, std::string>) {
                try {
                    size_t idx;
                    int num = std::stoi(val, &idx);
                    if (idx != val.length()) {
                        return nil{}; // 
                    }
                    return num;
                } catch (...) {
                    return nil{};
                }
            } else {
                return nil{};
            }
        }, arg);

        return result;
    }
    case build_in::TO_FLOAT:
    {
        if (ARGS_QUE.empty()) return nil{};
        if (ARGS_QUE.top().empty()) return nil{};

        Var arg = ARGS_QUE.top().front(); ARGS_QUE.pop();

        Var result = std::visit([](auto&& val) -> Var {
            using T = std::decay_t<decltype(val)>;

            if constexpr (std::is_same_v<T, nil>) {
                return nil{};
            } else if constexpr (std::is_same_v<T, bool>) {
                return val ? 1.0f : 0.0f;
            } else if constexpr (std::is_same_v<T, int>) {
                return static_cast<float>(val);
            } else if constexpr (std::is_same_v<T, float>) {
                return val;
            } else if constexpr (std::is_same_v<T, double>) {
                return static_cast<float>(val);
            } else if constexpr (std::is_same_v<T, std::string>) {
                try {
                    size_t idx;
                    float num = std::stof(val, &idx);
                    if (idx != val.length()) return nil{};
                    return num;
                } catch (...) {
                    return nil{};
                }
            } else {
                return nil{};
            }
        }, arg);

        return result;
    }


    case build_in::TO_DOUBLE:
    {
        if (ARGS_QUE.empty()) return nil{};
        if (ARGS_QUE.top().empty()) return nil{};

        Var arg = ARGS_QUE.top().front(); ARGS_QUE.pop();

        Var result = std::visit([](auto&& val) -> Var {
            using T = std::decay_t<decltype(val)>;

            if constexpr (std::is_same_v<T, nil>) {
                return nil{};
            } else if constexpr (std::is_same_v<T, bool>) {
                return val ? 1.0 : 0.0;
            } else if constexpr (std::is_same_v<T, int>) {
                return static_cast<double>(val);
            } else if constexpr (std::is_same_v<T, float>) {
                return static_cast<double>(val);
            } else if constexpr (std::is_same_v<T, double>) {
                return val;
            } else if constexpr (std::is_same_v<T, std::string>) {
                try {
                    size_t idx;
                    double num = std::stod(val, &idx);
                    if (idx != val.length()) return nil{};
                    return num;
                } catch (...) {
                    return nil{};
                }
            } else {
                return nil{};
            }
        }, arg);

        return result;
    }



    case build_in::SUB_STR:
        {
            if (ARGS_QUE.empty()) return nil{};
            if (ARGS_QUE.top().empty()) return nil{};

            std::queue<Var> args_q = ARGS_QUE.top();
            ARGS_QUE.pop();

            // 取第一個參數 (string)
            if (args_q.empty()) return nil{};
            Var arg0 = args_q.front(); args_q.pop();
            if (!std::holds_alternative<std::string>(arg0)) return nil{};
            std::string str = std::get<std::string>(arg0);

            // 取第二個參數 (start index)
            if (args_q.empty()) return nil{};
            Var arg1 = args_q.front(); args_q.pop();
            if (!std::holds_alternative<int>(arg1)) return nil{};
            int start = std::get<int>(arg1);
            if (start < 0 || start >= (int)str.length()) return nil{};

            // 第三個參數（長度）是可選的
            int length = -1;
            if (!args_q.empty()) {
                Var arg2 = args_q.front(); args_q.pop();
                if (!std::holds_alternative<int>(arg2)) return nil{};
                length = std::get<int>(arg2);
                if (length < 0) return nil{};
            }

            int substr_length = (length == -1) ? (int)str.length() - start : length;
            if (start + substr_length > (int)str.length()) return nil{};

            std::string substr = str.substr(start, substr_length);
            return substr;
        }









        case build_in::ERROR: {
        if (ARGS_QUE.empty()) {
            std::cerr << "\033[1;31m[ERROR]\033[0m No error message provided." << std::endl;
            return nil{};
        }

        std::queue<Var> temp_q = ARGS_QUE.top();
        ARGS_QUE.pop();

        std::vector<Var> args;
        while (!temp_q.empty()) {
            args.push_back(temp_q.front());
            temp_q.pop();
        }

        std::cerr << "\033[1;31m[ERROR] ";  // 紅色開始
        for (const auto& arg : args) {
            std::visit([](auto&& val) {
                using T = std::decay_t<decltype(val)>;
                if constexpr (std::is_same_v<T, nil>) {
                    std::cerr << "$nil ";
                } else if constexpr (std::is_same_v<T, bool>) {
                    std::cerr << (val ? "$true " : "$false ");
                } else if constexpr (std::is_same_v<T, std::string>) {
                    std::cerr << val ;
                } else if constexpr (std::is_same_v<T, function>) {
                    std::cerr << "$function ";
                } else if constexpr (std::is_same_v<T, std::shared_ptr<table>>) {
                    std::cerr << "$table " << val.get();
                } else {
                    std::cerr << val ;
                }
            }, arg);
        }
            std::cerr << "\033[0m" << std::endl; 
            std::exit(-1);
        }
        case build_in::PRINT: {
            if (ARGS_QUE.empty()) return nil{};
            if (ARGS_QUE.top().empty()) {cout << "$nil" ; return  nil{} ;} 
            

            vector<Var>  Args_v = vector<Var>(0);
            std::queue<Var> temp_q = ARGS_QUE.top();
            while (!temp_q.empty()) {
            Args_v.push_back(temp_q.front());
            temp_q.pop();
            }
            
        
            //    Var arg = ARGS_QUE.top().front(); ARGS_QUE.pop();
            
        Var last = nil{};
        for (const auto& arg : Args_v) {
            std::visit([](auto&& val) {
                using T = std::decay_t<decltype(val)>;
                if constexpr (std::is_same_v<T, nil>) {
                    std::cout << "$nil" ;
                } else if constexpr (std::is_same_v<T, bool>) {
                    std::cout << (val ? "$true" : "$false") ;
                } else if constexpr (std::is_same_v<T, std::string>) {
                    std::cout << val ;
                } else if constexpr (std::is_same_v<T, function>) {
                    std::cout << "$function";
                } else if constexpr (std::is_same_v<T, std::shared_ptr<table>>) {
                    std::cout << "$table: " << val.get() ;
                } else {
                    std::cout << val ;
                }
            }, arg);
            last = arg;
        }
        return last;
    }


    case build_in::FOREACH:
    {
        if (ARGS_QUE.empty()) return nil{};

        if (ARGS_QUE.top().empty()) return nil{} ; 
        Var arg = ARGS_QUE.top().front() ; ARGS_QUE.pop();
        if (is_table(arg))
        {
            shared_ptr<table> r_table = std::make_shared<table>(table{}); 
            shared_ptr<table> k_table = std::make_shared<table>(table{}); 
            shared_ptr<table> v_table = std::make_shared<table>(table{}); 
            shared_ptr<table> origin = var_to_table(arg) ;             
            (*r_table)["key"] = k_table ; 
            (*r_table)["value"] = v_table  ;   
            int counter = 0 ; 
            for (const auto&[key , value ] : *origin )
            {

                 (*k_table)[counter] = key ; 
                 (*v_table)[counter] = value ; 
                 counter ++ ; 
            }
            return  r_table  ; 
        }

        return nil{} ; 
    }

    case build_in::LEN: {
        if (ARGS_QUE.empty()) return nil{};
        if (ARGS_QUE.top().empty()) return nil{} ; 

        Var arg = ARGS_QUE.top().front(); ARGS_QUE.pop();
        
        if (is_string(arg)) {
            return static_cast<int>(var_to_string(arg).size());
        }
        if (is_table(arg)) {
            return static_cast<int>(var_to_table(arg)->size());
        }
        return Var(0);
    }

    case build_in::TYPE: {
        if (ARGS_QUE.empty()) return nil{};
        if (ARGS_QUE.top().empty()) return nil{} ; 

        Var arg = ARGS_QUE.top().front(); ARGS_QUE.pop();
        switch (get_var_type(arg)) {
            case VarType::INT: return Var("int");
            case VarType::FLOAT: return Var("float");
            case VarType::DOUBLE: return Var("double");
            case VarType::STRING: return Var("string");
            case VarType::FUNCTION: return Var("function");
            case VarType::TABLE: return Var("table");
            case VarType::NIL: return Var("nil");
            default: return Var("nil") ; 
        }
    }

    case build_in::INPUT: {
        std::string line;
        cin >> line ; 
        return Var(line);
    }

    case build_in::RANDOM: {
        return Var(rand() % 100);  // 0 到 99 的隨機數
    }

    default:
        throw std::runtime_error("Unhandled built-in function");
    }
}


void Interpreter::add_root_keywords()
{
    for (const auto& [name, func_enum] : BUILD_IN_FUNCTION) {
        (*GLOBAL_ENV).VARIABLE_TABLE[name] = function{};
    }
}





void Interpreter::execute() 
{

    add_root_keywords();

    for (int step = 0; step < Instructions.size(); step++ ) // 使用條件避免無窮迴圈
    {
        Bytecode_type type  = Instructions[step].type;
        Var data  = Instructions[step].data ; 
      //  print_ins_type(type) ;
       // cout << endl ; 
        


        switch (type)
        {

            case Bytecode_type::JUMP_IF_FALSE:
            {
                Var v = OPERATION_STACK.top();
                OPERATION_STACK.pop();
                bool should_jump = is_falsy(v);
                if (should_jump) {
                    step = var_to_int(Instructions[step].data)-1;  // 跳轉到新的步驟
                }
                break;
            }         
            case Bytecode_type::JUMP_IF_TRUE:
            {
                Var v = OPERATION_STACK.top();
                OPERATION_STACK.pop();
                bool should_jump = !is_falsy(v);
                if (should_jump) {
                    step = var_to_int(data)-1;  // 跳轉到新的步驟
                }
                break;
            }
            case Bytecode_type::ADD_BREAK:
            {
                step = var_to_int(Instructions[step].data)-1 ; 
                CURRENT_ENV = CONTINUE_TRACER_SCOPE.top() ; 
                break ; 
            }
            case Bytecode_type::START_LOOP:
            {
                CONTINUE_TRACER_SCOPE.push(CURRENT_ENV);
                CONTINUE_TRACER_STEP.push(step+1) ; 
                break;
            }
            case Bytecode_type::ADD_CONTINUE:
            {
                step = (CONTINUE_TRACER_STEP.top())-1;
                CURRENT_ENV= CONTINUE_TRACER_SCOPE.top() ; 
                break ; 
            }
            case Bytecode_type::END_LOOP:
            {
                CONTINUE_TRACER_SCOPE.pop() ; 
                CONTINUE_TRACER_STEP.pop() ; 
                break;
            }

            case Bytecode_type::JUMP:
            {
                step = var_to_int(data)-1;
                break;
            }
            case Bytecode_type::LOAD_VAR:
            {



                string finder = var_to_string(data) ; 
                Var v = Find_Var(var_to_string(data)) ; 

                OPERATION_STACK.push(v) ; 

                break;
            }
            case Bytecode_type::MAKING_TABLE:
            {
                TABLE_MAKER.push(std::make_shared<table>(table{})) ; 
                break;
            
            }
            case Bytecode_type::CLEANING_TABLE:
            {
                OPERATION_STACK.push(TABLE_MAKER.top()) ;  // make it shared 
                TABLE_MAKER.pop()  ; 
                break;
            }
            case Bytecode_type::TABLE_ADDING:
            {   
                Var val  = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                Var key  = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ;   
                (*TABLE_MAKER.top())[key] = val  ; 
                break;
            }
            case Bytecode_type::ADD_ARGS_QUE:
            {
                ARGS_QUE.push(queue<Var> ()) ; 
                break;
            }
            case Bytecode_type::CLEAN_ARGUMENT:
            {
                ARGS_QUE.pop();
                break;
            }
            case Bytecode_type::CLEAR:
            {
                OPERATION_STACK.pop() ; 
                break;
            }
            case Bytecode_type::POP_SCOPE:
            {
                Pop_Scope() ; 
                break; 
            }
            case Bytecode_type::PUSH_SCOPE:
            {
                Push_Scope() ; 
                break;
            }
            case Bytecode_type::INDEX:
            {
                Var key = OPERATION_STACK.top() ;
                OPERATION_STACK.pop() ; 
                Var chest  = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                if (is_table(chest))
                {
                    shared_ptr<table> table = var_to_table(chest) ; 
                    auto it = table->find(key);
                    if (it != table->end()) {
                        Var val = it->second;
                    //    cout << "step 1" << endl  ; 
                        OPERATION_STACK.push(val) ; 
                        break;
                    } else {
                        OPERATION_STACK.push(nil{}) ;
                  //      cout << "step 2 " << endl ;  
                        break;
                    }
                }
                //cout << "step 3 " << endl ;  

                OPERATION_STACK.push(nil{});
                break;
            }
            case Bytecode_type::LOAD_CONST:
            {   
                Var v = Instructions[step].data ; 
                OPERATION_STACK.push(v) ; 
                break;
            }
            case Bytecode_type::DECLARE:
            {
                Var var_val = OPERATION_STACK.top() ;
                OPERATION_STACK.pop() ; 
                string var_name = var_to_string(Instructions[step].data) ; 
                Declare_Var( var_name,  var_val) ; 
                OPERATION_STACK.push(var_val); /// 這裡要改
                break;
            }
            case Bytecode_type::DECLARE_FUNCTION:
            {

                Var val = Instructions[step].data ; 
                int step_goto = Instructions[step].data2 ; 
                function func { step_goto ,  CURRENT_ENV , var_to_string(val)} ;
                CURRENT_ENV->VARIABLE_TABLE[var_to_string(val)] = func;   
                break;
            }
            case Bytecode_type::PUSH_ARGUMENT:
            {   
                ARGS_QUE.top().push(OPERATION_STACK.top());
                OPERATION_STACK.pop();
              
                break;
            }
            case Bytecode_type::POP_ARGUMENT:
            {
                OPERATION_STACK.push(ARGS_QUE.top().front());
                ARGS_QUE.top().pop();
                break;
            }
            case Bytecode_type::PTR:
            {

                PTR_TAG.push(true) ; 
                break;
            }
            case Bytecode_type::ASSIGN:
            {

                bool IS_PTR_TAGGED = false ; 
                Var a = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                Var b = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                Var c  ; 
                if (!PTR_TAG.empty())
                {
                    c=OPERATION_STACK.top()  ; 
                    OPERATION_STACK.pop()  ; 
                    PTR_TAG.pop() ; 
                    IS_PTR_TAGGED = true ; 
                }
                if(IS_PTR_TAGGED)
                {
                    if (is_table(c) )
                    {
                        shared_ptr<table> c_table = var_to_table(c);
                        if (!is_nil(a)){
                        (*c_table)[b]=a;
                        }else
                        {
                        (*c_table).erase(b) ; 
                        }
                        OPERATION_STACK.push(a);
                        
                    }else
                    {
                        OPERATION_STACK.push(nil{});
                    }
                }else 
                {
                    if (!is_string(b)) // 這裡表示 identifer 才能通過
                    {
                        OPERATION_STACK.push(nil{});
                        break;
                    }
                    string b_str =var_to_string(b) ; 
                    shared_ptr<env> get_env = Find_Where(b_str) ; 
                    if (get_env)
                    {
                        get_env->VARIABLE_TABLE[b_str]=a;
                        OPERATION_STACK.push(a) ; 
                    }
                    else
                    {
                        OPERATION_STACK.push(nil{});
                    }
                }
                break; 
            }
            case Bytecode_type::CALL_FUNCTION:
            {
                string identifier  =var_to_string(Instructions[step].data);
                shared_ptr<env> get_env  = Find_Where(identifier) ; 
                //cout << identifier << endl ; 

                
                if (get_env != nullptr){
                    get_env->VARIABLE_TABLE[identifier] ; 


                    if (get_var_type(get_env->VARIABLE_TABLE[identifier]) == VarType::FUNCTION)
                    {
                        function func = var_to_function(get_env->VARIABLE_TABLE[identifier]);
                        if (get_env->parent == nullptr)
                        {
                            Var v = do_build_in(identifier);
                            OPERATION_STACK.push(v) ; 
                        }else{

                            STACK_TRACE.push(step+1) ; 
                            SCOPE_TRACER.push(CURRENT_ENV);
                            CURRENT_ENV = func.parent;
                            step = func.ptr-1; 
                    
                        }
                    }
                    else
                    {
                 //       cout << "call function " << "step 1.75" ; 
                        OPERATION_STACK.push(nil{}) ; 
          
                            ARGS_QUE.pop() ; 

                        
                    }
                    break;
                }
                else
                {
                    OPERATION_STACK.push(nil{}) ; 
                    ARGS_QUE.pop() ; 
                    
                        
                    break;
                }
                break;
            }
            case Bytecode_type::RETURN:
            {
                CURRENT_ENV = SCOPE_TRACER.top();
                step = STACK_TRACE.top()-1; 
                STACK_TRACE.pop() ; 
                SCOPE_TRACER.pop();
                break;
            }
//----------ARITHMATIC OPERATION -----------------
            case Bytecode_type::EQUAL:
            {
                Var a = OPERATION_STACK.top(); OPERATION_STACK.pop();
                Var b = OPERATION_STACK.top(); OPERATION_STACK.pop();

                VarType type = get_var_type(a);
                if (type != get_var_type(b)) {
                    OPERATION_STACK.push(false);
                    break;
                }

                if (type == VarType::NIL) {
                    OPERATION_STACK.push(true);
                }
                else if (type == VarType::FUNCTION) {
                    OPERATION_STACK.push(var_to_function(a).ptr == var_to_function(b).ptr);
                }
                else if (type == VarType::STRING) {
                    OPERATION_STACK.push(var_to_string(a) == var_to_string(b));
                }
                else if (type == VarType::DOUBLE) {
                    OPERATION_STACK.push(var_to_double(a) == var_to_double(b));
                }
                else if (type == VarType::INT) {
                    OPERATION_STACK.push(var_to_int(a) == var_to_int(b));
                }
                else if (type == VarType::FLOAT) {
                    OPERATION_STACK.push(var_to_float(a) == var_to_float(b)); // 修正成 var_to_float
                }
                else {
                    OPERATION_STACK.push(nil{}); // fallback：未知類型不相等
                }
                break;
            }
            case Bytecode_type::NOT_EQUAL:
                {
                Var a = OPERATION_STACK.top(); OPERATION_STACK.pop();
                Var b = OPERATION_STACK.top(); OPERATION_STACK.pop();

                VarType type = get_var_type(a);
                if (type != get_var_type(b)) {
                    OPERATION_STACK.push(false);
                    break;
                }
                if (type == VarType::NIL) {
                    OPERATION_STACK.push(false);
                }
                else if (type == VarType::FUNCTION) {
                    OPERATION_STACK.push(var_to_function(a).ptr != var_to_function(b).ptr);
                }
                else if (type == VarType::STRING) {
                    OPERATION_STACK.push(var_to_string(a) != var_to_string(b));
                }
                else if (type == VarType::DOUBLE) {
                    OPERATION_STACK.push(var_to_double(a) != var_to_double(b));
                }
                else if (type == VarType::INT) {
                    OPERATION_STACK.push(var_to_int(a) != var_to_int(b));
                }
                else if (type == VarType::FLOAT) {
                    OPERATION_STACK.push(var_to_float(a) != var_to_float(b)); // 修正成 var_to_float
                }
                else {
                    OPERATION_STACK.push(nil{}); // fallback：未知類型不相等
                }

                break;
            }
            case Bytecode_type::NOT_GREATER_THAN:
            {
                Var a = OPERATION_STACK.top(); OPERATION_STACK.pop();
                Var b = OPERATION_STACK.top(); OPERATION_STACK.pop();
            
                VarType type = get_var_type(a);
                if (type != get_var_type(b)) {
                    OPERATION_STACK.push(nil{});
                    break;
                }
                else if (type == VarType::STRING) {
                    OPERATION_STACK.push(var_to_string(b) <= var_to_string(a));
                }
                else if (type == VarType::DOUBLE) {
                    OPERATION_STACK.push(var_to_double(b) <= var_to_double(a));
                }
                else if (type == VarType::INT) {
                    OPERATION_STACK.push(var_to_int(b) <= var_to_int(a));
                }
                else if (type == VarType::FLOAT) {
                    OPERATION_STACK.push(var_to_float(b) <= var_to_float(a));
                }
                else {
                    OPERATION_STACK.push(nil{}); // fallback
                }
            
                break;
            }
            case Bytecode_type::NOT_LESS_THAN:
            {
                Var a = OPERATION_STACK.top(); OPERATION_STACK.pop();
                Var b = OPERATION_STACK.top(); OPERATION_STACK.pop();
            
                VarType type = get_var_type(a);
                if (type != get_var_type(b)) {
                    OPERATION_STACK.push(nil{});
                    break;
                }
                else if (type == VarType::STRING) {
                    OPERATION_STACK.push(var_to_string(b) >= var_to_string(a));
                }
                else if (type == VarType::DOUBLE) {
                    OPERATION_STACK.push(var_to_double(b) >= var_to_double(a));
                }
                else if (type == VarType::INT) {
                    OPERATION_STACK.push(var_to_int(b) >= var_to_int(a));
                }
                else if (type == VarType::FLOAT) {
                    OPERATION_STACK.push(var_to_float(b) >= var_to_float(a));
                }
                else {
                    OPERATION_STACK.push(nil{}); // fallback
                }
                break;
            }
            case Bytecode_type::LESS_THAN:
            {
                Var a = OPERATION_STACK.top(); OPERATION_STACK.pop();
                Var b = OPERATION_STACK.top(); OPERATION_STACK.pop();
            
                VarType type = get_var_type(a);
                if (type != get_var_type(b)) {
                    OPERATION_STACK.push(nil{});
                    break;
                }
                else if (type == VarType::STRING) {
                    OPERATION_STACK.push(var_to_string(b) < var_to_string(a));
                }
                else if (type == VarType::DOUBLE) {
                    OPERATION_STACK.push(var_to_double(b) < var_to_double(a));
                }
                else if (type == VarType::INT) {
                    OPERATION_STACK.push(var_to_int(b) < var_to_int(a));
                }
                else if (type == VarType::FLOAT) {
                    OPERATION_STACK.push(var_to_float(b) < var_to_float(a));
                }
                else {
                    OPERATION_STACK.push(nil{}); // fallback
                }
                break;
            }
            case Bytecode_type::GREATER_THAN:
            {
                Var a = OPERATION_STACK.top(); OPERATION_STACK.pop();
                Var b = OPERATION_STACK.top(); OPERATION_STACK.pop();
            
                VarType type = get_var_type(a);
                if (type != get_var_type(b)) {
                    OPERATION_STACK.push(nil{});
                    break;
                }
                else if (type == VarType::STRING) {
                    OPERATION_STACK.push(var_to_string(b) > var_to_string(a));
                }
                else if (type == VarType::DOUBLE) {
                    OPERATION_STACK.push(var_to_double(b) > var_to_double(a));
                }
                else if (type == VarType::INT) {
                    OPERATION_STACK.push(var_to_int(b) > var_to_int(a));
                }
                else if (type == VarType::FLOAT) {
                    OPERATION_STACK.push(var_to_float(b) >  var_to_float(a));
                }
                else {
                    OPERATION_STACK.push(nil{}); // fallback
                }
                break;
            }
            case Bytecode_type::NOT:
            {
                Var a = OPERATION_STACK.top() ; 
              //  cout << "step 1 " ; 
                OPERATION_STACK.pop(); 
             //   cout << "step 2 " ; 
                VarType type = get_var_type(a)  ; 
             //   cout << "step 2.5 " ; 
                if (is_falsy(a))
                {
                    OPERATION_STACK.push(true) ; 
               //     cout << "step 3 " ; 
                }else
                {
                    OPERATION_STACK.push(false) ; 
              //      cout << "step 4 " ; 
                }
                break ;
            }
            case Bytecode_type::AND:
            {
                Var a = OPERATION_STACK.top() ; // a 是右邊 
                OPERATION_STACK.pop() ; 
                Var b = OPERATION_STACK.top() ;
                OPERATION_STACK.pop() ; 

                if (is_falsy(a) || is_falsy(b))
                {
                    OPERATION_STACK.push(false) ; 
                }else
                {
                    OPERATION_STACK.push(a) ; 
                }
                break;
            }
            case Bytecode_type::OR:
            {
                Var a = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                Var b = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 

                if (is_falsy(a) && is_falsy(b))
                {
                    OPERATION_STACK.push(false) ; 
                }else
                {
                    OPERATION_STACK.push(a) ; 
                }
                break;
            }
            case Bytecode_type::BITWISE_NOT:
            {
                Var a = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 

                VarType type = get_var_type(a) ;
                if (type == VarType::INT)
                {
                    int a_i = var_to_int(a) ;
                    OPERATION_STACK.push(~a_i) ; 
                    break;
                }
                OPERATION_STACK.push(nil{});
                break;
            }
            
            case Bytecode_type::BITWISE_XOR:
            {
                Var a = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                Var b = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                VarType type = get_var_type(a) ;
                if (type != get_var_type(b))
                {
                    OPERATION_STACK.push(nil{});
                    break;
                }
                if (type == VarType::INT)
                {
                    int a_i = var_to_int(a) ;
                    int b_i = var_to_int(b);
                    OPERATION_STACK.push(a_i^b_i) ; 
                    break;
                }
                OPERATION_STACK.push(nil{});
                break;
            }
            case Bytecode_type::BITWISE_AND:
            {
                Var a = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                Var b = OPERATION_STACK.top() ;
                OPERATION_STACK.pop() ;  
                VarType type = get_var_type(a) ;
                if (type != get_var_type(b))
                {
                    OPERATION_STACK.push(nil{});
                    break;
                }
                if (type == VarType::INT)
                {
                    int a_i = var_to_int(a) ;
                    int b_i = var_to_int(b);
                    OPERATION_STACK.push(a_i&b_i) ; 
                    break;
                }
                OPERATION_STACK.push(nil{});
                break;
            }
            case Bytecode_type::BITWISE_OR:
            {
                Var a = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                Var b = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                VarType type = get_var_type(a) ;
                if (type != get_var_type(b))
                {
                    OPERATION_STACK.push(nil{});
                    break;
                }
                if (type == VarType::INT)
                {
                    int a_i = var_to_int(a) ;
                    int b_i = var_to_int(b);
                    OPERATION_STACK.push(a_i|b_i) ; 
                    break;
                }
                OPERATION_STACK.push(nil{});
                break;
            }
            case Bytecode_type::SHIFT_LEFT:
            {
                Var a = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                Var b = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                VarType type = get_var_type(a) ;
                if (type != get_var_type(b))
                {
                    OPERATION_STACK.push(nil{});
                    break;
                }
                if (type == VarType::INT)
                {
                    int a_i = var_to_int(a) ;
                    int b_i = var_to_int(b);
                    OPERATION_STACK.push(b_i<<a_i) ; 
                    break;
                }
                OPERATION_STACK.push(nil{});
                break;
            }
            case Bytecode_type::SHIFT_RIGHT:
            {
                Var a = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                Var b = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                VarType type = get_var_type(a) ;
                if (type != get_var_type(b))
                {
                    OPERATION_STACK.push(nil{});
                    break;
                }
                if (type == VarType::INT)
                {
                    int a_i = var_to_int(a) ;
                    int b_i = var_to_int(b);
                    OPERATION_STACK.push(b_i>>a_i) ; 
                    break;
                }
                OPERATION_STACK.push(nil{});
                break;
            }
            case Bytecode_type::ADD:
            {

                Var a = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                Var b = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                VarType type = get_var_type(a) ;
                if (type != get_var_type(b))
                {
                    OPERATION_STACK.push(nil{});
                    break;
                }
                if (type == VarType::INT)
                {
                    int a_i = var_to_int(a) ;
                    int b_i = var_to_int(b);
                    OPERATION_STACK.push(b_i+a_i) ; 
                    break;
                }
                if (type == VarType::DOUBLE)
                {
                    double a_d = var_to_double(a) ;
                    double b_d = var_to_double(b) ; 
                    OPERATION_STACK.push(b_d + a_d) ; 
                    break ; 
                }
                if (type == VarType::FLOAT)
                {
                    float a_f = var_to_float(a) ;
                    float b_f = var_to_float(b) ; 
                    OPERATION_STACK.push(b_f + a_f) ; 
                    break ; 
                }
                if (type == VarType::STRING)
                {
                    string a_str = var_to_string(a) ;
                    string b_str = var_to_string(b) ; 
                    OPERATION_STACK.push(b_str + a_str) ; 
                    break ; 
                }
                OPERATION_STACK.push(nil{});
                break;
            }
            case Bytecode_type::SUB:
            {

                Var a = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                Var b = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                VarType type = get_var_type(a) ;
                if (type != get_var_type(b))
                {
                    OPERATION_STACK.push(nil{});
                    break;
                }
                if (type == VarType::INT)
                {
                    int a_i = var_to_int(a) ;
                    int b_i = var_to_int(b);
                    OPERATION_STACK.push(b_i-a_i) ; 
                    break;
                }
                if (type == VarType::DOUBLE)
                {
                    double a_d = var_to_double(a) ;
                    double b_d = var_to_double(b) ; 
                    OPERATION_STACK.push(b_d - a_d) ; 
                    break ; 
                }
                if (type == VarType::FLOAT)
                {
                    float a_f = var_to_float(a) ;
                    float b_f = var_to_float(b) ; 
                    OPERATION_STACK.push(b_f - a_f) ; 
                    break ; 
                }

                OPERATION_STACK.push(nil{});
                break;
            }
            case Bytecode_type::NEG:
            {

                Var a = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ;
                VarType type = get_var_type(a) ;
                if (type == VarType::INT)
                {
                    int a_i = var_to_int(a) ;
                    OPERATION_STACK.push(-a_i) ; 
                    break;
                }
                if (type == VarType::DOUBLE)
                {
                    double a_d = var_to_double(a) ;
                    OPERATION_STACK.push(-a_d) ; 
                    break ; 
                }
                if (type == VarType::FLOAT)
                {
                    float a_f = var_to_float(a) ;
                    OPERATION_STACK.push(-a_f) ; 
                    break ; 
                }

                OPERATION_STACK.push(nil{});
                break;
            }
            case Bytecode_type::MUL:
            {
                Var a = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                Var b = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                VarType type = get_var_type(a) ;
                if (type != get_var_type(b))
                {
                    OPERATION_STACK.push(nil{});
                    break;
                }
                if (type == VarType::INT)
                {
                    int a_i = var_to_int(a) ;
                    int b_i = var_to_int(b);
                    OPERATION_STACK.push(b_i*a_i) ; 
                    break;
                }
                if (type == VarType::DOUBLE)
                {
                    double a_d = var_to_double(a) ;
                    double b_d = var_to_double(b) ; 
                    OPERATION_STACK.push(b_d * a_d) ; 
                    break ; 
                }
                if (type == VarType::FLOAT)
                {
                    float a_f = var_to_float(a) ;
                    float b_f = var_to_float(b) ; 
                    OPERATION_STACK.push(b_f * a_f) ; 
                    break ; 
                }

                OPERATION_STACK.push(nil{});
                break;
            }
            case Bytecode_type::DIV:
            {
                Var a = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                Var b = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                VarType type = get_var_type(a) ;
                if (type != get_var_type(b))
                {
                    OPERATION_STACK.push(nil{});
                    break;
                }
                if (type == VarType::INT)
                {
                    int a_i = var_to_int(a) ;
                    int b_i = var_to_int(b);
                    OPERATION_STACK.push(b_i/a_i) ; 
                    break;
                }
                if (type == VarType::DOUBLE)
                {
                    double a_d = var_to_double(a) ;
                    double b_d = var_to_double(b) ; 
                    OPERATION_STACK.push(b_d / a_d) ; 
                    break ; 
                }
                if (type == VarType::FLOAT)
                {
                    float a_f = var_to_float(a) ;
                    float b_f = var_to_float(b) ; 
                    OPERATION_STACK.push(b_f / a_f) ; 
                    break ; 
                }

                OPERATION_STACK.push(nil{});
                break;
            }
            case Bytecode_type::MOD:
            {
                Var a = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                Var b = OPERATION_STACK.top() ; 
                OPERATION_STACK.pop() ; 
                VarType type = get_var_type(a) ;
                if (type != get_var_type(b))
                {
                    OPERATION_STACK.push(nil{});
                    break;
                }
                if (type == VarType::INT)
                {
                    int a_i = var_to_int(a) ;
                    int b_i = var_to_int(b);
                    OPERATION_STACK.push(b_i%a_i) ; 
                    break;
                }
                if (type == VarType::DOUBLE)
                {
                    double a_d = var_to_double(a) ;
                    double b_d = var_to_double(b) ; 
                    OPERATION_STACK.push(std::fmod(b_d , a_d)) ; 
                    break ; 
                }
                if (type == VarType::FLOAT)
                {
                    float a_f = var_to_float(a) ;
                    float b_f = var_to_float(b) ; 
                    OPERATION_STACK.push(std::fmod(b_f , a_f)) ;  
                    break ; 
                }

                OPERATION_STACK.push(nil{});
                break;
            }
        
            default:
                break;
        }
    }
}




void Interpreter::print_ins_type (Bytecode_type type)
{
    switch (type) {
        case START_LOOP: cout << "START_LOOP" ; break; 
        case END_LOOP:cout << "END_LOOP" ; break; 
        case ADD_ARGS_QUE: cout << "ADD_ARGS_QUE" ; break;
        case POP_SCOPE: cout << "POP_SCOPE"; break;
        case PUSH_SCOPE: cout << "PUSH_SCOPE"; break;
        case PUSH_FUNCTION: cout << "PUSH_FUNCTION"; break;
        case POP_FUNCTION: cout << "POP_FUNCTION"; break;
        case ADD: cout << "ADD"; break;
        case SUB: cout << "SUB"; break;
        case ASSIGN: cout << "ASSIGN"; break;
        case DECLARE: cout << "DECLARE"; break;
        case MUL: cout << "MUL"; break;
        case DIV: cout << "DIV"; break;
        case MOD: cout << "MOD"; break;
        case JUMP: cout << "JUMP"; break;
        case JUMP_IF_FALSE: cout << "JUMP_IF_FALSE"; break;
        case JUMP_IF_TRUE: cout << "JUMP_IF_TRUE"; break;
        case LOAD_CONST: cout << "LOAD_CONST"; break;
        case LOAD_VAR: cout << "LOAD_VAR"; break;
        case CALL_FUNCTION: cout << "CALL_FUNCTION"; break;
        case DECLARE_FUNCTION: cout << "DECLARE_FUNCTION"; break;
        case RETURN: cout << "RETURN"; break;
        case AND: cout << "AND"; break;
        case OR: cout << "OR"; break;
        case NOT: cout << "NOT"; break;
        case NEG: cout << "NEG"; break;
        case TO_SCOPE: cout << "TO_SCOPE"; break;
        case EQUAL: cout << "EQUAL"; break;
        case NOT_EQUAL: cout << "NOT_EQUAL"; break;
        case NOT_GREATER_THAN: cout << "NOT_GREATER_THAN"; break;
        case NOT_LESS_THAN: cout << "NOT_LESS_THAN"; break;
        case SHIFT_LEFT: cout << "SHIFT_LEFT"; break;
        case SHIFT_RIGHT: cout << "SHIFT_RIGHT"; break;
        case BITWISE_OR: cout << "BITWISE_OR"; break;
        case BITWISE_AND: cout << "BITWISE_AND"; break;
        case BITWISE_XOR: cout << "BITWISE_XOR"; break;
        case BITWISE_NOT: cout << "BITWISE_NOT"; break;
        case LESS_THAN: cout << "LESS_THAN"; break;
        case GREATER_THAN: cout << "GREATER_THAN"; break;
        case CLEAR: cout << "CLEAR"; break;
        case TABLE_ADDING: cout << "TABLE_ADDING"; break;
        case MAKING_TABLE: cout << "MAKING_TABLE"; break;
        case CLEANING_TABLE: cout << "CLEANING_TABLE"; break;
        case PUSH_ARGUMENT: cout << "PUSH_ARGUMENT"; break;
        case POP_ARGUMENT: cout << "POP_ARGUMENT"; break;
        case CLEAN_ARGUMENT: cout << "CLEAN_ARGUMENT"; break;
        case INDEX: cout << "INDEX"; break;
        case ADD_CONTINUE: cout << "ADD_CONTINUE"; break;
        case ADD_BREAK: cout << "ADD_BREAK"; break;
        case PTR: cout << "PTR"; break;
}
}



void Interpreter::printByteCode() {

    for (const auto& instruction :Instructions) {
        switch (instruction.type) {
            case ADD_ARGS_QUE:cout << "ADD_ARGS_QUE"; break; 
            case POP_SCOPE: cout << "POP_SCOPE"; break;
            case PUSH_SCOPE: cout << "PUSH_SCOPE"; break;
            case PUSH_FUNCTION: cout << "PUSH_FUNCTION"; break;
            case POP_FUNCTION: cout << "POP_FUNCTION"; break;
            case ADD: cout << "ADD"; break;
            case SUB: cout << "SUB"; break;
            case ASSIGN: cout << "ASSIGN"; break;
            case DECLARE: cout << "DECLARE"; break;
            case MUL: cout << "MUL"; break;
            case DIV: cout << "DIV"; break;
            case MOD: cout << "MOD"; break;
            case JUMP: cout << "JUMP"; break;
            case JUMP_IF_FALSE: cout << "JUMP_IF_FALSE"; break;
            case JUMP_IF_TRUE: cout << "JUMP_IF_TRUE"; break;
            case LOAD_CONST: cout << "LOAD_CONST"; break;
            case LOAD_VAR: cout << "LOAD_VAR"; break;
            case CALL_FUNCTION: cout << "CALL_FUNCTION"; break;
            case DECLARE_FUNCTION: cout << "DECLARE_FUNCTION"; break;
            case RETURN: cout << "RETURN"; break;
            case AND: cout << "AND"; break;
            case OR: cout << "OR"; break;
            case NOT: cout << "NOT"; break;
            case NEG: cout << "NEG"; break;
            case START_LOOP: cout << "START_LOOP"; break;
            case END_LOOP: cout << "END_LOOP"; break;
            case TO_SCOPE: cout << "TO_SCOPE"; break;
            case EQUAL: cout << "EQUAL"; break;
            case NOT_EQUAL: cout << "NOT_EQUAL"; break;
            case NOT_GREATER_THAN: cout << "NOT_GREATER_THAN"; break;
            case NOT_LESS_THAN: cout << "NOT_LESS_THAN"; break;
            case SHIFT_LEFT: cout << "SHIFT_LEFT"; break;
            case SHIFT_RIGHT: cout << "SHIFT_RIGHT"; break;
            case BITWISE_OR: cout << "BITWISE_OR"; break;
            case BITWISE_AND: cout << "BITWISE_AND"; break;
            case BITWISE_XOR: cout << "BITWISE_XOR"; break;
            case BITWISE_NOT: cout << "BITWISE_NOT"; break;
            case LESS_THAN: cout << "LESS_THAN"; break;
            case GREATER_THAN: cout << "GREATER_THAN"; break;
            case CLEAR: cout << "CLEAR"; break;
            case TABLE_ADDING: cout << "TABLE_ADDING"; break;
            case MAKING_TABLE: cout << "MAKING_TABLE"; break;
            case CLEANING_TABLE: cout << "CLEANING_TABLE"; break;
            case PUSH_ARGUMENT: cout << "PUSH_ARGUMENT"; break;
            case POP_ARGUMENT: cout << "POP_ARGUMENT"; break;
            case CLEAN_ARGUMENT: cout << "CLEAN_ARGUMENT"; break;
            case INDEX: cout << "INDEX"; break;
            case ADD_CONTINUE: cout << "ADD_CONTINUE"; break;
            case ADD_BREAK: cout << "ADD_BREAK"; break;
            case PTR: cout << "PTR"; break;
        }

        // 若有操作數，則打印出額外數據
        if (instruction.hasOperand) {

          if (is_string(instruction.data))
          {
            cout << " DATA: " <<var_to_string(instruction.data) ;

          }
        }
        if (instruction.data2 != 0) {
        //    cout << " Data2: " << instruction.data2;
        }
        if (instruction.data3 != 0) {
           // cout << " Data3: " << instruction.data3;
        }
        if (instruction.where != 0) {
            //cout << " Where: " << instruction.where;
        }
        cout << " ";
    }
}