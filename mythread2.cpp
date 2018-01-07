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
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <getopt.h>
#include <termios.h>
#include "dynamixel.h"
#include <math.h>
#include <sys/time.h>
#include <ctime>
#include <cstdio>
#include <wait.h>
#include <QTime>
#include <QMessageBox>

#include <wiringPi.h> //this
#include <pthread.h> // and this
#include <termios.h>
#include <unistd.h>
// LED Pin - wiringPi pin 0 is BCM_GPIO 17.
#define	relayport	7
#define PI 3.14159265

#include "mythread2.h"
#include <QtCore>
#include <QDebug>

int retimes;
using namespace DXL_PRO;
using namespace std;
Dynamixel DXL("/dev/ttyUSB0");


#define PI                  3.141592f
// Control table address
#define P_TORQUE_ENABLE         562
#define P_GOAL_POSITION_LL      596
#define P_GOAL_VELOCITY         600
#define P_PRESENT_POSITION      611
#define P_PRESENT_VELOCITY      615
#define P_PRESENT_TEMPERATURE   625
// Defulat setting
#define DEFAULT_BAUDNUM     1 // 57,600
//#define DEFAULT_BAUDNUM     5 // 1M
#define STEP_THETA          (PI / 100.0f) // Large value is more fast
#define CONTROL_PERIOD      (10) // msec (Large value is more slow)

char input[128];
int NUM = 4;
int ID[4]={1,2,3,4};
int step_span;
int delay_time;
float omega_limit;
float move_time;
float scale;

int vig[4] = {};
int vpg[4] = {};
int ppg[4] = {};
int joint_use[4] = {};
int juse=0;

int group_GoalPos[1000][100][4]={};
int group_GoalVelo[1000][100][4]={};
int group_step[1000] = {};
int group_count;

float gearratio[4]={};
float posrange[4]={};
double first_reality_theta[4]={};
int vellimit[4]={};


MyThread2::MyThread2(QObject *parent) : QThread(parent){}

void suck_on()
{
    wiringPiSetup () ;
    pinMode (relayport, OUTPUT) ;

    digitalWrite (relayport, HIGH);  //high
}

void suck_off()
{
    wiringPiSetup () ;
    pinMode (relayport, OUTPUT) ;

    digitalWrite (relayport, LOW);  //high
}


void gpio_on(int ioport)//***gpio will replace suck_on/off for controlling mutli IO port
{
    cout << "enter gpio on---"<<ioport << endl;
    wiringPiSetup () ;
    pinMode (ioport, OUTPUT) ;

    digitalWrite(ioport, HIGH);  //high
}

void gpio_off(int ioport)
{
    cout << "enter gpio off---"<<ioport << endl;
    wiringPiSetup () ;
    pinMode (ioport, OUTPUT) ;

    digitalWrite(ioport, LOW);  //high
}//***


int _getch()
{
    struct termios oldt, newt;
    int ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}

//int clamping_relay (char ch)
//{
//    if (ch == '*')
//    {
//        cout << "open" << endl;
//        digitalWrite (relayport, HIGH);  //high
//    }
//    else if (ch == '/')
//    {
//        cout << "close" << endl;
//        digitalWrite (relayport, LOW); // low

//    }


//}

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void Usage(char *progname)
{
    fprintf(stderr, "-----------------------------------------------------------------------\n");
    fprintf(stderr, "Usage: %s\n" \
                    " [-h | --help]........: display this help\n" \
                    " [-d | --device]......: port to open                     (/dev/ttyUSB0)\n" \
                    , progname);
    fprintf(stderr, "-----------------------------------------------------------------------\n");
}

void Help()
{
    fprintf(stderr, "\n");
    fprintf(stderr, "COMMAND: \n\n" \
                    " baud [BAUD_NUM]           : Baudrate change to [BAUD_NUM] \n" \
                    "                              0:2400, 1:57600, 2:115200, 3:1M, 4:2M, 5:3M \n" \
                    "                              6:4M, 7:4.5M, 8:10.5M \n" \
                    " scan                      : Output the current status of all DXLs \n" \
                    " ping [ID] [ID] ...        : Output the current status of [ID] \n" \
                    " bp                        : Broadcast Ping \n" \
                    " wrb [ID] [ADDR] [VALUE]   : Write 1 byte [VALUE] to [ADDR] of [ID] \n" \
                    " wrw [ID] [ADDR] [VALUE]   : Write 2 byte [VALUE] to [ADDR] of [ID] \n" \
                    " wrd [ID] [ADDR] [VALUE]   : Write 4 byte [VALUE] to [ADDR] of [ID] \n" \
                    " rdb [ID] [ADDR]           : Read 1 byte value from [ADDR] of [ID] \n" \
                    " rdw [ID] [ADDR]           : Read 2 byte value from [ADDR] of [ID] \n" \
                    " rdd [ID] [ADDR]           : Read 4 byte value from [ADDR] of [ID] \n" \
                    " r [ID] [ADDR] [LENGTH]    : Dumps the control table of [ID] \n" \
                    "                              [LENGTH] bytes from [ADDR] \n" \
                    " mon [ID] [ADDR] b|w|d     : Refresh byte|word|dword from [ADDR] of [ID] \n" \
                    " reboot [ID]               : Reboot the dynamixel of [ID] \n" \
                    " reset [ID] [OPTION]       : Factory reset the dynamixel of [ID] \n" \
                    "                              OPTION: 255(All), 1(Except ID), 2(Except ID&Baud)\n" \
                    " exit                      : Exit this program \n" \
                    );
    fprintf(stderr, "\n");
}

// Print error bit of status packet
void PrintErrorCode(int ErrorCode)
{
    printf("ErrorCode : %d (0x%X)\n", ErrorCode, ErrorCode);
    if(ErrorCode & ERRBIT_VOLTAGE)
        printf("Input voltage error!\n");

    if(ErrorCode & ERRBIT_ANGLE)
        printf("Angle limit error!\n");

    if(ErrorCode & ERRBIT_OVERHEAT)
        printf("Overheat error!\n");

    if(ErrorCode & ERRBIT_RANGE)
        printf("Out of range error!\n");

    if(ErrorCode & ERRBIT_CHECKSUM)
        printf("Checksum error!\n");

    if(ErrorCode & ERRBIT_OVERLOAD)
        printf("Overload error!\n");

    if(ErrorCode & ERRBIT_INSTRUCTION)
        printf("Instruction code error!\n");
}

void Scan()
{
    fprintf(stderr, "\n");
    PingInfo *data = new PingInfo();
    for(int i = 1; i < 7; i++)
    {
        if(DXL.Ping(i, data, 0) == COMM_RXSUCCESS)
        {
            fprintf(stderr, "\n                                          ... SUCCESS \r");
            fprintf(stderr, " [ID:%.3d] Model No : %.5d (0x%.2X 0x%.2X) \n",
                    data->ID, data->ModelNumber, DXL_LOBYTE(data->ModelNumber), DXL_HIBYTE(data->ModelNumber));
        }
        else
            fprintf(stderr, ".");

        if(kbhit())
        {
            char c = _getch();
            if(c == 0x1b)
                break;
        }

        usleep(delay_time);
    }
    fprintf(stderr, "\n\n");
}

void Write(int id, int addr, long value, int len)
{
    int result = COMM_TXFAIL, error = 0;

    if(len == 1)
        result = DXL.WriteByte(id, addr, (int)value, &error);
    else if(len == 2)
        result = DXL.WriteWord(id, addr, (int)value, &error);
    else if(len == 4)
        result = DXL.WriteDWord(id, addr, value, &error);

    if(result != COMM_RXSUCCESS)
    {
        fprintf(stderr, "\n Fail to write! \n\n");
        return;
    }

    if(error != 0)
        PrintErrorCode(error);

    fprintf(stderr, "\n Success to write! \n\n");
}

void Read(int id, int addr, int len)
{
    int result = COMM_TXFAIL, error = 0;
    int ivalue = 0;
    long lvalue = 0;

    if(len == 1)
        result = DXL.ReadByte(id, addr, &ivalue, &error);
    else if(len == 2)
        result = DXL.ReadWord(id, addr, &ivalue, &error);
    else if(len == 4)
        result = DXL.ReadDWord(id, addr, &lvalue, &error);

    if(result != COMM_RXSUCCESS)
    {
        fprintf(stderr, "\n Fail to read! (result : %d) \n\n", result);
        return;
    }

    if(error != 0)
        PrintErrorCode(error);

    if(len == 1)
        fprintf(stderr, "\n READ VALUE : %d \n\n", ivalue);
    else if(len == 2)
        fprintf(stderr, "\n READ VALUE : (UNSIGNED) %u , (SIGNED) %d \n\n", ivalue, ivalue);
    else
        fprintf(stderr, "\n READ VALUE : (UNSIGNED) %lu , (SIGNED) %d \n\n", lvalue, lvalue);
}

void Dump(int id, int addr, int len)
{
    int result = COMM_TXFAIL, error = 0;
    unsigned char* data = (unsigned char*)calloc(len, sizeof(unsigned char));

    result = DXL.Read(id, addr, len, data, &error);
    if(result != COMM_RXSUCCESS)
    {
        fprintf(stderr, "\n Fail to read! (result : %d) \n\n", result);
        return;
    }

    if(error != 0)
        PrintErrorCode(error);

    if(id != BROADCAST_ID)
    {
        fprintf(stderr, "\n");
        for(int i = addr; i < addr+len; i++)
            fprintf(stderr, "ADDR %.3d [0x%.4X] :     %.3d [0x%.2X] \n", i, i, data[i-addr], data[i-addr]);
        fprintf(stderr, "\n");
    }
}

void Refresh(int id, int addr, int len)
{
    int result = COMM_TXFAIL, error = 0;
    int ivalue = 0;
    long lvalue = 0;

    fprintf(stderr, "\n [ESC] : Quit monitoring \n\n");
    while(1)
    {
        if(len == 1)
            result = DXL.ReadByte(id, addr, &ivalue, &error);
        else if(len == 2)
            result = DXL.ReadWord(id, addr, &ivalue, &error);
        else if(len == 4)
            result = DXL.ReadDWord(id, addr, &lvalue, &error);

        if(result != COMM_RXSUCCESS)
        {
            //fprintf(stderr, "\n Fail to read! (result : %d) \n\n", result);
            continue;
        }

        if(error != 0)
            PrintErrorCode(error);

        if(len == 1)
            fprintf(stderr, " READ VALUE : %.3d [0x%.2X] \r", ivalue, ivalue);
        else if(len == 2)
            fprintf(stderr, " READ VALUE : %.5d [0x%.2X 0x%.2X] \r", ivalue, DXL_LOBYTE(ivalue), DXL_HIBYTE(ivalue));
        else
            fprintf(stderr, " READ VALUE : %.10ld [0x%.2X 0x%.2X 0x%.2X 0x%.2X] \r",
                    lvalue, DXL_LOBYTE(DXL_LOWORD(lvalue)), DXL_HIBYTE(DXL_LOWORD(lvalue)), DXL_LOBYTE(DXL_HIWORD(lvalue)), DXL_HIBYTE(DXL_HIWORD(lvalue)));

        if(kbhit())
        {
            char c = _getch();
            if(c == 0x1b)
                break;
        }
    }
    fprintf(stderr, "\n\n");
}

void BroadcastPing()
{
    std::vector<PingInfo> vec_info = std::vector<PingInfo>();
    DXL.BroadcastPing(vec_info);

    if(vec_info.size() > 0)
    {
        fprintf(stderr, "\n");
        for(int i = 0; i < (int)vec_info.size(); i++)
        {
            fprintf(stderr, "                                          ... SUCCESS \r");
            fprintf(stderr, " [ID:%.3d] Model No : %.5d (0x%.2X 0x%.2X) \n",
                    vec_info.at(i).ID, vec_info.at(i).ModelNumber, DXL_LOBYTE(vec_info.at(i).ModelNumber), DXL_HIBYTE(vec_info.at(i).ModelNumber));
        }
        fprintf(stderr, "\n");
    }
}

void BeltTest()
{
    int result;
    long lvalue1 , lvalue2;

    while(true)
    {
        if(kbhit())
            break;

        result = DXL.ReadDWord(17, 611, &lvalue1, 0);
        result = DXL.ReadDWord(18, 611, &lvalue2, 0);
        fprintf(stderr, "\n READ VALUE : %d , %d \n\n", lvalue1, lvalue2);
    }
}

// Print communication result
void PrintCommStatus(int CommStatus)
{
    switch(CommStatus)
    {
    case COMM_TXFAIL:
        printf("COMM_TXFAIL: Failed transmit instruction packet!\n");
        break;

    case COMM_TXERROR:
        printf("COMM_TXERROR: Incorrect instruction packet!\n");
        break;

    case COMM_RXFAIL:
        printf("COMM_RXFAIL: Failed get status packet from device!\n");
        break;

    case COMM_RXWAITING:
        printf("COMM_RXWAITING: Now recieving status packet!\n");
        break;

    case COMM_RXTIMEOUT:
        printf("COMM_RXTIMEOUT: There is no status packet!\n");
        break;

    case COMM_RXCORRUPT:
        printf("COMM_RXCORRUPT: Incorrect status packet!\n");
        break;

    default:
        printf("This is unknown error code!\n");
        break;
    }
}



int reboot_all()
{
    int result;
    cout << "=========================================================\n";
    for(int i=0; i<NUM; i++)
    {
        if(joint_use[i] == 1)
        {
            //cout << "J" << ID[i] << " velo_P_gain ERROR, Reboot..." << endl;
            result = DXL.Reboot(i+1, 0);
            usleep(delay_time);
            if(result != COMM_RXSUCCESS)
            {
                cout << "Fail to reboot!" << endl;
                return 0;
            }
            else
            {
                cout << "Success to reboot!" << endl;
            }
        }

    }
    return 1;
}

int tonall()
{
    int result;
    int ivalue = 0;
    int error = 0;
    cout << "=========================================================\n";
    cout << "torque state (on):\n";
    for (int i=0;i<4;i++)
    {
        if(joint_use[i] == 1)
        {
            DXL.ReadByte(i+1, 562, &ivalue, &error);
            //usleep(3000);
            usleep(delay_time);
            //result=DXL.WriteByte(ID[i], P_TORQUE_ENABLE, 1, &error);
            if(DXL.WriteByte(i+1, P_TORQUE_ENABLE, 1, &error) != COMM_RXSUCCESS)
            {
                cout << "ID " << ID[i] << " torque on fail" << endl;
                //return ID[i], P_TORQUE_ENABLE;
                return 0;
            }
            else
            {
                cout << "ID " << ID[i] << " torque on successfully" << endl;
            }
            usleep(delay_time);
            //sleep(0.1);
        }
    }

    return 1;
}

int toffall()
{
    int result = 0;
    int error = 0;
    int str;
    cout << "=========================================================\n";
    cout << "torque state (off):\n";
    for (int i=0;i<NUM;i++)
    {
        if(joint_use[i] == 1)
        {
            result=DXL.WriteByte(i+1, P_TORQUE_ENABLE, 0, &error);

            if(result != COMM_RXSUCCESS)
            {
                cout << "ID " << ID[i] << " torque off fail" << endl;
                return ID[i], P_TORQUE_ENABLE;
                //return 0;
            }
            else
                cout << "ID " << ID[i] << " torque off successfully" << endl;
            usleep(delay_time);
        }
    }

    return 1;
}

int initialization()
{
    #define velo_I_gain 586
    #define velo_P_gain 588
    #define pos_P_gain 594
    #define P_TORQUE_ENABLE      562
    cout << "=========================================================\n";
    cout << "parameter setting" << endl;
    int result;
    int ivalue = 0;
    int error = 0;

    //ID 1,2,3 Gain modify

    for (int i=0;i<4;i++)
    {
        if(joint_use[i] == 1)
        {
            cout << "vig[" <<i<< "]:"<< vig[i]<< "\t";
            cout << "vpg[" <<i<< "]:"<< vpg[i]<< "\t";
            cout << "ppg[" <<i<< "]:"<< ppg[i]<< "\n";

            if(DXL.WriteWord(i+1,velo_I_gain,vig[i],&error) != COMM_RXSUCCESS)
            {
                cout << "J" << ID[i] << " velo_I_gain ERROR!" << endl;
                //return ID[i],velo_I_gain;
                return 2;
            }

            usleep(delay_time);

            //int result2=DXL.WriteWord(ID[i],velo_P_gain,vpg[i],&error);

            if(DXL.WriteWord(i+1,velo_P_gain,vpg[i],&error) != COMM_RXSUCCESS)
            {
                cout << "J" << ID[i] << " velo_P_gain ERROR" << endl;
                //return ID[i],velo_P_gain;
                return 2;
            }

            //int result3=DXL.WriteWord(ID[i],pos_P_gain,ppg[i],&error);
            usleep(delay_time);

            if(DXL.WriteWord(i+1,pos_P_gain,ppg[i],&error) != COMM_RXSUCCESS)
            {
                cout << "J" << ID[i] << " pos_P_gain ERROR" << endl;
                //return ID[i],pos_P_gain;
                return 2;

            }
            usleep(delay_time);
        }
    }

    for(int i=0;i<3;i++)
    {
        if(i==2)
        {
            cout << ID[i] << " acceleration ERROR!" << endl;
            while(DXL.WriteDWord(i+1,606,15,&error) == COMM_RXSUCCESS)
            {
                //cout << ID[i] << " acceleration ERROR!" << endl;
                //return ID[i],velo_I_gain;
                //return 2;
                break;
            }
        }
        else
        {
            cout << ID[i] << " acceleration ERROR!" << endl;
            while(DXL.WriteDWord(i+1,606,10,&error) == COMM_RXSUCCESS)
            {
                //cout << ID[i] << " acceleration ERROR!" << endl;
                //return ID[i],velo_I_gain;
                //return 2;
                break;
            }
        }

        usleep(delay_time);
    }

    //tonall();
    int tonall_result = tonall();

    if(tonall_result != 1)
    {
        return 0;
    }

    return 1;
}

void readall()
{
    cout << "Read gain" << endl;
    int ivalue = 0;
    int error = 0;
    int vig=0, vpg=0, ppg=0, tor=0;
    for (int i=0;i<4;i++)
    {
        cout << "ID " << i+1 << ":" << endl;
        DXL.ReadWord(i+1,562,&tor,&error);
        DXL.ReadWord(i+1,586,&vig,&error);
        DXL.ReadWord(i+1,588,&vpg,&error);
        DXL.ReadWord(i+1,594,&ppg,&error);

        cout << "torque, vig, vpg, ppg= " << tor << " ; " << vig << " ; " << vpg << " ; " << ppg << " ; " << endl;
    }
}

int MyThread2::readpos()
{
    //testing

    int result = 0;
    int error = 0;
    long lvalue = 0;
    long lvpos[4];
    cout << "===================================================\n";
    cout << "SDK read position:";
    fstream fstf;
    fstf.open("position_motion_first_4ax.txt",ios::out|ios::app);
    if (!fstf)
    {
        cout << "error open file"<< endl;
        //fail to write
        return 0;
    }
    for (int i=0;i<4;i++)
    {
        if(joint_use[i] == 1)
        {
            result = DXL.ReadDWord(i+1, 611, &lvalue, &error);    //~~~~
            if(result!=COMM_RXSUCCESS)
            {
                cout<<"fenggeshizhu@@@@@@@@@@@@@@@@"<<endl;
                return 0;
            }

            lvpos[i] = lvalue;

            this->first_reality_theta[i]=lvalue*180/posrange[i];
            fstf<<this->first_reality_theta[i]<<"\t";
        }
        else
        {
            lvpos[i] = 0;
        }

    }
    fstf.close();

    char filename[]="readpos.txt";
    fstream fst;
    //fst.open(filename,ios::out|ios::app);
    fst.open(filename,ios::out);
    //cout << "here" << endl;
    if (!fst)
    {
        cout << "error open file"<< endl;
        //fail to write
        return 0;
    }
    else
    {
        for (int i=0;i<NUM;i++)
        {
            fst << lvpos[i] <<"\t";
            fst << 1 << "\t"; //fix

        }
        fst <<"\n";

    }
    fst.close();

    return 1;

}



int read_each_pos(char filename[80], int id)
{
    //testing
    int result = 0;
    int error = 0;
    long lvalue = 0;
    //long lvpos[4] ;

    result = DXL.ReadDWord(id, 611, &lvalue, &error);   //~~~~

    if(result!=COMM_RXSUCCESS)
    {
        cout << "break";
        return 0;
    }

    cout << lvalue << "\t";
    //sleep(1);
    cout << endl;

    fstream fst;
    fst.open(filename,ios::out|ios::app);

    fst << lvalue <<"\t";
    fst <<"\n";
    fst.close();

    return 1;

}

void record_motion(int id,int step,long pos)
{
    char filename[80]={};

    stringstream ss;
    ss << id;
    string filename_str = ss.str()+".txt";

    fstream fst;
    fst.open(filename_str.c_str(),ios::out|ios::app);

    fst << step <<"\t" << id << "\t" << pos <<"\t\n";
    fst.close();
}




int w2txt(long pos[4], long vel[4],double readtime)
{
    fstream fst;
    fst.open("./pvread.txt", ios::out|ios::app);
    if (!fst)
    {
        cout << "Fail to open file" << endl;
        return 0;
    }
    else
    {
        //fst << readtime << "\t" ;
        for (int i=0;i<NUM;i++)
        {
            fst << pos[i] << "\t" << vel[i] << "\t" ;
        }
        fst << "\n";
    }
    fst.close();
    //cout << "timer get: " ;
    //printf("%f\n",readtime);

}

void MyThread2::motion(double readtime,int step,int motion_velo[4],int motion_posi[4])
{
    //test, bad result
    cout<<"into motion!!!!!!!11"<<endl;
    int check[4]={-1,-1,-1,-1};
    long vel[4]={1,1,1,1};
    long pos[4]={1,1,1,1};
    int error = 0;
    int count = 0;
    int motion_reality_velo[4]={0};
    int motion_reality_posi[4]={0};

    int juse=0;
    for(int i=0;i<4;i++)
    {
        if(joint_use[i] ==1)
        {
            juse+=1;
        }
    }
    //int cou=0;

    fstream fst;
    fst.open("record_motion",ios::out|ios::app);

    fst << "==================================================\n";

    //fstream fstd;
    //fstd.open("position_motion_4ax.txt",ios::out|ios::app);

    for(int i=0;i<4;i++)
    {
        fst << i << "\t" << motion_velo[i] << "\t";
    }
    fst << "\n";
    cout<<"before motion while!!!!!!!!"<<endl;
    while(this->stop == false) //vel[0] >= 50 || vel[1] != 0 ||vel[2] != 0 ||vel[3] != 0 ||vel[4] != 0 || vel[5] != 0
    {
        cout << "into motion while" << endl;
        int finish=0;
        for(int i=0;i<4;i++)
        {
            //cout << "end:" << i <<"\t"<< check[i] << "\t" << motion_velo[i];//
            if(check[i] != -1)
            {
                //cout << "\tcheck";//
                finish+=1;
            }
            //cout << "\n";//
        }

        //cout << "finish:" <<finish << "juse:" << juse <<endl;





        if(vel[0] <= 20 && vel[1] <= 20 && vel[2] <= 50 && vel[3] <= 5 && finish == juse && count >= 10 )
        {

            cout << "motion velocity become zero!!!" << endl;



            this->move = false; //***

            //cout << "now count is " << count << endl;
           // cout << "motion is finished!" << endl;
            break;
        }
        cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! without break, now count is " << count << endl;
        count += 1;

        struct timeval start,end;
        double timecost;

        gettimeofday(&start, NULL);

        int result = -1;

        for (int k=0;k<NUM;k++)
        {
            while(joint_use[k] == 1)
            {

                long lvalue = 100;

                //cout << "motion2\n";
                result = DXL.ReadDWord(k+1, 615, &lvalue,&error);   //present velocity~~~~
                //cout << "ok\n";
                vel[k] = abs(lvalue);

                fst << "=" << k <<"=" << "\t" << vel[k] << "\t" << motion_velo[k] <<"\ts:"<< result<<"\t+"<<check[k]<<"+"<< "\t";

                if(check[k] == -1)
                {
                    cout << "vel:" << k << "\t" <<vel[k] << "\n";
                    ;
                }

                if(result == 1)
                {
                    if(vel[k] >= check[k])
                    {
                        check[k] = vel[k];
                        cout<<"ID"<<k<<": "<<vel[k]<<endl;
                    }
                    cout<<"ID"<<k<<": "<<vel[k]<<endl;
                    break;
                }

                else
                {
                    cout<<"read ID"<<k<<"is failed,now vel["<<k<<"]="<<lvalue<<endl;
                }

//                if(vel[k] >= (motion_velo[k])*0.15 || motion_velo[k] <=30 || abs(motion_velo[k]-vel[k]) <=30)
//                {
//                    //cout << k << "\tcheck!\n";//

//                    if(vel[k] >= check[k])
//                    {
//                        check[k] = vel[k];
//                    }
//                }

            }
            /*else
            {
                vel[k] = 0;
                fst << "=" << k <<"=" << "\t" << vel[k] << "\t" << motion_velo[k] <<"\ts:"<< result<<"\t+"<<check[k]<<"+"<< "\t";
            }*/

            usleep(delay_time);
        }
        fst << "\n";

        gettimeofday(&end, NULL);
        timecost = ((end.tv_sec - start.tv_sec)*1000.0+(end.tv_usec - start.tv_usec)/1000.0);
        readtime += timecost;
    }
    int res = w2txt(pos, vel,readtime); //record when motion end

    fst.close();
    //cout << "motion end\n";

}

//int sync2write(int length, int step, int GoalPos[100][6], int GoalVelo[100][6], int retimes)
//{
//    int id[6]={1,2,3,4,5,6};
//    int angle_limit[6] = {180000,90340,140000,2048,1400,2048};
//    //int angle_limit[6] = {103860,103860,90928,2049,1356,2048};
//    //int angle_limit[6] = {103860,103860,144000,2049,2048,2048};
//    int tm = 0;
//    int tims = retimes;
//    //int NUM = 6;
//    int result = 0;
//    unsigned char param[100][54]={};
//    //cout << "enter syncwrite" << endl;
//    for (int i=0;i<step;i++)
//    {

//        for (int j=0;j<NUM;j++)
//        {
//            param[i][j*(1+8)+0] = (unsigned char)id[j];
//            param[i][j*(1+8)+1] = DXL_LOBYTE(DXL_LOWORD(GoalPos[i][j]));
//            param[i][j*(1+8)+2] = DXL_HIBYTE(DXL_LOWORD(GoalPos[i][j]));
//            param[i][j*(1+8)+3] = DXL_LOBYTE(DXL_HIWORD(GoalPos[i][j]));
//            param[i][j*(1+8)+4] = DXL_HIBYTE(DXL_HIWORD(GoalPos[i][j]));
//            param[i][j*(1+8)+5] = DXL_LOBYTE(DXL_LOWORD(GoalVelo[i][j]));
//            param[i][j*(1+8)+6] = DXL_HIBYTE(DXL_LOWORD(GoalVelo[i][j]));
//            param[i][j*(1+8)+7] = DXL_LOBYTE(DXL_HIWORD(GoalVelo[i][j]));
//            param[i][j*(1+8)+8] = DXL_HIBYTE(DXL_HIWORD(GoalVelo[i][j]));

//        }

//    }

//    for(int i=0;i<step;i++)
//    {
//        cout << "move: " << i+1 << endl;
//        for (int j=0;j<(NUM*9);j++)
//        {
//            cout << int(param[i][j]) << "\t";

//        }
//        cout << endl;
//    }
//    //check the file exist of not
//    fstream fst;
//    fst.open("./pvread.txt");
//    if (fst.is_open() == true)
//    {
//        fst.close();
//        remove("pvread.txt");
//    }

//    //check file exist end

//    unsigned char paramdata[54]={};
//    //tims=5;
//    //cout << "times?" <<tims<< endl;
//    //cin >> tims;





//    while (tm != tims)
//    {

//        for (int i=0; i<step; i++)
//        {
//            if(GoalPos[i][0]==999991 || GoalPos[i][0]==999990)
//            {
//                //cout << "in" <<endl;
//                int stime=GoalVelo[i][0];
//                if(GoalPos[i][0]==999991)
//                {
//                    suck_on();
//                    sleep(stime);
//                }
//                if(GoalPos[i][0]==999990)
//                {
//                    suck_off();
//                    sleep(stime);
//                }
//            }
//            else
//            {
//                //break;
//                double readtime = 0;

//                //block the angle limit

//                for (int k=0;k<NUM;k++)
//                {
//                    if (abs(GoalPos[i][k])>angle_limit[k])
//                    {
//                        cout << "####################################out of range----------------------------------------------" << endl;
//                        cout <<i<<"\t"<< GoalPos[i][k] << "\t" << angle_limit[k] <<"\t\n";
//                        return 0;
//                    }

//                }


//                for(int j=0;j<(NUM*9);j++)
//                {
//                    paramdata[j]=param[i][j];
//                }

//                //cout << "move " << i+1 << endl;
//                result = DXL.SyncWrite(596, length, paramdata, NUM*9);
//                cout <<"****************************************************************************************" << result <<"\n";

//                usleep(1000000);
//                for (int k=0;k<(NUM*9);k++)
//                {
//                    cout << int(paramdata[k]) << " " ;
//                }
//                cout << endl;
//                if( result != COMM_RXSUCCESS )
//                {
//                    PrintCommStatus(result);
//                    cout << "Status error, break" << endl;
//                }

//                motion(readtime,i);
//            }
//            sleep(step_span);

//        }
//        tm += 1;
//    }

//}

int posi_start_vel(int GoalPos[100][4], int GoalVelo[100][4])
{

    double now_theta[4]={0};
    double first_theta[4]={};
    int error=0;

    struct timeval start,end;
    double timecost;

    cout << "now===============\n";
    for (int i=0;i<NUM;i++)
    {
        if(joint_use[i] == 1)
        {
            long posivalue=0;

            int result=-1;

            gettimeofday(&start, NULL);

            while(1)
            {
                gettimeofday(&end, NULL);
                result = DXL.ReadDWord(i+1, 611, &posivalue, &error);  //~~~~
                timecost = ((end.tv_sec - start.tv_sec)*1000.0+(end.tv_usec - start.tv_usec)/1000.0);

                if(timecost >5000)
                {
                    return 1; //read pos fail
                }
                if(result == 1)
                {
                    now_theta[i] = posivalue*180/posrange[i];
                    cout << i << "\t" << posivalue << "\t" << posrange[i] << endl;
                    break;
                }
            }
        }
    }

    float vmax=0;
    float diff=0.0;

    //cout << "move===============\n";

    for (int i=0;i<NUM;i++)
    {
        first_theta[i] = GoalPos[0][i]*180/posrange[i];

        cout << i << "\t" << GoalPos[0][i] << "\t" << posrange[i] << endl;

        if(fabs(first_theta[i]-now_theta[i])>vmax)
        {
            vmax = fabs(first_theta[i]-now_theta[i]);
            cout << "vmax:" << vmax << "\t" << i <<endl;
        }
    }

    cout << "mmmmove time:" << move_time <<endl;
    cout << "ddd:" << (vmax*60)/(360*move_time) <<endl;
    if (((vmax*60)/(360*move_time)) > omega_limit)
    {
        move_time=(vmax*60)/(360*omega_limit);
        cout << "   exceed limit w, new defaulttime= " << move_time << endl;
    }

    for (int i=0;i<NUM;i++)
    {
        double now_omega = (fabs(first_theta[i]-now_theta[i])*60.0)/(move_time*360.0);

        if(gearratio[i] > 1)
        {
            GoalVelo[0][i] = now_omega*gearratio[i];
        }
        else
        {
            GoalVelo[0][i] = now_omega/gearratio[i];
        }

        cout << i << "\t" << GoalVelo[0][i] <<"\t" << now_omega <<"\t" << gearratio[i] <<endl;
    }


    cout << endl;
    return 0;
}

double posi_start_vel_change(int GoalPos[4], int NowPos1[4], double GoalVelo1[4])
{

    //double now_theta[4]={0};
    double first_theta[4]={};
    double nowpos2[4]={};
    //int error=0;


    float vmax=0;
    //float diff=0.0;

    //cout << "move===============\n";

    for (int i=0;i<NUM;i++)
    {
        first_theta[i] = GoalPos[i]*180/posrange[i];
        nowpos2[i]=NowPos1[i]*180/posrange[i];

        //cout << i << "\t" << GoalPos[0][i] << "\t" << posrange[i] << endl;

        if(fabs(first_theta[i]-nowpos2[i])>vmax)
        {
            vmax = fabs(first_theta[i]-nowpos2[i]);
            //cout << "vmax:" << vmax << "\t" << i <<endl;
        }
    }

    //cout << "mmmmove time:" << move_time <<endl;
    //cout << "ddd:" << (vmax*60)/(360*move_time) <<endl;
    if (((vmax*60)/(360*move_time)) > omega_limit)
    {
        move_time=(vmax*60)/(360*omega_limit);
        //cout << "   exceed limit w, new defaulttime= " << move_time << endl;
    }

    for (int i=0;i<NUM;i++)
    {
        double now_omega = (fabs(first_theta[i]-nowpos2[i])*60.0)/(move_time*360.0);

        if(gearratio[i] > 1)
        {
            GoalVelo1[i] = now_omega*gearratio[i];
        }
        else
        {
            GoalVelo1[i] = now_omega/gearratio[i];
        }

       // cout << i << "\t" << GoalVelo[i] <<"\t" << now_omega <<"\t" << gearratio[i] <<endl;
    }


    //cout << endl;
    return 0;
}

//int sync2write(int length, int step, int GoalPos[100][6], int GoalVelo[100][6], int retimes)
int MyThread2::sync2write(int length, int step, int GoalPos[100][4], int GoalVelo[100][4], int retimes)
{
    int id[4]={1,2,3,4};
    /*
    int angle_limit[4] = {};

    for(int i=0; i<4; i++)
    {
        angle_limit[i] = posrange[i];
    }
    */

    int tm = 0;
    int tims = retimes;
    //int NUM = 4;
    int result = 0;
    unsigned char param[100][54]={};
    int error = 0;

    fstream fst;
    fst.open("./sync2write.txt");

    //unsigned char paramdata[54]={};

    struct timeval start,end;
    double timecost;

    while (tm != tims)
    {
        cout << "==================================="<< tm <<"=======================================\n";

        fstream fst0;
        fst0.open("record_motion",ios::out);
        fst0.close();

        this->scan_ready = false;

        for (int i=0; i<step; i++)
        {

            fstream fstc;
            fstc.open("position_motion_4ax.txt",ios::out|ios::app);

            cout << "step:" << i <<endl;
            if(GoalPos[i][0]==999991 || GoalPos[i][0]==999990)
            { 
                int stime=GoalVelo[i][0];
                if(GoalPos[i][0]==999991)
                {
                    this->move = true;
                    suck_on();
                    cout << "vavle on" << endl;
                    sleep(stime);
                    this->move = false;
                }
                if(GoalPos[i][0]==999990)
                {
                    this->move = true;
                    suck_off();
                    cout << "vavle off" << endl;
                    sleep(stime);
                    this->move = false;
                }
            }
            else if(GoalPos[i][0]==9999995 || GoalPos[i][0]==9999996 || GoalPos[i][0]==9999997 || GoalPos[i][0]==9999998)   //~!@#$
            {
                this->done2 = false;
                switch(GoalPos[i][0])
                {
                    case 9999995:
                        cout << "********************left vacuum\n";
                        //emit NumberChanged2(-1);
                        //while(1)
                        //{
                        //    if(this->done2 == true)
                        //        break;
                        //}
                        break;
                    case 9999996:
                        cout << "********************right vacuum\n";
                        //emit NumberChanged2(-2);
                        //while(1)
                        //{
                        //    if(this->done2 == true)
                        //    break;
                        //}
                        break;
                    case 9999997:
                        cout << "********************left flow\n";
                        //emit NumberChanged2(-3);
                        //while(1)
                        //{
                        //    if(this->done2 == true)
                        //    break;
                        //}
                        break;
                    case 9999998:
                        cout << "********************right flow\n";
                        //emit NumberChanged2(-4);
                        //while(1)
                        //{
                        //    if(this->done2 == true)
                        //    break;
                        //}
                        break;
                }
                this->done2 = false;
            }
            else if(GoalPos[i][0]==9999991 || GoalPos[i][0]==9999990)  //***gpio
            {
                int stime=GoalVelo[i][0];
                if(GoalPos[i][0]==9999991)
                {
                    this->move = true;
                    gpio_on(GoalPos[step][1]);
                    cout << "GPIO on" << endl;
                    sleep(stime);
                    this->move = false;
                }
                if(GoalPos[i][0]==9999990)
                {
                    this->move = true;
                    gpio_off(GoalPos[step][1]);
                    cout << "GPIO off" << endl;
                    sleep(stime);
                    this->move = false;
                }
            }//***
            else if(GoalPos[i][0] == 999997)
            {
                cout << "waiting for scan" << endl;
                this->move = false;
                this->scan_ready = true;

                this->Scandone_check = false;

                while(1)
                {
                    if(this->Scandone_check == true)
                    {
                        //this->scan_ready = false;
                        break;
                    }
                }
            }
            else
            {
                int motion_velo[4]={};
                int motion_posi[4]={};
                double readtime = 0;
                QTime current_time=QTime::currentTime();
                int hour=current_time.hour();
                int minute=current_time.minute();
                int second=current_time.second();
                if(hour<10)
                    fstc<<"0"<<hour<<":";
                else
                    fstc<<hour<<":";
                if(minute<10)
                    fstc<<"0"<<minute<<":";
                else
                    fstc<<minute<<":";
                if(second<10)
                    fstc<<"0"<<second<<"\t";
                else
                    fstc<<second<<"\t";
                fstc << this->allcount2+1<<"\t"<<this->group+1 << "\t" << i+1 <<"\t";
                //block the angle limit
                /*
                for (int k=0;k<NUM;k++)
                {
                    if (abs(GoalPos[i][k])>angle_limit[k])
                    {
                        cout << "####################################out of range----------------------------------------------" << endl;
                        cout <<i<<"\t"<< GoalPos[i][k] << "\t" << angle_limit[k] <<"\t\n";
                        return 0;
                    }

                }

                cout << "check limit finish" << endl;
                */

                int vel_result;
                int pos_result;

                this->move = true;  //***


                unsigned char param_vel[20];
                for(int j=0;j<4;j++)
                {
                    gettimeofday(&start, NULL);
                    motion_velo[j] = GoalVelo[i][j];
                }
                for(int k=0;k<20;)
                {
                     param_vel[k]=k%4+1;
                    // cout<<"param_vel["<<k<<"]"<< param_vel[k]<<endl;
                     param_vel[k+1]=DXL_LOBYTE(DXL_LOWORD(motion_velo[k%4]));
                     param_vel[k+2]=DXL_HIBYTE(DXL_LOWORD(motion_velo[k%4]));
                     param_vel[k+3]=DXL_LOBYTE(DXL_HIWORD(motion_velo[k%4]));
                     param_vel[k+4]=DXL_HIBYTE(DXL_HIWORD(motion_velo[k%4]));
                     k=k+5;
                }
                while(1)
                {
                    //vel_result = DXL.WriteDWord(j+1, 600, GoalVelo[i][j], &error);
                    vel_result = DXL.SyncWrite(600,4,param_vel,20);
                    cout<<"vel_result="<<vel_result<<endl;
                    usleep(delay_time);

                    gettimeofday(&end, NULL);
                    timecost = ((end.tv_sec - start.tv_sec)*1000.0+(end.tv_usec - start.tv_usec)/1000.0);


                    if(timecost >=5000 )
                    {
                        cout<<"timecost>=5000"<<endl;
                        return 0;
                    }

                    if(vel_result == 1 )
                    {
                        //fst << "==" << j << "===" << "\t" << GoalVelo[i][j]<< "\t";
                        break;
                    }
                    else
                    {
                        cout<<"4 is send wrong!!!!!!!!!!!!11"<<endl;
                    }
                }

                unsigned char param_pos[20];
                for(int j=0;j<4;j++)
                {
                    gettimeofday(&start, NULL);
                    motion_posi[j] = GoalPos[i][j];
                }
                for(int k=0;k<20;)
                {
                    param_pos[k]=k%4+1;
                    param_pos[k+1]=DXL_LOBYTE(DXL_LOWORD(motion_posi[k%4]));
                    param_pos[k+2]=DXL_HIBYTE(DXL_LOWORD(motion_posi[k%4]));
                    param_pos[k+3]=DXL_LOBYTE(DXL_HIWORD(motion_posi[k%4]));
                    param_pos[k+4]=DXL_HIBYTE(DXL_HIWORD(motion_posi[k%4]));
                    k=k+5;

                }
                while(1)
                {
                    //vel_result = DXL.WriteDWord(j+1, 600, GoalVelo[i][j], &error);
                    pos_result = DXL.SyncWrite(596,4,param_pos,20);
                    cout<<"pos_result="<<pos_result<<endl;
                    //usleep(delay_time);

                    gettimeofday(&end, NULL);
                    timecost = ((end.tv_sec - start.tv_sec)*1000.0+(end.tv_usec - start.tv_usec)/1000.0);


                    if(timecost >=5000 )
                    {
                        return 0;
                        cout<<"in syncwrite write pos is time out!!!"<<endl;
                    }

                    if(pos_result == 1)
                    {
                        //fst << "==" << j << "===" << "\t" << GoalVelo[i][j]<< "\t";
                        cout<<"write vel is successful"<<endl;
                        break;
                    }
                    else
                    {
                        cout<<"4 is send wrong!!!!!!!!!!!!11"<<endl;
                    }
                }

                //usleep(1000);
                cout<<"before into motion!!!!!!!!!"<<endl;

                motion(readtime,i,motion_velo,motion_posi);
                int readpos_reasult_2=0;
                int motion_reality_pos[6]={0};
                float base2=0;

               /* for(int j=0;j<NUM;j++)
                {
                    while(joint_use[j]==1)
                        fstc<<motion_posi[j]<<"\t";
                }*/
                for(int j=0;j<NUM;j++)
                {
                    while(joint_use[j]==1)
                    {
                        long readpos_lvalue_2=0;
                        readpos_reasult_2=DXL.ReadDWord(j+1,611,&readpos_lvalue_2,&error);
                        if((readpos_reasult_2 == COMM_RXSUCCESS) )
                        {
                            motion_reality_pos[j]=readpos_lvalue_2;
                            fstc << readpos_lvalue_2<<"\t";
                            break;
                        }
                        else
                        {
                            cout<<j<<"\t"<<"read now position fail!!!!!!!!!!\n";
                            cout<<readpos_reasult_2<<endl;
                        }
                    }
                }
                for(int j=0;j<NUM;j++)
                {
                    if(joint_use[j]==1)
                    {
                        base2=(motion_posi[j]-motion_reality_pos[j])*180/pos_range[j];
                        fstc << fabs(base2) <<"\t";
                        emit SendError(j,base2);
                    }
                }
                fstc<<"\n";
                fstc.close();

                this->move = false;
            }
            cout << "step span:" << step_span << "\n";
            usleep(step_span);

        }
        tm += 1;
        cout << "====================================================================================\n";
    }
    fst.close();

    return 1;

}

int MyThread2::syncw(int retimes,char filename[80])
{
    int id[4]={1,2,3,4};
    int GoalPos[100][4]={};
    int GoalVelo[100][4]={};
    char Goal_limit[100][10]={};
    char Goal_coord[100][10]={};

    //char Goal_aVector[100][10]={};
    char Goal_oVector[100][10]={};

    int angle_limit[4] = {};

    for(int i=0; i<4; i++)
    {
        angle_limit[i] = posrange[i];
    }

    int result;
    int i,j;
    int dlength = 8;
    char line[100];
    char * pch;
    int step=0;
    int m,n;
    string str;
    fstream fst;
    int data=0;

    unsigned char param[100][54]={};
    unsigned char paramdata[54]={};

    //read file start
    fst.open(filename, ios::in);

    const char *pumpon="pumpon";
    char *pumpoff="pumpoff";
    char *gpioon="gpioon";
    char *gpiooff="gpiooff";
    char *together="together";
    char *leftabsorb="leftabsorb";
    char *rightdeflate="rightdeflate";

    cout << "SYNCW========================\n";
    while(fst.getline(line, sizeof(line), '\n'))
    {
        m=0;
        n=0;
        cout << line << endl;
        cout << sizeof(line) <<"\n\n"<< endl;

        if (line[sizeof(line)] == '\0')
        {
            pch=strtok(line,"\t");
            int count = 0;
            while(pch != NULL)
            {
                if(strcmp(pch, pumpon)==0 || strcmp(pch, pumpoff)==0)
                {
                    if(strcmp(pch, pumpon)==0)
                    {
                        //cout << "pumpon" <<endl;
                        GoalPos[step][m]=999991;
                        m+=1;
                    }
                    if(strcmp(pch, pumpoff)==0)
                    {
                        //cout << "pumpoff" <<endl;
                        GoalPos[step][m]=999990;
                        m+=1;
                    }
                }
                else if(strcmp(pch,leftabsorb)==0 || strcmp(pch,together)==0 || strcmp(pch,rightdeflate)==0)
                {
                    if(strcmp(pch, leftabsorb)==0)
                    {
                        GoalPos[step][m]=9999995;
                        m+=1;
                    }
                    if(strcmp(pch, together)==0)
                    {
                        GoalPos[step][m]=9999996;
                        m+=1;
                    }
                    if(strcmp(pch, rightdeflate)==0)
                    {
                        GoalPos[step][m]=9999997;
                        m+=1;
                    }

                    /*
                    switch(pch)
                    {
                        case leftabsorb:
                            GoalPos[step][m]=9999995;
                            m+=1;
                            break;
                        case leftdeflate:
                            GoalPos[step][m]=9999996;
                            m+=1;
                            break;
                        case rightabsorb:
                            GoalPos[step][m]=9999997;
                            m+=1;
                            break;
                        case rightdeflate:
                            GoalPos[step][m]=9999998;
                            m+=1;
                            break;
                    }
                    */
                }
                else if(strcmp(pch, gpioon)==0 || strcmp(pch, gpiooff)==0) //***gpio
                {
                    if(strcmp(pch, gpioon)==0)
                    {
                        GoalPos[step][m]=9999991;
                        m+=1;
                    }
                    if(strcmp(pch, gpiooff)==0)
                    {
                        GoalPos[step][m]=9999990;
                        m+=1;
                    }
                }
                else if(strcmp(pch, "ready") == 0)
                {                    
                    GoalPos[step][m]=9999992;
                    m+=1;
                }
                else if(strcmp(pch, "group") == 0)
                {
                    //cout << "group count:" <<group_count <<endl;
                    step = step-1;
                    break;
                }
                else
                {
                    data = atoi(pch);
                    cout << data << endl;

                    if(count == 8)
                    {
                        strcpy(Goal_limit[step], pch);
                    }
                    if(count == 9)
                    {
                        strcpy(Goal_coord[step], pch);
                    }
                    //if(count == 14)
                    //{
                    //    strcpy(Goal_aVector[step], pch);
                    //}
                    if(count == 10)
                    {
                        strcpy(Goal_oVector[step], pch);
                    }

                    if (count % 2 == 0)
                    {
                        GoalPos[step][m]=data;
                        m+=1;
                    }
                    else if (count % 2 == 1)
                    {
                        GoalVelo[step][n]=data;
                        n+=1;
                    }
                }

                pch = strtok(NULL, "\t");
                count += 1;
            }
        }

        if(step == 0)
        {
            posi_start_vel(GoalPos,GoalVelo);
        }

        for(int i=0;i<4;i++)
        {
            if(GoalVelo[step][i] > vellimit[i])
            {
                return 1; // vel limit
            }
        }
        cout << "check velocity limit finish" << endl;
        
        for (int k=0;k<NUM;k++)
        {
            if (abs(GoalPos[step][k]) > angle_limit[k])
            {
                cout << "####################################position out of range----------------------------------------------" << endl;
                cout <<i<<"\t"<< GoalPos[step][k] << "\t" << angle_limit[k] <<"\t\n";
                return 2;
            }
        }
        cout << "check position limit finish" << endl;

        step++;
    }
    fst.close();
//    cout << step << "\t" << m << "\t" << n << endl;

//    cout << "GoalPos array: " << endl;
//    for (i=0;i<step;i++)
//    {
//        for (j=0;j<m;j++)
//        {
//            cout << GoalPos[i][j] << "\t";
//        }
//        cout << endl;
//    }
//    cout << "GoalVelo array: " << endl;
//    for (i=0;i<step;i++)
//    {
//        for (j=0;j<n;j++)
//        {
//            cout << GoalVelo[i][j] << "\t";
//        }
//        cout << endl;
//    }
    //read file end

    fstream fst1;
    fst1.open("readpos.txt",ios::out);
    fst1.close();

    fstream fst2;
    fst2.open("readpos.txt",ios::out|ios::app);

    for (i=0;i<step;i++)
    {
        for (j=0;j<4;j++)
        {
            fst2 << GoalPos[i][j] << "\t" << GoalVelo[i][j] << "\t";

        }
        fst2 << Goal_limit[i] <<"\t"<< Goal_coord[i] <<"\t"<< Goal_oVector[i] <<"\t"<<"\n";
    }


    fst2.close();
    cout << "syncw end"<<endl;

    sync2write(dlength, step, GoalPos, GoalVelo,retimes);

    return 0;
}



//int group_sync2write(int length, int step, int retimes, int group)
//{
//    int id[6]={1,2,3,4,5,6};
//    //int angle_limit[6] = {70000,51930,128000,2048,1400,2048};
//    int angle_limit[6] = {103860,90340,140000,2048,1400,2048};
//    //int angle_limit[6] = {103860,103860,144000,2049,2048,2048};
//    int tm = 0;
//    int tims = retimes;
//    //int NUM = 6;
//    int result = 0;
//    unsigned char param[100][54]={};
//    //cout << "enter syncwrite" << endl;


//    //posi_start_vel(group);

//    for (int i=0;i<step;i++)
//    {
//        for (int j=0;j<NUM;j++)
//        {
//            //cout << group_GoalPos[group][i][j]  << "\t";
//            //cout << group_GoalVelo[group][i][j]  << "\t";
//            param[i][j*(1+8)+0] = (unsigned char)id[j];
//            param[i][j*(1+8)+1] = DXL_LOBYTE(DXL_LOWORD(group_GoalPos[group][i][j]));
//            param[i][j*(1+8)+2] = DXL_HIBYTE(DXL_LOWORD(group_GoalPos[group][i][j]));
//            param[i][j*(1+8)+3] = DXL_LOBYTE(DXL_HIWORD(group_GoalPos[group][i][j]));
//            param[i][j*(1+8)+4] = DXL_HIBYTE(DXL_HIWORD(group_GoalPos[group][i][j]));
//            param[i][j*(1+8)+5] = DXL_LOBYTE(DXL_LOWORD(group_GoalVelo[group][i][j]));
//            param[i][j*(1+8)+6] = DXL_HIBYTE(DXL_LOWORD(group_GoalVelo[group][i][j]));
//            param[i][j*(1+8)+7] = DXL_LOBYTE(DXL_HIWORD(group_GoalVelo[group][i][j]));
//            param[i][j*(1+8)+8] = DXL_HIBYTE(DXL_HIWORD(group_GoalVelo[group][i][j]));

//        }
//    }

//    //check the file exist of not
//    fstream fst;
//    fst.open("./pvread.txt");
//    if (fst.is_open() == true)
//    {
//        fst.close();
//        remove("pvread.txt");
//    }

//    //check file exist end

//    unsigned char paramdata[54]={};

//    while (tm != tims)
//    {

//        for (int i=0; i<step; i++)
//        {
//            if(group_GoalPos[group][i][0]==999991 || group_GoalPos[group][i][0]==999990)
//            {
//                //cout << "in" <<endl;
//                int stime=group_GoalVelo[group][i][0];
//                if(group_GoalPos[group][i][0]==999991)
//                {
//                    suck_on();
//                    //cout << "suck on" <<endl;
//                    sleep(stime);
//                    //cout << "sleep:"<< stime <<endl;
//                }
//                if(group_GoalPos[group][i][0]==999990)
//                {
//                    suck_off();
//                    //cout << "suck off" <<endl;
//                    sleep(stime);
//                    //cout << "sleep:"<< stime <<endl;
//                }
//            }
//            else
//            {
//                //break;
//                double readtime = 0;

//                //block the angle limit

//                for (int k=0;k<NUM;k++)
//                {
//                    if (abs(group_GoalPos[group][i][k])>angle_limit[k])
//                    {
//                        cout << "####################################out of range----------------------------------------------" << endl;
//                        cout << group_GoalPos[group][i][k] << "\t" << angle_limit[k] <<"\t\n";
//                        return 0;
//                    }
//                }


//                for(int j=0;j<(NUM*9);j++)
//                {
//                    paramdata[j]=param[i][j];
//                }

//                result = DXL.SyncWrite(596, length, paramdata, NUM*9);

//                usleep(1000000);

//                if( result != COMM_RXSUCCESS )
//                {
//                    PrintCommStatus(result);
//                }
//                motion(readtime,i);
//            }
//            sleep(step_span);

//        }
//        tm += 1;
//    }

//}


int MyThread2::group_sync2write(int length, int step, int retimes, int group)
{
    int time2=0;
    int id[4]={1,2,3,4};
    //int angle_limit[6] = {70000,51930,128000,2048,1400,2048};
    int angle_limit[4] = {};

    for(int i=0; i<4; i++)
    {
        angle_limit[i] = posrange[i];
    }
    //int angle_limit[6] = {103860,103860,144000,2049,2048,2048};
    int tm = 0;
    int tims = retimes;

    num=0;
    //emit sendnumber(num);

    //int NUM = 4;
    int result = 0;
    unsigned char param[100][54]={};
    int error = 0;

    //check the file exist of not

    fstream fsta;
    fsta.open("record_sync2write_posvel_group",ios::out|ios::app);


    fstream fstb;
    fstb.open("record_sync2write_group.txt",ios::out|ios::app);


//    fst.open("./pvread.txt");
//    if (fst.is_open() == true)
//    {
//        fst.close();
//        remove("pvread.txt");
//    }

    struct timeval start,end;
    double timecost;



    while (tm != tims && this->stop==false)
    {

        /*fstream fstr;
        fstr.open("record_time_4ax_without_connect.txt",ios::out|ios::app);

        QDateTime dt;
        QTime time;
        QTime cur_time=QTime::currentTime();
        QDate date;
        dt.setTime(time.currentTime());
        dt.setDate(date.currentDate());
        QString currentDate=dt.toString("yyyy:MM:dd:hh:mm:ss");
        fstr<<currentDate.toStdString().c_str()<<"\t";
        fstr<<(cur_time.hour()*3600+cur_time.minute()*60+cur_time.second()-time2)<<"\n";
        time2=cur_time.hour()*3600+cur_time.minute()*60+cur_time.second();
        fstr.close();*/

        num++;
        emit sendnumber(num);

        cout << "==================================="<< tm <<"=======================================\n";
        fsta << "==================================="<< tm <<"=======================================\n";
        fstream fst0;
        fst0.open("record_motion_group",ios::out);
        if(!fst0)
        {
            cout<<"open error"<<endl;
        }
        fst0.close();

        this->scan_ready = false;
        this->done2 = 0;
        this->gpio = false;
        cout<<"now step is "<<step<<endl;

        for (int i=0; i<step; i++)
        {
            fstream fstc;
            fstc.open("position_motion_4ax_group.txt",ios::out|ios::app);

            cout << "step:" <<i <<endl;
          //  emit test(QString::number(group_GoalPos[group][i][0]) + "\t" + QTime::currentTime().toString());

            if(group_GoalPos[group][i][0]==999991 || group_GoalPos[group][i][0]==999990)
            {
                cout << "in" <<endl;
                int stime=group_GoalVelo[group][i][0];
                if(group_GoalPos[group][i][0]==999991)
                {
                    cout << "sleep:"<< stime <<endl;
                    fsta << "valve on:"<< stime <<endl;

                    this->move = true;
                    suck_on();
                    cout << "valve on" <<endl;
                    sleep(stime);
                    this->move = false;
                }
                if(group_GoalPos[group][i][0]==999990)
                {
                    cout << "sleep:"<< stime <<endl;
                    fsta << "valve off:"<< stime <<endl;

                    this->move = true;
                    suck_off();
                    cout << "valve off" <<endl;
                    sleep(stime);
                    this->move = false;
                }
            }
            else if(group_GoalPos[group][i][0]==9999991 || group_GoalPos[group][i][0]==9999990)  //***gpio
            {
                int stime=group_GoalVelo[group][i][0];
                if(group_GoalPos[group][i][0]==9999991)
                {
                    cout << "sleep: "<<stime<<endl;

                    this->move = true;
                    gpio_on(group_GoalPos[group][i][1]);
                    cout << "GPIO on" << endl;
                    fsta << "GPIO on:"<< stime <<endl;
                    sleep(stime);
                    this->move = false;
                }
                if(group_GoalPos[group][i][0]==9999990)
                {
                    cout << "sleep: "<<stime<<endl;

                    this->move = true;
                    gpio_off(group_GoalPos[group][i][1]);
                    cout << "GPIO off" << endl;
                    fsta << "GPIO off: "<< stime <<endl;
                    sleep(stime);
                    this->move = false;
                }
            }
            else if(group_GoalPos[group][i][0]==9999995 || group_GoalPos[group][i][0]==9999996 || group_GoalPos[group][i][0]==9999997)   //~!@#$
            {
                this->gpio=true;

                switch(group_GoalPos[group][i][0])
                {
                    case 9999995:
                        cout << "********************left absorb\n";
                        //this->gpio=true;
                        while(this->stop==false)
                        {
                            wait(0);
                            if(this->done2 == 1)
                                break;
                            else if(this->done2 == 2)
                            {
                                this->stop=true;
                                i=step;
                            }
                        }
                        break;
                    case 9999996:
                        cout << "********************together work\n";
                        //this->gpio=true;
                        while(this->stop==false)
                        {
                            wait(0);
                            if(this->done2 == 1)
                                break;
                            else if(this->done2 == 2)
                            {
                                this->stop=true;
                                cout<<"in mThread2 together is warnning!!!!"<<endl;
                                i=step;
                            }
                        }
                        break;
                    case 9999997:
                        cout << "********************right deflate\n";
                        //this->gpio=true;
                        while(this->stop==false)
                        {
                            wait(0);
                            if(this->done2 == 1)
                                break;
                            else if(this->done2 == 2)
                            {
                                this->stop=true;
                                i= step;
                            }
                        }
                        break;
                }
                this->done2 = 0;
            }
            else if(group_GoalPos[group][i][0]==9999992 )
            {
                //cout << "waiting for scan!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1" << endl;
                //QMessageBox::information(NULL,"Error",QString("waiting for scan!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));

                this->move = false;
                this->scan_ready = true;

                this->Scandone_check = false;
                emit test("levy\t" + QTime::currentTime().toString());
                while(this->stop == false)
                {
                    //wait(0);
                    cout<<"check scan done!!!!";
                    if(this->Scandone_check == 1)
                    {
                        //this->scan_ready = false;
                        break;
                    }
                    else if(this->Scandone_check == 2)
                    {
                        this->stop=true;
                        i=step;
                        break;
                    }
                }
            }
            else
            {
                 emit test("into else\t"+ QTime::currentTime().toString());
                //break;
                int motion_velo[4]={};
                int motion_posi[4]={};
                double readtime = 0;
                QTime current_time=QTime::currentTime();
                int hour=current_time.hour();
                int minute=current_time.minute();
                int second=current_time.second();
                fstc<<hour<<":"<<minute<<":"<<second<<"\t";

                fstc << this->allcount2+1<<"\t"<<this->group+1 << "\t" << i+1 <<"\t";
                //fstc.close();

                //block the angle limit

                for (int k=0;k<NUM;k++)
                {
                    if (abs(group_GoalPos[group][i][k])>angle_limit[k])
                    {
                        cout << "####################################out of range----------------------------------------------" << endl;
                        cout << group_GoalPos[group][i][k] << "\t" << angle_limit[k] <<"\t\n";
                        return 0;
                    }
                }

                int vel_result=-1;
                int pos_result=-1;
                int pos_result_r=-1;

                //cout << "check limit finish\n";

                this->move = true;  //***

                fsta << "Vel:\n";
                for(int j=0;j<4;j++)
                {
                    gettimeofday(&start, NULL);
                    motion_velo[j]=group_GoalVelo[group][i][j];

                    fsta << "ID:" << j+1 << "\t" << group_GoalVelo[group][i][j] <<"\t";
                    while(this->stop == false)
                    {
                        cout<< "ID:" << j+1 << "\t" << group_GoalVelo[group][i][j] <<endl;
                        vel_result = DXL.WriteDWord(j+1, 600, group_GoalVelo[group][i][j], &error);
                        usleep(delay_time);
                        cout << "ok!\n";

                        gettimeofday(&end, NULL);
                        timecost = ((end.tv_sec - start.tv_sec)*1000.0+(end.tv_usec - start.tv_usec)/1000.0);

                        if(timecost >= 6000)
                        {
                            cout << "timeout\n";
                            fstb << "timeout:" << j+1 <<"\n";
                            return 0;
                        }

                        if(vel_result == 1 || joint_use[j] !=1)
                        {

                            break;
                        }
                        else
                        {
                            fstb<<"ID="<<j+1<<"vel_result="<<vel_result<<"////////////////////////////////////"<< timecost <<"////////////////////////////////"<<endl;
                            cout<<"ID="<<j+1<<"vel_result="<<vel_result<<"////////////////////////////////////"<< timecost <<"////////////////////////////////"<<endl;
                            //usleep(1000);
                        }
                    }
                    //usleep(5000);
                }
                //unsigned char param_vel[20];
                cout<<"Vel:"<<endl;
                for(int j=0;j<4;j++)
                {
                    //gettimeofday(&start, NULL);
                    motion_velo[j]=group_GoalVelo[group][i][j];
                    fsta << "ID:" << j+1 << "\t" << group_GoalVelo[group][i][j] <<"\t";
                    cout<<"ID:" << j+1 << "\t" << group_GoalVelo[group][i][j] <<"\t";
                }
                cout<<endl;
                /*for(int k=0;k<20;)
                {
                     param_vel[k]=k%4+1;
                     //cout<<"param_vel["<<k<<"]"<< param_vel[k]<<endl;
                     param_vel[k+1]=DXL_LOBYTE(DXL_LOWORD(motion_velo[k%4]));
                     param_vel[k+2]=DXL_HIBYTE(DXL_LOWORD(motion_velo[k%4]));
                     param_vel[k+3]=DXL_LOBYTE(DXL_HIWORD(motion_velo[k%4]));
                     param_vel[k+4]=DXL_HIBYTE(DXL_HIWORD(motion_velo[k%4]));
                     k=k+5;
                }

                //test_xxh
                while(this->stop == false)
                {
                    //vel_result = DXL.WriteDWord(j+1, 600, GoalVelo[i][j], &error);
                    vel_result = DXL.SyncWrite(600,4,param_vel,20);
                    usleep(delay_time);

                    gettimeofday(&end, NULL);
                    timecost = ((end.tv_sec - start.tv_sec)*1000.0+(end.tv_usec - start.tv_usec)/1000.0);


                    if(timecost >=6000 )
                    {
                        return 0;
                    }

                    if(vel_result == 1 )
                    {
                        //fst << "==" << j << "===" << "\t" << GoalVelo[i][j]<< "\t";
                        break;
                    }
                    else
                    {
                        cout<<"4 is send wrong!!!!!!!!!!!!11"<<endl;
                    }
                }*/
                fsta << "\n";
                fsta << "POS:\n";
                for(int j=0;j<4;j++)
                {
                    long now_pos=0;
                    motion_posi[j]=group_GoalPos[group][i][j];
                    fsta << "ID:" << j+1 << "\t" << group_GoalPos[group][i][j] <<"\t";
                    while(this->stop == false)
                    {
                        cout<< "ID:" << j+1 << "\t" << group_GoalPos[group][i][j] <<endl;
                        pos_result_r=DXL.ReadDWord(j+1,611,&now_pos,&error);
                        if(pos_result_r ==1)
                        {
                            double now_J=0;
                            double goal_J=0;
                            now_J=now_pos*180/pos_range[j];
                            goal_J=group_GoalPos[group][i][j]*180/pos_range[j];
                            if(fabs(now_J-goal_J)>0.08)
                            {
                                pos_result = DXL.WriteDWord(j+1, 596, group_GoalPos[group][i][j], &error);
                                usleep(delay_time);
                                cout << "ok!\n";
                                if(pos_result ==1 || joint_use[j] !=1)
                                {
                                    break;
                                }
                                else
                                {
                                    fstb <<"ID="<<j+1<<"vel_result="<<vel_result <<"pos_result="<<pos_result<<"******************************************"<<endl;
                                    cout<<"ID="<<j+1<<"vel_result="<<vel_result <<"pos_result="<<pos_result<<"******************************************"<<endl;
                                    //usleep(1000);
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                        else
                        {
                            cout<<"read j now pos is wrong!!!!!"<<endl;
                        }
                    }

                    //usleep(1000);
                }
                unsigned char param_pos[20];
                cout<<"Pos:"<<endl;
                for(int j=0;j<4;j++)
                {
                    //gettimeofday(&start, NULL);
                    motion_posi[j]=group_GoalPos[group][i][j];
                    fstc<<motion_posi[j]<<"\t";
                    fsta << "ID:" << j+1 << "\t" << group_GoalPos[group][i][j] <<"\t";
                    cout<<"ID:" << j+1 << "\t" << group_GoalPos[group][i][j] <<"\t";
                }
                cout<<endl;
                /*for(int k=0;k<20;)
                {
                    param_pos[k]=k%4+1;
                    param_pos[k+1]=DXL_LOBYTE(DXL_LOWORD(motion_posi[k%4]));
                    param_pos[k+2]=DXL_HIBYTE(DXL_LOWORD(motion_posi[k%4]));
                    param_pos[k+3]=DXL_LOBYTE(DXL_HIWORD(motion_posi[k%4]));
                    param_pos[k+4]=DXL_HIBYTE(DXL_HIWORD(motion_posi[k%4]));
                    k=k+5;

                }

                //test_xxh
                while(this->stop == false)
                {
                    //vel_result = DXL.WriteDWord(j+1, 600, GoalVelo[i][j], &error);
                    pos_result = DXL.SyncWrite(596,4,param_pos,20);
                    usleep(delay_time);

                    gettimeofday(&end, NULL);
                    timecost = ((end.tv_sec - start.tv_sec)*1000.0+(end.tv_usec - start.tv_usec)/1000.0);

                    if(pos_result == 1 )
                    {
                        //fst << "==" << j << "===" << "\t" << GoalVelo[i][j]<< "\t";
                        break;
                    }
                    else
                    {
                        cout<<"4 is send wrong!!!!!!!!!!!!11"<<endl;
                    }
                }*/

                fsta << "\n";
                //usleep(1000);
                cout << "motion" <<endl;
                motion(readtime,i,motion_velo,motion_posi);



                //QMessageBox::information(NULL,"Error2",QString("waiting for motion finish"));

                int readpos_reasult_2=0;
                int motion_reality_pos[6]={0};
                float base2=0;

                for(int j=0;j<NUM;j++)
                {
                    gettimeofday(&start, NULL);
                    while(joint_use[j]==1)
                    {
                        long readpos_lvalue_2=0;
                        readpos_reasult_2=DXL.ReadDWord(j+1,611,&readpos_lvalue_2,&error);
                        if((readpos_reasult_2 == COMM_RXSUCCESS) )
                        {
                            motion_reality_pos[j]=readpos_lvalue_2;
                            fstc << readpos_lvalue_2<<"\t";
                            break;
                        }
                        else
                        {
                            cout<<j<<"\t"<<"read now position fail!!!!!!!!!!\n";
                            cout<<readpos_reasult_2<<endl;
                        }
                        gettimeofday(&end, NULL);
                        timecost = ((end.tv_sec - start.tv_sec)*1000.0+(end.tv_usec - start.tv_usec)/1000.0);

                        if(timecost >= 6000)
                        {
                            cout << "timeout!!!!!!!!!!!!!!!!!!!\n";
                            break;
                        }
                    }
                }
                for(int j=0;j<NUM;j++)
                {
                    if(joint_use[j]==1)
                    {
                        base2=(motion_posi[j]-motion_reality_pos[j])*180/pos_range[j];
                        fstc << fabs(base2) <<"\t";
                        if(fabs(base2)>0.1)
                            cout<<j<<"\t"<<"error is "<<base2<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n\n";
                        emit SendError(j,base2);
                    }
                }
                fstc<<"\n";
                fstc.close();

               // QMessageBox::information(NULL,"Error3",QString("waiting for compare finish"));

                //cout << "motion finish" <<endl;
                //usleep(10000);
                cout<<"finish"<<endl<<"finish"<<endl<<"finish"<<endl<<"finish"<<endl<<"finish"<<endl<<"finish"<<endl<<"finish"<<endl<<"finish"<<endl<<"finish"<<endl<<"finish"<<endl<<"finish"<<endl;


                //cout << "step span:" << step_span << "\n";
                usleep(step_span);
                emit test("all is finish\t"+ QTime::currentTime().toString());

                this->move = false; //***
            }
        }


        //Read Temperature

        if(tm == (tims-1))
        {
            for(int i=0; i<4; i++)
            {
                int temp = -1;
                int readtemp_result = -1;


                if(joint_use[i] == 1)
                {
                    readtemp_result = DXL.ReadByte(i+1, P_PRESENT_TEMPERATURE, &temp, &error);
                    //cout<<"readtemp_result="<<readtemp_result<<"@@@@@@@@@@@@@@@@@@@@"<<endl;
                    if(readtemp_result != 1)
                    {
                        //cout << "Temperature Read Fail: J" << i+1 << endl;
                        emit NumberChanged2(9);
                    }
                    else
                    {
                        //cout << "Temprature J" << i+1 << ": " << temp << endl;
                        emit SendTemp(i, temp);
                    }
                    usleep(delay_time);
                }
            }
        }

        tm += 1;
    }

    fstb.close();
    fsta.close();

    return 1;

}

int posi_start_vel_group(int group)
{
    /*here
    #define L54S290Rv 144180.0
    #define L54S5000Rv 103860.0
    #define L42v 2048.0
    #define J1GRR 400
    #define J23GRR 290
    #define J456UNIT 0.114
    */
    double now_theta[4]={0};
    //int first_thetavalue[6]={};
    double first_theta[4]={};
    int error=0;

    struct timeval start,end;
    double timecost;

//    struct timeval start,end;
//    double timecost;
//    gettimeofday(&end, NULL);
//    timecost = ((end.tv_sec - start.tv_sec)*1000.0+(end.tv_usec - start.tv_usec)/1000.0);

    //cout << "input omega_start_vel => Go to the start position" << endl;
    //read thetavalue now and change to theta
    //cout << "thetanow: "<<endl;

    fstream fst1;
    fst1.open("readpos_record_group.txt",ios::out);
    fst1.close();

    fstream fst;
    fst.open("readpos_record_group.txt",ios::out| ios::app);

    //cout << "Read posi......"<<endl;
    for (int i=0;i<NUM;i++)
    {
        if(joint_use[i] == 1)
        {
            long posivalue=0;
            int result = -1;

            gettimeofday(&start, NULL);

            while(1)
            {
                result = DXL.ReadDWord(i+1, 611, &posivalue, &error);  //~~~~

                gettimeofday(&end, NULL);
                timecost = ((end.tv_sec - start.tv_sec)*1000.0+(end.tv_usec - start.tv_usec)/1000.0);
                //cout << result << "\n";

                if(timecost >=5000 )
                {
                    cout <<"Read posi timeout"<<endl;
                    break;
                }

                if(result == 1)
                {

                    now_theta[i]=posivalue*180/posrange[i];
                    fst << i << "\tok\n";
                    break;
                }
                else
                {
                    fst << i << "\tfail\n";
                }
                //sleep(1);
            }
        }
    }
    cout << endl;
    //default: set 5 sec to the start point
    //cout << "first_theta[i] inside start:" << endl;

    //double defaulttime = 1;
    float vmax=0;
    float diff=0.0;

    for (int i=0;i<NUM;i++)
    {
        //cout << "old:" << group_GoalVelo[group][0][i] <<endl;

        first_theta[i] = group_GoalPos[group][0][i]*180/posrange[i];

//        if (i==0)
//            first_theta[i] = group_GoalPos[group][0][i]*180/L54S290Rv;
//        else if (i==1 || i==2)
//            first_theta[i] = group_GoalPos[group][0][i]*180/L54S5000Rv;
//        else
//            first_theta[i] = group_GoalPos[group][0][i]*180/L42v;

        if(fabs(first_theta[i]-now_theta[i])>vmax)
        {
            vmax = fabs(first_theta[i]-now_theta[i]);
            //cout << "vmax:" <<vmax <<endl;
        }

        //double now_omega = (fabs(first_theta[i]-now_theta[i])*60.0)/(defaulttime*360.0);
    }

    //float omega_limit=20; //rpm

    cout << "ddd:" << (vmax*60)/(360*move_time) <<endl;
    if (((vmax*60)/(360*move_time)) > omega_limit)
    {
        move_time=(vmax*60)/(360*omega_limit);
        cout << "   exceed limit w, new defaulttime= " << move_time << endl;
    }

    for (int i=0;i<NUM;i++)
    {
        double now_omega = (fabs(first_theta[i]-now_theta[i])*60.0)/(move_time*360.0);
       // cout << "omega:" <<now_omega<<endl;
       // ::group_GoalVelo[group][0][i] = now_omega*gearratio[i];

        if(gearratio[i] > 1)
        {
            ::group_GoalVelo[group][0][i] = now_omega*gearratio[i];
        }
        else
        {
            ::group_GoalVelo[group][0][i] = now_omega/gearratio[i];
        }
//        if (i==2)
//            ::group_GoalVelo[group][0][i] = now_omega*J1GRR;
//        else if (i==0 || i==1)
//            ::group_GoalVelo[group][0][i] = now_omega*J23GRR;
//        else
//            ::group_GoalVelo[group][0][i] = now_omega/J456UNIT;

       // cout << "new:" << group_GoalVelo[group][0][i] <<endl;

    }


    cout << endl;
    fst.close();
}

int read_group(char filename[80])
{
    //cout << "in read_group" <<endl;
    ::group_count = -1;

    char line[1024];
    char * pch;
    int m,n;
    int data=0;

    memset(group_GoalPos, 0, sizeof(group_GoalPos));
    memset(group_GoalVelo, 0, sizeof(group_GoalVelo));
    memset(group_step, 0, sizeof(group_step));

    fstream fst;
    fst.open(filename, ios::in);

    while(fst.getline(line, sizeof(line), '\n'))
    {
        int step=0;
        m=0;
        n=0;
        cout << line << endl;
        cout << sizeof(line) << endl;
        if (line[strlen(line)] == '\0')
        {
            pch=strtok(line,"\t");
            int count = 0;
            while(pch != NULL)
            {
                if(strcmp(pch, "group") == 0)
                {
                    cout << "group count:" <<group_count <<endl;
                    ::group_count+=1;
                    ::group_step[group_count] = -1;
                    //step=group_step[group_count];
                    //cout << "!!!!!!!!!!!!!!!1step:" <<step <<endl;
                    break;
                }
                else if(strcmp(pch, "pumpon")==0)
                {
                    //cout << "pumpon" <<endl;
                    ::group_GoalPos[group_count][group_step[group_count]][m]=999991;
                    m+=1;
                }
                else if(strcmp(pch, "pumpoff")==0)
                {
                    //cout << "pumpoff" <<endl;
                    ::group_GoalPos[group_count][group_step[group_count]][m]=999990;
                    m+=1;
                }
                else if(strcmp(pch, "gpioon")==0)
                {
                    //cout << "gpioon" <<endl;
                    ::group_GoalPos[group_count][group_step[group_count]][m]=9999991;
                    m+=1;
                }
                else if(strcmp(pch, "gpiooff")==0)
                {
                    //cout << "gpiooff" <<endl;
                    ::group_GoalPos[group_count][group_step[group_count]][m]=9999990;
                    m+=1;
                }
                else if(strcmp(pch, "ready")==0)
                {                   
                    ::group_GoalPos[group_count][group_step[group_count]][m]=9999992;
                    //QMessageBox::information(NULL,"9999992",QString("waiting for scan!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
                    m+=1;
                }
                else if(strcmp(pch,"leftabsorb")==0)
                {
                    ::group_GoalPos[group_count][group_step[group_count]][m]=9999995;//~!@#$
                    m+=1;
                }
                else if(strcmp(pch,"together")==0)
                {
                    ::group_GoalPos[group_count][group_step[group_count]][m]=9999996;
                    m+=1;
                }
                else if(strcmp(pch,"rightdeflate")==0)
                {
                    ::group_GoalPos[group_count][group_step[group_count]][m]=9999997;
                    m+=1;
                }
                else
                {
                    data = atoi(pch);
                    //cout << data << endl;
                    if (count % 2 == 0)
                    {
                        //cout << "group0:"<<group_count<<"\tstep:"<<group_step[group_count]<<"\tm:"<<m;
                        ::group_GoalPos[group_count][group_step[group_count]][m]=data;
                        //cout << "pos:" << group_GoalPos[group_count][group_step[group_count]][m] <<endl;
                        m+=1;
                    }
                    else if (count % 2 == 1)
                    {
                        //cout << "group1:"<<group_count<<"\tstep:"<<group_step[group_count]<<"\tn:"<<n <<endl;
                        ::group_GoalVelo[group_count][group_step[group_count]][n]=data;
                        //cout << "vel:" << group_GoalVelo[group_count][group_step[group_count]][n] <<endl;
                        n+=1;
                    }

                }
                pch = strtok(NULL, "\t");
                count += 1;
            }
        }
        cout<<"wanghaoshizhu !!!!!!!!!!!!!!!!!\n\n";
        ::group_step[group_count]+=1;
        //step=group_step[group_count];
        //cout << "!!!!!!!!!!!!!!!1step:" <<step <<endl;
        //step += 1;
    }
    fst.close();

    cout << "POS:\n";
    for(int i=0; i<=group_count; i++)
    {
        cout << "i:" <<i<<endl;
        for(int j=0; j<group_step[group_count]; j++)
        {
            cout << "j:" <<j<<endl;
            for(int k=0; k<4; k++)
            {
               cout << group_GoalPos[i][j][k] <<endl;
            }
        }
    }

    cout << "vel:\n";
    for(int i=0; i<=group_count; i++)
    {
        cout << "i:" <<i<<endl;
        for(int j=0; j<group_step[group_count]; j++)
        {
            cout << "j:" <<j<<endl;
            for(int k=0; k<4; k++)
            {
               cout << group_GoalVelo[i][j][k] <<endl;
            }
        }
    }

    return 0;
}



//void suck()
//{
//    wiringPiSetup () ;
//    pinMode (relayport, OUTPUT) ;

//    char ch = '*';
//    clamping_relay(ch);
//}

float single_step_velchange(int Buff_Pos_pre[4],int Buff_Pos_now[4], float Buff_movetime, int speed_value[4])
{
    float vmax=0.0;
    float diff_value[4]={};

    for (int k=0;k<NUM;k++)
    {
        cout << "K:" << k << "\t" << Buff_Pos_now[k] << "\t" << Buff_Pos_pre[k] << "\n";
        diff_value[k]=(fabs(Buff_Pos_now[k]-Buff_Pos_pre[k]))*180.0/posrange[k];

        if (diff_value[k]>vmax)
        {
            vmax = diff_value[k];
        }
    }

    float rpm = ((vmax*60)/(360*Buff_movetime));

    cout << "vmax= " << vmax << "movetime[i][j]:" << Buff_movetime <<endl;
    cout << "vmax= " << rpm << " rpm"<< endl;

    if (rpm > omega_limit)
    {
        Buff_movetime = (vmax*60) / (360*omega_limit);
        cout << "omega_limit:" <<omega_limit <<endl;
        cout << "   exceed ljmjt w, new movetjme= " << Buff_movetime << endl;
    }

    for (int k=0;k<NUM;k++)
    {
        if(gearratio[k] > 1)
        {

            speed_value[k]=(diff_value[k]*60/(Buff_movetime*360))*gearratio[k];
            cout << "1\t" << speed_value[k] << "\t" << (diff_value[k]*60/(Buff_movetime*360)) << "\t" << gearratio[k] <<endl;
        }
        else
        {
            speed_value[k]=(diff_value[k]*60/(Buff_movetime*360))/gearratio[k];
            cout << "2\t" << speed_value[k] << "\t" << (diff_value[k]*60/(Buff_movetime*360)) << "\t" << gearratio[k] <<endl;
        }
    }

    return Buff_movetime;

    //---------------------------------------------------------------------------------------------
}

int change_group_index(int Buff_Pos[4], int group, int step_now, int step_pre, float movetime, int isnew)
{
    cout << "Step:" << step_now << "\n";


    //cout << "now    POS:" << j << "\t" << group_GoalPos[group][step_now][j] << "\n";

    if(isnew ==1)
    {
        for(int j=0;j<4;j++)
        {
            group_GoalPos[group][step_now][j] = Buff_Pos[j];
            cout << "change POS:" << j << "\t" << group_GoalPos[group][step_now][j]  << "\n";
        }
    }

    //cout << "now    POS:" << j << "\t" << group_GoalPos[group][step_now][j] << "\n";


    float new_movetime;

    for(int j=0;j<4;j++)
    {
        cout << "now    Vel:" << j << "\t" << group_GoalVelo[group][step_now][j] << "\n";
        group_GoalVelo[group][step_now][j] = 0;
          //cout << "change POS:" << j << "\t" << group_GoalPos[group][step][j]  << "\n";
    }

    if (step_now == 0)
    {
        ;
    }
    else
    {
        new_movetime = single_step_velchange(group_GoalPos[group][step_pre],group_GoalPos[group][step_now],movetime,group_GoalVelo[group][step_now]);
    }

//    for(int j=0;j<4;j++)
//    {
//        cout << "change Vel:" << j << "\t" << group_GoalVelo[group][step_now][j]  << "\n";
//    }


}

int group_velchange_test(char filename[80])
{

    cout << "omega_limit:" << omega_limit <<endl;
    cout << "scale:" << scale <<endl;

    #define oi_movetime 0.05
    //int id[6]={1,2,3,4,5,6};
    int BuffPos[50][100][4]={};
    int BuffVelo[50][100][4]={};
    int BuffPump[50][100][100]={0};
    //float BuffTheta[100][6]={};

    //char mt_temp[50][100]={};
    char coord_value[50][100][100]={};
    //int result;
    //int dlength = 8;
    char line[100];
    char * pch;
    //int buffstep=0;
    int bm,bn;
    //string str;
    fstream fst;
    int data=0;

    int buff_group_count =-1;
    int buff_group_step[100]={};

    float movetime[50][100] = {};

    fst.open(filename, ios::in);

    char *pumpon="pumpon";
    char *pumpoff="pumpoff";

    while(fst.getline(line, sizeof(line), '\n'))
    {
        bm=0;
        bn=0;
        cout << "line:+" <<line <<"+"<< endl;
        if(line == NULL || line =="")
        {
            break;
        }
        cout << "size11:" <<sizeof(line) << endl;

        if (line[sizeof(line)] == '\0')
            pch=strtok(line,"\t");
        int count = 0;

        while(pch != NULL)
        {
            if(strcmp(pch, "group") == 0)
            {
                //cout << "group count:" <<buff_group_count <<endl;
                buff_group_count+=1;
                buff_group_step[buff_group_count] = -1;
                //step=group_step[group_count];
                //cout << "!!!!!!!!!!!!!!!1step:" <<step <<endl;
                break;
            }
            if (strcmp(pch, pumpon)==0|| strcmp(pch, pumpoff)==0)
            {
                if(strcmp(pch, pumpon)==0)
                {
                    //cout << "pumpon" <<endl;
                    buff_group_step[buff_group_count] =buff_group_step[buff_group_count] -1; //and this one

                    cout << "pumpon:" << buff_group_step[buff_group_count] <<endl;

                    int m=0;
                    while(1)
                    {
                        if(BuffPump[buff_group_count][buff_group_step[buff_group_count]][m] == 0)
                        {
                            BuffPump[buff_group_count][buff_group_step[buff_group_count]][m]=999991;
                            break;
                        }
                        m+=1;
                    }


                    break; //this one
                }
                else if(strcmp(pch, pumpoff)==0)
                {
                    //cout << "pumpoff" <<endl;
                    buff_group_step[buff_group_count] =buff_group_step[buff_group_count] -1;
                    cout << "pumpoff:" << buff_group_step[buff_group_count] <<endl;

                    int m=0;
                    while(1)
                    {
                        if(BuffPump[buff_group_count][buff_group_step[buff_group_count]][m] == 0)
                        {
                            BuffPump[buff_group_count][buff_group_step[buff_group_count]][m]=999990;
                            break;
                        }
                        m+=1;
                    }

                    break;
                }

            }
            else
            {
                data = atoi(pch);
                //cout << data << endl;
                if(count == 8)
                {
                    float float_data = atof(pch);
                    //cout << "there is movetime:" <<float_data << endl;
                    movetime[buff_group_count][buff_group_step[buff_group_count]] = float_data*scale;
                    //break;
                }
                else if(count == 9)
                {
                    strcpy(coord_value[buff_group_count][buff_group_step[buff_group_count]],pch);
                    break;
                }
                else
                {
                    if (count % 2 == 0)
                    {
                        BuffPos[buff_group_count][buff_group_step[buff_group_count]][bm]=data;
                        bm+=1;

                    }
                    else if (count % 2 == 1)
                    {
                        BuffVelo[buff_group_count][buff_group_step[buff_group_count]][bn]=data;
                        bn+=1;
                    }
                }
            }
            pch = strtok(NULL, "\t");
            count +=1;
        }

       // buffstep += 1;
        buff_group_step[buff_group_count]+=1;
        cout << "step!!!!!!!:" <<buff_group_step[buff_group_count]<<endl;


    }
    fst.close();

    //velchange
    fst.open("./readpos4.txt", ios::out);

    for(int i=0;i<= buff_group_count;i++)
    {
        fst << "group\t\n";

        for (int j=0;j<buff_group_step[i];j++)
        {
            int speed_value[4]={};
            float new_movetime;

            if (j==0)
            {
                new_movetime = single_step_velchange(BuffPos[i][j],BuffPos[i][buff_group_step[i]-1],movetime[i][j],speed_value);
            }
            else
            {
                new_movetime = single_step_velchange(BuffPos[i][j],BuffPos[i][j-1],movetime[i][j],speed_value);
            }

            for (int k=0;k<NUM;k++)
            {
                fst << BuffPos[i][j][k] << "\t" << int(speed_value[k]) << "\t" ;
            }
            fst << new_movetime << "\t" << coord_value[i][j] << "\t";
            fst << "\n";

            int m=0;
            while(1)
            {
                if(BuffPump[i][j][m] == 0)
                {
                    break;
                }
                else if (BuffPump[i][j][m] == 999991)
                {
                    fst << "pumpon" << "\t" << 1 << "\t";
                    for (int k=2;k<NUM*2;k++)
                    {
                        fst << 0 << "\t";
                    }
                    fst << "\n";
                }
                else if (BuffPump[i][j][m] == 999990)
                {
                    fst << "pumpoff" << "\t" << 1 << "\t";
                    for (int k=2;k<NUM*2;k++)
                    {
                        fst << 0 << "\t";
                    }
                    fst << "\n";
                }
                m+=1;

            }
        }
    }

    return 0;
}

int group_velchange(char filename[80])
{

    cout << "omega_limit:" << omega_limit <<endl;
    cout << "scale:" << scale <<endl;

    #define oi_movetime 0.05
    int id[4]={1,2,3,4};
    int BuffPos[50][100][4]={};
    int BuffVelo[50][100][4]={};
    int BuffPump[50][100][100]={0};
    float BuffTheta[100][4]={};

    char mt_temp[50][100]={};
    char coord_value[50][100][100]={};
    int result;
    int dlength = 8;
    char line[100];
    char * pch;
    int buffstep=0;
    int bm,bn;
    string str;
    fstream fst;
    int data=0;

    int buff_group_count =-1;
    int buff_group_step[100]={};

    float movetime[50][100] = {};

    fst.open(filename, ios::in);

    char *pumpon="pumpon";
    char *pumpoff="pumpoff";

    while(fst.getline(line, sizeof(line), '\n'))
    {
        bm=0;
        bn=0;
        cout << "line:+" <<line <<"+"<< endl;
        if(line == NULL || line =="")
        {
            break;
        }
        cout << "size11:" <<sizeof(line) << endl;

        if (line[sizeof(line)] == '\0')
            pch=strtok(line,"\t");
        int count = 0;

        while(pch != NULL)
        {
            if(strcmp(pch, "group") == 0)
            {
                //cout << "group count:" <<buff_group_count <<endl;
                buff_group_count+=1;
                buff_group_step[buff_group_count] = -1;
                //step=group_step[group_count];
                //cout << "!!!!!!!!!!!!!!!1step:" <<step <<endl;
                break;
            }
            if (strcmp(pch, pumpon)==0|| strcmp(pch, pumpoff)==0)
            {
                if(strcmp(pch, pumpon)==0)
                {
                    //cout << "pumpon" <<endl;
                    buff_group_step[buff_group_count] =buff_group_step[buff_group_count] -1; //and this one

                    cout << "pumpon:" << buff_group_step[buff_group_count] <<endl;

                    int m=0;
                    while(1)
                    {
                        if(BuffPump[buff_group_count][buff_group_step[buff_group_count]][m] == 0)
                        {
                            BuffPump[buff_group_count][buff_group_step[buff_group_count]][m]=999991;
                            break;
                        }
                        m+=1;
                    }


                    break; //this one
                }
                else if(strcmp(pch, pumpoff)==0)
                {
                    //cout << "pumpoff" <<endl;
                    buff_group_step[buff_group_count] =buff_group_step[buff_group_count] -1;
                    cout << "pumpoff:" << buff_group_step[buff_group_count] <<endl;

                    int m=0;
                    while(1)
                    {
                        if(BuffPump[buff_group_count][buff_group_step[buff_group_count]][m] == 0)
                        {
                            BuffPump[buff_group_count][buff_group_step[buff_group_count]][m]=999990;
                            break;
                        }
                        m+=1;
                    }

                    break;
                }

            }
            else
            {
                data = atoi(pch);
                //cout << data << endl;
                if(count == 8)
                {
                    float float_data = atof(pch);
                    //cout << "there is movetime:" <<float_data << endl;
                    movetime[buff_group_count][buff_group_step[buff_group_count]] = float_data*scale;
                    //break;
                }
                else if(count == 9)
                {
                    strcpy(coord_value[buff_group_count][buff_group_step[buff_group_count]],pch);
                    break;
                }
                else
                {
                    if (count % 2 == 0)
                    {
                        BuffPos[buff_group_count][buff_group_step[buff_group_count]][bm]=data;
                        bm+=1;

                    }
                    else if (count % 2 == 1)
                    {
                        BuffVelo[buff_group_count][buff_group_step[buff_group_count]][bn]=data;
                        bn+=1;
                    }
                }
            }
            pch = strtok(NULL, "\t");
            count +=1;
        }

       // buffstep += 1;
        buff_group_step[buff_group_count]+=1;
        cout << "step!!!!!!!:" <<buff_group_step[buff_group_count]<<endl;


    }
    fst.close();


    //    cout << "##############################POS:\n";
    //    for(int i=0;i<= buff_group_count;i++)
    //    {
    //        cout << "i:" <<i<<endl;
    //        for(int j=0;j<buff_group_step[i];j++)
    //        {
    //            cout << "j:" <<j<<endl;
    //            for(int k=0;k<6;k++)
    //            {
    //               cout << BuffPos[i][j][k] <<endl;
    //            }
    //        }
    //    }
    //    cout << "############################vel:\n";
    //    for(int i=0;i<= buff_group_count;i++)
    //    {
    //        cout << "i:" <<i<<endl;
    //        for(int j=0;j<buff_group_step[i];j++)
    //        {
    //            cout << "j:" <<j<<endl;
    //            for(int k=0;k<6;k++)
    //            {
    //               cout << BuffVelo[i][j][k] <<endl;
    //            }
    //        }
    //    }

    //velchange
    int speed_value[50][100][4]={};
    float vmax=0.0;

    float diff_value[50][100][4]={}; //2-1

    //cout << "diff_value: " << endl;
    for(int i=0;i<= buff_group_count;i++)
    {
        for (int j=0;j<buff_group_step[i];j++)
        {
            if (j==0)
            {
                for (int k=0;k<NUM;k++)
                {
                    diff_value[i][j][k]=abs(BuffPos[i][j][k]-BuffPos[i][buff_group_step[i]-1][k]);
                    //cout << diff_value[i][j][k] << "\t" ;
                }
                //cout << endl;
            }
            else
            {
                for (int k=0;k<NUM;k++)
                {
                    diff_value[i][j][k]=abs(BuffPos[i][j][k]-BuffPos[i][j-1][k]);
                    //cout << diff_value[i][j][k] << "\t" ;
                }
                //cout << endl;
            }
        }
        //cout << endl;
    }

    //---------------------------------------------------------------------------------------------
    for(int i=0;i<= buff_group_count;i++)
    {
        for (int j=0;j<buff_group_step[i];j++)
        {
            for (int k=0;k<NUM;k++)
            {
                diff_value[i][j][k] = diff_value[i][j][k]*180.0/posrange[k];
            }
        }
    }

    //---------------------------------------------------------------------------------------------
    fst.open("./readpos3.txt", ios::out);
    for(int i=0;i<= buff_group_count;i++)
    {
        fst << "group\t\n";
        for (int j=0;j<buff_group_step[i];j++)
        {
            vmax=0;
            for (int k=0;k<NUM;k++)
            {
                if (diff_value[i][j][k]>vmax)
                    vmax = diff_value[i][j][k];
            }
            float buffvmax = vmax;
            cout << "vmax= " << vmax << "movetime[i][j]:" << movetime[i][j]<<endl;

            vmax = ((vmax*60)/(360*movetime[i][j]));
            cout << "vmax= " << vmax << " rpm"<< endl;
            if (vmax > omega_limit)
            {
                movetime[i][j] = (buffvmax*60) / (360*omega_limit);
                cout << "omega_limit:" <<omega_limit <<endl;
                cout << "   exceed ljmjt w, new movetjme= " << movetime[i][j] << endl;
            }

            float omega[4]={};

            cout << "opt++++++++++++++++++++++++\n";
            for (int k=0;k<NUM;k++)
            {
                omega[k] = (diff_value[i][j][k]*60/(movetime[i][j]*360));


                if(gearratio[k] > 1)
                {

                    speed_value[i][j][k]=omega[k]*gearratio[k];
                    cout << "1\t" << speed_value[i][j][k] << "\t" << omega[k] << "\t" << gearratio[k] <<endl;
                }
                else
                {
                    speed_value[i][j][k]=omega[k]/gearratio[k];
                    cout << "2\t" << speed_value[i][j][k] << "\t" << omega[k] << "\t" << gearratio[k] <<endl;
                }
            }
        }

        if (!fst)
        {
            return 0;
        }
        else
        {
            for (int j=0;j<buff_group_step[i];j++)
            {
                cout << "step:::" << j <<endl;
                for (int k=0;k<NUM;k++)
                {
                    fst << BuffPos[i][j][k] << "\t" << speed_value[i][j][k] << "\t" ;
                }
                fst << movetime[i][j] << "\t" << coord_value[i][j] << "\t";
                fst << "\n";


                int m=0;
                while(1)
                {
                    if(BuffPump[i][j][m] == 0)
                    {
                        break;
                    }
                    else if (BuffPump[i][j][m] == 999991)
                    {
                        fst << "pumpon" << "\t" << 1 << "\t";
                        for (int k=2;k<NUM*2;k++)
                        {
                            fst << 0 << "\t";
                        }
                        fst << "\n";
                    }
                    else if (BuffPump[i][j][m] == 999990)
                    {
                        fst << "pumpoff" << "\t" << 1 << "\t";
                        for (int k=2;k<NUM*2;k++)
                        {
                            fst << 0 << "\t";
                        }
                        fst << "\n";
                    }
                    m+=1;

                }

            }
        }
    }

    fst.close();

    return 0;
}

int velchange(char filename[80])
{
    /*here
    #define J1GRR 400
    #define J23GRR 290
    #define J456UNIT 0.114
    #define L54S290Rv 144180.0
    #define L54S5000Rv 103860.0
    #define L42v 2048.0
    */
    cout << "omega_limit:" << omega_limit <<endl;
    cout << "scale:" << scale <<endl;

    #define oi_movetime 0.05
    int id[4]={1,2,3,4};
    int BuffPos[100][4]={};
    int BuffVelo[100][4]={};
    int BuffPump[100]={0};
    float BuffTheta[100][4]={};
    int result;
    int dlength = 8;
    char line[100];
    char * pch;
    int buffstep=0;
    int bm,bn;
    string str;
    fstream fst;
    int data=0;

    float movetime[100] = {};

//    for (int i=0;i<buffstep;i++)
//        {
//            movetime[i]=oi_movetime;
//            //cout << movetime[i] << "\t";
//        }
//        cout << endl;

    //unsigned char param[100][54]={};
    //unsigned char paramdata[54]={};


    //read file start
    fst.open(filename, ios::in);

    char *pumpon="pumpon";
    char *pumpoff="pumpoff";

    while(fst.getline(line, sizeof(line), '\n'))
    {
        bm=0;
        bn=0;
        cout << line << endl;
        cout << sizeof(line) << endl;

        if (line[sizeof(line)] == '\0')
            pch=strtok(line,"\t");
        int count = 0;
        while(pch != NULL)
        {
            if (strcmp(pch, pumpon)==0|| strcmp(pch, pumpoff)==0)
            {
                if(strcmp(pch, pumpon)==0)
                {
                    //cout << "pumpon" <<endl;
                    BuffPump[buffstep]=999991;
                    buffstep =buffstep -1; //and this one
                    break; //this one
                }
                else if(strcmp(pch, pumpoff)==0)
                {
                    //cout << "pumpoff" <<endl;
                    BuffPump[buffstep]=999990;
                    buffstep =buffstep -1;
                    break;
                }
                else
                {
                    BuffPump[buffstep]=0;
                    break;
                }
            }
            else
            {
                data = atoi(pch);
                //cout << data << endl;
                if(count == 8)
                {
                    float float_data = atof(pch);
                    //cout << "there is movetime:" <<float_data << endl;

                    movetime[buffstep]= float_data*scale;
                    break;
                }
                else
                {
                    if (count % 2 == 0)
                    {
                        BuffPos[buffstep][bm]=data;
                        bm+=1;

                    }
                    else if (count % 2 == 1)
                    {
                        BuffVelo[buffstep][bn]=data;
                        bn+=1;
                    }
                }
            }
            pch = strtok(NULL, "\t");
            count +=1;
        }

        buffstep += 1;
    }
    fst.close();


    //velchange
    double omega[4]={};
    int speed_value[100][4]={};
    float vmax=0.0;

    //calculate theta diffenence
    float diff_value[100][4]={}; //2-1

    //cout << "diff_value: " << endl;
    for (int i=0;i<buffstep;i++)
    {
        if (i==0)
        {
            for (int j=0;j<NUM;j++)
            {
                diff_value[i][j]=abs(BuffPos[i][j]-BuffPos[buffstep-1][j]);
                //cout << diff_value[i][j] << "\t" ;
            }
            //cout << endl;
        }
        else
        {
            for (int j=0;j<NUM;j++)
            {
                diff_value[i][j]=abs(BuffPos[i][j]-BuffPos[i-1][j]);
                //cout << diff_value[i][j] << "\t" ;
            }
            //cout << endl;
        }
    }
    //cout << endl;


    for (int i=0;i<buffstep;i++)
    {
        for (int j=0;j<bm;j++)
        {
            diff_value[i][j] = diff_value[i][j]*180.0/posrange[j];

//            if (j==0)
//                diff_value[i][j] = diff_value[i][j]*180.0/L54S290Rv;
//            else if (j==1 || j==2)
//                diff_value[i][j] = diff_value[i][j]*180.0/L54S5000Rv;
//            else
//                diff_value[i][j] = diff_value[i][j]*180.0/L42v;
            //cout << diff_value[i][j] << "\t";
        }
        //cout << endl;
    }
//
    for (int i=0;i<buffstep;i++)
    {
        //diff_value are theta from here
        vmax=0;
        for (int j=0;j<NUM;j++)
        {
            if (diff_value[i][j]>vmax)
                vmax = diff_value[i][j];
        }
        float buffvmax = vmax;
        vmax = ((vmax*60)/(360*movetime[i]));

        if (vmax > omega_limit)
        {
            movetime[i] = (buffvmax*60) / (360*omega_limit);
        }

        for (int j=0;j<NUM;j++)
        {
            omega[j] = (diff_value[i][j]*60/(movetime[i]*360));


            if(gearratio[j] > 1)
            {
                speed_value[i][j]=omega[j]*gearratio[j];
            }
            else
            {
                speed_value[i][j]=omega[j]/gearratio[j];
            }

//            if (j==0)
//                speed_value[i][j]=omega[j]*J1GRR;
//            else if (j==1 || j==2)
//                speed_value[i][j]=omega[j]*J23GRR;
//            else
//                speed_value[i][j]=omega[j]/J456UNIT;
        }
    }

    fst.open("./readpos3.txt", ios::out);
    if (!fst)
    {
        cout << "Fail to open file" << endl;
        return 0;
    }
    else
    {
        for (int i=0;i<buffstep;i++)
        {
            if (BuffPump[i] == 999991)
            {
                fst << "pumpon" << "\t" << 1 << "\t";
                for (int j=2;j<NUM*2;j++)
                {
                    fst << 0 << "\t";
                }
                fst << "\n";
            }
            else if (BuffPump[i] == 999990)
            {
                fst << "pumpoff" << "\t" << 1 << "\t";
                for (int j=2;j<NUM*2;j++)
                {
                    fst << 0 << "\t";
                }
                fst << "\n";
            }
            for (int j=0;j<NUM;j++)
            {
                fst << BuffPos[i][j] << "\t" << speed_value[i][j] << "\t" ;
            }
            fst << "\n";

        }
    }
    fst.close();
}



void MyThread2::run()
{
    //*#@!
    //cout<<"mThread2 is start!!!!\n";
    char *dev = (char*)"/dev/ttyUSB0"; //change devies id for mutiple arm

    //cout << "COM:" << this->dev << endl;

    if(DXL.Connect() == false)
    {
        fprintf(stderr, " Fail to open USB2Dyanmixel! [%s] \n\n", dev);
        emit NumberChanged2(-1);
    }

    for(int i=0;i<4;i++)
    {
        ::vig[i] = this->vig[i];
    }

    for(int i=0;i<4;i++)
    {
        ::vpg[i] = this->vpg[i];
    }

    for(int i=0;i<4;i++)
    {
        ::ppg[i] = this->ppg[i];
    }

    for(int i=0;i<4;i++)
    {
        ::joint_use[i] = this->joint_use[i];
    }

    for(int i=0;i<4;i++)
    {
        ::posrange[i] = this->pos_range[i]; //cout << i << "\t" << posrange[i] <<endl;
        ::gearratio[i] = this->gear_ratio[i]; //cout << i << "\t" << gearratio[i] <<endl;
        ::vellimit[i] = this->vel_limit[i]; //cout << i << "\t" << vellimit[i] <<endl;
    }

    //#@!
    DXL.SetBaudrate(1);

    char filename[256]={};

    strcpy(filename,this->filename);

    char input[80];
    char cmd[80];
    strcpy(input, this->cmd);

    char param[20][30];
    int num_param;

    retimes=this->times;
    char* token;

    ::step_span = this->step_span*1000; //cout << "step span!!:" << step_span << "\n";
    ::delay_time = this->delay_time*1000;
    ::omega_limit = this->omega_limit;
    ::move_time = this->move_time;
    ::scale = this->scale;

    if(strlen(input) == 0)
        printf("no cmd\n");

    token = strtok(input, " ");

    if(token == 0)
        printf("no cmd2\n");

    strcpy(cmd, token);
    token = strtok(0, " ");
    num_param = 0;

    while(token != 0)
    {
        strcpy(param[num_param++], token);
        token = strtok(0, " ");
    }

    if(strcmp(cmd, "help") == 0 || strcmp(cmd, "h") == 0 || strcmp(cmd, "?") == 0)
    {
        Help();
    }
    else if(strcmp(cmd,"find_point")==0)
    {
        cout<<"in mThread2 find_point!!!!!!!!!!!!!\n";
        int error=0;
        long posivalue=0;
        int result=-1;
        int height1;
        height1=this->height1;
        cout<<"height1="<<height1<<endl;
        for(int i=0;i<4;i++)
        {
            while(1)
            {
                result = DXL.ReadDWord(i+1, 611, &posivalue, &error);  //~~~~

                if(result == 1)
                {
                    this->now_theta_dialog[i]=posivalue*180/posrange[i];
                    cout<<"now_theta_dialog["<<i<<"]="<<this->now_theta_dialog[i]<<endl;
                    cout<<posivalue<<endl;
                    break;
                }
                else
                {
                    cout<<"in dialog read i is wrong!!!\n";
                }
            }
        }
        this->now_theta_dialog[2]=2*180*height1/16;
        for(int i=0;i<4;i++)
        {
            cout<<"after now_theta_dialog["<<i<<"]="<<this->now_theta_dialog[i]<<endl;
        }
    }
    else if(strcmp(cmd, "syncw") == 0)
    {
        cout << "times:" << retimes<<endl;
        cout << "filename:" << filename<<endl;
        
        int result_syncw = syncw(retimes,filename);
        
        if(result_syncw == 1)
        {
            emit NumberChanged2(8);
        }
        else if( result_syncw == 2)
        {
            emit NumberChanged2(10);
        }
        else
        {
            ;
        }
    }
    else if(strcmp(cmd, "first_step") == 0)
    {
        posi_start_vel_group(this->group);
    }
    else if(strcmp(cmd, "dynamic_syncw_group") == 0)
    {
        cout << "group:" << this->group <<endl;
        cout << "change_step_now:" << this->change_step_now <<endl;
        cout << "change_step_pre:" << this->change_step_pre <<endl;
        cout << "this->move_time:" << this->move_time <<endl;

        for(int i=0;i<4;i++)
        {
            cout << "i:" << i << "\t" << this->change_pos[i] << "\n";
        }

        change_group_index(this->change_pos, this->change_group, this->change_step_now, this->change_step_pre, this->move_time, this->isnewvalue);


    }
    else if(strcmp(cmd, "syncw_group") == 0)
    {
        //cout << "group:" << this->group <<endl;
        //cout << "times:" << retimes<<endl;

        posi_start_vel_group(this->group);    //#@!
        int state = group_sync2write(8, group_step[this->group], retimes, this->group);

        fstream fst;

        char filename[20] = "./";
        char str[20] = ".txt";

        strcat(filename, this->ID);
        strcat(filename, str);

        fst.open(filename,ios::out);

        char cptime[50];
        time_t now = time(NULL);

        strftime(cptime, 50, "%Y %m %d %T", localtime(&now));

        if(state == 1)
        {
            fst << string(cptime) << "\t"<< this->group << "\t" << this->ID << "\tok\t\n";
            this->state = 1;

        }
        else
        {
            fst << string(cptime) << "\t"<< this->group << "\t" << this->ID << "\tfail\t\n";
            this->state = 0;
        }
        fst.close();

    }
    else if (strcmp(cmd, "list_group") == 0)
    {
        for(int i=0;i<group_step[this->change_group];i++)
        {
            cout << "step:" << i << "\t";

            if(group_GoalPos[this->change_group][i][0]==999991 || group_GoalPos[this->change_group][i][0]==999990)
            {
                cout << group_GoalPos[this->change_group][i][0] << "\t";
            }
            else
            {
                for(int j=0;j<4;j++)
                {
                    cout << group_GoalPos[this->change_group][i][j] << "\t";
                }
            }

            cout << "\n";
        }
    }
    else if (strcmp(cmd, "read_group") == 0)
    {
        //cout << "case: read_group filename:" << filename <<endl;
        read_group(filename);
    }
    else if(strcmp(cmd, "syncw_action") == 0)
    {
        int ini=initialization();
        qDebug()<< ini ;
        syncw(retimes,filename);
    }
    else if(strcmp(cmd, "toffall") == 0)
    {
        //cout << "enter" << endl;
        int t_off_all=toffall();
        if(t_off_all == 1)
        {
            cout << "torque off all successfully!" <<endl;
            emit NumberChanged2(2);
        }
        else
        {
            cout << "torque off all fail!" <<endl;
            emit NumberChanged2(3);
        }
    }
    else if(strcmp(cmd, "tonall") == 0)
    {
        //cout << "enter" << endl;
        int t_on_all=tonall();

        if(t_on_all == 1)
        {
            cout << "torque on all successfully!" <<endl;
            emit NumberChanged2(4);
        }
        else
        {
            cout << "torque on all fail!" <<endl;
            emit NumberChanged2(5);
        }
    }
    else if(strcmp(cmd, "init") == 0)
    {
        //cout<< "ini+++" <<endl;
        int ini=initialization();

        if(ini == 1)
        {
            emit NumberChanged2(1); //initail success
        }
        else if(ini == 0)
        {
            emit NumberChanged2(5); //torque on fail
        }
        else if(ini == 2)
        {
            emit NumberChanged2(0);  //modify gain fail
        }
        else
        {
            emit NumberChanged2(-1);
        }
        //cout<< "ini+:" << ini <<"+"<<endl;
    }
    else if(strcmp(cmd, "readall") == 0)
    {
        readall();
    }
    else if(strcmp(cmd, "suck_on") == 0)
    {
        suck_on();
    }
    else if(strcmp(cmd, "suck_off") == 0)
    {
        suck_off();
    }
    else if(strcmp(cmd,"change")==0)
    {
        int Goalpos[4]={0};
        int Nowpos[4]={0};
        int Goalo[4]={0};
        double Goalvel[4]={0};
        char filename1[80]={};
        strcpy(filename1,this->filename1);

        for(int m=0;m<NUM;m++)
        {
            Goalpos[m]=this->Goalpos[m];
            //cout<<"before cul Goalpos["<<m<<"]"<<Goalpos[m]<<"\t"<<endl;
            Nowpos[m]=this ->Nowpos[m];
            //cout<<"before cul Nowpos["<<m<<"]"<<Nowpos[m]<<"\t"<<endl;
            Goalo[m]=this->Goalo[m];
        }
        cout<<endl;
       // cout<<"now omega limit is"<<this->omega_limit<<endl;
       // cout<<"now move time is"<<this->move_time<<endl;
        posi_start_vel_change(Goalpos,Nowpos,Goalvel);
        fstream fst1;
        fst1.open(filename1,ios::out|ios::app);
        if(!fst1)
        {
            cout<<"fst1 open is wrong!!!"<<endl;
        }
        for(int m=0;m<NUM;m++)
        {
            fst1<<Goalpos[m]<<"\t"<<Goalvel[m]<<"\t";
        }
        fst1<<move_time<<"\t";
    }
    /*
    else if(strcmp(cmd, "gpio_on") == 0)
    {
        gpio_on();
    }
    else if(strcmp(cmd, "gpio_off") == 0)
    {
        gpio_off();
    }
    */
    else if(strcmp(cmd, "readpos") == 0)
    {
        int state=readpos();
        if(state == 0)
        {
            this->state=0;
        }
        if(state == 1)
        {
            this->state=1;
        }

    }
    else if(strcmp(cmd, "optimize") == 0)
    {
        //cout << "filename:" <<filename;
        //group_velchange(filename, this->omega_limit);
        velchange(filename);
    }
    else if(strcmp(cmd, "optimize2") == 0)
    {
        group_velchange(filename);
        cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";
        group_velchange_test(filename);
    }
    else if(strcmp(cmd, "reboot_all") == 0)
    {
        int state=reboot_all();
        if(state == 1)
        {
            cout << "reboot ok!" <<endl;
            this->state=1;
            emit NumberChanged2(6);
        }
        else
        {
            cout << "reboot fail!" <<endl;
            this->state=0;
            emit NumberChanged2(7);
        }
    }
    else if(strcmp(cmd, "ping") == 0)
    {
        if(num_param == 0)
        {
            fprintf(stderr, " Invalid parameters! \n");
            //continue;
        }

        fprintf(stderr, "\n");
        PingInfo *data = new PingInfo();
        for(int i = 0; i < num_param; i++)
        {
            if(DXL.Ping(atoi(param[i]), data, 0) == COMM_RXSUCCESS)
            {
                fprintf(stderr, "                                          ... SUCCESS \r");
                fprintf(stderr, " [ID:%.3d] Model No : %.5d (0x%.2X 0x%.2X) \n",
                        data->ID, data->ModelNumber, DXL_LOBYTE(data->ModelNumber), DXL_HIBYTE(data->ModelNumber));
            }
            else
            {
                fprintf(stderr, "                                          ... FAIL \r");
                fprintf(stderr, " [ID:%.3d] \n", atoi(param[i]));
            }

            usleep(delay_time);
        }
        fprintf(stderr, "\n");
    }
    else if(strcmp(cmd, "scan") == 0)
    {
        Scan();
    }
    else if(strcmp(cmd, "baud") == 0)
    {
        if(num_param != 1)
        {
            fprintf(stderr, " Invalid parameters! \n");
        }

        if(DXL.SetBaudrate(atoi(param[0])) == false)
            fprintf(stderr, " Failed to change baudrate! \n");
        else
            fprintf(stderr, " Success to change baudrate! [ BAUD NUM: %d ]\n", atoi(param[0]));
    }
    else if(strcmp(cmd, "wrb") == 0 || strcmp(cmd, "w") == 0)
    {
        if(num_param != 3)
        {
            fprintf(stderr, " Invalid parameters! \n");
        }

        Write(atoi(param[0]), atoi(param[1]), atoi(param[2]), 1);
    }
    else if(strcmp(cmd, "wrw") == 0)
    {
        if(num_param != 3)
        {
            fprintf(stderr, " Invalid parameters! \n");
        }

        Write(atoi(param[0]), atoi(param[1]), atoi(param[2]), 2);
    }
    else if(strcmp(cmd, "wrd") == 0)
    {
        if(num_param != 3)
        {
            fprintf(stderr, " Invalid parameters! \n");
        }

        Write(atoi(param[0]), atoi(param[1]), atol(param[2]), 4);
    }
    else if(strcmp(cmd, "rdb") == 0)
    {
        if(num_param != 2)
        {
            fprintf(stderr, " Invalid parameters! \n");
        }

        Read(atoi(param[0]), atoi(param[1]), 1);
    }
    else if(strcmp(cmd, "rdw") == 0)
    {
        if(num_param != 2)
        {
            fprintf(stderr, " Invalid parameters! \n");
        }

        Read(atoi(param[0]), atoi(param[1]), 2);
    }
    else if(strcmp(cmd, "rdd") == 0)
    {
        if(num_param != 2)
        {
            fprintf(stderr, " Invalid parameters! \n");
        }

        Read(atoi(param[0]), atoi(param[1]), 4);
    }
    else if(strcmp(cmd, "r") == 0)
    {
        if(num_param != 3)
        {
            fprintf(stderr, " Invalid parameters! \n");
        }

        Dump(atoi(param[0]), atoi(param[1]), atoi(param[2]));
    }
    else if(strcmp(cmd, "mon") == 0)
    {
        int len = 0;

        if(num_param > 2 && strcmp(param[2],"b") == 0) len = 1;
        else if(num_param > 2 && strcmp(param[2],"w") == 0) len = 2;
        else if(num_param > 2 && strcmp(param[2],"d") == 0) len = 4;

        if(num_param != 3 || len == 0)
        {
            fprintf(stderr, " Invalid parameters! \n");
        }

        Refresh(atoi(param[0]), atoi(param[1]), len);
    }
    else if(strcmp(cmd, "bp") == 0)
    {
        if(num_param != 0)
        {
            fprintf(stderr, " Invalid parameters! \n");
        }

        BroadcastPing();
    }
    else if(strcmp(cmd, "reboot") == 0)
    {
        if(num_param != 1)
        {
            fprintf(stderr, " Invalid parameters! \n");
        }

        int result = DXL.Reboot(atoi(param[0]), 0);
        if(result != COMM_RXSUCCESS)
            fprintf(stderr, "\n Fail to reboot! \n\n");
        else
            fprintf(stderr, "\n Success to reboot! \n\n");
    }
    else if(strcmp(cmd, "reset") == 0)
    {
        if(num_param != 2)
        {
            fprintf(stderr, " Invalid parameters! \n");
        }

        int result = DXL.FactoryReset(atoi(param[0]), atoi(param[1]), 0);

        if(result != COMM_RXSUCCESS)
            fprintf(stderr, "\n Fail to reset! \n\n");
        else
            fprintf(stderr, "\n Success to reset! \n\n");
    }
    else if(strcmp(cmd, "exit") == 0)
    {
        DXL.Disconnect();
    }

    //cout << "mythread2 is finished" << endl;
}
