#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    percent=50;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    packet[0]=0x16;
    packet[1]=0x05;
    packet[2]=percent;
    cs=0;
    for(k=0;k<3;k++)
    {
        cs=cs+packet[k];
    }
    packet[3]=cs;
    packet[4]=0x19;
    qDebug()<<"Porcentaje y checksum:"<<packet[2]<<" y "<<packet[3];
}

void Widget::on_horizontalSlider_valueChanged(int value)
{
    percent=value;
    ui->label->setNum(percent);
}


void Widget::on_pushButton_2_clicked()
{
    percent=ui->lineEdit->text().toInt();
    ui->horizontalSlider->setValue(ui->lineEdit->text().toInt());
}
