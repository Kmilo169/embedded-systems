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
    qDebug()<<"Start...";
    scanports();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::readport()
{
    uint32_t tiempo=0;
    buf=css->readAll();
    qDebug()<<"Hola:";
    tiempo=((buf[2]<<8)&0xFF00)|((buf[3])&0x00FF);
    QString tiempopalabra=QString::number(tiempo/1000);
    QMessageBox msg;
    msg.setText("Ey, esperate faltan "+tiempopalabra+" segundos");
    msg.exec();
    qDebug()<<"Faltan (seg):"<<tiempo/1000;
}

void Widget::on_pushButton_clicked()
{
    openport();
}

void Widget::openport()
{
    QString ptoo=ui->comboBox->currentText();
    if(("Abrir Puerto")==(ui->pushButton->text()))
    {
        css->setPortName(ptoo);
        css->setBaudRate(QSerialPort::Baud115200);
        css->setDataBits(QSerialPort::Data8);
        css->setFlowControl(QSerialPort::NoFlowControl);
        css->setParity(QSerialPort::NoParity);
        css->setStopBits(QSerialPort::OneStop);
        css->open(QSerialPort::ReadWrite);
        connect(css,SIGNAL(readyRead()),this,SLOT(readport()));
        ui->pushButton->setText("Cerrar Puerto");
    }else{
        css->close();
        disconnect(css,SIGNAL(readyRead()),this,SLOT(readport()));
        ui->pushButton->setText("Abrir Puerto");
    }
}

void Widget::scanports()
{
    ui->comboBox->clear();
    css=new QSerialPort(this);
    foreach(const QSerialPortInfo &infoport,QSerialPortInfo::availablePorts()) {
        QString portname=infoport.portName();
        ui->comboBox->addItem(portname);
    }
}
void Widget::on_pushButton_2_clicked()
{
    scanports();
}
