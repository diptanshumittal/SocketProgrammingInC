#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>


#define PORT 9001
#define BUFF_SIZE 4097

int main()
{
    struct sockaddr_in ser;
    ser.sin_family = AF_INET;  
    ser.sin_port = htons(PORT);  
    ser.sin_addr.s_addr = INADDR_ANY;  
    char co[1] ;
    int sock;
    long int a = 0 ;
    char buffer[BUFF_SIZE] = {0};
    long int tot = 0 ;
    char fileName[1024];
    long int itr = 0 ;
    FILE *fptr;


    //restart:
    sock = socket(AF_INET, SOCK_STREAM, 0);
    retry:
    if ( connect( sock, (struct sockaddr * )&ser, sizeof(ser) ) == -1 )
        goto retry;
    printf("You are now connected on port number %d\n",PORT);
    filenot:
    printf("Enter filename \n");
    scanf("%s", &fileName);
    send(sock, fileName, strlen(fileName), 0);
    printf("Name sent\n");
    
    if ((fptr = fopen(fileName,"wb")) == NULL){
        printf("Error! opening file");
        //close(sock);
        //exit(1);
    }

    char sizechar[2] = {0};
    recv(sock, sizechar, 1 , 0);
    printf("%s  \n",sizechar);
    if(strcmp(sizechar,"E")==0){
        fclose(fptr);
        printf("Server   : File not found \n");
        goto filenot;
    }
    
    while(1){
        a = recv(sock, buffer , 1025 , 0);
        if(a==0)
            break;
        tot+= a ;
        if(a==-1)
            printf("recv file error");
        if (fwrite(&buffer, sizeof(char), a, fptr)!=a)
            printf("Write file error"); 
        memset(buffer,0,BUFF_SIZE);
    }
    //printf("herere");
    fclose(fptr); 
    printf("Total byte value recv : %ld\n",tot);
    if(tot==0){
        printf("File not found or size was 0 bytesn");
    }
    printf("Ending \n");
    close(sock);
    /*printf("Restart (Y/n) : ");
    scanf("%s",co);
    if(strcmp(co,"Y")==0)
        goto restart;*/
}