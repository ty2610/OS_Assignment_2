#include <iostream>
#include <string>
#include <algorithm>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/wait.h>
#include <pwd.h>


//This is a CPP that will be compiled under c++ standard 11
//compilable with g++ -o main main.cpp -std=c++11

using namespace std;

string* splitStringWithDelimeter(string path, string delimeter);
string SearchPath(string path, string *splitted);
void lsr(string arg);
void lsRecursion(DIR *parent, string parentPath, int howDeep);
string trimWhiteSpace(string str);

int main() {
    //getenv() will determine the PATH environment variable
    //use fork() and execv() for spawning new processes
    string id;
    string path = getenv("PATH");
    string colonDelimiter = ":";
    string *splitted = splitStringWithDelimeter(path,colonDelimiter); // this function is to only be used on delimeters of String type of size 1

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

        if(id != "") {
            //Looked for best way to trim whitespace
            //https://stackoverflow.com/questions/25829143/c-trim-whitespace-from-a-string
            if (trimWhiteSpace(id) == "lsr") {
                lsr("");
            } else if (trimWhiteSpace(id).substr(0,4) == "lsr ") {
                lsr(trimWhiteSpace(id.substr(3)));
            } else {
                string spaceDelimiter = " ";
                string *temp = splitStringWithDelimeter(id, spaceDelimiter);
                int returnStatus;

                int len = count(id.begin(), id.end(), ' ');
                char **arg = new char *[len + 1];
                for (int i = 0; i < len + 1; i++) {
                    arg[i] = (char *) temp[i].c_str();
                }
                arg[len + 1] = NULL;
                string pathLocation = SearchPath(arg[0], splitted);
                //cout << "The given arguments are: " << endl;
                /*for(int i=0; i<len+1; i++) {
                    cout << arg[i] << endl;
                }*/
                //cout << "The found path is: " << pathLocation << endl;
                if (pathLocation != "") {
                    pid_t pFork = fork();
                    switch (pFork) {
                        case -1: {
                            // Error Handling
                            //cout << "ERROR FORK" << endl;
                        }
                            break;
                        case 0: {
                            // Child Process
                            //cout << "Child Process " << endl;
                            execv(pathLocation.c_str(), arg);
                        }
                            break;
                        default: {
                            // Parent Process
                            //cout << "PARENT PROCESS" << endl;
                            //wait(NULL);
                            waitpid(pFork, &returnStatus, 0);
                            //cout << "PARENT DONE WAITING" << endl;
                        }
                            break;

                    }

                } else {
                    cout << id << ": Error running command" << endl;
                }
            }
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
            //cout << "Error Opening " << splitted[i].c_str() << endl;
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


string trimWhiteSpace(string str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

void lsr(string arg)
//if they type in an argument, pass that. If not, pass an empty string
{
	
	if(arg.compare("")==0)
	{
		DIR *dir = opendir("./");
		lsRecursion(dir,"./", 0);
	} else {
		if(!arg.compare(0,1,"~"))
		{//if using shorthand for home folder
			arg.replace(0,1,getpwuid(getuid())->pw_dir);
		}

		DIR *dir = opendir(arg.c_str());

		//input checking
		if( dir == NULL)
		{//if unable to open file
			perror("Unable to open");
		}
		else
		{
			DIR *dir = opendir(arg.c_str());
			lsRecursion(dir,arg, 0);
		}	
	}	
}

void lsRecursion(DIR *parent,string parentPath, int howDeep)
{
	struct stat s;
	dirent *currentEntry;
    
	while(( currentEntry = readdir(parent)) != NULL)
	{
		if(currentEntry->d_name[0]!='.' )
		{   
		    //if the name of the entry doesn't start with a period
			string f = "";
            for(int i=0; i<howDeep; i++)
            {
                f += "    ";
            }
		    //(if it starts with a period, we can ignore it)
		    // Directory Char 0x4 and File Char 0x8 from cplusplus.com
			if(/*stat(currentEntry->d_name, &s) == 0 && S_ISDIR(s.st_mode) &&*/ currentEntry->d_type == 0x4	)
			{//if it's a directory
			    cout << f << "\033[1;34m" << currentEntry->d_name << " (Directory)\033[0m\n";
			    DIR *recursiveDir = opendir( (parentPath + "/" + currentEntry->d_name).c_str() );
			    //cout << (parentPath + "/" + currentEntry->d_name).c_str() << "\n";
			    if(recursiveDir != NULL)
			    {
				lsRecursion(recursiveDir,parentPath + "/" + currentEntry->d_name, howDeep+1);
				}
			}
			else if( currentEntry->d_type == 0x8)
			{
			    if(stat(currentEntry->d_name, &s) == 0 && s.st_mode & S_IXUSR)
			    {
			        cout << f << "\033[1;32m" << currentEntry->d_name << " (" << s.st_size << " bytes)"<< "\n";
			    }
			    else
			    {
			        cout << f << "\033[0m" << currentEntry->d_name << " (" << s.st_size << " bytes)"<< "\n";
			    }
			    cout << "\033[0m";
			}
		} else {
		}
	}
	closedir(parent);
}

