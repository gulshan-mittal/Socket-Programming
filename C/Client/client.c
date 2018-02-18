// Client side C/C++ program to demonstrate Socket programming

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>


#define LENGTH 512

int main(int argc, char *argv[])
{
	
	int sockfd, PORT, rcv_bytes = 0;
	char rcv_buffer[LENGTH], dir[1000];
	struct sockaddr_in serv_addr;

	getcwd(dir,sizeof(dir));
	strcat(dir, "/"); strcat(dir, argv[3]);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Socket Failed!");
		exit(EXIT_FAILURE);
	}

	
	memset(&serv_addr, '0', sizeof(serv_addr));   //to make sure the struct is empty. Essentially sets sin_zero as 0 
	                                              // which is meant to be, and rest is defined below

	PORT = atoi(argv[2]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Converts an IP address in numbers-and-dots notation into either a 
	 // struct in_addr or a struct in6_addr depending on whether you specify AF_INET or AF_INET6.

	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) < 0)
	{
		perror("Invalid Address or Address is in use!");
		exit(EXIT_FAILURE);
	}

	bzero(&(serv_addr.sin_zero), 8);

	
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0) // connect to the server address
	{
		perror("Connection Failed !");
		exit(EXIT_FAILURE);
	}

	printf("Connected Established at port %d... Ok!\n", PORT);

	send(sockfd , argv[3], strlen(argv[3]), 0);   // send the message containinf filename. 
	
	char signal_msg[26];
	int valread;
	valread = read(sockfd, signal_msg, 26);  // receive message back from server if file present or not
	signal_msg[1] = '\0';
	
	if(signal_msg[0]== 'y')
	{
		printf("File Found on Server\n");
	}
	else
	{
		printf("File not Found\n");
		return 0;

	}


	printf("Receiveing file from Server ....\n");
	
	// char* fr_name = "/home/gulshan/Videos/gulshan.txt";

	FILE *file_pointer = fopen(dir, "w+");              //open the file to be downloaded
	
	if(file_pointer == NULL)
	{
		printf("File %s Cannot be opened.\n", dir);    // error handling if file cannot be written
		exit(EXIT_FAILURE);
	}

	bzero(rcv_buffer, LENGTH);


	while((rcv_bytes = read(sockfd, rcv_buffer, LENGTH)) > 0)       // recieve data from server
	{
		if(fwrite(rcv_buffer, sizeof(char), rcv_bytes, file_pointer) < rcv_bytes)
		{
			perror("File write failed! ");
			exit(EXIT_FAILURE);
		}

		bzero(rcv_buffer, LENGTH);

		if (rcv_bytes != LENGTH)
		{
			break;
		}
	}

	if(rcv_bytes < 0 && errno == EAGAIN)
	{
		perror("Connection time out!");
		fclose(file_pointer);
		close (sockfd);
		exit(EXIT_FAILURE);
	}

	else if(rcv_bytes < 0 && errno != EAGAIN)
	{
		fprintf(stderr, "recv() failed due to errno = %d\n", errno);
		fclose(file_pointer);
		close (sockfd);
		exit(EXIT_FAILURE);
	}

	printf("File recieved successfully!\n");
	fclose(file_pointer);
	close (sockfd);
	printf("Connection lost. => {Client}\n");
	return (0);
}
