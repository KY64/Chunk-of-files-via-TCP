#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

int main()
{
	//Create variabel
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	int opt = 1;

	//Creating TCP socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	//Prevent error "Address already in use"
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
				   &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	//Using IPv4
	address.sin_family = AF_INET;
	//Using any IP available
	address.sin_addr.s_addr = INADDR_ANY;
	//Set up PORT to 8080
	address.sin_port = htons(PORT);

	//Attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
			 sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	//Listen to incoming connection
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	//Awaiting connection from client
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
							 (socklen_t *)&addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	//Specify which file need to send
	char filename[] = "file.txt";
	FILE *fh = fopen(filename, "r");

	//Make sure the file exists
	if (fh == NULL)
		perror("Error");

	//Checking file size
	fseek(fh, 0, SEEK_END);
	int fileSize = ftell(fh);
	fseek(fh, 0, SEEK_SET);
	//Create buffer based on file size
	char *buffer = new char[fileSize];
	//Create variable
	int r, chunkSize = fileSize, chunkAmount = 1;

	//Read incoming message from client
	valread = read(new_socket, buffer, 2048);
	printf("Got message '%s' from %s:%d\n", buffer, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

	//Read file and save each byte in buffer
	r = fread(buffer, chunkSize, chunkAmount, fh);
	//Send file to client
	write(new_socket, buffer, fileSize);
	printf("File size %d bytes\n", fileSize);
	printf("Send %d bytes file\n", r);
	return 0;
}