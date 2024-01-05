/****************
LE2: Basic Shell

ZACHARY MCDOWELL 227007875
****************/
#include <iostream>
#include <unistd.h> // pipe, fork, dup2, execvp, close
#include <sys/wait.h> // wait
#include "Tokenizer.h"
using namespace std;

int main () {
    // TODO: insert lab exercise 2 main code here

    int in = dup(0); // saving STDIN 
    int out = dup(1); // saving STDOUT  

    string cmd; // string to hold command
    
    
    // until user gives an error or calls exit, run 
    while (true){
        cout << "input command: ";
        getline(cin, cmd); // input command 

        // if user asks to exit 
        if (cmd == "exit"){
            break;
        }

        // if blank command
        if (cmd == ""){
            continue;
        }
        
        Tokenizer token(cmd); // creating token

        // isError continue
        if (token.hasError() == true){
            break; // break if error
        }

        for (size_t i = 0; i < token.commands.size(); i++){

            int fd[2]; // int[] fd = new int[2]; // 0th index read, 1st write
            pipe(fd); // create pipe
            pid_t pid = fork(); // int pid = fork(); // fork

            // Child to run first command
            if (pid == 0){
                
                // vector of strings that hold arguments
                vector<string> args = token.commands.at(i)->args;

                // want to make sure last one continues to go to the terminal

                // if last, we do not want to redirect the output 
                if (i < token.commands.size() - 1){
                    dup2(fd[1], 1); // set stdout to pipe
                }
                
                
                
                close(fd[0]); // closing read end of the pipe
                
                // run the command
                // command -> args => list of arguments
                char** arguments = new char*[token.commands.at(i)-> args.size()+1];


                for(size_t y = 0; y < args.size(); y++){
                    arguments[y] = (char *)args.at(y).c_str(); // converting to char* and adding to arguments
                }

                // adding the nullptr to our arguments
                arguments[args.size()] = nullptr;

                execvp(arguments[0], arguments); // exececute commands

                delete[] arguments; // deleting the newly allocated char**                

            }
                
            // parent
            else{

                wait(0); // wait until last command finishes

                dup2(fd[0], 0); // read rather from stdin, read form pipe

                close(fd[1]); // close write end of the pipe

                
                
            }
        }

            // resetting STDIN and STDOUT
            dup2(in, 0);
            dup2(out, 1);
    }

    

    

    return 0;
}