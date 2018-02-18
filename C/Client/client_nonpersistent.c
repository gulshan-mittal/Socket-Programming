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
	
	while(1)
	{

	
		int sockfd, PORT, rcv_bytes = 0;
		struct sockaddr_in serv_addr;	

		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
		{
			perror("Socket Failed!");
			exit(EXIT_FAILURE);
		}

		
		memset(&serv_addr, '0', sizeof(serv_addr));

		PORT = atoi(argv[2]);
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(PORT);

		if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) < 0)
		{
			perror("Invalid Address or Address is in use!");
			exit(EXIT_FAILURE);
		}

		bzero(&(serv_addr.sin_zero), 8);
		if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
		{
			perror("Connection Failed !");
			exit(EXIT_FAILURE);
		}

		printf("Connected Established at port %d... Ok!\n", PORT);

	
	
		printf("Enter the Filename or Type 'q' to exit\n");
	
		char file_name[10000];
		scanf ("%s", file_name);
		
		printf("%s\n",file_name);

		if(file_name[0] == 'q')
		{
			close (sockfd);
			printf("Connection lost. => {Client}\n");
			break;
		}

		send(sockfd , file_name, strlen(file_name), 0);

		char *rcv_buffer;
		rcv_buffer = (char *) malloc(LENGTH*sizeof(char));	
			
		char signal_msg[LENGTH];
		int valread;
		memset(signal_msg,0,sizeof(signal_msg));
		valread = read(sockfd, signal_msg,LENGTH);
		signal_msg[1] = '\0';
		//printf("%s\n", signal_msg);

		if(signal_msg[0] == 'n')
		{
			printf("File not Found on server !\n");
		}
		else
		{
			printf("File found on server\n");
		


			printf("Receiveing file from Server ....\n");
			

			FILE *file_pointer = fopen(file_name, "w");
			
			if(file_pointer == NULL)
			{
				printf("File %s Cannot be opened.\n", file_name);
				exit(EXIT_FAILURE);
			}

			memset(rcv_buffer,'0',sizeof(rcv_buffer));

			while((rcv_bytes = read(sockfd, rcv_buffer, LENGTH)) > 0)
			{
				 
				if((fwrite(rcv_buffer, sizeof(char), rcv_bytes, file_pointer)) < rcv_bytes)
				{
					perror("File write failed! ");
					exit(EXIT_FAILURE);
				}

				memset(rcv_buffer,'0',sizeof(rcv_buffer));

				if (rcv_bytes != LENGTH)
				{
					break;
				}
			}

			if(rcv_bytes < 0 && errno == EAGAIN)
			{
				printf("Connection time out!\n");
				fclose(file_pointer);
			
			}

			else if(rcv_bytes < 0 && errno != EAGAIN)
			{
				fprintf(stderr, "recv() failed due to errno = %d\n", errno);
				fclose(file_pointer);
			
			}
			else
			{	
				free(rcv_buffer);
				printf("File recieved successfully!\n");
				fclose(file_pointer);
			}
		}
			

		close (sockfd);
		printf("Connection lost. => {Client}\n");
		
	}
	
	return (0);
}
