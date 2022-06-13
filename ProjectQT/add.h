#ifndef ADD_H
#define ADD_H

#include "mainwindow.h"
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class add : public QDialog
{
    Q_OBJECT
    QPushButton *send_button;
    std::vector<QLineEdit*> lines;
    std::vector<QLabel*> labels;

public:
    explicit add(QWidget *parent = nullptr);
    ~add();
    size_t Row = -1;

signals:
    void send_data(Bank &var, size_t &Row);


private slots:
    void apply_data();

public slots:
    void recieve_data(Bank &var, size_t &row);
};

#endif // ADD_H
