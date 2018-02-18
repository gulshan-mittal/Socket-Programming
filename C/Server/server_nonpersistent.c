#include <stdio.h>
#include <signal.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/wait.h>
#include <ctype.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>

#define LENGTH 512

int main (int argc, char *argv[])
{
	
		
		int server_fd, newsocket, valread, rcv_bytes, PORT;
		struct sockaddr_in addr_local; 
		struct sockaddr_in addr_remote; 
		int addrlen = sizeof(struct sockaddr_in);
		

		if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0 )
		{
			perror("Socket failed!");
			exit(EXIT_FAILURE);
		}
		
		printf("Socket Descriptor obtained successfully. => {Server}\n");

		PORT = atoi(argv[1]);
		addr_local.sin_family = AF_INET; 
		addr_local.sin_addr.s_addr = INADDR_ANY; 
		addr_local.sin_port = htons(PORT); 
		
		bzero(&(addr_local.sin_zero), 8);   // want to know

		
		if(bind(server_fd, (struct sockaddr*)&addr_local, sizeof(addr_local)) < 0)
		{
			perror("Binding Socket to the PORT failed!");
			exit(EXIT_FAILURE);
		}
		
		printf("PORT 8080 of TCP Connection Binded Sucessfully. => {Server} \n");

		
		if(listen(server_fd, 10) < 0)
		{
			perror("Listening to the Connection Failed!");
			exit(EXIT_FAILURE);
		}
		
		printf ("Listening the PORT 8080 OK => {Server}.\n");


		while(1)
		{	
			if ((newsocket = accept(server_fd, (struct sockaddr *)NULL, NULL)) < 0)
			{
				perror("Error in Connection");
				exit(EXIT_FAILURE);
			}
		

			printf("Connection Established from %s. => {Server} \n", inet_ntoa(addr_remote.sin_addr));

		
			
			char dir[10000], cwd[1000];
			char *rcv_buffer;
			rcv_buffer = (char *) malloc(LENGTH*sizeof(char));

			bzero(dir,10000);
			valread = read(newsocket, dir, LENGTH);
			int j = strlen(dir);
			dir[j] = '\0';

			getcwd(cwd,sizeof(cwd));
			strcat(cwd,"/Data/");
			strcat(cwd,dir);

			if(dir[0] == 'q')
			{		
				close(newsocket);
				sleep(2.3);
				printf("Connection with client closes.\n");

			}

			if(valread < 0)
			{
					perror("Error in reading path of file from client");
					exit(EXIT_FAILURE);
			}


			FILE *fs = fopen(cwd, "r");
			memset(cwd,0,sizeof(cwd));

			if(fs == NULL)
			{
				send(newsocket,"n", strlen("n"),0);
			
			}
			else
			{
				printf("%s Sending to the Client... => {Server}\n", dir);
				send(newsocket,"y", strlen("y"),0);
				
			
				bzero(rcv_buffer, LENGTH);
				

				while((rcv_bytes = fread(rcv_buffer, sizeof(char), LENGTH, fs))>0)
				{	
					
					int send_bytes = send(newsocket, rcv_buffer, rcv_bytes, 0);
					if(send_bytes < 0)
					{
						perror("Failed to send file !");
						exit(EXIT_FAILURE);
					}

					memset(rcv_buffer,'0',sizeof(rcv_buffer));
					if(feof(fs))
					{
						free(rcv_buffer);
						break;
					}
				}
				printf("File sent successfully!\n");
			}

			close(newsocket);
			sleep(2.3);
			printf("Connection lost. => {Server}\n");
		}		
    

	return 0;	   
}
