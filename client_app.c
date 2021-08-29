//
//half duplex communication
#include<stdio.h>
#include <sys/types.h>          //header for socket system call and accept
#include <sys/socket.h>        ////header for socket system call and accept
#include <stdlib.h>           //header for exit
#include <sys/socket.h>       //man 7 ip for (socket address struct sockaddr_in)
#include <netinet/in.h>       //man 7 ip for (socket address struct sockaddr_in
#include <netinet/ip.h>       //man 7 ip for (socket address struct sockaddr_in
#include <strings.h>           //bzero
#include<arpa/inet.h>
#include<unistd.h>

void error(char *msg)
{
        perror(msg);
        exit(1);
}

int main(int argc, char* argv[])
{
	//return types of socket
	int sockfd;
	//server and client structure declaration of type sockaddr_in (man 7 ip for details)
	struct sockaddr_in serv_addr;
//	clear the structure buffer with either bzero or memset

        bzero((char*)&serv_addr,sizeof(serv_addr));

	//server port number
	int port_number;
	//buffer to be used for reading and writing
	char buffer[1024];

	//condition check for command line argument
	if(argc<2){
                printf("kindly provide the port number !!\n");
                printf("use this format : ./filename <portnumber> \n");
        }

	//first argument - command line - port number
	port_number = atoi(argv[1]);   //typecast to interger format

	//initialize the structure member
	serv_addr.sin_family = AF_INET;   //address family = IPv4
	serv_addr.sin_port = htons(port_number);   //host byte order to network byte order (short int)
	serv_addr.sin_addr.s_addr = INADDR_ANY; //can be any IP address like "your system IP/loopback/localhost/ethernet"
                                               // e.g. "192.168.1.4"
	//serv_addr.sin_addr.s_addr = htonl("192.168.1.5");  //specific ip must in network byte order

	//create a listing socket to accept imcoming request (used only for listenning)
	sockfd = socket(AF_INET,SOCK_STREAM,0);  //TCP = SOCK_STREAM
	if(sockfd<0){
		error("Socket creation failed !!\n");
		exit(1);
	}
	else
		printf("master socket creation sucussful !!\n");

	//connect system call
	
	int retconnect = connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	if(retconnect == 0){
		printf("Bind/connect successful with the server \n");
	}
	else{
		printf("connection failed with the TCP server \n");
	}

	while(1){

	//clear the buffer
	bzero(buffer,sizeof(buffer));
	int buffer_count = 0;
	printf("Mseg to server : ");
	while((buffer[buffer_count++] = getchar()) != '\n');
	write(sockfd,buffer,sizeof(buffer));
	bzero(buffer,sizeof(buffer));
	read(sockfd,&buffer,sizeof(buffer));
	printf("Msg from server : %s\n",buffer);
	}

	close(sockfd);  // close the socket /file descriptor


	return 0;
}


