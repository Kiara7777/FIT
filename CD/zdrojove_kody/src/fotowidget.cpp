#include "fotowidget.h"
#include <QDebug>

FotoWidget::FotoWidget(QPixmap pixmap, QString name, int index,
                       int group, bool isGroup, QWidget *parent) : QFrame(parent)
{
    pixmapLabel = new QLabel;
    nameLabel = new QLabel(name);
    nameLabel->setAlignment(Qt::AlignCenter);

    this->index = index;
    this->group = group;
    this->isInGroup = isGroup;

    pixmapLabel->setPixmap(pixmap);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(pixmapLabel);
    layout->addWidget(nameLabel,Qt::AlignCenter);

    setLayout(layout);

    itIsGroup = false;

    setObjectName("myObject");

    restoreBorder();




}

const QPixmap *FotoWidget::getPixmap()
{
    return pixmapLabel->pixmap();
}

int FotoWidget::getIndex()
{
    return index;
}

int FotoWidget::getGroup()
{
    return group;
}

bool FotoWidget::isFromGroup()
{
    return isInGroup;
}

bool FotoWidget::itsGroupIndicator()
{
    return itIsGroup;
}

void FotoWidget::setItsGroup(bool itsGroup)
{
    this->itIsGroup = itsGroup;

}

void FotoWidget::restoreBorder()
{
    QString string = "#myObject { border: 3px solid ";
    string += palette().color(QWidget::backgroundRole()).name();
    string += "; }";

    setStyleSheet(string);
}

void FotoWidget::mousePressEvent(QMouseEvent *event)
{
    emit clicked(this);
}

void FotoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit doubleClicked(group);
}
