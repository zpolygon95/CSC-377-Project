# Process Management Simulation #

Members: Zachary Polikarpus, Jordan VanDusen, Nickxit Bhardwaj

## Tasks ##

Simulate a process management system

+ [x] Commander Process: Start Simulation
    + [x] Create Pipe
    + [x] Spawn Process Manager
    + [x] Read one command per second from STDIN

+ [ ] Process Manager:
    + [ ] Simulate process functions
        + [ ] creation
        + [ ] replacement
        + [ ] state transitions
        + [ ] scheduling
        + [ ] context switching
    + [ ] Data structures
        + [x] Time: Integer intialized to 0
        + [x] PCBTable: one entry for every process that hasn't finished yet
            + [x] process id
            + [x] parent process id
            + [x] pointer to program counter value (initially 0)
            + [x] integer value
            + [x] priority
            + [x] state
            + [x] start time
            + [x] run time (time used so far)
        + [x] ReadyState: stores all simulated process that are ready to run
        + [x] BlockedState: stores all simulated process that are blocked
        + [x] RunningState: stores PCBTable index of currently running process
    + [ ] Processing input commands
        + [x] Get commands from pipe
            + [ ] Q: tick
                + [ ] Execute next instruction of currently running process
                + [ ] Increment PC (except for F & R instructions)
                + [ ] Increment Time
                + [ ] Perform scheduling
            + [ ] U: Move first process in "blocked" queue to "ready"
            + [ ] P: spawn new reporter process
            + [x] T: terminate
                + [x] Spawn reporter process
                + [x] terminate
            + [ ] Ensure only one reporter process is running
    + [ ] Executing simulated processes
        + [ ] S n, A n, D n: update value stored in CPU (Set, Add, Subtract)
        + [ ] B: block current process
            + [ ] move running process to "blocked" queue
            + [ ] "ready" to "running"
            + [ ] (context switch)
        + [ ] E: terminate running process
            + [ ] free memory
            + [ ] move from "ready" to "running"
            + [ ] (context switch)
        + [ ] F n: create new simulated process
            + [ ] create new entry in PCBTable
            + [ ] assign unique PID
            + [ ] assign running PID to PPID
            + [ ] set start time to current time
            + [ ] set run time to 0
            + [ ] program array and integer copied from parent
            + [ ] priority same as parent
            + [ ] PC of new process 1 + parent PC
            + [ ] PC of parent process set to n + parent PC
            + [ ] new process queued in ready
        + [ ] R filename: replace running process image
            + [ ] program array overwritten by code in filename
            + [ ] PC set to 0
            + [ ] integer value undefined
            + [ ] All else remains the same
    + [ ] Scheduling:
        + [ ] multiple queues with priority classes
        + [ ] 4 priority classes 0-3 (0 highest priority)
        + [ ] initial process given priority 0
        + [ ] quantum size depends on priority
            + [ ] priority 0 = 1 unit
            + [ ] priority 1 = 2 unit
            + [ ] priority 2 = 4 unit
            + [ ] priority 3 = 8 unit
        + [ ] If a running process uses it's time slice completely, it is preempted
          and it's priority is lowered
        + [ ] If a process blocks before it's quantum expires, it's priority is
          raised.
    + [ ] Context switching: Swap process states in and out of the CPU and PCBTable

+ [ ] Reporter Process: Print the current state of the system, then terminate
  + [ ] Current System Time
  + [ ] Information about currently running process
      + [ ] PID
      + [ ] PPID
      + [ ] priority
      + [ ] value
      + [ ] start time
      + [ ] run time
  + [ ] List of Blocked Processes (as above)
  + [ ] List of Ready Processes (as above)
      + [ ] Separated by priority
