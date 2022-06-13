#ifndef NOSHOW_H
#define NOSHOW_H

#include "mainwindow.h"
#include <QDialog>
#include <QLabel>

class noshow : public QDialog
{
    Q_OBJECT
    QLabel *text;

public:
    explicit noshow(QWidget *parent = nullptr);
    ~noshow();

};

#endif // NOSHOW_H
