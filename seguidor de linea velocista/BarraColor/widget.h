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
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void readport();

    void on_horizontalSlider_sliderMoved(int position);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::Widget *ui;
    QSerialPort *css=nullptr;
    void openport();
    void scanports();
    void initbarandpack();
    QString bc{"background-color: rgb("},g,c{","},f{")"};
    QByteArray buf;
    char pts[5];
    int contador=0;
};

#endif // WIDGET_H
