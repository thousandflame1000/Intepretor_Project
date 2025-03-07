#include<vector>
#include<string >  
#include<fstream> 
#include<iostream>
#include<set>
#include<unordered_set>
#include<unordered_map>
#include<stack > 


using namespace std  ; 







enum class Virtual_Code : int {
    _FOR ,     
    _WHILE = 10,    
    _IF = 1,       
    _ELSE = 2,      
    _ELIF = 3,      
    _ADD = 4,       
    _SUB = 5,       
    _MUL = 6,       
    _DIV = 7,       
    _MOD = 8,       
    _RET = 9 ,
    _END ,
    _FUNCTION
};
enum class Data_Type :int {
    INT_ , 
    BOOL_,
    STRING_,
    FLOAT_,
    DOUBLE_
};

std::unordered_map<std::string, int> Token_Dict = {
    {"if", static_cast<int>(Virtual_Code::_IF)},       // if
    {"else", static_cast<int>(Virtual_Code::_ELSE)},   // else
    {"return", static_cast<int>(Virtual_Code::_RET)},  // return
    {"while", static_cast<int>(Virtual_Code::_WHILE)}, // while
    {"for", static_cast<int>(Virtual_Code::_FOR)},     // for
    {"+", static_cast<int>(Virtual_Code::_ADD)},       // +
    {"-", static_cast<int>(Virtual_Code::_SUB)},       // -
    {"*", static_cast<int>(Virtual_Code::_MUL)},       // *
    {"/", static_cast<int>(Virtual_Code::_DIV)},       // /
    {"%", static_cast<int>(Virtual_Code::_MOD)}        // %

};

std::unordered_map<std::string , int > Virtual_Data  ; 
std::unordered_map<std::string , vector<int>> Function_name ; 
bool isDelimiter(char c) {
    return c == ' ' || c == '\n' || c == ';' || c == '(' || c == ')' || c == '{' || c == '}' || c == '\"' 
    || c == '\''
    
    
    ;
}

void FSM (vector<int> steps) 
{




}



int main ( ) 
{
    string File_Path = "" ; 

    std::cin >> File_Path ;  

     
    
    stack<int> call_stack  ;  
    vector<string> tokens = {} ; 
    
    vector<vector<int>> Functions ;  
    
    

    std::ifstream file(File_Path); 
    if (!file) {
        std::cerr << "Can't Open the File \n";
        return 1;
    }

    std::string line;
    string Hold_Token = ""; 

    char c ; 
    string token ; 

    while (file.get(c)) {  
        if (isDelimiter(c)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            if (c != ' ' && c != '\n') { 
                tokens.push_back(string(1, c));
            }
        } else {
            token += c;
        }
    }

    
    stack <int> openrator ; 
    stack <char> operators ;
    stack <string> expression_operation ; 
    unordered_map<string , int > expression_address ; 
    int current_function = 0 ; 
    int current_executing  = 0 ; 
    vector<int> sub_control_flow ; 
    bool Is_Main = true ; 
    vector<int> main_control_flow ; 
    for (int i = 0 ; i < tokens.size() ; i++)
        {   
            auto it  = Token_Dict.find(tokens[i]) ;
            
            if (it != Token_Dict.end()) {
                main_control_flow.push_back(it->second) ; 
                
            }     
         
         
        }
    
}