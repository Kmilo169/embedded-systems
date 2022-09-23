#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QtSerialPort>

namespace Ui {
class Widget;
}

class Widget : public QMainWindow
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void readSerial();

private:
    Ui::Widget *ui;
    int contador=0;
    double voltaje=0;
    QSerialPort *puertecito=nullptr;
    void openport();
};

#endif // WIDGET_H
