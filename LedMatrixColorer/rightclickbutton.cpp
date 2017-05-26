#include "rightclickbutton.h"

RightClickButton::RightClickButton(QWidget *parent) :
    QPushButton(parent)
{
}

void RightClickButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::RightButton){
        emit rightClicked();
    }
    if(e->button()==Qt::LeftButton){
        emit leftClicked();
    }
}
