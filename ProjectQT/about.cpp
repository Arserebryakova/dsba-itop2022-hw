#include "about.h"
#include "ui_about.h"
#include <QPainter>
#include <QLayout>

about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
}

about::~about()
{
    delete ui;
}

void about::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setPen({QColorConstants::Svg::green, 25.0});
    painter.setBrush(QBrush(QColorConstants::Svg::white));
    painter.translate(width()/2,height()/2);
    painter.drawEllipse(-150,-250,300,300);
    painter.setPen({QColorConstants::Svg::green, 25.0});
    painter.drawLine(5,-100,130,-175);
    painter.drawLine(-5,-100,-130,-175);
    painter.drawLine(5,-150,100,-205);
    painter.drawLine(-5,-150,-100,-205);
    painter.drawLine(5,-200,55,-230);
    painter.drawLine(-5,-200,-55,-230);
}
