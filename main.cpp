#include <iostream>
#include <string>
#include <algorithm>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h> 

//This is a CPP that will be compiled under c++ standard 11
//compilable with g++ -o main main.cpp -std=c++11

using namespace std;

string* splitStringWithDelimeter(string path, string delimeter);

string SearchPath(string path, string *splitted); 

int main() {
    //getenv() will determine the PATH environment variable
    //use fork() and execv() for spawning new processes
    string id;
    bool valid = false;
    string path = getenv("PATH");
    long amountOfColons = count(path.begin(),path.end(),':'); //10 colons  11 strings
    string *splitted =  new string[amountOfColons+1];
    int position = 0;
    string delimiter = ":";

    //cout << path << endl;

    for(int i=0; i<amountOfColons+1; i++) {
        position = path.find(delimiter);
        splitted[i] = path.substr(0, position);
        //cout << splitted[i] << endl;
        path.erase(0, position + delimiter.length());
    }
    
    bool running = true;
    
    // Input/Output to screen
    cout << "Welcome to OSShell!! Please enter your commands ('exit' to quit)." << endl;
    while(running == true)
    {
        // cout << "\033[1;32m" <<  "Hello world"  << "\033[0m\n"; 
        // Colors: Green(32) and Blue(34) from cplusplus.com 
        cout << "osshell> ";
        getline (cin, id);
        if(id == "exit")
        {
            running = false;
            break;
        }
        
        int spaces = count(id.begin(),id.end(),' ');
        string *arg = new string[spaces+1]; // One less space than arguments
        int position = 0;
        string delimiter = " ";
        for(int i=0; i<amountOfColons+1; i++) {
            position = id.find(delimiter);
            arg[i] = id.substr(0, position);
            id.erase(0, position + delimiter.length());
        }
        
        string pathLocation = SearchPath(arg[0],splitted);
        cout << pathLocation << endl;
        if (pathLocation != "")
        {
            pid_t pFork = fork();
            switch(pFork)
            {
                case -1:
                {
                    // Error Handling
                } break;
                case 0:
                {
                    // Child Process
                    execv(pathLocation.c_str(),arg);
                } break;
                default:
                {
                    // Parent Process
                } break;
            
            }
            
        }
        else
        {
            cout << id << ": Error running command" << endl;
        }
    }  
    return 0;
}

string* splitStringWithDelimeter(string path, string delimiter) {
    int position = 0;
    long amountOfColons = count(path.begin(),path.end(),':');
    string *splitted =  new string[amountOfColons+1];
    for(int i=0; i<amountOfColons; i++) {
        position = path.find(delimiter);
        splitted[i] = path.substr(0, position);
        cout << splitted[i] << endl;
        path.erase(0, position + delimiter.length());
    }
}

string SearchPath(string path, string *splitted) 
// Search the path directories to see if command exists
{
    /* Used cplusplus.com to understand file directories 
    in C++ and how to manuver in them*/
    for(int i = 0; i < 11; i++)
    {   
        DIR *dir = opendir(splitted[i].c_str());
        if (dir == NULL)
        {
            cout << "Error Opening " << splitted[i].c_str() << endl;
        }
        else
        {
            struct dirent *ent;
            while((ent = readdir(dir)) != NULL)
            {
                string fname = ent->d_name;
                if (fname == path)
                {
                    return splitted[i] + "/" + path;
                }
            }
        }
    }  
    return "";
}
