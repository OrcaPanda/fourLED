#include "window.h"
#include <QApplication>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

Window::Window(QWidget *parent) : QWidget(parent){
    setFixedSize(663, 470);
    m_frame = new QFrame(this);
    m_frame -> setGeometry(10, 10, 180, 450);
    m_frame -> setFrameShape(QFrame::Box);
    m_frame -> setLineWidth(1);
    m_frame -> setFrameShadow(QFrame::Sunken);

    title = new QLabel(this);
    title -> setText("LED\nMatrix\nColorer");
    title -> setGeometry(30, 20, 140, 120);
    title -> setStyleSheet("font-size: 35px;");
    title -> setAlignment(Qt::AlignCenter);

    author = new QLabel(this);
    author -> setText("by Jianwei Sun");
    author -> setGeometry(30, 155, 140, 20);
    author -> setAlignment(Qt::AlignCenter);

    connect_status = new QLabel(this);
    connect_status -> setText("Disconnected");
    connect_status -> setStyleSheet("color:red;");
    connect_status -> setGeometry(30, 180, 140, 30);
    connect_status -> setAlignment(Qt::AlignCenter);

    connect_button = new QPushButton("Connect", this);
    connect_button -> setGeometry(30, 210, 140, 30);
    connect(connect_button, SIGNAL(clicked(bool)), this, SLOT(connectButtonClicked(bool)));

    color_frame = new QFrame(this);
    color_frame -> setGeometry(70, 260, 60, 60);
    color_frame -> setStyleSheet("background-color:red;");
    color_frame -> setFrameShape(QFrame::StyledPanel);
    color_frame -> setFrameShadow(QFrame::Raised);

    color_button = new QPushButton("Red", this);
    color_button -> setGeometry(30, 340, 140, 30);
    color = RED;
    connect(color_button, SIGNAL(clicked(bool)), this, SLOT(colorButtonClicked(bool)));

    clear_all_button = new QPushButton("Clear all", this);
    clear_all_button -> setGeometry(30, 380, 140, 30);
    connect(clear_all_button, SIGNAL(clicked(bool)), this, SLOT(clearAllButtonClicked(bool)));

    quit_button = new QPushButton("Quit", this);
    quit_button -> setGeometry(30, 420, 140, 30);
    connect(quit_button, SIGNAL(clicked(bool)), QApplication::instance(), SLOT(quit()));

    int row, col;
    signalMapper = new QSignalMapper(this);
    rightSignalMapper = new QSignalMapper(this);
    for(row = 0; row < 16; row++){
        for(col = 0; col < 16; col++){
            tiles[row][col] = new RightClickButton(this);
            tiles[row][col] -> setGeometry(220 + 26*row, 28 + 26*col, 24, 24);
            tiles[row][col] -> setStyleSheet("background-color:rgb(225,225,225); border: 1px inset; border-color:rgb(173,173,173);");
            connect(tiles[row][col], SIGNAL(leftClicked()), signalMapper, SLOT(map()));
            signalMapper -> setMapping(tiles[row][col], row*16+col);
            connect(tiles[row][col], SIGNAL(rightClicked()), rightSignalMapper, SLOT(map()));
            rightSignalMapper -> setMapping(tiles[row][col], row*16+col+256);
        }
    }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(updateColor(int)));
    connect(rightSignalMapper, SIGNAL(mapped(int)), this, SLOT(updateColor(int)));

    serialPort = new QSerialPort(this);
}

void Window::connectButtonClicked(bool clicked){
    if(connected){
        serialPort -> close();
        connected = false;
        connect_status -> setText("Disconnected");
        connect_status -> setStyleSheet("color:red;");
        connect_button -> setText("Connect");
        return;
    }
    QList<QSerialPortInfo> portsList = QSerialPortInfo::availablePorts();
    bool deviceExists = false;
    QSerialPortInfo targetPort;
    for(int i = 0; i < portsList.size(); i++){
        if(portsList.at(i).vendorIdentifier() == (quint16)0x2341 && portsList.at(i).productIdentifier() == (quint16)0x8036){
            deviceExists = true;
            targetPort = portsList.at(i);
            break;
        }
    }
    if(deviceExists == false){
        connect_status -> setText("Device not found");
        return;
    }
    serialPort -> setPort(targetPort);
    if(!serialPort -> open(QIODevice::WriteOnly)){
        connect_status -> setText("Port opening unsuccessful");
        return;
    }
    serialPort -> setBaudRate((qint32)115200);
    connected = true;
    connect_status -> setText("Connected");
    connect_status -> setStyleSheet("color:green;");
    connect_button -> setText("Disconnect");
    for(int row = 0; row < 16; row++){
        for(int col = 0; col < 16; col++){
            writeColor(row, col, colorState[row][col]);
        }
    }
}

void Window::colorButtonClicked(bool clicked){
    if(color == RED){
        color_button -> setText("Green");
        color = GREEN;
        color_frame -> setStyleSheet("background-color:green;");
    } else{
        color_button -> setText("Red");
        color = RED;
        color_frame -> setStyleSheet("background-color:red;");
    }
}

void Window::clearAllButtonClicked(bool clicked){
    for(int row = 0; row < 16; row++){
        for(int col = 0; col < 16; col++){
            tiles[row][col] -> setStyleSheet("background-color:rgb(225,225,225); border: 1px inset; border-color:rgb(173,173,173);");
            colorState[row][col] = '0';
            writeColor(row, col, colorState[row][col]);
        }
    }
}

void Window::updateColor(int id){
    int row, col, select;
    char dataChar;
    if(id >= 256){
        row = (id - 256) / 16;
        col = (id - 256) % 16;
        select = BLANK;
    } else {
        row = id / 16;
        col = id % 16;
        select = color;
    }
    switch(select){
        case BLANK: tiles[row][col] -> setStyleSheet("background-color:rgb(225,225,225); border: 1px inset; border-color:rgb(173,173,173);");
            dataChar = (char)'0';
            break;
        case RED: tiles[row][col] -> setStyleSheet("background-color:red; border: 1px inset;");
            dataChar = (char)'r';
            break;
        case GREEN: tiles[row][col] -> setStyleSheet("background-color: green;border: 1px inset;");
            dataChar = (char)'g';
            break;
        default: tiles[row][col] -> setStyleSheet("background-color:rgb(225,225,225); border: 1px inset; border-color:rgb(173,173,173);");
            dataChar = (char)'0';
    }
    colorState[row][col] = dataChar;
    writeColor(row, col, dataChar);
}

void Window::writeColor(int row, int col, char data){
    if(connected){
        char command[8] = {'S',' ', '0',' ', '0',' ', '0', '\r'};
        command[2] = col >= 10 ? (char)col - 10 + 'a' : (char)col + '0';
        command[4] = row >= 10 ? (char)row - 10 + 'a' : (char)row + '0';
        command[6] = data;
        serialPort -> write(command, sizeof(command));
    }
}
