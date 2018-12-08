# Process Management Simulation #

Members: Zachary Polikarpus, Jordan VanDusen, Nickxit Bhardwaj

## Building ##

Requires `make` of some sort, `g++`, and a POSIX environment. On Windows, the
Linux subsytem will do. To build without running, make the default target

```
$ make
```

To build and run with the example set of programs, make the `run` target

```
$ make run
```

## Running ##

The program accepts one necessary and one optional command line argument. The
necessary first argument is the name of the initial program file to simulate.
The optional second argument is a flag that prevents the program from sleeping
for a second between input symbols (as dictated by the specification).

```
$ bin/simulate <program> [-f]
```

Once the program starts, the symbols read from STDIN control the flow of the
simulation. In order to facilitiate efficient testing, unrecognized symbols
(such as a line-feed character) are quietly ignored.

Recognized symbols are:
+ Q: Advance the state of the simulation
+ U: Unblock the first process on the "blocked" queue
+ P: Print the current state of the simulation
+ T: Terminate the simulation

Please note that, due to the specification's insistence that the
"manager process" had to receive communications from the "commander process" via
a pipe, there is no way for the manager process to inform the commander process
that it has completed successfully. Therefore, the commander process must be
terminated by sending an EOF (ctrl-D in a POSIX shell) after terminating the
manager process with a T. Alternatively, a file of commands can be piped to
STDIN; in which case the EOF is sent when the end of the input file is reached.
