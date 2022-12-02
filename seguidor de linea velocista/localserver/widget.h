#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class LocalServer;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_iniciar_clicked();

    void on_enviar_clicked();

    void on_quitar_clicked();

private:
    Ui::Widget *ui;
    LocalServer *mLocalServer;
};

#endif // WIDGET_H
