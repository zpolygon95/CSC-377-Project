#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

map<string, vector<string>> files;

void parse_files(string file)
{
    if (!files.count(file))
    {
        ifstream in(file);
        string str;
        vector<string> instructions;
        while(getline(in, str))
        {
            if (str.size() > 0)
                instructions.push_back(str);
        }
        if (instructions.size() > 0)
            files.insert(pair<string, vector<string>>(file, instructions));
        for (int i = 0; i < instructions.size(); i++)
        {
            if (instructions[i][0] == 'R')
            {
                string nextpath = instructions[i].substr(2);
                parse_files(nextpath);
            }
        }
    }
}


int main(int argc, char const *argv[]) {
    if (argc < 2) {
        cout << "Usage: simulate <file> [-f]" << endl;
        return 1;
    }
    bool fast = (argc > 2) && (string(argv[2]) == "-f");
    string path(argv[1]);
    parse_files(path);
    map<string, vector<string>>::iterator it;
    for(it = files.begin(); it != files.end(); it++)
    {
        cout << it->first << " => " << it->second.size() << endl;
    }
    return 0;
}
