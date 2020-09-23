#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 12000

using namespace std;

int main(){

	int msg_count = 0;

	int sockfd;
	int n;
	socklen_t len;
	char buffer[1024];
   	struct sockaddr_in servaddr;

	const char *message = "Hello from client";

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;

	while(msg_count < 10){

		sendto(sockfd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		cout << "Message sent to server" << endl;

		n = recvfrom(sockfd, (char *)buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
		buffer[n] = '\0';
		cout << "Response received from server" << endl;

		msg_count++;
	}
	close(sockfd);

   	return 0;
}
