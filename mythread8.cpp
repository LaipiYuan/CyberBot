//planning
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <string.h>
#include <sstream>
#include <time.h>
#include "mythread8.h"

using namespace std;

#define PI 3.14159265

#include <QtCore>
#include <QDebug>

int tray_number;
float tray_length;
float tray_width;

/*
int step_span;
int delay_time;
float omega_limit;
float move_time;
float scale;

int joint_used[6] = {};
int juse=0;

float position_range[6]={};
float gear_ratio[6]={};
//int velocity_limit[6]={};

int group_GoalPos[50][100][6]={};
int group_GoalVelo[50][100][6]={};
int group_step[50] = {};
int group_count;
*/

MyThread8::MyThread8(QObject *parent) : QThread(parent){}

void cross_vector(double coord_1[3], double coord_2[3], double coord_3[3], double normAx[3])
{
    double vector_1[3]={0}, vector_2[3]={0}, normal_vector[3]={0};
    double sum=0.0, norm=0.0;
    int i;

    for(i=0; i<3; i++)
    {
        vector_1[i] = coord_2[i] - coord_1[i];
        vector_2[i] = coord_3[i] - coord_1[i];
    }

    //cross matrix => the normal vector of the plane
    normal_vector[0] = vector_1[1]*vector_2[2] - vector_1[2]*vector_2[1];
    normal_vector[1] = vector_1[2]*vector_2[0] - vector_1[0]*vector_2[2];
    normal_vector[2] = vector_1[0]*vector_2[1] - vector_1[1]*vector_2[0];


    for(i=0; i<3; i++)
        sum += pow(normal_vector[i],2);
    norm = sqrt(sum);

    for(i=0; i<3; i++)
    {
             normAx[i] = normal_vector[i]/norm;
    }


    fstream fst;
    fst.open("./normal_vector.txt", ios::out);

    for(i=0; i<3; i++)
    {
        fst << (normAx[i]) << "\t";
    }
    fst << "\n";

    fst.close();

}

/*
void tray_coordinate(double coord_o[3], double coord_w[3], double coord_l[3], int numberL, int numberW, double tray_length, double tray_width, char filename[80], int modifyZ_check, double normAx_n[3], int coord_o2[3])
{

    for (int i=0;i<3;i++)
    {
        cout<<"coord_o["<<i<<"]"<<coord_o2[i]<<endl;
    }
    double tray_coord[numberW][numberL][3];
    tray_coord[numberW][numberL][3]={0};
    int i, j, k;


    double unitAx_w[3]={0}, unitAx_l[3]={0};

    for(i=0; i<3; i++)
    {
        unitAx_w[i] = (coord_w[i] - coord_o[i])/(numberW-1);
        unitAx_l[i] = (coord_l[i] - coord_o[i])/(numberL-1);
    }


    if(modifyZ_check == 0)
    {
        cout<<"11111111111111111111\n";
        for(i=0; i<numberW; i++)
        {
            for(j=0; j<numberL; j++)
            {
                for(k=0; k<3; k++)
                {
                    tray_coord[i][j][k] = coord_o[k] + unitAx_w[k]*i + unitAx_l[k]*j;
                }
            }
        }


        fstream fst;
        fst.open(filename, ios::out);

        std::stringstream s1;
        std::stringstream s2;
        std::stringstream s3;

        for(i=0; i<numberW; i++)
        {
            cout<<"i="<<i<<endl;
            for(j=0; j<numberL; j++)
            {
                cout<<"j="<<j<<endl;
                s1 << fixed << setprecision(2) << tray_coord[i][j][0];
                s2 << fixed << setprecision(2) << tray_coord[i][j][1];
                s3 << fixed << setprecision(2) << tray_coord[i][j][2];

                string s = s1.str() + "\t " + s2.str() + "\t " + s3.str() + "\t";
                string s6=   QString::number(coord_o2[0]).toStdString()+"\t"+QString::number(coord_o2[1]).toStdString()+"\t"+QString::number(coord_o2[2]).toStdString()+"\t";

                fst << s << s6<< "\n";

                s1.str("");
                s2.str("");
                s3.str("");
                //s.clear();

            }

            //fst << "\n";
        }

        fst.close();
    }
    else if(modifyZ_check == 1)
    {
        cout<<"22222222222222222222222"<<endl;
        double vector_w[3]={0};
        double sum=0.0, norm=0.0;
        int i;

        for(i=0; i<3; i++)
        {
            vector_w[i] = coord_w[i] - coord_o[i];
        }

        for(i=0; i<3; i++)
            sum += pow(vector_w[i],2);
        norm = sqrt(sum);

        double normAx_w[3]={0};
        for(i=0; i<3; i++)
        {
                 normAx_w[i] = vector_w[i]/norm;
        }


        //char line[100];
        //int data=0;
        //char * pch;
        //int element=0;

        //fstream fst1;
        //fst1.open("normal_vector.txt", ios::in);

        //while(fst1.getline(line, sizeof(line), '\n'))
        //{
        //    if (line[sizeof(line)] == '\0')
        //    {
        //        pch=strtok(line,"\t");
        //        while(pch != NULL)
        //        {
        //            data = atoi(pch);
        //            normAx_n[element]=data;
        //            element += 1;
        //        }
        //    }
        //}


        //fst1.close();


        //calculate the third vector of the tray (the director of l)

        double normAx_l[3]={0};

        normAx_l[0] = normAx_n[1]*normAx_w[2] - normAx_n[2]*normAx_w[1];
        normAx_l[1] = normAx_n[2]*normAx_w[0] - normAx_n[0]*normAx_w[2];
        normAx_l[2] = normAx_n[0]*normAx_w[1] - normAx_n[1]*normAx_w[0];

        double modify_width=0.0, modify_length =0.0;


        for(i=0; i<3; i++)
        {
            modify_width += pow(unitAx_w[i],2);
            modify_length += pow(unitAx_l[i],2);
        }

        for(i=0; i<numberW; i++)
        {
            for(j=0; j<numberL; j++)
            {
                for(k=0; k<3; k++)
                {
                    //tray_coord[i][j][k] = coord_o[k] + normAx_w[k]*(tray_width/10)*i + normAx_l[k]*(tray_length/10)*j;
                    tray_coord[i][j][k] = coord_o[k] + normAx_w[k]*modify_width*i + normAx_l[k]*modify_length*j;
                }
            }
        }


        fstream fst1;
        fst1.open(filename, ios::out);

        std::stringstream s4;
        std::stringstream s5;
        std::stringstream s6;

        for(i=0; i<numberW; i++)
        {
            for(j=0; j<numberL; j++)
            {
                s4 << fixed << setprecision(2) << tray_coord[i][j][0];
                s5 << fixed << setprecision(2) << tray_coord[i][j][1];
                s6 << fixed << setprecision(2) << tray_coord[i][j][2];

                string s =   s4.str() + "\t " + s5.str() + "\t" + s6.str() + "\t";
                string s1=   QString::number(coord_o2[0]).toStdString()+"\t"+QString::number(coord_o2[1]).toStdString()+"\t"+QString::number(coord_o2[2]).toStdString()+"\t";

                fst1 << s << s1<< "\n";

                s4.str("");
                s5.str("");
                s6.str("");
                //s.clear();

            }

            //fst1 << "\n";
        }

        fst1.close();

    }
}
*/

void MyThread8::tray_coordinate(double coord_o[3], double coord_w[3], double coord_l[3], double coord_d[3], double coord_5[3], double coord_6[3], int numberL, int numberW, int column, int row, char filename[80], int coord_o2[3], int middleCoordUsed, int shiftUsed)
{
    //modify the coordinate
    double tray_coord_old[numberW][numberL][6];
    double tray_coord_new[numberW][numberL][6];
    double tray_coord_modify[numberW][numberL][3];
    int i, j, k;

    //initial
    for(i=0; i<numberW; i++){
        for(j=0; j<numberL; j++){
            for(k=0; k<6; k++){
                tray_coord_old[i][j][k] = 0;
                tray_coord_new[i][j][k] = 0;
            }
        }
    }

    for(i=0; i<numberW; i++){
        for(j=0; j<numberL; j++){
            for(k=0; k<3; k++){
                tray_coord_modify[i][j][k]=0;
            }
        }
    }


    char line[1024];
    char *pch;
    int countW=0, countL=0;
    double data;

    fstream fstOld;
    fstOld.open("tested_tray_coord_old.txt", ios::in);

    if(fstOld.is_open() && shiftUsed == 1)
    {
        while(fstOld.getline(line, sizeof(line), '\n'))
        {
            if (line[strlen(line)] == '\0')
            {
                pch=strtok(line,"\t");
                int count = 0;
                data = 0.0;
                while(pch != NULL)
                {
                    data = atof(pch);
                    tray_coord_old[countW][countL][count]=data;
                    pch = strtok(NULL, "\t");
                    count += 1;
                }
                countL += 1;
                if(countL == numberL)
                {
                    countL = 0;
                    countW += 1;
                }
            }
        }

    }
    else
    {
        if(shiftUsed == 1)
        {
            cout << "tested_tray_coord_old.txt OPEN ERROR"<<endl;
            emit NumberChanged8(5);
        }
    }
    fstOld.close();


    fstream fstNew;
    fstNew.open("tested_tray_coord_new.txt", ios::in);
    countW = 0;
    countL = 0;
    if(fstNew.is_open() && shiftUsed == 1)
    {
        while(fstNew.getline(line, sizeof(line), '\n'))
        {
            if (line[strlen(line)] == '\0')
            {
                pch=strtok(line,"\t");
                int count = 0;
                data = 0.0;
                while(pch != NULL)
                {
                    data = atof(pch);
                    tray_coord_new[countW][countL][count] = data;
                    pch = strtok(NULL, "\t");
                    count += 1;
                }
                countL += 1;
                if(countL == numberL)
                {
                    countL = 0;
                    countW += 1;
                }
            }
        }
    }
    else
    {
        if(shiftUsed == 1)
        {
            cout << "tested_tray_coord_new.txt OPEN ERROR"<<endl;
            emit NumberChanged8(5);
        }
    }   
    fstNew.close();



    //the shift of the coordinate
    for(i=0; i<numberW; i++){
        for(j=0; j<numberL; j++){
            for(k=0; k<3; k++){
                tray_coord_modify[i][j][k] = tray_coord_new[i][j][k] - tray_coord_old[i][j][k];
            }
        }
    }



    ////generate the tray coordinate:
    //coord_o: 1, Right and Down, @Robot reference
    //coord_w: 2, Right and Up
    //coord_l: 3, Left and Down
    //coord_d: 4, Left and Up

    double tray_coord[numberW][numberL][3];

    //initial the three dimension array, must modify dynamic
    for(i=0; i<numberW; i++){
        for(j=0; j<numberL; j++){
            for(k=0; k<3; k++){
                tray_coord[i][j][k] = 0;
            }
        }
    }


    if(middleCoordUsed == 0)
    {
        double unitAx_12[3]={0}, unitAx_34[3]={0}, unitAx_13[3]={0}, unitAx_24[3]={0};

        for(i=0; i<3; i++)
        {
            unitAx_12[i] = (coord_w[i] - coord_o[i])/(numberW-row-1);
            unitAx_34[i] = (coord_d[i] - coord_l[i])/(numberW-row-1);
            unitAx_13[i] = (coord_l[i] - coord_o[i])/(numberL-1);
            unitAx_24[i] = (coord_d[i] - coord_w[i])/(numberL-1);
        }


        //calculate the coordinate of boundary length(0) and length(L)
        double unitAx_L[numberW][3];
        for(j=0; j<numberW; j++){
            for(k=0; k<3; k++){
                unitAx_L[j][k]= 0;
            }
        }

        int index=0;
        for(i=row; i<numberW; i++){
            for(k=0; k<3; k++){

                //coordinate
                tray_coord[i][0][k] = coord_o[k] + unitAx_12[k]*index;
                tray_coord[i][numberL-1][k] = coord_l[k] + unitAx_34[k]*index;
            }
            index += 1;
        }


        //calculate the coordinate of tray_coord[i<row][0] and tray_coord[i<row][numberL-1]
        for(i=0; i<row; i++){
            for(k=0; k<3; k++){

                //coordinate
                tray_coord[i][0][k] = coord_o[k] - unitAx_12[k]*(row-i);
                tray_coord[i][numberL-1][k] = coord_l[k] - unitAx_34[k]*(row-i);
            }
        }



        //calculate the all of the unit vector of direction L
        for(i=0; i<numberW; i++){
            for(k=0; k<3; k++){

                //unit vector of direction L
                unitAx_L[i][k] = tray_coord[i][numberL-1][k] - tray_coord[i][0][k];
                unitAx_L[i][k] = unitAx_L[i][k]/(numberL-1);
            }
        }


        //calculate all of the tray coordinate
        for(i=0; i<numberW; i++){
            for(j=1; j<(numberL-1); j++){
                for(k=0; k<3; k++)
                {
                    tray_coord[i][j][k] = tray_coord[i][0][k] + unitAx_L[i][k]*j;
                }
            }
        }


        //modify the coordinate
        for(i=0; i<numberW; i++){
            for(j=0; j<numberL; j++){
                for(k=0; k<3; k++){
                    tray_coord[i][j][k] = tray_coord[i][j][k] + tray_coord_modify[i][j][k];
                }
            }
        }



        fstream fst;
        fst.open(filename, ios::out);

        std::stringstream s1;
        std::stringstream s2;
        std::stringstream s3;

        for(i=0; i<numberW; i++)
        {
            //cout<<"i="<<i<<endl;
            for(j=0; j<numberL; j++)
            {
                //cout<<"j="<<j<<endl;
                s1 << fixed << setprecision(2) << tray_coord[i][j][0];
                s2 << fixed << setprecision(2) << tray_coord[i][j][1];
                s3 << fixed << setprecision(2) << tray_coord[i][j][2];

                string s = s1.str() + "\t " + s2.str() + "\t " + s3.str() + "\t";
                string s6=   QString::number(coord_o2[0]).toStdString()+"\t"+QString::number(coord_o2[1]).toStdString()+"\t"+QString::number(coord_o2[2]).toStdString()+"\t";

                fst << s << s6<< "\n";

                s1.str("");
                s2.str("");
                s3.str("");
                //s.clear();

            }

            //fst << "\n";
        }

        fst.close();

    }
    else if(middleCoordUsed == 1)
    {
        double unitAx_12[3]={0}, unitAx_34[3]={0}, unitAx_56[3]={0};
        double unitAx_15[3]={0}, unitAx_26[3]={0}, unitAx_53[3]={0}, unitAx_64[3]={0};

        int numberL1, numberL2;
        numberL1 = column;
        numberL2 = numberL - numberL1 -1;

        for(i=0; i<3; i++)
        {
            unitAx_12[i] = (coord_w[i] - coord_o[i])/(numberW-row-1);
            unitAx_34[i] = (coord_d[i] - coord_l[i])/(numberW-row-1);
            unitAx_56[i] = (coord_6[i] - coord_5[i])/(numberW-row-1);

            unitAx_15[i] = (coord_5[i] - coord_o[i])/(numberL1);
            unitAx_26[i] = (coord_6[i] - coord_w[i])/(numberL1);
            unitAx_53[i] = (coord_l[i] - coord_5[i])/(numberL2);
            unitAx_64[i] = (coord_d[i] - coord_6[i])/(numberL2);
        }


        //calculate the coordinate of boundary length(0) and length(L)
        double unitAx_LR[numberW][3];
        double unitAx_LL[numberW][3];
        for(j=0; j<numberW; j++){
            for(k=0; k<3; k++){
                unitAx_LR[j][k]= 0;
                unitAx_LL[j][k]= 0;
            }
        }

        int index=0;
        for(i=row; i<numberW; i++){
            for(k=0; k<3; k++){

                //coordinate
                tray_coord[i][0][k] = coord_o[k] + unitAx_12[k]*index;
                tray_coord[i][numberL1][k] = coord_5[k] + unitAx_56[k]*index;
                tray_coord[i][numberL-1][k] = coord_l[k] + unitAx_34[k]*index;
            }
            index += 1;
        }


        //calculate the coordinate of tray_coord[i<row][0] and tray_coord[i<row][numberL-1]
        for(i=0; i<row; i++){
            for(k=0; k<3; k++){

                //coordinate
                tray_coord[i][0][k] = coord_o[k] - unitAx_12[k]*(row-i);
                tray_coord[i][numberL1][k] = coord_5[k] - unitAx_56[k]*(row-i);
                tray_coord[i][numberL-1][k] = coord_l[k] - unitAx_34[k]*(row-i);
            }
        }


        //calculate the all of the unit vector of direction L
        for(i=0; i<numberW; i++){
            for(k=0; k<3; k++){

                //unit vector of direction L, Right part
                unitAx_LR[i][k] = tray_coord[i][numberL1][k] - tray_coord[i][0][k];
                unitAx_LR[i][k] = unitAx_LR[i][k]/(numberL1);

                //unit vector of direction L, Left part
                unitAx_LL[i][k] = tray_coord[i][numberL-1][k] - tray_coord[i][numberL1][k];
                unitAx_LL[i][k] = unitAx_LL[i][k]/(numberL2);
            }
        }


        //calculate all of the tray coordinate
        for(i=0; i<numberW; i++){
            for(j=1; j<(numberL1); j++){
                for(k=0; k<3; k++)
                {
                    tray_coord[i][j][k] = tray_coord[i][0][k] + unitAx_LR[i][k]*j;
                }
            }

            for(j=(numberL1+1); j<(numberL-1); j++){
                for(k=0; k<3; k++)
                {
                    tray_coord[i][j][k] = tray_coord[i][numberL1][k] + unitAx_LL[i][k]*(j-numberL1);
                }
            }

        }


        //modify the coordinate
        for(i=0; i<numberW; i++){
            for(j=0; j<numberL; j++){
                for(k=0; k<3; k++){
                    tray_coord[i][j][k] = tray_coord[i][j][k] + tray_coord_modify[i][j][k];
                }
            }
        }



        fstream fst;
        fst.open(filename, ios::out);

        std::stringstream s1;
        std::stringstream s2;
        std::stringstream s3;

        for(i=0; i<numberW; i++)
        {
            //cout<<"i="<<i<<endl;
            for(j=0; j<numberL; j++)
            {
                //cout<<"j="<<j<<endl;
                s1 << fixed << setprecision(2) << tray_coord[i][j][0];
                s2 << fixed << setprecision(2) << tray_coord[i][j][1];
                s3 << fixed << setprecision(2) << tray_coord[i][j][2];

                string s = s1.str() + "\t " + s2.str() + "\t " + s3.str() + "\t";
                string s6=   QString::number(coord_o2[0]).toStdString()+"\t"+QString::number(coord_o2[1]).toStdString()+"\t"+QString::number(coord_o2[2]).toStdString()+"\t";

                fst << s << s6<< "\n";

                s1.str("");
                s2.str("");
                s3.str("");
                //s.clear();

            }

            //fst << "\n";
        }

        fst.close();
    }


}


void MyThread8::run()
{   
    /*
    ::step_span = this->step_span*1000;
    ::delay_time = this->delay_time*1000;
    ::omega_limit = this->omega_limit;
    ::move_time = this->move_time;
    ::scale = this->scale;

    for(int i=0;i<6;i++)
    {
        ::joint_used[i] = this->joint_use[i];
        ::position_range[i] = this->pos_range[i];
        ::gear_ratio[i] = this->gear_ratio[i];
        //::velociy_limit[i] = this->vel_limit[i];
    }
    */
    int numberL = this->number_L;
    int numberW = this->number_W;
    float tray_length = this->tray_length;
    float tray_width = this->tray_width;
    int row = this->rowW;
    int column = this->columnL;

    int modifyZ = this->modify_Z_use;
    int middleCoordUsed = this->middleCoordUse;
    int shiftUsed = this->shiftUse;

    double coord_1[3]={0};
    double coord_2[3]={0};
    double coord_3[3]={0};
    double coord_4[3]={0};
    double coord_5[3]={0};
    double coord_6[3]={0};
    int coord_o2[3]={0};
    double normAx_n[3]={0};

    for(int i=0; i<3; i++)
    {
        coord_1[i] = this->coord_1[i];
        coord_2[i] = this->coord_2[i];
        coord_3[i] = this->coord_3[i];
        coord_4[i] = this->coord_4[i];
        coord_5[i] = this->coord_5[i];
        coord_6[i] = this->coord_6[i];
        coord_o2[i] = this->coord_o[i];
        //cout << "coord_vector[" << i << "] = " << coord_o[i] << endl;
    }

    char cmd[80]={};
    strcpy(cmd, this->cmd);


    if(strcmp(cmd, "generate") == 0) //tested tray
    {
        char filename[256]={};
        strcpy(filename,this->filename);

        if(this->modify_Z_use == 1)
        {
            normAx_n[0] = this->normAx_n[0];
            normAx_n[1] = this->normAx_n[1];
            normAx_n[2] = this->normAx_n[2];
        }

        //tray_coordinate(coord_1, coord_2, coord_3, numberL, numberW, tray_length, tray_width, filename, modifyZ, normAx_n, coord_o2);
        tray_coordinate(coord_1, coord_2, coord_3, coord_4, coord_5, coord_6, numberL, numberW, column, row, filename, coord_o2, middleCoordUsed, shiftUsed);
        emit NumberChanged8(1);
    }
    else if(strcmp(cmd, "modifyz") == 0) //modify z vecotr
    {
        cross_vector(coord_1, coord_2, coord_3, normAx_n);

        this->normAx_n[0]= normAx_n[0];
        this->normAx_n[1]= normAx_n[1];
        this->normAx_n[2]= normAx_n[2];
        emit NumberChanged8(2);
    }
    else
    {
        emit NumberChanged8(0);
    }

}
