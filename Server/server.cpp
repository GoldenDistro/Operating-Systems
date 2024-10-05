//gcc not g++
//compile to make sure it works
/*Required Headers*/
 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>    //and apparently c doesnt include write() by default

 
int main(){
    srand(time(0));

    char str[100];
    int listen_fd, comm_fd;
    struct sockaddr_in servaddr;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero( &servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(12121);
    bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    listen(listen_fd, 10);
    comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
    while(true){
        bzero( str, 100);
        read(comm_fd,str,100);


        
        int intKey = rand()%9+1; //I'd do the whole 26 domain, but itoa() is for C++ and I dont want to debug another 10 lines to implement that
        char charKey = '0' + intKey;
        char caesarResponse[100] = "Caesar Cipher key: ";
        strcat(caesarResponse, &charKey);
        char newLine = '\n'; //because strcat only likes char* and cant just put in '\n' directly
        strcat(caesarResponse, &newLine);
        char* i;
        for(i = str; *i != '\0'; i++){ //no idea why isalpha() isnt working
            if(('z' - *i) >= 0 && ('z' - *i) < 26){	//if lowercase alpha
                *i = ((*i)-'a'+intKey)%26 + 'a'; 		//lowercase shift
            }
            if(('Z' - *i) >= 0 && ('Z' - *i) < 26){ 	//if uppercase alpha
                *i = ((*i)-'A'+intKey)%26 + 'A'; 		//uppercase shift
            }
        }
        strcat(caesarResponse, str);

        printf("Echoing back - %s",caesarResponse);
        write(comm_fd, caesarResponse, strlen(caesarResponse)+1);
    }
}

// close (###);
// /home/faculty/Adudenho/CS3310/public
//run P1-server
//run p1-client
//ctrl C to close server side
