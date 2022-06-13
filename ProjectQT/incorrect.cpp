#include "incorrect.h"
#include "mainwindow.h"
#include <QDialog>
#include <QGridLayout>
#include <fstream>
#include <sstream>
incorrect::incorrect(QWidget *parent) :
    QDialog(parent)
{

    QGridLayout *window = new QGridLayout(this);

    text = new QLabel;
    text->setText("Inproper data entered");
    text->setStyleSheet(QString("font-size: %1px").arg(30));

    window->addWidget(text);

}

incorrect::~incorrect()
{
}
