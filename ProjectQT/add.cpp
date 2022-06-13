#include "add.h"
#include "incorrect.h"
#include "mainwindow.h"
#include <QDialog>
#include <QDebug>
#include <QGridLayout>
#include <fstream>
#include <sstream>

add::add(QWidget *parent) :
    QDialog(parent)
{
    QGridLayout *window = new QGridLayout(this);

    for(int i = 0; i < 7; i++) {
        QLineEdit *ln = new QLineEdit();
        QLabel *lb = new QLabel();
        ln->setStyleSheet(QString("font-size: %1px").arg(20));
        lb->setStyleSheet(QString("font-size: %1px").arg(20));
        lines.push_back(ln);
        labels.push_back(lb);
    }

    labels[0]->setText("Id: ");
    labels[1]->setText("Gender: ");
    labels[2]->setText("Own car: ");
    labels[3]->setText("Own realty: ");
    labels[4]->setText("Amount of children: ");
    labels[5]->setText("Annual income: ");
    labels[6]->setText("Education level: ");

    send_button = new QPushButton;
    send_button->setStyleSheet(QString("font-size: %1px").arg(30));
    send_button->setText("Apply");

    connect(send_button, SIGNAL(clicked()), this, SLOT(apply_data()));

    for(int i = 0; i < 7; i++)
    {
        window->addWidget(labels[i]);
        window->addWidget(lines[i]);
    }
    window->addWidget(send_button);
}

void add::apply_data()
{
    try
    {
        Bank res;
        bool flag = true;
        for(int i = 0; i < 7; i++){
            res.data[i] = lines[i]->text();
            if(res.data[i] == ""){
                incorrect a;
                a.exec();
                flag = false;
                break;
            }
        }
        if(flag){
            emit send_data(res, Row);
            close();
        }
    }
    catch(...)
    {
        close();
    }

}

void add::recieve_data(Bank & var, size_t &row)
{
    Row = row;
    for(int i = 0; i < 7; i++)
        lines[i]->setText(var.data[i].toString());
}

add::~add()
{

}
