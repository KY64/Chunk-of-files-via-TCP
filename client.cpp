#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
	//Create variable
	struct sockaddr_in serv_addr;
	int sock = 0, valread;
	int chunkSize = 5024;
	char* buffer = new char[chunkSize];
	//Message
	char const *msg = "ping";
	//Output file
	FILE *fh = fopen("result.txt", "ab");

	//Create TCP socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	//Using IPv4
	serv_addr.sin_family = AF_INET;
	//Using PORT 8080
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	//Connect to server
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	//Send message to server
	send(sock, msg, strlen(msg) + 1, 0);
	printf("message sent\n");

	//Receiving file from server
	while ((valread = read(sock, buffer, chunkSize)) > 0)
	{
		printf("receive %d\n", valread);

		//Save received bytes to a new file
		if (fwrite(buffer, valread, 1, fh) < 0)
			printf("File error");
	}

	if (valread < 0)
		perror("Read error ...\n");


	return 0;
}