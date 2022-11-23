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
    initbarandpack();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::readport()
{
    buf=css->readAll();

    QString accion="malo";
    int m=1;

    switch(buf[3])
    {
        case 0x22:
            accion="CONEXIÓN";
            m=0;
        break;
        case 0x33:
            accion="CALIBRACIÓN";
            m=0;
            ui->label_28->setNum(((buf[6]<<8)&0xFF00)|((buf[7])&0xFF));
            ui->label_27->setNum(((buf[8]<<8)&0xFF00)|((buf[9])&0xFF));
            ui->label_26->setNum(((buf[10]<<8)&0xFF00)|((buf[11])&0xFF));
            ui->label_25->setNum(((buf[12]<<8)&0xFF00)|((buf[13])&0xFF));
            ui->label_24->setNum(((buf[14]<<8)&0xFF00)|((buf[15])&0xFF));
            ui->label_23->setNum(((buf[16]<<8)&0xFF00)|((buf[17])&0xFF));
            ui->label_22->setNum(((buf[18]<<8)&0xFF00)|((buf[19])&0xFF));
            ui->label_21->setNum(((buf[20]<<8)&0xFF00)|((buf[21])&0xFF));
            ui->label_13->setNum(((buf[23]<<8)&0xFF00)|((buf[24])&0xFF));
            ui->label_14->setNum(((buf[25]<<8)&0xFF00)|((buf[26])&0xFF));
            ui->label_15->setNum(((buf[27]<<8)&0xFF00)|((buf[28])&0xFF));
            ui->label_16->setNum(((buf[29]<<8)&0xFF00)|((buf[30])&0xFF));
            ui->label_17->setNum(((buf[31]<<8)&0xFF00)|((buf[32])&0xFF));
            ui->label_18->setNum(((buf[33]<<8)&0xFF00)|((buf[34])&0xFF));
            ui->label_19->setNum(((buf[35]<<8)&0xFF00)|((buf[36])&0xFF));
            ui->label_20->setNum(((buf[37]<<8)&0xFF00)|((buf[38])&0xFF));
        break;
        case 0x44:
            accion="PRUEBA DE MOTORES";
            m=0;
        break;
        case 0x55:
            m=1;
            contador++;
            ui->label_10->setText("Estoy corriendo==> "+QString::number(contador));
        break;
        case 0x66:
            accion="CARRERA";
            m=0;
        break;
        case 0x77:
            m=1;
            g=QString::number((((buf[5]<<8)&0xFF00)|((buf[6])&0x00FF))/16); //c1
            ui->label->setStyleSheet(bc+g+c+g+c+g+f);

            g=QString::number((((buf[7]<<8)&0xFF00)|((buf[8])&0x00FF))/16); //c2
            ui->label_2->setStyleSheet(bc+g+c+g+c+g+f);

            g=QString::number((((buf[9]<<8)&0xFF00)|((buf[10])&0x00FF))/16); //c3
            ui->label_3->setStyleSheet(bc+g+c+g+c+g+f);

            g=QString::number((((buf[11]<<8)&0xFF00)|((buf[12])&0x00FF))/16); //c4
            ui->label_4->setStyleSheet(bc+g+c+g+c+g+f);

            g=QString::number((((buf[13]<<8)&0xFF00)|((buf[14])&0x00FF))/16); //c5
            ui->label_5->setStyleSheet(bc+g+c+g+c+g+f);

            g=QString::number((((buf[15]<<8)&0xFF00)|((buf[16])&0x00FF))/16); //c6
            ui->label_6->setStyleSheet(bc+g+c+g+c+g+f);

            g=QString::number((((buf[17]<<8)&0xFF00)|((buf[18])&0x00FF))/16); //c7
            ui->label_7->setStyleSheet(bc+g+c+g+c+g+f);

            g=QString::number((((buf[19]<<8)&0xFF00)|((buf[20])&0x00FF))/16); //c8
            ui->label_8->setStyleSheet(bc+g+c+g+c+g+f);
        break;
        case 0x27:
            m=1;
            if((((buf[5]<<8)&0xFF00)|((buf[6])&0xFF))<2)
            {
                accion="baja";
            }else{
                accion="bien";
            }
            ui->label_30->setText("la bateria esta "+accion);
            accion="El valor ADC de la bateria es: "+QString::number(((buf[5]<<8)&0xFF00)|((buf[6])&0xFF));
            ui->label_29->setText(accion);
        break;
    }
    if(m==0)
    {
        ui->label_9->setText("La acción "+accion+" ha sido completada.");
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
    pts[0]=0x16; // Inicio
    pts[1]=0x05; // Tamaño
    pts[2]=0x00; // Estado Carro
    pts[3]=0x69; // Checksum
    pts[4]=0x19; // Final

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


void Widget::on_pushButton_3_clicked()
{
  if(css->isOpen())
  {
      pts[2]=0x33;
      css->write(pts,pts[1]);
  }else{
      QMessageBox msg;
      msg.setText("El puerto esta cerrado :/");
      msg.exec();
  }
}

void Widget::on_pushButton_4_clicked()
{
    if(css->isOpen())
    {
        pts[2]=0x22;
        css->write(pts,pts[1]);
    }else{
        QMessageBox msg;
        msg.setText("El puerto esta cerrado :/");
        msg.exec();
    }
}

void Widget::on_pushButton_5_clicked()
{
    if(css->isOpen())
    {
        pts[2]=0x44;
        css->write(pts,pts[1]);
    }else{
        QMessageBox msg;
        msg.setText("El puerto esta cerrado :/");
        msg.exec();
    }
}

/*//if(((((buf[3]<<8)&0x00FF)|((buf[4])&0x00FF))/4)<590)//c0
//{
    //pts[4]=0x72;
  //  css->write(pts,pts[1]);
//}else{
    pts[4]=0x27;

    g=QString::number((((buf[5]<<8)&0xFF00)|((buf[6])&0x00FF))/16); //c1
    ui->label->setStyleSheet(bc+g+c+g+c+g+f);

    g=QString::number((((buf[7]<<8)&0xFF00)|((buf[8])&0x00FF))/16); //c2
    ui->label_2->setStyleSheet(bc+g+c+g+c+g+f);

    g=QString::number((((buf[9]<<8)&0xFF00)|((buf[10])&0x00FF))/16); //c3
    ui->label_3->setStyleSheet(bc+g+c+g+c+g+f);

    g=QString::number((((buf[11]<<8)&0xFF00)|((buf[12])&0x00FF))/16); //c4
    ui->label_4->setStyleSheet(bc+g+c+g+c+g+f);

    g=QString::number((((buf[13]<<8)&0xFF00)|((buf[14])&0x00FF))/16); //c5
    ui->label_5->setStyleSheet(bc+g+c+g+c+g+f);

    g=QString::number((((buf[15]<<8)&0xFF00)|((buf[16])&0x00FF))/16); //c6
    ui->label_6->setStyleSheet(bc+g+c+g+c+g+f);

    g=QString::number((((buf[17]<<8)&0xFF00)|((buf[18])&0x00FF))/16); //c7
    ui->label_7->setStyleSheet(bc+g+c+g+c+g+f);

    g=QString::number((((buf[19]<<8)&0xFF00)|((buf[20])&0x00FF))/16); //c8
    ui->label_8->setStyleSheet(bc+g+c+g+c+g+f);//*/
//}*/
