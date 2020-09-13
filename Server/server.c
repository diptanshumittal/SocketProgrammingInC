#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>

#define PORT 9001
#define BUFF_SIZE 4097
int main()
{
    char name[2048];
    int socketId ;
    int client ;
    char fileName[BUFF_SIZE] = {0};
    long int tot ;
    long int sendData ;
    FILE *fptr;
    char buffer[BUFF_SIZE] = {0};
    char co[1] ;
    //long int sizeFile ;

    printf("Enter the shared directory address (Along with the \"/\" at the end) :  ");
    scanf("%s",name);
    struct sockaddr_in ser;
    int addLen = sizeof(ser);
    ser.sin_family = AF_INET;
    ser.sin_port = htons(PORT);
    ser.sin_addr.s_addr = INADDR_ANY;
    

    restart:
    memset(fileName,0,BUFF_SIZE);
    memset(buffer,0,BUFF_SIZE);
    tot = 0 ;
    sendData = 0 ;
    socketId = socket(AF_INET, SOCK_STREAM, 0);
    bind(socketId, (struct sockaddr *)&ser, sizeof(ser));
    listen(socketId, 0);
    printf("Ready\n");
    client = accept(socketId, (struct sockaddr *)&ser, (socklen_t *)&addLen);
    printf("Client Connected \n");
    retry:
    memset(fileName,0,BUFF_SIZE);
    recv(client, fileName, BUFF_SIZE , 0);
    printf("Client : %s\n", fileName);
    char newname[2048] = {0} ;
    strcat(newname,name);
    strcat(newname,fileName);
    if ((fptr = fopen(newname , "rb")) == NULL){
        printf("Error! opening file \n");
        char filesizechar[] = "E" ;
        //printf("%s file ",filesizechar);
        send(client,filesizechar,1,0);
        goto retry;
        //close(socketId);
        //goto restart;
    }
    char filesizechar[] = "Y" ;
    //printf("%s file ",filesizechar);
    send(client,filesizechar,1,0);
    /*fseek(fp, 0L, SEEK_END); 
    sizeFile = ftell(fp); */
    while(!feof(fptr)){
        int n = fread(&buffer, sizeof(char), BUFF_SIZE, fptr); 
        tot+=n;
        int re = send(client,buffer,n,0) ;
        sendData+=re;
        if ( re == -1)
            printf("Sending error \n");
        memset(buffer,0,BUFF_SIZE);
    }
    printf("Total bytes send : %ld\n",sendData);
    fclose(fptr); 
    printf("Total byte value read : %ld\n",tot);
    close(socketId);
    /*printf("Restart (Y/n) : ");
    scanf("%s",co);
    if(strcmp(co,"Y")==0)
        goto restart;*/
    
    
}