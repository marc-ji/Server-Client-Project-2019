// client that reads commands from file, sends to server, recieves output, displays output, writes output to file
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
	time(& timep);
	timeInfo = localtime(&timep);
	fp = fopen(argv[3], "r"); // open file name
        
	command[i] = malloc(bufsize);	// dynamically read into array of commands
	while(fgets(command[i], bufsize, fp) != NULL){
		i++;	
		command[i] = malloc(bufsize);
		
	}
	
	int size = i; // get size of array
	
	// set up a socket
	sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&serverAddr, 0, sizeof(serverAddr));
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);// take address as argument
	serverAddr.sin_port = htons(atoi(argv[2])); // take port number as argument
	
	if(connect(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0){ // if we coulf not connect, let know
		perror("Could not connect!");
		exit(0);
	}
	
	for(int j = 0; j < size-1; j++){ // as long as there are still commands, keep going
		int pid = getpid();// print pid of client
		printf("\n%s", "Client pid: ");
		printf("%d", pid);
		printf("\nTime: %s", asctime(timeInfo));
		printf("\n%s", "Command sent: ");
		printf("%s", command[j]);	
		send(sock, command[j], strlen(command[j]), 0); // send command to server
	
	
		if(recv(sock, recbuf, 4096, 0) == 0){ // exit if server terminates connection
			perror("Server died");
			exit(0);
		}
                		
		printf("%s\n", "Output: ");
		fputs(recbuf, stdout); // print output sent by server
		printf("Writing to \"Output-File.txt\"\n");
		out = fopen("Output-File.txt", "a"); // write output to file in append mode because loop, otherwise we get only last line
		fprintf(out, "%s", "Command: ");
		fputs(command[j], out);
		fprintf(out, "%s\n", "Output: ");
		fputs(recbuf, out);
		fclose(out); // close file each time
	}

	fclose(fp);// close file
	exit(0);
}

