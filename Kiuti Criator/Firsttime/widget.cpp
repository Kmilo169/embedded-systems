#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    contador=100;
    qDebug()<<"El valor del contador es:"<<contador;
    qDebug()<<"Start APP";
    puertecito=new QSerialPort(this);
    foreach(const QSerialPortInfo &infopuertecito, QSerialPortInfo::availablePorts())
    {
        QString nombrepuerto=infopuertecito.portName();
        qDebug()<<"Nombre del puerto: "<<nombrepuerto;
        ui->comboBox->addItem(nombrepuerto);
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    openport();
}


void Widget::on_pushButton_2_clicked()
{
    int voltaje2=ui->lineEdit->text().toInt();
    QByteArray mensaje;
    mensaje.append(voltaje2);
    if(puertecito->isOpen())
    {
        puertecito->write(mensaje);
    }else{
        QMessageBox ven;
        ven.setText("Puerto Cerrado");
        ven.exec();
    }

}

void Widget::on_horizontalSlider_valueChanged(int value)
{
    voltaje=value;
    qDebug()<<"Voltaje:"<<voltaje;
    QString voltaje_txt=QString::number(value);
    ui->label_2->setText(voltaje_txt);
}

void Widget::readSerial()
{
    QByteArray buffer=puertecito->readAll();
    qDebug()<<buffer;
}

void Widget::openport()
{
    QString puertoname=ui->comboBox->currentText();
    if(ui->pushButton->text() == "Abrir Puerto")
    {
        puertecito->setPortName(puertoname);
        puertecito->setBaudRate(QSerialPort::Baud115200);
        puertecito->setDataBits(QSerialPort::Data8);
        puertecito->setFlowControl(QSerialPort::NoFlowControl);
        puertecito->setParity(QSerialPort::NoParity);
        puertecito->setStopBits(QSerialPort::OneStop);
        puertecito->open(QSerialPort::ReadWrite);
        connect(puertecito,SIGNAL(readyRead()),this,SLOT(readSerial()));

        ui->pushButton->setText("Cerrar puerto");

    }
    else
    {
        puertecito->close();
        disconnect(puertecito,SIGNAL(readyRead()),this,SLOT(readSerial()));
        ui->pushButton->setText("Abrir puerto");
    }
}
