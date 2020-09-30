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

    //Keep track of the number of messages sent
	int msg_count = 0;

    //Timeval varibales used to calculate RTT
	struct timeval t1;
	struct timeval t2;
	struct timeval res;
    
    //Timeval used for 1 second timeout
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

    //Socket that will connect to the server
	int sockfd;
    //Address length
	socklen_t len;
    //Buffer used to store messages from server
	char buffer[1024];
    //Server address 
   	struct sockaddr_in servaddr;

    //Message to be sent to server
	const char *message = "Hello!";

    //Create a UDP socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    //Used to set a timeout when waiting for responses from the server
	if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) < 0){
		perror("Error");
	}

    //Create memory space for the server address
	memset(&servaddr, 0, sizeof(servaddr));

    //Fill in server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;

    //While 10 messages have not been sent
	while(msg_count < 10){

        //Client sends a message to the server
		sendto(sockfd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		cout << "Message "<< msg_count+1  << " " << "sent to server." << endl;

        //Get time when message is sent
		gettimeofday(&t1, NULL);

        //Wait for message to be received back from server, if message is not received after the 1 second timeout the packet is considered to be dropped
		if(recvfrom(sockfd, (char *)buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *) &servaddr, &len) < 0){
			cout << "Packet was dropped." << endl;
		}
		else{
            //Get time after message is received
			gettimeofday(&t2, NULL);
            //Calculate RTT
			timersub(&t2, &t1, &res);

			cout << "Server: " << buffer << " (RTT: " << res.tv_usec << " microseconds)" << endl;

		}
        //Message count increases
		msg_count++;
	}

    //Close socket when finished
	close(sockfd);
   	return 0;
}
