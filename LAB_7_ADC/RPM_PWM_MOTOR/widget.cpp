#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QVector>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    rpmset=500;
    rpmsrecive=0;
    qDebug()<<"START APP";
    apunport=new QSerialPort(this);
    foreach(const QSerialPortInfo &serialPortInfo,QSerialPortInfo::availablePorts())
    {
        QString portname=serialPortInfo.portName();
        ui->comboBox->addItem(portname);
    }
    Widget::setupPlot();
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
    if((ui->lineEdit->text().toInt()<0)||(ui->lineEdit->text().toInt()>1000))
    {
        QMessageBox msg;
        msg.setText("Introduzca un valor valido, debe estar entre 0 y 1000");
        msg.exec();
    }else{
        rpmset=ui->lineEdit->text().toInt();
        ui->horizontalSlider->setValue(ui->lineEdit->text().toInt());
    }
}
void Widget::readport()
{
    QByteArray resp=apunport->readAll();

    rpmsrecive=((resp[2]<<8)&0xF00)|((resp[3]<<4)&0xF0)|(resp[4]);
    adc=((resp[5]<<8)&0xF00)|((resp[6]<<4)&0xF0)|(resp[7]);
    makePlot((rpmsrecive-(rpmsrecive*0.4)),adc);
    uint16_t prueba=0;
    qDebug()<<"xx";
    qDebug()<<resp[2]+65<<" - "<<resp[3]+65<<" - "<<resp[4]+65;
    prueba=((resp[2]<<8)&0xF00)|((resp[3]<<4)&0xF0)|((resp[4])&0xF);
    qDebug()<<prueba;
    qDebug()<<"----------------------------------------------------------------------";
    qDebug()<<"xx";
    qDebug()<<"======================================================================";
    qDebug()<<"xx1";
    qDebug()<<resp[5]+65<<" - "<<resp[6]+65<<" - "<<resp[7]+65;
    prueba=((resp[5]<<8)&0xF00)|((resp[6]<<4)&0xF0)|((resp[7])&0xF);
    qDebug()<<prueba;

    ui->label_3->setNum(rpmsrecive-(rpmsrecive*0.4));
    ui->label_5->setNum(adc);
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
    packet[0]=0x16;
    packet[1]=0x08;
    packet[2]=0x21;
    if(rpmset==0)
    {
        packet[3]=0x00;
        packet[4]=0x00;
        packet[5]=0x00;
    }else{
        packet[3]=((rpmset>>8)&0x0F);
        packet[4]=((rpmset>>4)&0x0F);
        packet[5]=((rpmset)&0x0F);
    }
    qDebug()<<packet[3]+65;
    qDebug()<<packet[4]+65;
    qDebug()<<packet[5]+65;
    uint16_t pruebita=((packet[3]<<8)&0xF00)|((packet[4]<<4)&0xF0)|(packet[5]);
    qDebug()<<pruebita;
    cs=0;
    for(k=0;k<6;k++)
    {
        cs=cs+packet[k];
    }
    packet[6]=cs;
    packet[7]=0x19;
    if(apunport->isOpen())
    {
        apunport->write(packet,packet[1]);
    }else{
        QMessageBox msg;
        msg.setText("El puerto esta cerrado :/");
        msg.exec();
    }
}
void Widget::makePlot(double rpm, double corriente_ma)
{
    for (int i=0; i<100; ++i)
    {
      y[i] = y[i + 1];
      w[i] = w[i + 1];
    }
    y[100] = rpm;
    w[100] = corriente_ma;
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->graph(1)->setData(z, w);
    ui->customPlot->replot();
}
void Widget::setupPlot(){

    x.resize(101);
    y.resize(101);
    z.resize(101);
    w.resize(101);
    for (int i=0; i<101; ++i)
    {
      x[i] = (double)i;
      y[i] = (double)2;
      z[i] = (double)i;
      w[i] = (double)4;
    }

    ui->customPlot->addGraph(ui->customPlot->xAxis, ui->customPlot->yAxis);
    ui->customPlot->addGraph(ui->customPlot->xAxis, ui->customPlot->yAxis2);
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->graph(1)->setData(z, w);
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot->graph(0)->setName("RPM");
    ui->customPlot->graph(1)->setName("Corriente");
    ui->customPlot->plotLayout()->insertRow(0);
    ui->customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->customPlot, "Velocidad - Consumo Motor DC", QFont("sans", 12, QFont::Bold)));


    ui->customPlot->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
    //ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

    ui->customPlot->xAxis->setLabel("Time Relative");
    ui->customPlot->yAxis->setLabel("RPM.");
    ui->customPlot->xAxis->setRange(0, 100);
    ui->customPlot->yAxis->setRange(0, 1500);
    ui->customPlot->yAxis2->setLabel("mA");
    ui->customPlot->yAxis2->setRange(0, 100);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->replot();
}
