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
    tim=new QTimer(this);
    connect(tim,SIGNAL(timeout()),this,SLOT(xx()));
    tim->start(500);
    scanports();
    initbarandpack();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::xx()
{
    if(css->isOpen())
    {
        qDebug()<<"Preguntando Datos...";
        css->write(pts,pts[1]);
    }else{
        tim->start(5000);
        QMessageBox msg;
        msg.setText("El puerto esta cerrado :/, por favor abralo");
        msg.exec();
    }
}


void Widget::readport()
{
    buf=css->readAll();

    if(((((buf[3]<<8)&0x00FF)|((buf[4])&0x00FF))/4)<590)//c0
    {
        tim->stop();
        pts[4]=0x72;
    }else{
        pts[4]=0x27;
        tim->start(500);

        g=QString::number((((buf[5]<<8)&0xFF00)|((buf[6])&0x00FF))/4); //c1
        ui->label->setStyleSheet(bc+g+c+g+c+g+f);

        g=QString::number((((buf[7]<<8)&0xFF00)|((buf[8])&0x00FF))/4); //c2
        ui->label_2->setStyleSheet(bc+g+c+g+c+g+f);

        g=QString::number((((buf[9]<<8)&0xFF00)|((buf[10])&0x00FF))/4); //c3
        ui->label_3->setStyleSheet(bc+g+c+g+c+g+f);

        g=QString::number((((buf[11]<<8)&0xFF00)|((buf[12])&0x00FF))/4); //c4
        ui->label_4->setStyleSheet(bc+g+c+g+c+g+f);

        g=QString::number((((buf[13]<<8)&0xFF00)|((buf[14])&0x00FF))/4); //c5
        ui->label_5->setStyleSheet(bc+g+c+g+c+g+f);

        g=QString::number((((buf[15]<<8)&0xFF00)|((buf[16])&0x00FF))/4); //c6
        ui->label_6->setStyleSheet(bc+g+c+g+c+g+f);

        g=QString::number((((buf[17]<<8)&0xFF00)|((buf[18])&0x00FF))/4); //c7
        ui->label_7->setStyleSheet(bc+g+c+g+c+g+f);

        g=QString::number((((buf[19]<<8)&0xFF00)|((buf[20])&0x00FF))/4); //c8
        ui->label_8->setStyleSheet(bc+g+c+g+c+g+f);//*/
    }
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

void Widget::initbarandpack()
{
    pts[0]=0x16; //inicio
    pts[1]=0x07; //tamaño
    pts[2]=0x09; // Inicio Data
    pts[3]=0x21; // Preguntar datos
    pts[4]=0x27; // Bateria check
    pts[5]=0x6E; // checksum
    pts[6]=0x19; // Final
    g=QString::number(0);
    ui->label->setStyleSheet(bc+g+c+g+c+g+f);
    ui->label_8->setStyleSheet(bc+g+c+g+c+g+f);
    g=QString::number(85);
    ui->label_2->setStyleSheet(bc+g+c+g+c+g+f);
    ui->label_7->setStyleSheet(bc+g+c+g+c+g+f);
    g=QString::number(170);
    ui->label_3->setStyleSheet(bc+g+c+g+c+g+f);
    ui->label_6->setStyleSheet(bc+g+c+g+c+g+f);
    g=QString::number(255);
    ui->label_4->setStyleSheet(bc+g+c+g+c+g+f);
    ui->label_5->setStyleSheet(bc+g+c+g+c+g+f);

}
void Widget::on_pushButton_2_clicked()
{
    scanports();
}

void Widget::on_pushButton_clicked()
{
    openport();
}

void Widget::on_horizontalSlider_sliderMoved(int position)
{
    g=QString::number(position);
    ui->label->setStyleSheet(bc+g+c+g+c+g+f);
    ui->label_2->setStyleSheet(bc+g+c+g+c+g+f);
    ui->label_3->setStyleSheet(bc+g+c+g+c+g+f);
    ui->label_4->setStyleSheet(bc+g+c+g+c+g+f);
    ui->label_5->setStyleSheet(bc+g+c+g+c+g+f);
    ui->label_6->setStyleSheet(bc+g+c+g+c+g+f);
    ui->label_7->setStyleSheet(bc+g+c+g+c+g+f);
    ui->label_8->setStyleSheet(bc+g+c+g+c+g+f);
}
