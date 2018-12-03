#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

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

typedef struct
{
    int pid;
    int ppid;
    int pc;
    int value;
    int priority;
    int state;
    int startt;
    int runt;
} process_t;

map<string, vector<instruction_t>> files;
int pipefd[2];
int SLEEPY_TIME = 1;

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

void tick()
{
    // TODO
}

void unblock()
{
    // TODO
}

int mgrHandleInput(char input)
{
    // TODO: handle input
    return 0;
}

void printReport()
{
    // TODO: print report
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

    // commander process
    pipe(pipefd);
    // spawn process manager
    pid_t pmanager = fork();
    if (pmanager < 0)
    {
        perror("commander: fork() failed");
        exit(EXIT_FAILURE);
    }
    else if (pmanager == 0)
    {
        // manager process
        char in;
        int error;
        int status = 1;
        fprintf(stderr, "manager: starting...\n");
        while(status)
        {
            error = read(pipefd[0], &in, 1);
            switch (error) {
                case -1:
                    perror("manager: read() failed");
                case 0:
                    status = 0;
                    break;
                default:
                    status = mgrHandleInput(in);
                    break;
            }
        }
        fprintf(stderr, "manager: done\n");
    }
    else
    {
        // commander process
        int in;
        fprintf(stderr, "commander: starting...\n");
        while ((in = getchar()) != EOF)
        {
            write(pipefd[1], &in, 1);
            sleep(SLEEPY_TIME);
        }
        fprintf(stderr, "commander: done\n");
    }

    return 0;
}
