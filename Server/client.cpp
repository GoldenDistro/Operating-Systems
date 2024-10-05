#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include<string.h>

#include <arpa/inet.h> //apparently the website failed to include this one for inet_pton
#include <unistd.h>    //and apparently c doesnt include write() by default
 
int main(int argc, char **argv)
{
    int sockfd,n;
    char sendline[100];
    char recvline[100];
    struct sockaddr_in servaddr;
 
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof servaddr);
 
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(12121);
 
    inet_pton(AF_INET,"127.0.0.1",&(servaddr.sin_addr));
 
    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
 

    for(int i = 1; i < argc; i++){ //store argv's char** as a single char*
	strcat(sendline, argv[i]);
	char space = ' ';
	strcat(sendline, &space); //WHY ONLY CHAR*
    }

    bzero( recvline, 100);
    write(sockfd,sendline,strlen(sendline)+1);
    read(sockfd,recvline,100);

    printf("\n");
    printf(recvline);
    printf("\n");
 
}