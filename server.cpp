#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 

int main() 
{ 
	int server_fd, new_socket, valread; 
	struct sockaddr_in address; 
	int addrlen = sizeof(address);
    int opt = 1;

	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 

	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	}

	char filename[] = "../Barn-owl.jpeg";
	FILE *fh = fopen(filename, "r");
	fseek(fh, 0, SEEK_END);
	int fileSize = ftell(fh);
	fseek(fh, 0, SEEK_SET);
	char* buffer = new char[fileSize];
	int r, chunkSize = fileSize, chunkAmount = 1;

	valread = read( new_socket , buffer, 2048);
	printf("%s\n",buffer );
	r = fread(buffer, chunkSize, chunkAmount, fh);
	write(new_socket , buffer , r);
	printf("File size %d bytes\n", fileSize);
	printf("send %d bytes file\n", r);
	return 0;
}