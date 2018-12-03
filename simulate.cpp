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

class CPU
{
public:
    int current_time = 0;
    vector<process_t> PCBTable;
    deque<int> ReadyState;
    deque<int> BlockedState;
    int RunningState;

    process_t get_proc_by_id(int pid)
    {
        // TODO
    }

    void tick()
    {
        // TODO
        current_time++;
    }

    void load_proc()
    {
        rproc = ReadyState.pop_front();
        rproc.state = RUNNING;
    }

    void park_proc()
    {
        rproc = get_proc_by_id(RunningState);
        rproc.state = READY;
        ReadyState.push_back(rproc.pid);
        load_proc();
    }

    void block()
    {
        rproc = get_proc_by_id(RunningState);
        rproc.state = BLOCKED;
        BlockedState.push_back(rproc.pid);
        load_proc();
    }

    void unblock()
    {
        bproc = BlockedState.pop_front();
        bproc.state = READY;
        ReadyState.push_back(bproc.pid);
    }

    void print_current_state()
    {
        // TODO
    }

    CPU(string init)
    {
        process_t init_proc = make_proc(init);
        PCBTable.insert(init_proc);
        RunningState = init_proc.pid;
    }
}

typedef struct
{
    int pid;      // process id
    int ppid;     // parent pid
    int pc;       // index into the currently running file
    string file;  // file the program is currently running
    int value;    // value of the program's register
    int priority; // quantum size = 1 << priority
    int state;    // 2 for running, 1 for ready, 0 for blocked
    int startt;   // value of current_time at which the process was spawned
    int runt;     // total run time in ticks
    int slice;    // stores the number of ticks used of the current quantum
} process_t;

#define RUNNING 2
#define READY 1
#define BLOCKED 0

process_t make_proc(string file, int ppid = -1, int cpu_time = 0)
{
    process_t out;
    out.pid = next_pid;
    out.ppid = ppid;
    out.pc = 0;
    out.file = file;
    out.state = READY;
    out.startt = cpu_time;
    out.runt = 0;
    out.slice = 0;
}

map<string, vector<instruction_t>> files;
CPU cpu;

int next_pid = 0;
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

int mgrHandleInput(char input)
{
    // TODO: handle input
    int out = 1;
    switch (input) {
        case 'Q':
            cpu.tick();
            return 1;
        case 'U':
            cpu.unblock();
            return 1;
        case 'T':
            out = 0;
        case 'P':
            printReport();
            return out;
        default:
            return 0;
    }
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
