#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QtSerialPort>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_2_clicked();

    void readport();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Widget *ui;
    int rpmset=0,rpmsrecive=0;
    char packet[8],pack[8],cs=0,k=0,seprpm[3];
    QSerialPort *apunport=nullptr;
    void openport();
};

#endif // WIDGET_H
