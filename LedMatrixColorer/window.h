#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QSignalMapper>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "rightclickbutton.h"

#define BLANK 0
#define RED 1
#define GREEN 2

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
public slots:
private slots:
    void connectButtonClicked(bool);
    void colorButtonClicked(bool);
    void updateColor(int id);
    void clearAllButtonClicked(bool);
signals:
private:
    QFrame *m_frame, *color_frame;
    QPushButton *connect_button, *color_button, *clear_all_button, *quit_button;
    QLabel *title, *author, *connect_status;
    RightClickButton *tiles[16][16];
    QSignalMapper *signalMapper, *rightSignalMapper;
    QSerialPort *serialPort;
    int color;
    bool connected = false;
    char colorState[16][16] = {0};
    void writeColor(int, int, char);
};

#endif // WINDOW_H
