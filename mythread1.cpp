#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>

#include "mythread1.h"
#include <QtCore>

using namespace std;

MyThread1::MyThread1(QObject *parent) : QThread(parent){}

void MyThread1::run()
{

    int port=atoi(this->port);

    qDebug() <<"porttt:"<< port;

    int sockfd, res;
    struct sockaddr_in dest;

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&dest,sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(uint16_t(port));
    dest.sin_addr.s_addr = INADDR_ANY;
    bind(sockfd,(struct sockaddr*)&dest, sizeof(struct sockaddr));
    listen(sockfd,20);
    qDebug() <<"o1"<< port;
    //char filename[]="../../../data/out.txt";
    char filename[]="out.txt";
    fstream fp;

    string bye="bye";
    char Bye[256] ="bye";
    char Null[256] ="null";
    char Done[256] ="done";

    while(1)
    {
        char Hello[256] ="Hello";
        char buffer[256]={};

        //printf("--------initial!!------\n");
        //emit NumberChanged(i);

        int clientfd;
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);

        printf("wait..\n");
        clientfd = accept(sockfd,(struct sockaddr*)&client_addr, &addrlen);
        qDebug() <<"o2"<< port;
        //printf("-------------connection success----\n");

        bzero(buffer, 256);
        res = recv(clientfd, buffer, sizeof(buffer), 0);

        if(res >0)
        {
            printf("%s\n", buffer);

            if(strcmp(buffer, Bye) == 0)
            {
                qDebug() <<"8:";
                send(clientfd, Bye, sizeof(Bye), 0);
                this->Stop = false;
                close(clientfd);
                break;
            }
            else if(strcmp(buffer, Done) == 0)
            {
                qDebug() <<"998:"<< port;
                //send(clientfd, Hello, sizeof(Hello), 0);
                emit NumberChanged(1);
            }
            else
            {
                qDebug() <<"7:";
                fp.open(filename, ios::out|ios::app);
                fp << buffer << "\n";
                fp.close();
                send(clientfd, Hello, sizeof(Hello), 0);
                close(clientfd);
            }
        }
        else
        {
            emit NumberChanged(0);
        }
    }

    qDebug() <<"888:e"<< port;
    close(sockfd);
}
