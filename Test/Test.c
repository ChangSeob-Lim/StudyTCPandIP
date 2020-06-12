#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1000000

char webpage[] = "HTTP/1.1 200 OK\r\n"
                "Server:Linux Web Server\r\n"
                "Content-Type:text/html; charset=UTF-8\r\n\r\n"
                "<!DOCTYPE html>\r\n"
                "<html><head><title> LCS Web Page </title>\r\n"
                "<style>body {background-color: #AAAAAA }</style></head>\r\n"
                "<body><center><h1>Hello world!!</h1><br>\r\n"
                "<img src=\"cat.jpg\"></center></body></html>\r\n";

void error_handling(char *msg);

int img;
char img_buf[BUF_SIZE];

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int clnt_adr_sz;
    pid_t fk;

    if(argc!=2)
    {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("bind() error");

    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error");

    int str_len;
    char buf[BUF_SIZE];
    while(1)
    {
        clnt_adr_sz=sizeof(clnt_adr);
        clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
        fk = fork();

        if(fk==0)
        {
            close(serv_sock);
            memset(buf, 0, BUF_SIZE);
            read(clnt_sock, buf, BUF_SIZE);
            fputs(buf, stdout);

            if(strstr(buf, "/cat.jpg"))
            {
                img = open("cat.jpg", O_RDONLY);
                read(img, img_buf, sizeof(img_buf));
                write(clnt_sock, img_buf, sizeof(img_buf));
                
                close(img);
            }
            else
            {
                write(clnt_sock, webpage, sizeof(webpage));
            }
            close(clnt_sock);
            fputs("Close client...\n", stdout);
            return 0;
        }
        
        close(clnt_sock);
    }

    close(serv_sock);

    return 0;
}

void error_handling(char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}