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
    percent=50;
    qDebug()<<"START APP";
    apunport=new QSerialPort(this);
    foreach(const QSerialPortInfo &serialPortInfo,QSerialPortInfo::availablePorts())
    {
        QString portname=serialPortInfo.portName();
        qDebug()<<portname;
        ui->comboBox->addItem(portname);
    }
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
    if(apunport->isOpen())
    {
        apunport->write(packet,strlen(packet));
    }else{
        QMessageBox msg;
        msg.setText("El puerto esta cerrado :/");
        msg.exec();
    }
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

void Widget::on_pushButton_3_clicked()
{
    openport();
}

void Widget::readport()
{
    QByteArray infor=apunport->readAll();
    qDebug()<<infor;
    ui->label_3->setText(infor);
}

void Widget::openport()
{
    QString apunport_name=ui->comboBox->currentText();

    if(ui->pushButton_3->text() == "Abrir Puerto")
    {
        apunport->setPortName(apunport_name);
        apunport->setBaudRate(QSerialPort::Baud115200);
        apunport->setDataBits(QSerialPort::Data8);
        apunport->setFlowControl(QSerialPort::NoFlowControl);
        apunport->setParity(QSerialPort::NoParity);
        apunport->setStopBits(QSerialPort::OneStop);
        apunport->open(QSerialPort::ReadWrite);

        connect(apunport,SIGNAL(readyRead()),this,SLOT(readport()));

        ui->pushButton_3->setText("Cerrar Puerto");
    }else{
        apunport->close();
        disconnect(apunport,SIGNAL(readyRead()),this,SLOT(readport()));
        ui->pushButton_3->setText("Abrir Puerto");
    }
}
