// modified base code provided in assignment 4 CS 3377.0W2 
// handles alarm call to terminate program and output message
// works with more than one client
// runs commands sent by client, outputs result, send result to client
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAXLINE 4096 /*max text line length*/

#define LISTENQ 8 /*maximum number of client connections*/


static void sig_alrm(int signo){
	printf("Server timeout\n"); // if SIGALRM signal caught, display time ran out
	raise(SIGINT); // raise SIGINT to terminate execution
}

int main (int argc, char **argv)
{
 int listenfd, connfd, n, servpid;
 pid_t childpid;
 socklen_t clilen;
 char buf[MAXLINE];
 struct sockaddr_in cliaddr, servaddr;
 FILE *fp; // to store output of commands
 char holdThis[MAXLINE], sender[MAXLINE];
 //Create a socket for the socket
 //If sockfd<0 there was an error in the creation of the socket


signal(SIGALRM, sig_alrm);//handle SIGALRM
alarm(300); // raise alarm signal after 5 minutes (3000seconds)

if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  	perror("Problem in creating the socket");
  	exit(2);
 }

 //preparation of the socket address
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 servaddr.sin_port = htons(atoi(argv[1])); // modified to take port number as argument

 //bind the socket
 bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

 //listen to the socket by creating a connection queue, then wait for clients
 listen (listenfd, LISTENQ);

 printf("%s\n","Server running...waiting for connections.");
 int i = 0;
 for ( ;; ) {
  clilen = sizeof(cliaddr);
  //accept a connection
  i++;
  connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
  printf("%s\n","Received request...");

  if ( (childpid = fork ()) == 0 ) {	//if it’s 0, it’s child process
    	printf ("%s%d\n","Child created for dealing with client: ", i);
    	
	//close listening socket
    	close (listenfd);
		
    	while ( (n = recv(connfd, buf, MAXLINE,0)) > 0)  {
	
	servpid = getpid();
	printf("\n");		
	printf("%s", "Server pid: ");
	printf("%d\n", servpid); // prints out pid 
	
	system("date"); // print out time and date

	printf("%s","String received from the client: ");
     	
	puts(buf); // print out command received
		
	if(strstr(buf, "end") != NULL){ // check for end, terminate if its there
		printf("Connection Terminated\n");
		break;		
	}

	fp = popen(buf, "r"); // run command, store output in fp FILE object
	
	while(fgets(holdThis, sizeof(holdThis), fp) != NULL){ //output result of popen call
	
		printf("%s", holdThis);
		strncat(sender, holdThis, sizeof(holdThis)); //concatenate to sender, what we send to client
	
	}

	
	pclose(fp);
    	send(connfd, sender, sizeof(sender), 0); // send output to client
	memset(sender, 0, sizeof(sender));			// get rid of previous command output
	memset(buf, 0, sizeof(buf));// get rid of sent string each iteration, to avoid garbled strings	
     }
     if (n < 0)
      	printf("%s\n", "Read error");
     exit(0);
    
  }
  //close socket of the server
  close(connfd);

 }

}	
