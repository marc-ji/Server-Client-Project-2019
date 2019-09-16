// terminates on alarm or when end is read
// outputs everything to a log file, no console output will just run and end
#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<time.h>
int main(int argc, char **argv){
	struct sockaddr_in serverAddr;
	FILE *fp, *out;
	int sock,i = 0, bufsize = 4096;
	char buf[4096], recbuf[4096];
	char* command[4096];
	time_t timep;
	struct tm * timeInfo;
	alarm(300); // set alarm to 5 minutes, no handler, default action is to interrupt
	time(& timep);
	timeInfo = localtime(&timep); // get time
	fp = fopen(argv[3], "r"); // open name of commmand file
        
	command[i] = malloc(bufsize);	// dynamically read into command array
	while(fgets(command[i], bufsize, fp) != NULL){
		i++;	
		command[i] = malloc(bufsize);
		
	}
	
	int size = i; // get argc
	
	// set up socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]); // take address and port as arguments
	serverAddr.sin_port = htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0){ // connect
		perror("Could not connect!");
		exit(0);
	}
	
	for(int j = 0; j < size-1; j++){ // keep going as long as thers arguments
		out = fopen(argv[4], "a"); // open file in append mode since loop (otherwise we'll only get last line using w)
		int pid = getpid();
		fprintf(out, "\n%s", "Client pid: "); // write out all to file
		fprintf(out, "%d", pid);
		fprintf(out, "\nTime: %s", asctime(timeInfo));
		fprintf(out, "\n%s", "Command sent: ");
		fprintf(out, "%s", command[j]);	
		send(sock, command[j], strlen(command[j]), 0); // send command
	

		if(recv(sock, recbuf, 4096, 0) == 0){ // receive output of server
			fputs("Server died", out);
			exit(0);
		}
		
		fprintf(out, "%s\n", "Output: ");
		fputs(recbuf, out);
		

		fclose(out);
	}

	fclose(fp);
	exit(0);
}

