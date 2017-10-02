#include <iostream>
#include <string>
#include <algorithm>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>

//This is a CPP that will be compiled under c++ standard 11
//compilable with g++ -o main main.cpp -std=c++11

using namespace std;

string* splitStringWithDelimeter(string path, string delimeter);

string SearchPath(string path, string *splitted);
void lsr(string osLocation, string arg);
void lsRecursion(DIR *parent);


int main() {
    //getenv() will determine the PATH environment variable
    //use fork() and execv() for spawning new processes
	//lsr("C:\\Users\\mill0604\\Desktop\\testFolder",""); //just used for testing lsr()
    string id;
    string path = getenv("PATH");
    string *splitted;
    string colonDelimiter = ":";

    cout << path << endl;

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

        string *arg;
        string spaceDelimiter = " ";
        /*for(int i=0; i<amountOfColons+1; i++) {
            position = id.find(delimiter);
            arg[i] = id.substr(0, position);
            id.erase(0, position + delimiter.length());
        }*/
        arg = splitStringWithDelimeter(id,spaceDelimiter);

        string pathLocation = SearchPath(arg[0],splitted);
        cout << pathLocation << endl;
        if (pathLocation != "")
        {
            /*pid_t pFork = fork();
            switch(pFork)
            {
                case -1:
                {
                    // Error Handling
                } break;
                case 0:
                {
                    // Child Process
                    //execv(pathLocation.c_str(),arg);
                    //input is a string* but the function needs a char* const*
                } break;
                default:
                {
                    // Parent Process
                } break;

            }*/

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

void lsr(string osLocation, string arg)
/*TODO:
	-specifics mentioned in lsRecursion
	-implement printDir
	-error checking

*/

//if they don't type in an argument, pass the directory osshell is using
//if they do, pass that as well
{
	
	if(!arg.compare(""))
	{
		printf("no parent\n");
		DIR *dir = opendir(osLocation.c_str());
		lsRecursion(dir);
		
	} else {
		std::string temp = arg; //for putting the directory together
		printf("yes parent\n");
		temp = osLocation + temp;
		cout << temp;
		DIR *dir = opendir(temp.c_str());
		
		
		
	}
	
	
}

void lsRecursion(DIR *parent)

{
	struct stat s;
	dirent *currentEntry;
	readdir(parent);
	readdir(parent);
	while( (currentEntry = readdir(parent)) != NULL)
	{
		stat(currentEntry->d_name, &s);
		if ((s.st_mode & S_IFMT) == S_IFDIR) //for some reason this doesn't identify if it's a folder - never true
		{
			printf("WE'RE DOING RECURSION\n\n\n");
			lsRecursion((DIR*)currentEntry);//incorrect way to change dirent into DIR, gotta find out how
		}
		printf("I have a file\n");
		cout << currentEntry->d_name;
	}
	
	/*
	string str(readdir(parent)->d_name);
	cout << str <<"\n";
	string str2(readdir(parent)->d_name);
	cout << str2 <<"\n";
	string str3(readdir(parent)->d_name);
	cout << str3 <<"\n";
	string str4(readdir(parent)->d_name);
	cout << str4 <<"\n";
	string str5(readdir(parent)->d_name);
	cout << str5 <<"\n";
	string str6(readdir(parent)->d_name);
	cout << str6 <<"\n";
	*/
}

/*
void printDir(DIR *directory) //The below is an idea or start for printing things out from the directory 
{
	if ((data.attribute & _A_SUBDIR) == _A_SUBDIR)
	{
		cout <<	"[" <<data.name<< "]" << endl;
	}
	else
	{
		cout << data.name << endl;
	}
	
}
	*/
