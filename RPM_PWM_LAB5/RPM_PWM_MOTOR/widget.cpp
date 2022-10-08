#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    rpmset=500;
    qDebug()<<"START APP";
    apunport=new QSerialPort(this);
    foreach(const QSerialPortInfo &serialPortInfo,QSerialPortInfo::availablePorts())
    {
        QString portname=serialPortInfo.portName();
        ui->comboBox->addItem(portname);
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_horizontalSlider_valueChanged(int value)
{
    ui->label_2->setNum(value);
    rpmset=value;
}

void Widget::on_pushButton_2_clicked()
{
    if((ui->lineEdit->text().toInt()<5)||(ui->lineEdit->text().toInt()>1000))
    {
        QMessageBox msg;
        msg.setText("Introduzca un valor valido, debe estar entre 5 y 1000");
        msg.exec();
    }else{
        rpmset=ui->lineEdit->text().toInt();
        ui->horizontalSlider->setValue(ui->lineEdit->text().toInt());
    }
}
void Widget::readport()
{
    for(k=0;k<8;k++)
    {
        pack[k]=apunport->readAll()[k];
    }
    rpmsrecive=((pack[2]<<8)&0xF00)|((pack[3]<<4)&0xF0)|(pack[4]);
    ui->label_3->setNum(rpmsrecive);
}

void Widget::openport()
{
    QString apunport_name=ui->comboBox->currentText();

    if(ui->pushButton->text() == "Abrir Puerto")
    {
        apunport->setPortName(apunport_name);
        apunport->setBaudRate(QSerialPort::Baud115200);
        apunport->setDataBits(QSerialPort::Data8);
        apunport->setFlowControl(QSerialPort::NoFlowControl);
        apunport->setParity(QSerialPort::NoParity);
        apunport->setStopBits(QSerialPort::OneStop);
        apunport->open(QSerialPort::ReadWrite);

        connect(apunport,SIGNAL(readyRead()),this,SLOT(readport()));

        ui->pushButton->setText("Cerrar Puerto");
    }else{
        apunport->close();
        disconnect(apunport,SIGNAL(readyRead()),this,SLOT(readport()));
        ui->pushButton->setText("Abrir Puerto");
    }
}

void Widget::on_pushButton_clicked()
{
    openport();
}

void Widget::on_pushButton_3_clicked()
{
    for(k=0;k<3;k++)
    {
        seprpm[k]=0;
    }
    itoa(rpmset,seprpm,16);
    for(k=0;k<3;k++)
    {
        if(seprpm[k]<58)
        {
            seprpm[k]=seprpm[k]-48;
        }
        else
        {
            seprpm[k]=seprpm[k]-87;
        }
    }
    packet[0]=0x16;
    packet[1]=0x07;
    packet[2]=seprpm[0];
    packet[3]=seprpm[1];
    packet[4]=seprpm[2];
    cs=0;
    for(k=0;k<5;k++)
    {
        cs=cs+packet[k];
    }
    packet[5]=cs;
    packet[6]=0x19;
    if(apunport->isOpen())
    {
        apunport->write(packet,strlen(packet));
    }else{
        QMessageBox msg;
        msg.setText("El puerto esta cerrado :/");
        msg.exec();
    }
}

