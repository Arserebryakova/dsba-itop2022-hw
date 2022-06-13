#ifndef INCORRECT_H
#define INCORRECT_H

#include "mainwindow.h"
#include <QDialog>
#include <QLabel>

class incorrect : public QDialog
{
    Q_OBJECT
    QLabel *text;

public:
    explicit incorrect(QWidget *parent = nullptr);
    ~incorrect();

};

#endif // INCORRECT_H
