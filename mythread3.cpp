#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <netdb.h>



#include "mythread3.h"
#include <QtCore>

using namespace std;

MyThread3::MyThread3(QObject *parent) : QThread(parent){}
void MyThread3::run()
{
    char ip[50]={};
    strcpy(ip,this->host);
    int port=atoi(this->port);

    qDebug()<< "ip:"<<ip<<"+" ;
    qDebug()<<"port:"<<port;

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    //cout << "Hello world!:" << cmd<< endl;

    int sockfd;
    struct sockaddr_in dest;

    char buffer[128];
    char cmd[128] = "quit";
    char resp[20] = "clienttack";

    sockfd = socket(PF_INET, SOCK_STREAM, 0);

    bzero(&dest,sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    dest.sin_addr.s_addr = inet_addr(host);

    //setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));

    ::connect(sockfd, (struct sockaddr*)&dest, sizeof(dest));

    bzero(buffer, 128);

    while(1)
    {
        if(recv(sockfd, buffer, sizeof(buffer), 0) >0)
        {
            if(strcmp(buffer, "acept") == 0)
            {
                cout << "server state1:" <<buffer << endl;
                break;
            }
        }
    }

    send(sockfd, cmd, 128, 0);

    bzero(buffer, 128);

//    while(1)
//    {
//        if(recv(sockfd, buffer, sizeof(buffer), 0) > 0)
//        {
//            cout << "server state2:" <<buffer << endl;
//            int count = 0;
//            char *delim = "\t";
//            char * pch;
//            pch = strtok(buffer,delim);

//            if(pch != NULL && strcmp(pch, "path") == 0)
//            {
//                //cout << "break:" << pch << endl;
//                //pch = strtok (NULL, delim);
//                break;
//            }
//        }
//    }

    close(sockfd);

//    //char filename[]="Angle.txt";
//    char filename[256]={};
//    qDebug()<< "this->filename:"<<this->filename<<"+";
//    strcpy(filename,this->filename);

//    qDebug()<< "filename:"<<filename<<"+";

//    char line[100];
//    fstream fin;
//    fin.open(filename, ios::in);

//    char Bye[10]="bye";
//    char Null[10]="null";

//    if(strcmp(this->str,Bye)==0)
//    {
//      char buffer[256];
//      char res[256]={};

//      string temp="bye";
//      strcpy(res, temp.c_str());

//      int a=0;
//      int sockfd;
//      sockfd = socket(PF_INET, SOCK_STREAM, 0);

//      if(::connect(sockfd, (struct sockaddr*)&dest, sizeof(dest))>0)
//      {
//          qDebug()<<"---1---";

//          send(sockfd, res, sizeof(res), 0);

//          bzero(buffer, 256);
//          recv(sockfd, buffer, sizeof(buffer), 0);
//          printf("server: %s\n", buffer);
//      }
//      close(sockfd);
//    }

//    if(fin.is_open() && strcmp(this->str,Null)==0)
//    {
//      int size=0;
//      int done=0;
//      while(fin.getline(line, sizeof(line), '\n'))
//      {
//          cout<<line<<endl;
//          char buffer[256];
//          char res[256]={};

//          int sockfd;
//          sockfd = socket(PF_INET, SOCK_STREAM, 0);

//          if(::connect(sockfd, (struct sockaddr*)&dest, sizeof(dest))<0)
//          {
//              emit NumberChanged3(0);
//              close(sockfd);
//              break;
//          }
//          else
//          {
//              strcpy(res, line);

//              if(send(sockfd, res, sizeof(res), 0)<=0)
//              {
//                  emit NumberChanged3(3);
//                  close(sockfd);
//                  break;
//              }
//              else
//              {
//                  bzero(buffer, 256);

//                  int r=recv(sockfd, buffer, sizeof(buffer), 0);

//                  if(r>0)
//                  {
//                      qDebug()<<"---!!!!!!!!!!!!!!!!!---";
//                      printf("server: %s\n", buffer);
//                      done+=1;
//                  }
//                  if(r==0)
//                  {
//                      qDebug()<< res <<","<<buffer;
//                      emit NumberChanged3(4);

//                  }
//                  if(r<0)
//                  {
//                      //emit NumberChanged3(2);
//                      qDebug()<<"---2---";
//                  }
//              }

//          }
//          size+=1;
//          close(sockfd);
//      }

//      int sockfd;
//      sockfd = socket(PF_INET, SOCK_STREAM, 0);

//      char Done[256] ="done";
//      char Fail[256] ="Fail";

//      if(::connect(sockfd, (struct sockaddr*)&dest, sizeof(dest))<0)
//      {
//          //emit NumberChanged3(0);
//          //close(sockfd);
//          //break;
//          ;
//      }
//      else
//      {
//          if(size == done && size != 0)
//          {
//            qDebug()<<"---ok---";
//            send(sockfd, Done, sizeof(done), 0);
//            emit NumberChanged3(1);
//          }
//          else
//          {
//            qDebug()<<"---no---";
//            emit NumberChanged3(2);
//          }
//      }

//    }
}
