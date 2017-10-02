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
    string path = getenv("PATH");
    string *splitted;
    string colonDelimiter = ":";

    //cout << path << endl;

    /*for(int i=0; i<amountOfColons+1; i++) {
        position = path.find(delimiter);
        splitted[i] = path.substr(0, position);
        cout << splitted[i] << endl;
        path.erase(0, position + delimiter.length());
    }*/

    splitted = splitStringWithDelimeter(path,colonDelimiter); // this function is to only be used on delimeters of String type of size 1

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
        string spaceDelimiter = " ";
        /*for(int i=0; i<amountOfColons+1; i++) {
            position = id.find(delimiter);
            arg[i] = id.substr(0, position);
            id.erase(0, position + delimiter.length());
        }*/
        string *temp = splitStringWithDelimeter(id,spaceDelimiter);
        int len = count(id.begin(),id.end(),' ');
        char **arg = new char*[len + 1];
        for(int i = 0; i < len+1; i++)
        {
            arg[i] = (char*)temp[i].c_str();
        }

        string pathLocation = SearchPath(arg[0],splitted);
        //cout << pathLocation << endl;
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
                    //input is a string* but the function needs a char* const*
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
    long amountOfColons = count(path.begin(),path.end(),delimiter.at(0));
    string *splitted =  new string[amountOfColons+1];
    for(int i=0; i<amountOfColons+1; i++) {
        position = path.find(delimiter);
        splitted[i] = path.substr(0, position);
        //cout << splitted[i] << endl;
        path.erase(0, position + delimiter.length());
    }
    return splitted;
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
