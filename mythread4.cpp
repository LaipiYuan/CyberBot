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
#include <cstring>
#include <string>
#define PI 3.14159265


using namespace std;
//double coord[3]={0};
float unit=0.0;
float angle=0.0;
//double theta[6] = {};
//double vec_J5[3]={};
//double vec_J6[3]={};
//double p6x, p6y, p6z;
double ax=0, ay=0, az=-1;
double ox=-1, oy=0, oz=0;//objeck coordinate
double first_goal_theta[4]={};
int posture=0;


double tmp_ax=0, tmp_ay=0, tmp_az=-1;
double tmp_ox=-1, tmp_oy=0, tmp_oz=0;//objeck coordinate

//double adj_j4;
//double adj_j1;

float adjjoint[4]={};

float gear_ratio[4]={};
float pos_range[4]={};
int CW[4] = {};

int NUM2 = 4;
void printall(double theta[4],double coord[3],int theta2value[4]);

#include "mythread4.h"
#include "mythread2.h"
#include <QtCore>
#include <QDebug>

using namespace std;

MyThread4::MyThread4(QObject *parent) : QThread(parent)
{
    //mThread5 = new MyThread2(this);
    //connect(mThread5, SIGNAL(NumberChanged2(int)), this, SLOT(onNumberChanged2(int)));
}
/* reads from keypress, doesn't echo */
int getch(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

/* reads from keypress, and echoes */
int getche(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}



void trig(double *arr)
{
       double *radian;
       radian=arr;
       for(int i=0; i<4; i++)
           *(radian+i) = *(radian+i)*M_PI/180;
       return;
}

void transform(int m, int n, double rotAxisJ[][3][3], double angle, int q, double rotMatrixJ[][3][3])
{
       double rad;
       rad = angle;

       int i, j, k;
       double sum=0.0, norm=0.0;
       for(i=0; i<3; i++)
                sum += pow(rotAxisJ[m][n][i],2);
       norm = sqrt(sum);

       double unitAx[3]={0};
       for(i=0; i<3; i++)
       {
                unitAx[i] = rotAxisJ[m][n][i]/norm;
       }


       double a, b, c, d;
       a = cos(rad/2);
       b = -(unitAx[0])*sin(rad/2);
       c = -(unitAx[1])*sin(rad/2);
       d = -(unitAx[2])*sin(rad/2);

       double matrix[3][3]={{a*a+b*b-c*c-d*d, 2*(b*c+a*d), 2*(b*d-a*c)},
                            {2*(b*c-a*d), a*a-b*b+c*c-d*d, 2*(c*d+a*b)},
                            {2*(b*d+a*c), 2*(c*d-a*b), a*a-b*b-c*c+d*d}};

       for(j=0; j<3; j++)
       {
                for(k=0; k<3; k++)
                         rotMatrixJ[q][j][k] = matrix[j][k];
                         //*(*(*(rotMatrixJ+q)+j)+k) = matrix[j][k];
       }

       return;
}

double dot(int q, double rotMatrixJ[][3][3], int m, int n, double rotAxisJ[][3][3], int row)
{
     int i;
     double Vector = 0;

     for(i=0; i<3; i++){
          Vector += rotMatrixJ[q][row][i]*rotAxisJ[m][n][i];
     }
//     cout << "Vector[" << row << "] = " << Vector << endl;

     return Vector;
}

double dot(int q, double rotMatrixJ[][3][3], int m, double point[][3], int row)
{
     int i;
     double pst = 0;

     for(i=0; i<3; i++){
          pst += rotMatrixJ[q][row][i]*point[m][i];
     }

     return pst;
}

int FK(double theta[4],double coord[3])
{

    double Ls=25.0, Le=25.0;
    double helical_p=2.0, gear_ratio=8;
    double theta_FK[4]={0};
    int i, j, k;

    for (int i=0;i<4;i++)
    {
        theta_FK[i]=theta[i];
    }


    trig(theta_FK);

    double temp;
    temp = theta_FK[2];
    theta_FK[2] = theta_FK[3];
    theta_FK[3] = temp;


    double point1[3]={0, 0, 0};
    double point2[2][3]={0};
    point2[0][0] = Ls;
    double point3[3][3]={0};
    point3[0][0] = Ls+Le;
    double point4[3]={0, 0, 0};

    double rotAxisJ[4][3][3]={0};
    rotAxisJ[0][0][0]=1;
    rotAxisJ[0][1][1]=1;
    rotAxisJ[0][2][2]=1;

    double rotMatrixJ[3][3][3] = {0};


    for(i=0; i<3; i++){
        int rotAxIndex = 2;
        transform(i, rotAxIndex, rotAxisJ, theta_FK[i], i, rotMatrixJ);

        for(j=0; j<3; j++){
             for(k=0; k<3; k++){
                      rotAxisJ[i+1][j][k] = dot(i, rotMatrixJ, i, j, rotAxisJ, k);
             }
        }
    }


    for(i=0; i<1; i++){
             for(j=0; j<3; j++){
                      point2[i+1][j] = dot(i, rotMatrixJ, i, point2, j);
             }
    }


    for(i=0; i<2; i++){
             for(j=0; j<3; j++){
                      point3[i+1][j] = dot(i, rotMatrixJ, i, point3, j);
             }
             if(i==0){
                     for(k=0; k<3; k++){
                              point3[1][k] -= point2[1][k];
                     }
             }
             else if(i==1){
                    for(k=0; k<3; k++){

                             point3[2][k] += point2[1][k];
                    }
             }
    }

    double delta_z;
    delta_z = (1/(2*M_PI))*helical_p*gear_ratio*theta_FK[3];


    for(i=0; i<3; i++){
             point4[i] = point3[2][i];
             if (i==2){
                point4[i] = point4[i] + delta_z;
             }
             //cout << "point4[" << i << "]=" << point4[i] << endl;
    }

    double vec_J4[3]={0,0,0};
    for (int i=0;i<3;i++)
    {
        coord[i] = point4[i];
        vec_J4[i] = rotAxisJ[3][0][i];
    }

    ::ox= vec_J4[0];
    ::oy= vec_J4[1];
    ::oz= vec_J4[2];

    return 0;
}



void IK(double coord[3], double theta[4])
{

    double Ls=25.0, Le=25.0;
    double helical_p=2.0, gear_ratio=8;
    double d, cos_312;
    double coord_J1[3]={0};

    double objvec_J1[3]={0};
    double objvec_J2[3]={0};
    double norm, j4_temp1, j4_temp2;



    d = sqrt(pow(coord[0], 2.0) + pow(coord[1], 2.0));
    cos_312 = (d*d + Ls*Ls - Le*Le)/(2*Ls*d);
    if (abs(cos_312) > 1)
       cout << "point not reachable" << endl;
    else if (posture == 0)
         theta[0] = atan2(coord[1],coord[0]) - acos(cos_312);
    else if (posture == 1)
         theta[0] = atan2(coord[1],coord[0]) + acos(cos_312);

    coord_J1[0] = coord[0]*cos(theta[0]) + coord[1]*sin(theta[0]) - Ls;
    coord_J1[1] = -coord[0]*sin(theta[0]) + coord[1]*cos(theta[0]);
    coord_J1[2] = coord[2];
    theta[1] = atan2(coord_J1[1],coord_J1[0]);
    theta[2] = 2*PI*(1/(helical_p*gear_ratio))*coord[2];

    objvec_J1[0] = ox*cos(theta[0]) + oy*sin(theta[0]);
    objvec_J1[1] = -ox*sin(theta[0]) + oy*cos(theta[0]);
    objvec_J1[2] = oz;
    objvec_J2[0] = objvec_J1[0]*cos(theta[1]) + objvec_J1[1]*sin(theta[1]);
    objvec_J2[1] = -objvec_J1[0]*sin(theta[1]) + objvec_J1[1]*cos(theta[1]);
    objvec_J2[2] = objvec_J1[2];

    norm = sqrt(pow(objvec_J2[0],2)+pow(objvec_J2[1],2)+pow(objvec_J2[2],2));

    j4_temp1 = acos(objvec_J2[0]/norm);
    j4_temp2 = acos(objvec_J2[1]/norm);

    if(j4_temp2 > 0.5*PI)
        theta[3] = -j4_temp1;
    else
        theta[3] = j4_temp1;


    for(int i=0; i<4; i++)
    {

        theta[i] = theta[i]*180/PI;
        theta[i] += adjjoint[i];
    }
    //cout << "\n";

}





void readfile()
{
    char line[100];
    char * pch;
    int data=0;
    int readposi[4]={};
    int m=0;
    fstream fst;
    fst.open("../../readposi.txt", ios::in);
    while(fst.getline(line, sizeof(line), '\n'))
    {

        //cout << line << endl;

        pch=strtok(line,"\t");
        //cout << "pch" << pch << endl;
        while(pch != '\0')
        {
            data = atoi(pch);
            //cout << data << endl;

            readposi[m]=data;
            m+=1;

            pch = strtok(NULL, "\t");
        }
        //cout << ">////<" << endl;
//    cout << "Readposi: " ;
//    for (int j=0;j<m;j++)
//        cout << readposi[j] << "\t";
//    cout << endl;

    }
    fst.close();
}

void theta_trans(double theta[4],int theta2value[4])
{

    for(int i=0;i<4;i++)
    {
        theta2value[i] = theta[i]*pos_range[i]/180;
    }

}

void value_trans(int theta2value[4],double theta[4], double coord[3])
{

    for(int i=0;i<4;i++)
    {
        theta[i] = theta2value[i]*180.0/pos_range[i];
    }


    FK(theta,coord);

    //printall(theta,coord,theta2value);
}


void coordmove(float dx, float dy, float dz, double theta[4],double coord[3],int theta2value[4])
{
    //unit = 0.1;
    //cout << "step unit++= " << unit << endl ;
    //cout << dx << "\t"<< dy << "\t"<< dz << "\t" << endl;
    coord[0] = coord[0]+dx;
    coord[1] = coord[1]+dy;
    coord[2] = coord[2]+dz;

    if (abs(coord[0]) < unit/10000)
        coord[0] = 0.0;
    else if (abs(coord[1]) < unit/10000)
        coord[1] = 0.0;
    else if (abs(coord[2]) < unit/10000)
        coord[2] = 0.0;



    IK(coord,theta);
    theta_trans(theta,theta2value);
}

void singlemove(int theta2value[4], int id, int singlestep, double theta[4],double coord[3])
{
    theta2value[id-1] = theta2value[id-1] + singlestep;
    value_trans(theta2value,theta,coord);
}

int xyz(char ch,int type, float unit,double theta[4],double coord[3],int theta2value[4])
{
    int single_theta_move = 1;
    int single_step = 0;
    int chbuffer = 0;
    int step_buffer=0;

    switch (ch)
    {

        case 'w':
            //cout << "forward" << endl;
            //coord[0]+=unit;
            coordmove(unit, 0 , 0, theta ,coord,theta2value);
            break;
        case 's':
            //cout << "backward" << endl;
            //coord[0]+=-unit;
            coordmove(-unit, 0 , 0 , theta,coord,theta2value);
            break;
        case 'a':
            //cout << "left" << endl;
            //coord[1]+=-unit;
            coordmove(0, -unit , 0 , theta,coord,theta2value);
            break;
        case 'd':
            //cout << "right" << endl;
            coordmove(0, unit , 0 , theta,coord,theta2value);
            break;
        case 'r':
            //cout << "up" << endl;
            //coord[2]+=unit;
            coordmove(0, 0 , unit, theta,coord,theta2value);
            break;
        case 'f':
            //cout << "down" << endl;
            //coord[2]+=-unit;
            coordmove(0, 0 , -unit, theta,coord,theta2value);
            break;
        case 'p':
            //cout << "READFILE" << endl ;
            readfile();
            break;
        case '1':
        case '2':
        case '3':
        case '4':

            chbuffer = ch-'0';

            //cout << "chbuffer:" << chbuffer<<endl;
            single_step = single_theta_move*pos_range[chbuffer-1]/180;

//            if (chbuffer == 1)
//                single_step = single_theta_move*L54S290Rv/180;
//            else if (chbuffer == 2 || chbuffer == 3)
//                single_step = single_theta_move*L54S500Rv/180;
//            else
//                single_step = single_theta_move*L42v/180;

            //cout << "chbuffer:" << chbuffer<<endl;
            //cout << "type:" << type <<endl;
            //cout << "aaaaangle:" <<angle<<endl;

            if (type == 1)
                step_buffer = single_step*angle;
            else if(type == 2)
                step_buffer= -single_step*angle;
            else
                step_buffer= 0;

            singlemove(theta2value, chbuffer, step_buffer, theta,coord);

            break;
        default:
            //cout << ch <<" (unknown)" << endl;
            break;
        }

    return 0;
}

int vel(double theta[4], double buff_theta[4],int speed_value[4],float movetime, float omega_limit)
{
    double omega[4]={};
    //float omega_lowlimit = 5.0;


    double diff_theta[4]={};


    for (int i=0; i<4; i++)
    {
        diff_theta[i]=abs(theta[i]-buff_theta[i]);
    }

    float vmax=0.0;
    float vmin=360.0;
    for (int i=0; i<4; i++)
    {
        if (diff_theta[i]>vmax)
            vmax = diff_theta[i];
        if (diff_theta[i]<vmin)
            vmin = diff_theta[i];
    }

    float buffvmax = vmax;
    vmax = (vmax*60.0/(360.0*movetime));

    if (vmax > omega_limit)
    {
        movetime = (buffvmax*60) / (360*omega_limit);
    }

    for (int i=0;i<4;i++)
    {
        omega[i] = (diff_theta[i]*60/(movetime*360));


        if(gear_ratio[i] > 1)
        {
            speed_value[i]=omega[i]*gear_ratio[i];
        }
        else
        {
            speed_value[i]=omega[i]/gear_ratio[i];
        }
        //cout << omega[i] << "\t" ;
//        if (i==0)
//            speed_value[i]=omega[i]*J1GRR;
//        else if (i==1 || i==2)
//            speed_value[i]=omega[i]*J23GRR;
//        else
//            speed_value[i]=omega[i]/J456UNIT;
    }
}


void w2txt(int theta2value[4],double theta[4], double buff_theta[4], int sync_type,double coord[3], float move_time, float omega_limit)
{

    fstream fst;
    fst.open("readpos.txt", ios::out);

    fstream fst2;
    fst2.open("readpos_IKFK.txt", ios::out|ios::app);
    //here???
    if(sync_type == 1) //sync
    {
        //cout << "sync 1 " << endl;
        int speed_value[4]={1,1,1,1};
       // vel(theta,buff_theta,speed_value,move_time,omega_limit);

        //block J5 velocity
//        if (speed_value[4] > 400)
//        {
//            speed_value[4]=400;
//        }

        for(int i=0; i<NUM2; i++)
        {
            //if (i==0 || i==1 || i==4)
//            if ( i==1 || i==4)
//            {
//                fst << -(theta2value[i]) << "\t";
//                fst2 << -(theta2value[i]) << "\t";

//                //cout << -(theta2value[i]) << "\t";
//            }
           // else
            //{
            fst << (theta2value[i]*CW[i]) << "\t";
            fst2 << (theta2value[i]*CW[i]) << "\t";
           // }
            //write default velocity
            //cout << "0\t";
            fst << speed_value[i] << "\t";
            fst2 << speed_value[i] << "\t";

            //fst << "0" <<"\t";
            //fst2 << "0\t";
        }
        fst << move_time << "\t";
        fst2 << move_time << "\t";

        std::stringstream s1;
        s1 << coord[0];
        std::stringstream s2;
        s2 << coord[1];
        std::stringstream s3;
        s3 << coord[2];

        string s= "("+s1.str()+","+s2.str()+","+s3.str()+")";
        //cout << "this issssssssssssssssssssss coord:" << s <<endl;

        fst << s << "\t";
        fst2 << s << "\t";

        //fst << "\n";  //#@!
        //fst2 << "\n";



        //#@!
        /*
        std::stringstream s4;
        s4 << ax;
        std::stringstream s5;
        s5 << ay;
        std::stringstream s6;
        s6 << az;

        string s_av= "("+s4.str()+","+s5.str()+","+s6.str()+")";

        fst << s_av << "\t";
        fst2 << s_av << "\t";
        */


        std::stringstream s7;
        s7 << ox;
        std::stringstream s8;
        s8 << oy;
        std::stringstream s9;
        s9 << oz;

        string s_ov= "("+s7.str()+","+s8.str()+","+s9.str()+")";

        fst << s_ov << "\t";
        fst2 << s_ov << "\t";

        fst << "\n";
        fst2 << "\n";
    }
//    if(sync_type == 2) //non-sync
//    {
//        for(int i=0; i<NUM2; i++)
//        {
//            if (i==0 || i==1 || i==4)
//            {
//                fst << -(theta2value[i]) << "\t";
//                fst2 << -(theta2value[i]) << "\t";
//            }
//            else
//            {
//                fst <<theta2value[i] << "\t";
//                fst2 <<theta2value[i] << "\t";
//            }
//            //write default velocity

//            if (i<3)
//            {
//                fst << 800 << "\t";
//                fst2 << 800 << "\t";
//            }
//            else
//            {
//                fst << 100 << "\t";
//                fst2 << 100 << "\t";
//            }
//        }

//        std::stringstream s1;
//        s1 << coord[0];
//        std::stringstream s2;
//        s2 << coord[1];
//        std::stringstream s3;
//        s3 << coord[2];

//        string s= "("+s1.str()+","+s2.str()+","+s3.str()+")";
//        //cout << "this issssssssssssssssssssss coord:" << s <<endl;

//        fst << s << "\t";
//        fst2 << s << "\t";

//        fst << "\n";
//        fst2 << "\n";
//    }

    fst.close();
    fst2.close();
}

void printall(double theta[4],double coord[3],int theta2value[4])
{
    cout << "coordinate: ";
    cout << "(" << coord[0] << ", " << coord[1] << ", "<< coord[2] << ")" <<endl;
    cout << "theta: " ;
    for (int i=0;i<NUM2;i++)
        cout << theta[i]<<"\t";
    cout << endl;
    cout << "theta2value: " ;
    for (int i=0;i<NUM2;i++)
        cout << theta2value[i]<<"\t";
    cout << endl;
    //cout << "vec_J5: ";
    //cout << "(" << vec_J5[0] << ", " << vec_J5[1] << ", "<< vec_J5[2] << ")" <<endl;
    //cout << "vec_J6: ";
    //cout << "(" << vec_J6[0] << ", " << vec_J6[1] << ", "<< vec_J6[2] << ")" <<endl;
}


void MyThread4::run()
{
//    char* case1="w";
//    char* case2="s";
//    char* case3="a";
//    char* case4="d";
//    char* case5="r";
//    char* case6="f";
//    char* case0="exit";
    cout<<"mThread4 is start!!!!\n";
    ::unit = this->unit;
    ::angle = this->angle;
    int sync_type = this->user_sync_type;
    float move_time = this->move_time;
    float omega_limit = this->omega_limit;
    double now_theta_dialog[4]={};
    double now_coord_dialog[3]={};
    int now_theta_dialog_int[4]={};
    double theta1[4]={};

    for(int i=0;i<4;i++)
    {
        ::pos_range[i] = this->pos_range[i];
        ::gear_ratio[i] = this->gear_ratio[i];
        ::adjjoint[i] = this->adj_joint[i];
//        cout << i << "+\t" << ::pos_range[i] <<endl;
//        cout << i << "+\t" << ::gear_ratio[i] <<endl;
        //cout << "adj:" << i << "\t" << ::adjjoint[i] <<endl;
        ::CW[i] = this->cw[i];

    }

    if(this->mode ==1)
    {
        coord[0]=this->coord[0];
        coord[1]=this->coord[1];
        coord[2]=this->coord[2];
    }
    else if(this->mode == 2)
    {
        cout<<"in mThread2 :this->mode =2"<<endl;
        for(int i=0;i<4;i++)
        {
            now_theta_dialog[i]=(this->now_theta_dialog[i]-adjjoint[i]);
            cout<<"in mThread4 now_theta_dialog["<<i<<"]="<<now_theta_dialog[i]<<endl;
        }
        FK(now_theta_dialog,now_coord_dialog);
        for(int i=0;i<3;i++)
        {
            this->now_coord_dialog[i]=now_coord_dialog[i];
            cout<<"in mThread4 now_coord_dialog["<<i<<"]="<<now_coord_dialog[i]<<endl;
        }
        for(int i=0;i<4;i++)
        {
            now_theta_dialog_int[i]=(now_theta_dialog[i]+adjjoint[i])*pos_range[i]/180;
        }
        now_theta_dialog_int[3]=now_theta_dialog_int[3]*cw[3];
        w2txt(now_theta_dialog_int,now_theta_dialog,now_theta_dialog,sync_type,now_coord_dialog,move_time,omega_limit);
        cout<<"in mThread4 mode=2 is finished!!!!!!!!\n";

    }

    else
    {
        cout<<"in mThread4->mode =0"<<endl;
        double theta[4]={};
        double coord[3]={};
        double buff_theta[4]={};
        int theta2value[4] = {};
        ::ox=-1;
        ::oy=0;
        ::oz=0;

        //while(1)
        //{
            sleep(0.1);
            if(this->Check == true)
            {
                int t[8];
                //::adj_j4 = 0; //
                //::adj_j1 = -0.6; //
                std::string filename("readpos.txt");
                std::ifstream in(filename.c_str());
                char buffer[256];


                //cout << "IK read position & velocity:";
                while (!in.eof() )
                {
                    cout<<"check!!!!!!!!!!!!111"<<endl;
                    in.getline (buffer,100);
                    //cout << "CW0" << CW[5] <<endl;

                    char *delim = "\t";
                    char * pch;
                    pch = strtok(buffer,delim);

                    int col=0;
                    int count=0;
                    //qDebug() <<"+++:" <<pch;

                    while (pch != NULL)
                    {
                        cout << pch << "\t";
                        if(count == 8)
                        {
                            break;
                        }

                        t[col]=atof(pch);
                        pch = strtok (NULL, delim);
                        col+=1;
                        count+=1;
                    }
                }
                //cout << endl;


                in.close();

                int th[4]={};
                //th[0]=-(t[0]);
                th[0]= t[0]*CW[0];
                th[1]= t[2]*CW[1];
                th[2]= t[4]*CW[2];
                th[3]= t[6]*CW[3];


//                cout << "CW0" << CW[0] <<endl;
//                cout << "CW0" << CW[1] <<endl;
//                cout << "CW0" << CW[2] <<endl;
//                cout << "CW0" << CW[3] <<endl;
//                cout << "CW0" << CW[4] <<endl;

                value_trans(th,theta,coord);
                //FK(theta,coord);   //trans theta to coord
                //IK(coord, theta);  //trans theta back
                //theta_trans(theta,theta2value);
                //printall(theta,coord,theta2value);
                //w2txt(theta2value, theta,buff_theta,sync_type,coord,move_time,omega_limit);
                //printall(theta,coord,theta2value);

                ::unit = this->unit;
                ::angle = this->angle;
                sync_type = this->user_sync_type;
                move_time = this->move_time;
                omega_limit = this->omega_limit;

//                cout << "Thread move_time:" << this->move_time;
//                cout << "Thread unit:" << ::unit;
//                cout << "Thread move_time:" << ::angle;

                ::ox = this->ox;
                ::oy = this->oy;
                ::oz = this->oz;
                ::posture = this->posture;
                cout<<"before choose!!!!!!"<<endl;
                cout<<"now C = "<<this->C<<endl;

                for (int i=0;i<4;i++)
                {
                    buff_theta[i]=theta[i];
                }

                if(this->C=='t')
                {
                    coord[0]=this->coord[0];
                    coord[1]=this->coord[1];
                    coord[2]=this->coord[2];

                    if (coord[2] > 10 || coord[2] < -10)
                    {
                        qDebug()<< "the high distance of object: " << coord[2] << endl;
                        emit NumberChanged4(17);
                    }

                    //calculate the coordinate range!
                    double distance;
                    distance = sqrt(pow(coord[0],2)+pow(coord[1],2));

                    if(distance <= 11.5)
                    {
                        qDebug()<< "the distance of object and center: " << distance << endl;
                        emit NumberChanged4(18);
                    }
                    else if(distance > 11.5 && distance <= 18)
                    {
                        qDebug()<< "the distance of object and center: " << distance << endl;
                        emit NumberChanged4(19);
                    }
                    else
                    {
                        ;
                    }


                    //::adj_j4 = -2;
                    //::adj_j1 = 3.9;
                    //::adj_j4 = 0;
                    //::adj_j1 = -0.4;

                    //FK(theta,coord);   //trans theta to coord
                    cout << "caseT" <<endl;
                    IK(coord, theta);
                    theta_trans(theta,theta2value);

                    //qDebug() <<"t" ;
                }
                if(this->C=='b')
                {
                    coord[0]=this->coord[0];
                    coord[1]=this->coord[1];
                    coord[2]=this->coord[2];
                    cout << "caseT" <<endl;
                    IK(coord, theta);
                    fstream fste;
                    fste.open("position_motion_first_4ax.txt",ios::out|ios::app);

                    QDateTime dt;
                    QTime time;
                    QDate date;
                    dt.setTime(time.currentTime());
                    dt.setDate(date.currentDate());
                    QString currentDate=dt.toString("yyyy:MM:dd:hh:mm:ss");
                    fste<<currentDate.toStdString().c_str()<<"\t";
                    //const char *p=currentDate.toLocal8Bit().data();
                    //const char *p=currentDate.toStdString().data();
                    //fste<<*p<<"\t";
                    fste<<"first"<<"\t"<<"0"<<"\t"<<"0"<<"\t";
                    for(int i=0; i<4; i++)
                    {
                        theta[i]=(theta[i]*CW[i]);
                        fste<<theta[i]<<"\t";
                    }
                    for(int i=0; i<4; i++)
                    {
                        this->first_goal_theta[i]=theta[i];
                    }
                    fste.close();
                }
                if(this->C=='w')
                {
                   qDebug() <<"w" ;
                   emit NumberChanged4(1);
                }
                if(this->C=='s')
                {
                   qDebug() <<"s" ;
                   emit NumberChanged4(2);
                }
                if(this->C=='a')
                {
                   qDebug() <<"a" ;
                   emit NumberChanged4(3);
                }
                if(this->C=='d')
                {
                   qDebug() <<"d" ;
                   emit NumberChanged4(4);
                }
                if(this->C=='c')
                {
                    cout<<"in 4 c=c"<<endl;
                    coord[0]=this->coord_cul[0];
                    coord[1]=this->coord_cul[1];
                    coord[2]=this->coord_cul[2];
                    cout<<"ox->"<<this->ox<<endl;
                    cout<<"oy->"<<this->oy<<endl;
                    cout<<"oz->"<<this->oz<<endl;
                    IK(coord, theta);
                    theta_trans(theta,theta2value);
                }
                if(this->C=='r')
                {
                   qDebug() <<"r" ;
                   emit NumberChanged4(5);
                }
                if(this->C=='f')
                {
                   qDebug() <<"f" ;
                   emit NumberChanged4(6);
                }
                if(this->C=='q')
                {
                   qDebug() <<"q" ;
                   emit NumberChanged4(0);
                   this->Check= false;
                   //break;
                }
                if(this->C=='1')
                {
                   emit NumberChanged4(13);
                }
                if(this->C=='2')
                {
                   emit NumberChanged4(14);
                }
                if(this->C=='3')
                {
                   emit NumberChanged4(15);
                }
                if(this->C=='4')
                {
                   emit NumberChanged4(16);
                }

                if(this->C != 't' && this->C !='c')
                {
                    //cout <<this->C <<endl;
                    //cout << "case0" <<endl;
                    xyz(this->C,this->type,::unit, theta,coord,theta2value);
                   // cout << endl;
                }

                //printall(theta,coord,theta2value);
                w2txt(theta2value, theta,buff_theta,sync_type,coord,move_time,omega_limit);

                this->theta2value[0] = theta2value[0]*CW[0];
                this->theta2value[1] = theta2value[1]*CW[1];
                this->theta2value[2] = theta2value[2]*CW[2];
                this->theta2value[3] = theta2value[3]*CW[3];

               // cout << "this->theta2value[0]" << this->theta2value[0] <<endl;
               // cout << "this->theta2value[1]" << this->theta2value[1] <<endl;
               // cout << "this->theta2value[2]" << this->theta2value[2] <<endl;
               // cout << "this->theta2value[3]" << this->theta2value[3] <<endl;

                //buff_theta
                for (int i=0; i<4; i++)
                {
                    buff_theta[i]=theta[i];
                }
                this->coord[0] = coord[0];
                this->coord[1] = coord[1];
                this->coord[2] = coord[2];


                this->Check= false;
                emit NumberChanged4(-1);

                cout<<"mThread4 is finished!!!!!!!!!!!\n";

            }
        //}
    }
}
