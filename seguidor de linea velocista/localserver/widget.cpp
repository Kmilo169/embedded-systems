#include "widget.h"
#include "ui_widget.h"
#include "localserver.h"
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    mLocalServer=new LocalServer(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_iniciar_clicked()
{
    if(("LISTEN")==(ui->iniciar->text()))
    {
        ui->iniciar->setText("STOP");
        mLocalServer->listen(QHostAddress::Any,ui->puerto->value());
    }else{
        ui->iniciar->setText("LISTEN");
        mLocalServer->close();
    }
}
