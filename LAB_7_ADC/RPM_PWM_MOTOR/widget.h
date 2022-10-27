#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVector>
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


    void makePlot(double rpm,double corriente_ma);
private:
    Ui::Widget *ui;
    void setupPlot();
    //double x[100],y[100];
    QVector<double> x;
    QVector<double> y;
    QVector<double> z;
    QVector<double> w;
    int rpmset=0,rpmsrecive=0,rpmssend=0,adc=0;
    char packet[8],cs=0,k=0,tempo;
    QSerialPort *apunport=nullptr;
    void openport();
};

#endif // WIDGET_H
