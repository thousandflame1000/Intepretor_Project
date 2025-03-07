#include<vector>
#include<string >  
#include <fstream> 
#include<iostream>
#include<set>
#include<unordered_set>
#include<stack > 


using namespace std  ; 









bool isDelimiter(char c) {
    return c == ' ' || c == '\n' || c == ';' || c == '(' || c == ')' || c == '{' || c == '}';
}




int main ( ) 
{
    string File_Path = "" ; 

    std::cin >> File_Path ;  

    unordered_set Token_Dict  = {"if" , "else" , "elif" , "for" , "while" , "{" , "}" ,"(" , ")", "[" ,"]" , "+" , "-" ,"*" ,"/" , "\\" ,"" } ; 
    
    vector<string> tokens = {} ; 
    

    std::ifstream file(File_Path); // 打开文件
    if (!file) {
        std::cerr << "无法打开文件\n";
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

    for (int i = 0 ; i < tokens.size() ; i++)
    {
        cout << tokens[i] << endl ; 
    }





}


