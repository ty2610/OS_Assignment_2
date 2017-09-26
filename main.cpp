#include <iostream>
#include <string>
#include <algorithm>
//This is a CPP that will be compiled under c++ standard 11
//compilable with g++ -o main main.cpp -std=c++11

using namespace std;

string* splitStringWithDelimeter(string path, string delimeter);

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

    cout << path << endl;

    for(int i=0; i<amountOfColons+1; i++) {
        position = path.find(delimiter);
        splitted[i] = path.substr(0, position);
        cout << splitted[i] << endl;
        path.erase(0, position + delimiter.length());
    }
    /*while(!valid) {
        getline(cin, id);
        if(false){
            valid = true;
        } else {
            getline(cin, id);
        }
    }*/

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