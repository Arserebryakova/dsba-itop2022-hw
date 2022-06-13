#ifndef SHOW_H
#define SHOW_H

#include "mainwindow.h"
#include <QDialog>
#include <QLabel>

class show : public QDialog
{
    Q_OBJECT
    QLabel *id;
    QLabel *gender;
    QLabel *car;
    QLabel *realty;
    QLabel *children;
    QLabel *income;
    QLabel *education;

public:
    explicit show(QWidget *parent = nullptr);
    ~show();

signals:
    void update_cart_sum();

private slots:
    void recieveData(Bank& var);
};

#endif // SHOW_H
