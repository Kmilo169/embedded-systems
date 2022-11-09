#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void xx();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void readport();

    void on_horizontalSlider_sliderMoved(int position);

private:
    Ui::Widget *ui;
    QSerialPort *css=nullptr;
    void openport();
    void scanports();
    void initbarandpack();
    QString bc{"background-color: rgb("},g,c{","},f{")"};
    QByteArray buf;
    QTimer *tim;
    char pts[7];
};

#endif // WIDGET_H
