//modbus
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "libmodbus/modbus.h"

#include "mythread6.h"
#include <iostream>
#include <QDebug>
#include <wait.h>
#include <time.h>
#include <QDateTime>
#include <QTime>
#include <fstream>
#include <sys/time.h>

using namespace std;

#define MODBUS_SERIAL_DEV       "/dev/ttyUSB1"
#define MODBUS_SERIAL_BAUDRATE  9600            /* 9600, 38400, 115200, ... */
#define MODBUS_SERIAL_PARITY    'N'             /* 'N', 'E', or 'O' */
#define MODBUS_SERIAL_DATABITS  8               /* 5, 6, 7, or 8 */
#define MODBUS_SERIAL_STOPBITS  1               /* 1 or 2 */
#define MODBUS_DEVICE_ID        1
#define MODBUS_TIMEOUT_SEC      3
#define MODBUS_TIMEOUT_USEC     0
#define MODBUS_DEBUG            ON
#define MODBUS_RO_BITS          32
#define MODBUS_RW_BITS          32
#define MODBUS_RO_REGISTERS     64
#define MODBUS_RW_REGISTERS     64

#define MODBUS_INPUT_ADDR       0
#define MODBUS_INPUT_LEN        64
#define MODBUS_HOLDING_ADDR     0
#define MODBUS_HOLDING_LEN      64


#define tray_full_anaddr            201
#define tray_inpart_anaddr          202
#define all_addr                    203
#define leftabsorb_done_addr        204
#define leftabsorb_done_anaddr      205
#define cell_addr                   230
#define cell_anaddr                 206
#define passorfail_addr             231
#define passorfail_anaddr           207
#define rightleft_inpart_addr       208
#define rightleft_done_addr         209
#define rightleft_done_anaddr       210
#define scanready_addr              211
#define scanready_anaddr            212
#define scandone_addr               213
#define scandone_anaddr             214
#define rightdeflate_inpart_addr    215
#define rightdeflate_done_addr      216
#define rightdeflate_done_anaddr    217
#define fail_full_addr              218
#define fail_inpart_addr            219
#define emergency_stop              220



MyThread6::MyThread6(QObject *parent) : QThread(parent){}

void MyThread6::set_PLC_Dev(QString dev)
{
    this->dev = dev;
}

void MyThread6::run()
{
    cout<<"Mthread6 is start!!!!";
    float time2=0;
    modbus_t            *ctx;
    modbus_mapping_t    *data_mapping;
    //struct timeval      timeout;

    struct timeval start6,end6;
    double timecost;
    gettimeofday(&start6, NULL);

    //char *dev = this->dev.toLatin1().data();

    int tested_tray_num = this->tested_tray_num;
    int pass_tray_num = this->pass_tray_num;
    int fail_tray_num = this->fail_tray_num;
    int cell_max_num = this->cell_max_num;
    int start_point = this->start_point;

    uint16_t    tab_reg[64]={0};
    int         emergencystop, cellnum, passorfail, scanready, scandone,automation,testinpart,nowcount,failtray;
    int         group_count=0, cell_num=0, count_pass=0, count_fail=0;
    int         m=0;
    int         side=0;
    bool        warnning=false;

    //#@!
    ctx = modbus_new_rtu(MODBUS_SERIAL_DEV,
                         this->baudrate,
                         MODBUS_SERIAL_PARITY,
                         MODBUS_SERIAL_DATABITS,
                         MODBUS_SERIAL_STOPBITS);

    if (ctx == NULL){
       fprintf(stderr, "Unable to create the libmodbus context.\n");
       exit(-1);
    }

    //Set slave device ID
    modbus_set_slave(ctx, MODBUS_DEVICE_ID);

    //Debug mode
    modbus_set_debug(ctx, MODBUS_DEBUG);

    data_mapping = modbus_mapping_new(MODBUS_RO_BITS, MODBUS_RW_BITS,
                                      MODBUS_RO_REGISTERS, MODBUS_RW_REGISTERS);

    if (data_mapping == NULL){
        fprintf(stderr, "Failed to allocate the mapping: %s\n", modbus_strerror(errno));
        modbus_free(ctx);

        emit NumberChanged6(-1);
    }


    //open serial interface
    modbus_connect(ctx);

    if (modbus_connect(ctx) != -1)
    {
        emit NumberChanged6(-3);
        this->allcount=0;

        int M400=0;
        cout<<"send M400:"<<start_point<<endl;
        M400=modbus_write_bit(ctx,400,start_point);//arm in start point
        cout<<"the result of send M400:"<<M400<<endl;


        while(this->Exit != true && warnning !=true)
        {
            fstream fstr;
            fstr.open("record_time_4ax_withconnect.txt",ios::out|ios::app);

            QDateTime dt;
            QTime time;
            //QTime cur_time=QTime::currentTime();
            QDate date;
            dt.setTime(time.currentTime());
            dt.setDate(date.currentDate());
            QString currentDate=dt.toString("yyyy:MM:dd:hh:mm:ss");
            fstr<<currentDate.toStdString().c_str()<<"\t";
            /*fstr<<(cur_time.hour()*3600+cur_time.minute()*60+cur_time.second()-time2)<<"\n";
            time2=cur_time.hour()*3600+cur_time.minute()*60+cur_time.second();*/
            gettimeofday(&end6, NULL);
            timecost = ((end6.tv_sec - start6.tv_sec)*1000.0+(end6.tv_usec - start6.tv_usec)/1000.0);
            fstr<<timecost/1000<<"s"<<endl;
            gettimeofday(&start6, NULL);
            fstr.close();

            this->scan_ready = false;
            this->Scandone_check = 0;
            this->mode_gpio = 0;
            this->done = 0;


            if(this->Exit == true || warnning == true)
            {
                modbus_write_register(ctx,tray_full_anaddr,0);
                modbus_write_register(ctx,fail_full_addr,0);
                modbus_write_register(ctx,all_addr,0);
                modbus_write_register(ctx,leftabsorb_done_addr,0);
                modbus_write_register(ctx,cell_anaddr,0);
                modbus_write_register(ctx,fail_full_addr,0);


                modbus_mapping_free(data_mapping);
                modbus_free(ctx);
                cout << "mythread6 connect closed!" << endl;
                this->Exit = false;
                break;
            }
            else
            {

                //check automation
                while(this->Exit != true && warnning == false)
                {
                    //cout<<"check automation!!!!!!!!!!!"<<endl;
                    int l=modbus_write_register(ctx,199,1);
                    cout<<"l= "<<l<<endl;
                    automation=modbus_read_registers(ctx,200,1,tab_reg);
                    if(automation==-1)
                    {
                         fprintf(stderr, "%s\n", modbus_strerror(errno));
                    }
                    else
                    {
                        if(tab_reg[0]==1)
                        {
                            modbus_write_register(ctx,199,0);
                            if(group_count==0)
                            {
                                tab_reg[0]=100;
                                while(this->Exit!=true && warnning == false)
                                {
                                    nowcount=modbus_read_registers(ctx,300,1,tab_reg);
                                    if(nowcount==-1)
                                    {
                                        fprintf(stderr, "%s\n", modbus_strerror(errno));
                                    }
                                    else
                                    {
                                        if(tab_reg[0]!=100)
                                        {
                                            group_count=tab_reg[0];
                                            tab_reg[0]=0;
                                            break;
                                        }
                                    }
                                }
                            }
                            break;
                        }
                    }
                }

                while(this->Exit !=true && warnning == false)
                {
                    tab_reg[0]=2;
                    emergencystop=modbus_read_registers(ctx,emergency_stop,1,tab_reg);
                    if(testinpart==-1)
                    {
                        fprintf(stderr, "%s\n", modbus_strerror(errno));
                    }
                    else if(tab_reg[0]==1)
                    {
                       testinpart=0;
                       tab_reg[0]=0;
                       warnning = true;
                       break;
                    }
                    else if(tab_reg[0]==0)
                        break;
                }


                //take tested PCB
                struct timeval start,end;
                double timecost;
                gettimeofday(&start, NULL);
                while(this->Exit != true && warnning == false)
                {
                    if(this->check==true)
                    {
                        if(group_count==tested_tray_num)
                        {
                            cout << "The test tray is full" << endl;
                            modbus_write_register(ctx,tray_full_anaddr,2);
                            while(this->Exit!=true && warnning == false)
                            {
                                testinpart=modbus_read_registers(ctx,tray_inpart_anaddr,1,tab_reg);

                                if(testinpart==-1)
                                {
                                    fprintf(stderr, "%s\n", modbus_strerror(errno));
                                }
                                else if(tab_reg[0]==1)
                                {
                                   testinpart=0;
                                   tab_reg[0]=0;
                                   group_count = 0;
                                   count_pass = 0;
                                   modbus_write_register(ctx,fail_full_addr,0);
                                   this->group = group_count;
                                   emit NumberChanged6(1);
                                   modbus_write_bit(ctx,400,0);
                                   this->check = false;
                                   break;
                                }
                                else if(tab_reg[0]==2)
                                {
                                    warnning = true;
                                    tab_reg[0]=0;
                                    break;
                                }
                                gettimeofday(&end,NULL);
                                timecost = ((end.tv_sec - start.tv_sec)*1000.0+(end.tv_usec - start.tv_usec)/1000.0);

                            }
                            break;
                        }
                        else
                        {
                            tab_reg[0]=0;
                            modbus_write_register(ctx,fail_full_addr,0);
                            modbus_write_register(ctx,tray_full_anaddr,1);
                            while(this->Exit!=true && warnning == false)
                            {
                                testinpart=modbus_read_registers(ctx,tray_inpart_anaddr,1,tab_reg);
                                if(testinpart==-1)
                                {
                                    fprintf(stderr, "%s\n", modbus_strerror(errno));
                                }
                                else if(tab_reg[0]==1)
                                {
                                   testinpart=0;
                                   tab_reg[0]=0;
                                   this->group = group_count;
                                   emit NumberChanged6(1);
                                   modbus_write_bit(ctx,400,0);
                                   this->check = false;
                                   break;
                                }
                            }
                             break;
                        }

                    }
                }

                while(this->Exit !=true && warnning == false)
                {
                    cout<<"111111111111111111111111111111"<<endl;
                    tab_reg[0]=2;
                    emergencystop=modbus_read_registers(ctx,emergency_stop,1,tab_reg);
                    if(testinpart==-1)
                    {
                        fprintf(stderr, "%s\n", modbus_strerror(errno));
                    }
                    else if(tab_reg[0]==1)
                    {
                       this->done=2;
                       testinpart=0;
                       tab_reg[0]=0;
                       warnning = true;
                       break;
                    }
                    else if(tab_reg[0]==0)
                        break;
                }

                //send left absorb
                while(this->Exit != true && warnning == false)
                {
                    if(this->mode_gpio==1)
                    {
                        this->mode_gpio=0;
                        modbus_write_register(ctx,all_addr,1);
                        modbus_write_register(ctx,leftabsorb_done_addr,1);
                        cout<<"send 1234 is 1!!!!!!!!!!!!!!!!!111"<<endl;
                        while(this->Exit!=true && warnning == false)
                        {
                            cout<<"222222222222222222222222222222"<<endl;
                            modbus_read_registers(ctx,leftabsorb_done_anaddr,1,tab_reg);
                            if(tab_reg[0]==1)
                            {
                                modbus_write_register(ctx,tray_full_anaddr,0);
                                cout<<"tray full anaddr is cleared"<<endl;
                                modbus_write_register(ctx,all_addr,0);
                                modbus_write_register(ctx,leftabsorb_done_addr,0);
                                tab_reg[0]=0;
                                this->done=1;
                                cout<<"in 6 done is true#########################\n";
                                break;
                            }
                            else if(tab_reg[0]==2)
                            {
                                warnning=true;
                                this->done=2;
                                modbus_write_register(ctx,tray_full_anaddr,0);
                                modbus_write_register(ctx,all_addr,0);
                                modbus_write_register(ctx,leftabsorb_done_addr,0);
                                tab_reg[0]=0;
                                //break;
                            }
                        }
                        break;
                    }
                }


                while(this->Exit !=true && warnning == false)
                {
                    tab_reg[0]=2;
                    emergencystop=modbus_read_registers(ctx,emergency_stop,1,tab_reg);
                    if(testinpart==-1)
                    {
                        fprintf(stderr, "%s\n", modbus_strerror(errno));
                    }
                    else if(tab_reg[0]==1)
                    {
                       testinpart=0;
                       tab_reg[0]=0;
                       warnning = true;
                       break;
                    }
                    else if(tab_reg[0]==0)
                        break;
                }

                //Receive Cell Number
                while(this->Exit != true && warnning == false)
                {
                    cout << "****** Receive Cell Number ******" << endl;
                    cellnum = modbus_read_registers(ctx,cell_addr,1,tab_reg);

                    if (cellnum == -1)
                    {
                        fprintf(stderr, "%s\n", modbus_strerror(errno));
                    }
                    else
                    {
                        cout<<"11111111111111111111111\n";
                        if( (tab_reg[0]<14) & (tab_reg[0]>0) )
                        {
                            modbus_write_register(ctx,cell_anaddr,1);

                            for(cell_num=1; cell_num<cell_max_num+1; cell_num++)
                            {
                                if(tab_reg[0]==cell_num)
                                {
                                    this->group = cell_num + tested_tray_num -1;
                                    cout << "cell number: " << this->group << endl;

                                    this->check = false;
                                    emit NumberChanged6(2); //***
                                    break;
                                }
                            }
                            tab_reg[0]=0;
                            break;
                        }
                    }
                }

                //Receive result of tested
                while(this->Exit != true && warnning == false)
                {
                    //cout << "****** Receive the Result of tested ******" << endl;
                    passorfail = modbus_read_registers(ctx,passorfail_addr,1,tab_reg);

                    if(tab_reg[0]!=0 && passorfail != -1)
                    {
                        modbus_write_register(ctx,passorfail_anaddr,1);
                        modbus_write_register(ctx,cell_anaddr,0);

                        if(tab_reg[0]==1||tab_reg[0]==2)
                        {
                            if(tab_reg[0]==1)   //pass
                            {
                                //m = tested_tray_num + cell_max_num + count_pass;
                                m=tested_tray_num + cell_max_num + group_count;
                                //count_pass += 1;
                            }
                            else if(tab_reg[0]==2)  //fail
                            {
                                //m = tested_tray_num + cell_max_num + count_fail;
                                while(count_fail==0)
                                {
                                    tab_reg[0]=100;
                                    failtray=modbus_read_registers(ctx,301,1,tab_reg);
                                    if(failtray==-1)
                                    {
                                        fprintf(stderr, "%s\n", modbus_strerror(errno));
                                    }
                                    else
                                    {
                                        if(tab_reg[0]!=100)
                                        {
                                            count_fail=tab_reg[0];
                                            tab_reg[0]=0;
                                            break;
                                        }
                                    }
                                }
                                m = tested_tray_num + cell_max_num + pass_tray_num + count_fail;
                                count_fail += 1;
                                modbus_write_register(ctx,301,count_fail);
                            }
                            tab_reg[0]=0;
                            break;
                        }
                        else if(tab_reg[0]==8)
                        {
                            while(count_fail==0)
                            {
                                tab_reg[0]=100;
                                failtray=modbus_read_registers(ctx,301,1,tab_reg);
                                if(failtray==-1)
                                {
                                    fprintf(stderr, "%s\n", modbus_strerror(errno));
                                }
                                else
                                {
                                    if(tab_reg[0]!=100)
                                    {
                                        count_fail=tab_reg[0];
                                        tab_reg[0]=0;
                                        break;
                                    }
                                }
                            }
                            m = tested_tray_num + cell_max_num + pass_tray_num + count_fail;
                            count_fail += 1;
                            modbus_write_register(ctx,301,count_fail);
                            tab_reg[0]=0;
                            break;
                        }

                    }
                    else
                    {
                        fprintf(stderr, "%s\n", modbus_strerror(errno));
                    }
                }


                while(this->Exit !=true && warnning == false)
                {
                    tab_reg[0]=2;
                    emergencystop=modbus_read_registers(ctx,emergency_stop,1,tab_reg);
                    if(testinpart==-1)
                    {
                        fprintf(stderr, "%s\n", modbus_strerror(errno));
                    }
                    else if(tab_reg[0]==1)
                    {
                       testinpart=0;
                       tab_reg[0]=0;
                       warnning = true;
                       this->done = 2;
                       break;
                    }
                    else if(tab_reg[0]==0)
                        break;
                }

                //send right absorb and left deflate
                while(this->Exit != true && warnning == false)
                {
                    if(this->mode_gpio==2)
                    {
                        this->mode_gpio=0;
                        modbus_read_registers(ctx,rightleft_inpart_addr,1,tab_reg);
                        if(tab_reg[0]==1)
                        {
                            modbus_write_register(ctx,passorfail_anaddr,0);
                            modbus_write_register(ctx,all_addr,2);
                            tab_reg[0]=0;

                            modbus_write_register(ctx,rightleft_done_addr,1);
                            while(this->Exit != true && warnning == false)
                            {
                                modbus_read_registers(ctx,rightleft_done_anaddr,1,tab_reg);
                                if(tab_reg[0]==1)
                                {
                                    tab_reg[0]=0;
                                    modbus_write_register(ctx,rightleft_done_addr,0);
                                    this->done=1;
                                    break;
                                }
                                else if(tab_reg[0]==2)
                                {
                                    tab_reg[0]=0;
                                    this->done=2;
                                    modbus_write_register(ctx,rightleft_done_addr,0);
                                    warnning=true;
                                }
                            }
                            break;
                        }
                    }
                }


                //Check Scan Ready
                while(this->Exit != true && warnning == false)
                {
                    cout << "****** Check Scan Ready ******" << endl;
                    scanready = modbus_read_registers(ctx, scanready_addr, 1, tab_reg);

                    if (scanready == -1)
                    {
                        fprintf(stderr, "%s\n", modbus_strerror(errno));
                    }
                    else
                    {
                        if(tab_reg[0]==1)
                        {
                            //this->scan_ready = false;

                            cout << "check move" << endl;
                            while(this->Exit!=true && warnning == false)
                            {
                                wait(0);
                                if(this->scan_ready==true)   //check mythread2!
                                {
                                    modbus_write_register(ctx,scanready_anaddr,1);
                                    tab_reg[0]=0;
                                    break;
                                }
                            }

                            break;
                        }
                    }
                }

                while(this->Exit !=true && warnning == false)
                {
                    tab_reg[0]=2;
                    emergencystop=modbus_read_registers(ctx,emergency_stop,1,tab_reg);
                    if(testinpart==-1)
                    {
                        fprintf(stderr, "%s\n", modbus_strerror(errno));
                    }
                    else if(tab_reg[0]==1)
                    {
                       testinpart=0;
                       tab_reg[0]=0;
                       warnning = true;
                       break;
                    }
                    else if(tab_reg[0]==0)
                        break;
                }

                //Check Scan done
                while(this->Exit != true && warnning == false)
                {
                    modbus_write_register(ctx,scandone_addr,1);
                    scandone=modbus_read_registers(ctx,scandone_anaddr,1,tab_reg);
                    if (scandone == -1)
                    {
                        fprintf(stderr, "%s\n", modbus_strerror(errno));
                    }
                    else
                    {
                        if(tab_reg[0]==1)
                        {
                             modbus_write_register(ctx,scanready_anaddr,0);
                             modbus_write_register(ctx,scandone_addr,0);
                             this->Scandone_check = 1;

                             //this->check = false;
                             cout << "waiting for mythread2 is finished" << endl;
                             while(this->Exit!=true && warnning == false)
                             {

                                 if(this->check == true)
                                 {
                                     this->group = m;
                                     cout<<"m="<<m<<endl;
                                     emit NumberChanged6(1);    //***

                                     modbus_write_register(ctx,301,count_fail);

                                     this->check = false;
                                     group_count++;
                                     modbus_write_register(ctx,300,group_count);
                                     //this->Scandone_check = false;
                                     break;
                                 }
                             }

                             tab_reg[0]=0;

                             break;
                        }
                        else if(tab_reg[0]==2)
                        {
                            modbus_write_register(ctx,scanready_anaddr,0);
                            modbus_write_register(ctx,scandone_addr,0);
                            this->Scandone_check = 2;
                            cout<<"scan is fail!!!!!!!!!!!\n";
                            warnning=true;
                            break;
                        }
                    }
                }

                while(this->Exit !=true && warnning == false)
                {
                    tab_reg[0]=2;
                    emergencystop=modbus_read_registers(ctx,emergency_stop,1,tab_reg);
                    if(testinpart==-1)
                    {
                        fprintf(stderr, "%s\n", modbus_strerror(errno));
                    }
                    else if(tab_reg[0]==1)
                    {
                       testinpart=0;
                       tab_reg[0]=0;
                       warnning = true;
                       this->done = 2;
                       break;
                    }
                    else if(tab_reg[0]==0)
                        break;
                }

                //check right deflate
                while(this->Exit != true && warnning == false)
                {
                    if(this->mode_gpio==1)
                    {
                        this->mode_gpio=0;
                        modbus_read_registers(ctx,rightdeflate_inpart_addr,1,tab_reg);
                        if(tab_reg[0]==1)
                        {
                            modbus_write_register(ctx,all_addr,4);
                            tab_reg[0]=0;

                            modbus_write_register(ctx,rightdeflate_done_addr,1);
                            while(this->Exit!=true && warnning == false)
                            {
                                modbus_read_registers(ctx,rightdeflate_done_anaddr,1,tab_reg);
                                if(tab_reg[0]==1)
                                {
                                    tab_reg[0]==0;
                                    modbus_write_register(ctx,all_addr,0);
                                    modbus_write_register(ctx,rightdeflate_done_addr,0);
                                    this->done=1;
                                    break;
                                }
                                else if(tab_reg[0]==2)
                                {
                                    tab_reg[0]=0;
                                    this->done=2;
                                    modbus_write_register(ctx,all_addr,0);
                                    modbus_write_register(ctx,rightdeflate_done_addr,0);
                                    warnning=true;
                                }
                            }
                            break;
                        }
                    }
                }


                //Check mythread2 is finished!
                //cout << "waiting for the third group........" << endl;

                while(this->Exit != true && warnning == false)
                {
                    if(this->check == true)
                    {
                        cout << "The third group is finished" << endl;
                        break;
                    }

                }

                while(this->Exit !=true && warnning == false)
                {
                    tab_reg[0]=2;
                    emergencystop=modbus_read_registers(ctx,emergency_stop,1,tab_reg);
                    if(testinpart==-1)
                    {
                        fprintf(stderr, "%s\n", modbus_strerror(errno));
                    }
                    else if(tab_reg[0]==1)
                    {
                       testinpart=0;
                       tab_reg[0]=0;
                       warnning = true;
                       break;
                    }
                    else if(tab_reg[0]==0)
                        break;
                }

                //check fail is fulled?
                while(this->Exit != true && warnning == false)
                {
                    if(count_fail==fail_tray_num)
                    {
                        cout << "The fail tray is full" << endl;
                        modbus_write_register(ctx,fail_full_addr,2);
                        count_fail=0;
                        tab_reg[0]=0;
                        while(this->Exit!=true && warnning == false)
                        {
                            cout<<"check 219!!!!!!!!!!!!!!!!!\n";
                            modbus_read_registers(ctx,fail_inpart_addr,1,tab_reg);
                            if(tab_reg[0]==1)
                            {
                                tab_reg[0]=0;
                                break;
                            }
                            else if(tab_reg[0]==2)
                            {
                                tab_reg[0]=0;
                                warnning = true;
                                cout<<"in fail tray warnning is true"<<endl;
                                break;
                            }
                            tab_reg[0]=0;
                        }
                        break;
                    }
                    else
                    {
                        modbus_write_register(ctx,fail_full_addr,1);
                        tab_reg[0]=0;
                        break;
                    }
                }

                while(warnning == true)
                {
                    modbus_write_register(ctx,tray_full_anaddr,0);
                    modbus_write_register(ctx,fail_full_addr,0);
                    modbus_write_register(ctx,all_addr,0);
                    modbus_write_register(ctx,leftabsorb_done_addr,0);
                    modbus_write_register(ctx,cell_anaddr,0);
                    modbus_write_register(ctx,fail_full_addr,0);


                    modbus_mapping_free(data_mapping);
                    modbus_free(ctx);
                    cout << "when warnning is true mythread6 connect closed!" << endl;
                    break;
                }

            }

            this->allcount=this->allcount+1;
        }

    }
    else
    {
        fprintf(stderr, "Connexion failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);

        emit NumberChanged6(-2);    //***
    }
    cout<<"mThread6 is finished!!!!!!!!!!!"<<endl;

}
