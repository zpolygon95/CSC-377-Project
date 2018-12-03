#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class instruction_t {
public:
    string opcode;
    int arg_int;
    string arg_string;

    instruction_t (string line)
    {
        opcode = line[0];
        arg_int =
            (opcode == "R" || line.size() <= 2) ? 0 : stoi(line.substr(2));
        arg_string =
            (opcode != "R" || line.size() <= 2) ? "" : line.substr(2);
    }
};

map<string, vector<instruction_t>> files;

void parse_files(string file)
{
    if (!files.count(file))
    {
        ifstream in(file);
        string str;
        vector<instruction_t> instructions;
        while(getline(in, str))
        {
            if (str.size() > 0)
                instructions.push_back(instruction_t(str));
        }
        if (instructions.size() > 0)
            files.insert(pair<string, vector<instruction_t>>(file, instructions));
        for (int i = 0; i < instructions.size(); i++)
        {
            if (instructions[i].opcode == "R")
            {
                string nextpath = instructions[i].arg_string;
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
    map<string, vector<instruction_t>>::iterator it;
    for(it = files.begin(); it != files.end(); it++)
    {
        cout << it->first << " => " << endl;
        for (int i = 0; i < it->second.size(); i++)
        {
            cout << "  " << it->second[i].opcode << " " << it->second[i].arg_int << "|" << it->second[i].arg_string << endl;
        }
    }
    return 0;
}
