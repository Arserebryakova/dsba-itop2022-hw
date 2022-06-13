#include "noshow.h"
#include "mainwindow.h"
#include <QDialog>
#include <QGridLayout>
#include <fstream>
#include <sstream>

noshow::noshow(QWidget *parent) :
    QDialog(parent)
{
    QGridLayout *window = new QGridLayout(this);

    text = new QLabel;
    text->setText("ID doesn't exist!");
    text->setStyleSheet(QString("font-size: %1px").arg(30));

    window->addWidget(text);
}

noshow::~noshow()
{
}
