//modbus teaching mode
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "libmodbus/modbus.h"

#include "mythread7.h"
#include<iostream>
#include <QDebug>
using namespace std;

#define MODBUS_SERIAL_DEV       "/dev/ttyUSB1"
//#define MODBUS_SERIAL_BAUDRATE  9600            /* 9600, 38400, 115200, ... */
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

MyThread7::MyThread7(QObject *parent) : QThread(parent){}

void MyThread7::set_PLC_Dev(QString dev)
{
    this->dev = dev;    //get "the QString value" of ui of dialog.cpp
}

void MyThread7::run()
{
    modbus_t            *ctx;
    modbus_mapping_t    *data_mapping;
    //struct timeval      timeout;

    //char *dev = this->dev.toLatin1().data();    //dev(QThread) get the "this->dev" value of mThread7(dialog)
    //char *parity = this->parity;

    //int16_t    tab_reg[64]={0};

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
    modbus_set_slave(ctx, this->id);

    //Debug mode
    modbus_set_debug(ctx, MODBUS_DEBUG);

    data_mapping = modbus_mapping_new(MODBUS_RO_BITS, MODBUS_RW_BITS,
                                      MODBUS_RO_REGISTERS, MODBUS_RW_REGISTERS);

    if (data_mapping == NULL){
        fprintf(stderr, "Failed to allocate the mapping: %s\n", modbus_strerror(errno));
        modbus_free(ctx);

        emit NumberChanged7(-1);
    }


    //open serial interface
    modbus_connect(ctx);

    if (modbus_connect(ctx) != -1)
    {
        emit NumberChanged7(-3);

        this->leftabsorb7 = false;
        this->rightabsorb7 = false;
        this->leftdeflate7 = false;
        this->rightdeflate7 = false;
        this->down7 = false;
        this->up7 = false;

        while(this->Exit != true)
        {
            if(this->Exit == true)
            {
                modbus_mapping_free(data_mapping);
                modbus_free(ctx);
                cout << "mythread7 connect closed!" << endl;
                this->Exit = false;
                break;
            }
            else
            {
                if(this->leftabsorb7 == true)
                {
                    this->leftabsorb7 = false;
                    modbus_write_register(ctx,1234,2);
                }

                if(this->rightabsorb7 == true)
                {
                    this->rightabsorb7 = false;
                    modbus_write_register(ctx,1234,3);
                }

                if(this->leftdeflate7 == true)
                {
                    this->leftdeflate7 = false;
                    modbus_write_register(ctx,1234,5);
                }

                if(this->rightdeflate7 == true)
                {
                    this->rightdeflate7 = false;
                    modbus_write_register(ctx,1234,6);
                }

                if(this->down7 == true)
                {
                    this->down7 = false;
                    modbus_write_register(ctx,1234,1);
                }

                if(this->up7 == true)
                {
                    this->up7 = false;
                    modbus_write_register(ctx,1234,4);
                }


            }

        }

    }
    else
    {
        fprintf(stderr, "Connexion failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);

        emit NumberChanged7(-2);
    }

}
