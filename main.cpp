/*
$$\      $$\           $$\   $$\     $$\        $$$$$$\                                    $$\       $$\                    
$$$\    $$$ |          $$ |  $$ |    \__|      $$  __$$\                                   $$ |      $$ |                   
$$$$\  $$$$ |$$\   $$\ $$ |$$$$$$\   $$\       $$ /  \__| $$$$$$\  $$\   $$\ $$$$$$\$$$$\  $$$$$$$\  $$ | $$$$$$\  $$$$$$$\ 
$$\$$\$$ $$ |$$ |  $$ |$$ |\_$$  _|  $$ |      $$ |      $$  __$$\ $$ |  $$ |$$  _$$  _$$\ $$  __$$\ $$ |$$  __$$\$$  _____|
$$ \$$$  $$ |$$ |  $$ |$$ |  $$ |    $$ |      $$ |      $$ |  \__|$$ |  $$ |$$ / $$ / $$ |$$ |  $$ |$$ |$$$$$$$$ \$$$$$$\  
$$ |\$  /$$ |$$ |  $$ |$$ |  $$ |$$\ $$ |      $$ |  $$\ $$ |      $$ |  $$ |$$ | $$ | $$ |$$ |  $$ |$$ |$$   ____|\____$$\ 
$$ | \_/ $$ |\$$$$$$  |$$ |  \$$$$  |$$ |      \$$$$$$  |$$ |      \$$$$$$  |$$ | $$ | $$ |$$$$$$$  |$$ |\$$$$$$$\$$$$$$$  |
\__|     \__| \______/ \__|   \____/ \__|       \______/ \__|       \______/ \__| \__| \__|\_______/ \__| \_______\_______/                                                                                                                 
*/
#include<vector>
#include <iostream>
#include<string>
#include <fstream>
#include <cstdlib>  // srand
#include <ctime>    // time



#include "lexer.h"
#include "parser.h"
#include "evaluate.h"
using namespace std ;

int& sum_reference  ( int& ref )
{
    return ref  ; 
}


//#include "parser.h"s
std::string preprocessString(const std::string& input) {
    std::string result;
    bool escape = false;

    for (size_t i = 0; i < input.length(); ++i) {
        char c = input[i];

        if (escape) {
            switch (c) {
                case 'n': result += '\n'; break;
                case 't': result += '\t'; break;
                case 'a': result += '\a'; break;
                case 'b': result += '\b'; break;
                case 'r': result += '\r'; break;
                case 'f': result += '\f'; break;
                case 'v': result += '\v'; break;
                case '\\': result += '\\'; break;
                case '\'': result += '\''; break;
                case '\"': result += '\"'; break;
                case '0': result += '\0'; break;
                default:
                    // Unknown escape, keep as-is
                    result += '\\';
                    result += c;
                    break;
            }
            escape = false;
        } else if (c == '\\') {
            escape = true;
        } else {
            result += c;
        }
    }

    // If input ends with a lone backslash
    if (escape) {
        result += '\\';
    }

    return result;
}


int main(int argc, char *argv[]){
    
    srand(time(nullptr));
    string str = "" ;
    if (argc < 2) {
        cerr << "Please Enter The file you Want to Run" << endl;
        return 1;
    }

    string filename = argv[1];  // argv[0] 是程式名稱，argv[1] 是第一個參數(檔名)
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Can not Open The File  " << filename << endl;
        return 1;
    }


    string line ;
    string code="{"  ;  
    while (getline(file, line)) {
        code = code + line + " " ; 
    }
    code = code + "}" ; 
    
    
 
    Lexer a(code) ;
    int mm  = 2 ;
    sum_reference(mm) = 3 ;    
    vector<Token> tks = a.tokenize() ; 

   
    for (int i = 0 ;  i < tks.size()  ; i++)
    {
      //  cout << tks[i].GetValue()  << " "   
    }

    


    for (int i = 0 ; i < tks.size () ; i++)
    {
        if (tks[i].GetType() == TokenType::STRING)
        {


            tks[i].ChangeValue(preprocessString(tks[i].GetValue())) ; 
        }

    }





    
    string aabb ; 
    Parser p (tks) ; 
    shared_ptr<Node> flo = p.ParseBlock() ; 

    
    //flo->print();  
    Interpreter machine  ("Boot") ;
    if (! p.PARSING_ERROR){
        machine.Code_Generation(flo) ;
        //machine.printByteCode();
        string a ; 
        
        machine.execute() ; 
    } 






//    flo->print() ; 

    //flo->print() ; 

    (1+1);  
    return 0;
   
}