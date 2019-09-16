This assignment contains 4 codes: 
a4Server1.c - runs a server that runs commands and dies upon reception of end command
a4Client1.c - runs a client that sends commands, receives output, outputs to 'Output-File'
a4Server2.c - runs a server that only terminates connection upon reception of end command, terminates itself with alarm, handles more than one server
a4Client2.c - runs a client that sends commands, receives output, sends ALL output(no console output) to a log file given as argument
Both clients require address, port number, and a command file given as arguments.
a4Client2.c requires a fourth argument, a log file name to output to.
Both servers require port number as an argument
To compile use make with given Makefile
