#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>

#define PORT 12000

using namespace std;

int main(){

	int msg_count = 0;

	struct timeval t1;
	struct timeval t2;
	struct timeval res;

	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	int sockfd;
	socklen_t len;
	char buffer[1024];
   	struct sockaddr_in servaddr;

	const char *message = "Hello!";

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) < 0){
		perror("Error");
	}

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;

	while(msg_count < 10){

		sendto(sockfd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		cout << "Message "<< msg_count+1  << " " << "sent to server." << endl;

		gettimeofday(&t1, NULL);

		if(recvfrom(sockfd, (char *)buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *) &servaddr, &len) < 0){
			cout << "Packet was dropped." << endl;
		}
		else{

			gettimeofday(&t2, NULL);
			timersub(&t2, &t1, &res);

			cout << "Server: " << buffer << " (RTT: " << res.tv_usec << " microseconds)" << endl;

		}
		msg_count++;
	}

	close(sockfd);
   	return 0;
}
