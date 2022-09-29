#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QSerialPort>

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
    void on_pushButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void readport();
private:
    Ui::Widget *ui;
    int percent=0;
    char packet[5],cs=0,k=0;
    QSerialPort *apunport=nullptr;
    void openport();
};

#endif // WIDGET_H
