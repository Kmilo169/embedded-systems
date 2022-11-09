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

    void on_pushButton_2_clicked();

    void readport();

private:
    Ui::Widget *ui;
    QSerialPort *css=nullptr;
    void openport();
    void scanports();
    QByteArray buf;
};

#endif // WIDGET_H
