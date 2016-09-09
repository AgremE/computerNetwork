#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
const int MIN_REQUEST_INPUT = 8;
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
// To check the validity of the comment input
// This function is already tested
bool checkCommandLine(int argc, char *command[]){
    
    if(argc < 8){
        return false;
    }
    if(strcmp("-h",command[0])){
        if(strcmp("-p",command[2])){
            if(strcmp("-o",command[4])){
                if(strcmp("-s",command[6])){
                    return true;
                }
                else{
                    return false;
                }
            }
            else{
                    return false;
            }
        }
        else{

            return false;
        }
    }

    return false;

}

int main(int argc, char *argv[])
{
    
    int sockfd, portno, n, shift, lenght, len_inet, listen_zone, connect;// Socket File Descriptor, Port Number, Shift, lenght

    struct sockaddr_in serv_addr;// Addree to attach to socket

    struct hostent *server;

    char* server_addr;

    struct in_addr server_addr; 

    char buffer[131072]; // This is store for 10 MB, it is represent data

    portno = atoi(argv[3]); // Assign the port number

    while(!checkCommandLine(argc,argv)){

        printf("""You might have input the wrong command format\n
                  Please following below format:
                  –h 143.248.111.222 –p 1234 –o 0 –s 5
                  Host: 143.248.111.222, Port: 1234, Operation: encrypt, Shift 5
               """);

    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM is for TCP protocol 
                                              // AF_INET is for address family
    if (sockfd < 0) 
        //Fail in init socket
        error("ERROR opening socket");
    //Create a server address
    
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = hton(atoi(portno));
    //server_addr = inet_ntop(argv[1]); // Convert the address from string
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);// argv[1] store the current address of server
    //server = gethostbyaddr(server_addr,server_addr,AF_INET);
    
    if(serv_addr.sin_addr.s_addr == INADDR_NONE){
        bail("Bad Address.");
    }
    
    len_inet = sizeof(serv_addr);
    //connect = accept
    
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        
        error("ERROR connecting");
    
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);

    n = write(sockfd,buffer,strlen(buffer));
    
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    
    if (n < 0) 
         error("ERROR reading from socket");
    
    printf("%s\n",buffer);
    close(sockfd);
    
    return 0;
}
