// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
	struct sockaddr_in serv_addr;
	int sock = 0, valread;
	char buffer[5024];
	char const *msg = "ping";
	FILE *fh = fopen("result.txt", "ab");

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	fflush(stdout);

	send(sock, msg, strlen(msg) + 1, 0);
	printf("message sent\n");

	while ((valread = read(sock, buffer, 5024)) > 0)
	{
		printf("receive %d", valread);

		fflush(stdout);
		if (fwrite(buffer, 1, valread, fh) < 0)
			printf("File error");
	}

	if (valread < 0)
		printf("Read error ...\n");


	return 0;
}