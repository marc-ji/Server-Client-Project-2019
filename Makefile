all: a4Server1 a4Client1 a4Server2 a4Client2

a4Server1: a4Server1.c
	gcc a4Server1.c -o a4Server1
a4Client1: a4Client1.c
	gcc -std=c99 a4Client1.c -o a4Client1
a4Server2: a4Server2.c
	gcc a4Server2.c -o a4Server2
a4Client2: a4Client2.c
	gcc -std=c99 a4Client2.c -o a4Client2
