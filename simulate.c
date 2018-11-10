#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int pipefd[2];
int SLEEPY_TIME = 1;

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

// TODO: implemenent data structures

void parse_files(char *)
{
    // TODO
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

int main(int argc, char * const argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "f")) != -1)
    {
        switch (opt)
        {
            // "fast" flag
            case 'f': SLEEPY_TIME = 0;
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

    exit(EXIT_SUCCESS);
}
