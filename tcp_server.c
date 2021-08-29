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
	//return types of socket,bind and accept
	int sockfd,retbind,connfd;
	//server and client structure declaration of type sockaddr_in (man 7 ip for details)
	struct sockaddr_in serv_addr,cli_addr;

	//server port number and client length variable
	int port_number,cli_len;
	//buffer to be used for reading and writing
	char buffer[1024];
	bzero((char*)&serv_addr,sizeof(serv_addr));

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

	//clear the structure buffer with either bzero or memset
	
//	bzero((char*)&serv_addr,sizeof(serv_addr));

	retbind = bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

	if(retbind<0){
		perror("failed to bind the IPv4 address to the socket !!\n");
		exit(1);
	}
	else{
		printf("server is running at port number : %d\n" , port_number);
	}

	int number_of_backlog_connection = 5;
	int retlisten = listen(sockfd,number_of_backlog_connection);

	if(retlisten == 0){  // or retlisten<=0
		printf("listen system call is working perfectly \n");
	}
	else{
		printf("listen system call failed to keep connection in the backlog \n");
	}

	cli_len = sizeof(cli_addr);
	//accept system call
	//
	connfd = accept(sockfd,(struct sockaddr*)&cli_addr,&cli_len);
	if(connfd < 0){
		printf("unable to create socket using accept system call \n");
	}
	else{
		printf("connfd socket descriptor is created sucussfully having number as %d\n",connfd);
	}
	
	//clear the buffer 
	bzero(buffer,sizeof(buffer));
	int readbytes = read(connfd,&buffer,sizeof(buffer));
	//read the data from client
	if(readbytes < 0)
	error("no Data is received from the server !! \n");
	
	else
	printf("data from client :  %s\n",buffer);


	char ack[1024] = "Bingo !! we have got your message \n";

	int writebytes = write(connfd,ack,sizeof(ack));
	if(writebytes < 0)
	printf("failed to send ack to client \n");

	else
	printf("ack is sent successfully to the client \n");
	
	

	close(connfd);  // close the actual socket/file descriptor
	close(sockfd);  // close the socket /file descriptor

	return 0;
}



