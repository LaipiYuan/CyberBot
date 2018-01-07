#-------------------------------------------------
#
# Project created by QtCreator 2015-07-24T13:40:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RobotGui
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    mythread1.cpp \
    mythread2.cpp \
    mythread3.cpp \
    serialport.cpp \
    dialog2.cpp \
    mythread4.cpp \
    mythread5.cpp \
    mythread6.cpp \
    libmodbus/modbus.c \
    libmodbus/modbus-data.c \
    libmodbus/modbus-rtu.c \
    libmodbus/modbus-tcp.c \
    mythread8.cpp \
    mythread7.cpp

HEADERS  += dialog.h \
    mythread1.h \
    mythread2.h \
    mythread3.h \
    dynamixel.h \
    serialport.h \
    dialog2.h \
    mythread4.h \
    mythread5.h \
    mythread6.h \
    libmodbus/config.h \
    libmodbus/modbus.h \
    libmodbus/modbus-private.h \
    libmodbus/modbus-rtu.h \
    libmodbus/modbus-rtu-private.h \
    libmodbus/modbus-tcp.h \
    libmodbus/modbus-tcp-private.h \
    libmodbus/modbus-version.h \
    mythread8.h \
    mythread7.h

FORMS    += dialog.ui \
    dialog2.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/release/ -lwiringPi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/debug/ -lwiringPi
else:unix: LIBS += -L$$PWD/ -lwiringPi

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
