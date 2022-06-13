#include "show.h"
#include "mainwindow.h"
#include <QDialog>
#include <QGridLayout>
#include <fstream>
#include <sstream>

show::show(QWidget *parent) :
    QDialog(parent)
{
    QGridLayout *window = new QGridLayout(this);

    id = new QLabel;
    gender = new QLabel;
    car = new QLabel;
    realty = new QLabel;
    children = new QLabel;
    income = new QLabel;
    education = new QLabel;

    id->setStyleSheet(QString("font-size: %1px").arg(20));
    gender->setStyleSheet(QString("font-size: %1px").arg(20));
    car->setStyleSheet(QString("font-size: %1px").arg(20));
    realty->setStyleSheet(QString("font-size: %1px").arg(20));
    children->setStyleSheet(QString("font-size: %1px").arg(20));
    income->setStyleSheet(QString("font-size: %1px").arg(20));
    education->setStyleSheet(QString("font-size: %1px").arg(20));

    window->addWidget(id);
    window->addWidget(gender);
    window->addWidget(car);
    window->addWidget(realty);
    window->addWidget(children);
    window->addWidget(income);
    window->addWidget(education);
}
void show::recieveData(Bank& air)
{
    id->setText(("Id: " + air.data[0].toString().toStdString()).c_str());
    gender->setText(("Gender: " + air.data[1].toString().toStdString()).c_str());
    car->setText(("Own car: " + air.data[2].toString().toStdString()).c_str());
    realty->setText(("Own realty: " + air.data[3].toString().toStdString()).c_str());
    children->setText(("Amount of children: " + air.data[4].toString().toStdString()).c_str());
    income->setText(("Annual income: " + air.data[5].toString().toStdString()).c_str());
    education->setText(("Education level: " + air.data[6].toString().toStdString()).c_str());
}

show::~show()
{
}
