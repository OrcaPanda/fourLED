#ifndef RIGHTCLICKBUTTON_H
#define RIGHTCLICKBUTTON_H


#include <QPushButton>
#include <QMouseEvent>

class RightClickButton : public QPushButton
{
    Q_OBJECT

public:
    explicit RightClickButton(QWidget *parent = 0);

private slots:
    void mousePressEvent(QMouseEvent *e);

signals:
    void rightClicked();
    void leftClicked();
public slots:

};

#endif // RIGHTCLICKBUTTON_H
