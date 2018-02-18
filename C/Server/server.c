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
	
	int server_fd, newsocket, valread, rcv_bytes, PORT, opt = 1;
	struct sockaddr_in addr_local;  // sockaddr_in - references elements of the socket address. "in" for internet
	struct sockaddr_in addr_remote; 
	int addrlen = sizeof(struct sockaddr_in);
	char cwd[10000] , dir[10000], rcv_buffer[LENGTH];

	getcwd(cwd,sizeof(cwd));
	strcat(cwd, "/Data/");
	

	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0 )  // Creating socket file descriptor 
	{
		perror("Socket failed!");                           // creates socket, SOCK_STREAM is for TCP. SOCK_DGRAM for UDP
		exit(EXIT_FAILURE);
	}
	
	printf("Socket Descriptor obtained successfully. => {Server}\n");


    // This is to lose the pesky "Address already in use" error message
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt))) // SOL_SOCKET is the socket layer itself
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

	PORT = atoi(argv[1]);
	addr_local.sin_family = AF_INET;  // Address family. For IPv6, it's AF_INET6. 29 others exist like AF_UNIX etc.
	addr_local.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP address - listens from all interfaces.
	addr_local.sin_port = htons(PORT);   // Server port to open. Htons converts to Big Endian - Left to Right. RTL is Little Endian
	
	bzero(&(addr_local.sin_zero), 8);   // want to know

	// attaching socket to the port
	if( bind(server_fd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) < 0)
	{
		perror("Binding Socket to the PORT failed!");
		exit(EXIT_FAILURE);
	}
	
	printf("PORT %d of TCP Connection Binded Sucessfully. => {Server} \n",PORT);

	// Port bind is done. You want to wait for incoming connections and handle them in some way.
	 // The process is two step: first you listen(), then you accept()
	if(listen(server_fd, 4) < 0)
	{
		perror("Listening to the Connection Failed!");
		exit(EXIT_FAILURE);
	}
	
	printf ("Listening the PORT OK => {Server}.\n");


	// returns a brand new socket file descriptor to use for this single accepted connection. Once done, use send and recv
	if ((newsocket = accept(server_fd, (struct sockaddr *)&addr_remote, &addrlen)) < 0)
	{
		perror("Error in Connection");
		exit(EXIT_FAILURE);
	}
	

	printf("Connection Established from %s. => {Server} \n", inet_ntoa(addr_remote.sin_addr));

	valread = read(newsocket, dir, 10000);   // read infromation received into the buffer
	strcat(cwd, dir);
	cwd[strlen(cwd)] = '\0';
	printf("%s\n",cwd);
	
	if(valread < 0)
	{
		perror("Error in reading path of file from client");
		exit(EXIT_FAILURE);
	}

    printf("%s Sending to the Client... => {Server}\n", cwd);
	
   

    FILE *fs = fopen(cwd, "r");

    if(fs == NULL)
    {
		send(newsocket,"n", strlen("n"),0);
        perror("File not found on server!");
		exit(EXIT_FAILURE);
		return 0;
    }
	else
	{
		send(newsocket,"y", strlen("y"),0);
		
	}

    memset(rcv_buffer,'0',sizeof(rcv_buffer));
    

    while((rcv_bytes = fread(rcv_buffer, sizeof(char), LENGTH, fs))>0)   // send file in chunks to clients
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
			break;
		}
    }
    printf("File sent successfully!\n");
    close(newsocket);
    sleep(1.5);
    printf("Connection with Client closed !\n");

	return 0;	   
}
