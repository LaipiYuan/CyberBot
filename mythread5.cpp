#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "mythread5.h"
using namespace std;

mythread5::mythread5(QObject *parent) : QThread(parent){}

void mythread5::run()
{
    int sockfd;
    struct sockaddr_in dest;
    int port=atoi(this->port);

    char acept[20] = "acept";
    char done[20] = "done";

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&dest,sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(uint16_t(port));
    dest.sin_addr.s_addr = INADDR_ANY;

    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;

    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));

    int inuse = bind(sockfd,(struct sockaddr*)&dest, sizeof(struct sockaddr));

    if(inuse == 0)
    {
        cout << "free\n";
        emit NumberChanged5(-2);

        listen(sockfd,20);
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);

        cout << "port:" << port <<endl;
        //case
        //char* casew="w";
        //char* cases="s";
        //char* casea="a";
        //char* cased="d";
        //char* caser="r";
        //char* casef="f";
        //char* casep="p";
        //

        while(1)
        {
            cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n" <<endl;


            if(this->Exit == true)
            {
                close(sockfd);
                cout << "bye" <<endl;
                this->Exit = false;
                break;
            }
            if(this->Check == true && this->Exit != true)
            {

                char buffer[256];
                int clientfd;
                int case_temp = 1;

                cout << "Exit" << this->Exit << endl;
                cout << "Check" << this->Check << endl;

                clientfd = accept(sockfd,(struct sockaddr*)&client_addr, &addrlen);

                cout << "clientfd:" << clientfd <<endl;

                if(clientfd != -1)
                {
                    send(clientfd, acept, sizeof(acept), 0);
                    cout << "send:" << acept <<endl;
                    int res = recv(clientfd, buffer, sizeof(buffer), 0);
                    cout << buffer << endl;

                    char case_array[70][40] = {};
                    int count = 0;  
                    char *delim = "\t";
                    char * pch;
                    pch = strtok(buffer,delim);
                    cout << "pch:" <<pch <<endl;

                    while (pch != NULL)
                    {
                        cout <<"sub:" <<pch <<endl;
                        strcpy(case_array[count],pch);
                        cout <<"array:" <<case_array[count] <<endl;
                        pch = strtok (NULL, delim);
                        count+=1;
                    }

                    cout << "case::+" <<case_array[0]<<"+" <<endl;
                    if(strcmp("coordinatestep",case_array[0])==0)
                    {
                        strcpy(this->case_option,case_array[0]);
                        strcpy(this->option,case_array[1]);
                        this->type = atoi(case_array[2]);


                        emit NumberChanged5(1);
                    }
                    else if(strcmp("play",case_array[0])==0)
                    {
                        emit NumberChanged5(2);
                    }
                    else if(strcmp("teach",case_array[0])==0)
                    {
                        emit NumberChanged5(12);
                    }
                    else if(strcmp("reboot_all",case_array[0])==0)
                    {
                        strcpy(this->case_option,case_array[0]);
                        emit NumberChanged5(14);
                    }
                    else if(strcmp("dynamic_syncw_group",case_array[0])==0)
                    {
                        cout << "in dynamic_syncw_group\n" <<endl;
                        strcpy(this->case_option,case_array[0]);
                        this->coord[0] = atof(case_array[1]);
                        this->coord[1] = atof(case_array[2]);
                        this->coord[2] = atof(case_array[3]);
                        this->change_group = atoi(case_array[4]);
                        this->change_step_now = atoi(case_array[5]);
                        this->change_step_pre = atoi(case_array[6]);

                        emit NumberChanged5(18);

                    }
                    else if(strcmp("initialization",case_array[0])==0)
                    {
                        cout << "initialization\n" <<endl;
                        strcpy(this->case_option,case_array[0]);
                        this->type = atoi(case_array[1]);
                        this->unit = atof(case_array[2]);
                        this->angle = atof(case_array[3]);
                        this->omega_limit = atof(case_array[4]);
                        this->move_time = atof(case_array[5]);
                        this->user_sync_type = atoi(case_array[6]);
                        this->replay_times = atoi(case_array[7]);
                        this->time_cell = atof(case_array[8]);

                        this->vig[0] = atoi(case_array[9]);
                        this->vig[1] = atoi(case_array[10]);
                        this->vig[2] = atoi(case_array[11]);
                        this->vig[3] = atoi(case_array[12]);
                        //this->vig[4] = atoi(case_array[13]);
                        //this->vig[5] = atoi(case_array[14]);

                        this->vpg[0] = atoi(case_array[15]);
                        this->vpg[1] = atoi(case_array[16]);
                        this->vpg[2] = atoi(case_array[17]);
                        this->vpg[3] = atoi(case_array[18]);
                        //this->vpg[4] = atoi(case_array[19]);
                        //this->vpg[5] = atoi(case_array[20]);

                        this->ppg[0] = atoi(case_array[21]);
                        this->ppg[1] = atoi(case_array[22]);
                        this->ppg[2] = atoi(case_array[23]);
                        this->ppg[3] = atoi(case_array[24]);
                        //this->ppg[4] = atoi(case_array[25]);
                        //this->ppg[5] = atoi(case_array[26]);

                        //this->ax = atof(case_array[27]);
                        //this->ay = atof(case_array[28]);
                        //this->az = atof(case_array[29]);
                        this->ox = atof(case_array[30]);
                        this->oy = atof(case_array[31]);
                        this->oz = atof(case_array[32]);

                        this->gear_ratio[0] = atof(case_array[33]);
                        this->gear_ratio[1] = atof(case_array[34]);
                        this->gear_ratio[2] = atof(case_array[35]);
                        this->gear_ratio[3] = atof(case_array[36]);
                        //this->gear_ratio[4] = atof(case_array[37]);
                        //this->gear_ratio[5] = atof(case_array[38]);

                        this->pos_range[0] = atof(case_array[39]);
                        this->pos_range[1] = atof(case_array[40]);
                        this->pos_range[2] = atof(case_array[41]);
                        this->pos_range[3] = atof(case_array[42]);
                        //this->pos_range[4] = atof(case_array[43]);
                        //this->pos_range[5] = atof(case_array[44]);

                        this->cw[0] = atoi(case_array[45]);
                        this->cw[1] = atoi(case_array[46]);
                        this->cw[2] = atoi(case_array[47]);
                        this->cw[3] = atoi(case_array[48]);
                        //this->cw[4] = atoi(case_array[49]);
                        //this->cw[5] = atoi(case_array[50]);

                        this->adj_joint[0] = atof(case_array[51]);
                        this->adj_joint[1] = atof(case_array[52]);
                        this->adj_joint[2] = atof(case_array[53]);
                        this->adj_joint[3] = atof(case_array[54]);
                        //this->adj_joint[4] = atof(case_array[55]);
                        //this->adj_joint[5] = atof(case_array[56]);

                        this->vel_limit[0] = atoi(case_array[57]);
                        this->vel_limit[1] = atoi(case_array[58]);
                        this->vel_limit[2] = atoi(case_array[59]);
                        this->vel_limit[3] = atoi(case_array[60]);
                        //this->vel_limit[4] = atoi(case_array[61]);
                        //this->vel_limit[5] = atoi(case_array[62]);

                        emit NumberChanged5(13);
                        cout << "out initialization\n" <<endl;
                    }
                    else if(strcmp("coordinatevalue",case_array[0])==0)
                    {
                        strcpy(this->case_option,case_array[0]);
                        this->coord[0] = atof(case_array[1]);
                        this->coord[1] = atof(case_array[2]);
                        this->coord[2] = atof(case_array[3]);
                        emit NumberChanged5(4);
                    }
                    else if(strcmp("g",case_array[0])==0) //group play
                    {
                        strcpy(this->case_option,case_array[0]);
                        this->group = atoi(case_array[1]);
                        emit NumberChanged5(11);
                    }
                    else if(strcmp("singlejoint",case_array[0])==0)
                    {
                        strcpy(this->case_option,case_array[0]);
                        strcpy(this->option,case_array[1]);
                        this->type = atoi(case_array[2]);
                        emit NumberChanged5(3);
                    }
                    else if(strcmp("z",case_array[0])==0) //socket_set_path
                    {
                        this->file_start = atoi(case_array[1]);
                        strcpy(this->filename,case_array[2]);

                        char temp[100] = {};
                        char str[20] = "\t";

                        for(int i = 3; i<17; i++)
                        {
                            strcat(temp,case_array[i]);
                            strcat(temp,str);
                        }

                        strcpy(this->path_message, temp);

                        cout << "T5:" << this->path_message << "+" <<endl;
                        emit NumberChanged5(5);
                    }
                    else if(strcmp("valveon",case_array[0])==0) //pumpon
                    {
                        emit NumberChanged5(6);
                    }
                    else if(strcmp("valveoff",case_array[0])==0) //pumpoff
                    {
                        emit NumberChanged5(7);
                    }
                    else if(strcmp("touqueon",case_array[0])==0) //torque on
                    {
                        emit NumberChanged5(9);
                    }
                    else if(strcmp("quit",case_array[0])==0) //quit
                    {
                        close(clientfd);
                        close(sockfd);
                        this->Exit = false;
                        emit NumberChanged5(-3);
                        break;
                    }
                    else if(strcmp("touqueoff",case_array[0])==0) //torque off
                    {
                        emit NumberChanged5(10);
                    }
                    else if(strcmp("v",case_array[0])==0) //gain
                    {
                        strcpy(this->case_option,case_array[0]);
                        this->omega_limit = atof(case_array[3]);
                        this->replay_times = atoi(case_array[6]);
                        this->time_cell = atof(case_array[7]);

                        this->vig[0] = atoi(case_array[8]);
                        this->vig[1] = atoi(case_array[9]);
                        this->vig[2] = atoi(case_array[10]);

                        this->vpg[0] = atoi(case_array[11]);
                        this->vpg[1] = atoi(case_array[12]);
                        this->vpg[2] = atoi(case_array[13]);

                        this->ppg[0] = atoi(case_array[14]);
                        this->ppg[1] = atoi(case_array[15]);
                        this->ppg[2] = atoi(case_array[16]);

                        emit NumberChanged5(8);
                    }
                    else if(strcmp("set_path_group",case_array[0])==0) //set_path_group
                    {
                        strcpy(this->case_option,case_array[0]);
                        strcpy(this->filename,case_array[1]);
                        emit NumberChanged5(15);
                    }
                    else if(strcmp("state",case_array[0])==0) //set_path_group
                    {
                        strcpy(this->case_option,case_array[0]);
                        strcpy(this->id,case_array[1]);

                        emit NumberChanged5(17);
                    }
                    else if(strcmp("optimize_group",case_array[0])==0) //optimize_group
                    {
                        strcpy(this->case_option,case_array[0]);
                        emit NumberChanged5(16);
                    }
                    else
                    {
                        cout << "no case\n";
                        case_temp = 0;
                    }

                    if(case_temp != 0)
                    {
                        cout << "casetemp != 0\n";
                        this->Check = false;

                        while(1)
                        {
                            //cout << "this->check:" <<this->Check <<endl;
                            if(this->Check == true)
                            {

                                //char test[100] = "123456789123456789";
                                char test[100] = {};
                                strcpy(test, this->message);
                                cout << "size:" << sizeof(test) <<endl;
                                //send(clientfd, this->message, 100, 0);
                                send(clientfd, test, sizeof(test), 0);
                                cout << "send message:" << this->message <<endl;
                                break;
                            }
                        }
                    }

                }

                close(clientfd);
            }

        }

        close(sockfd);
        cout << "bye2" <<endl;
    }
    else
    {
        cout << "inuse\n";
        emit NumberChanged5(-1);
    }

}
