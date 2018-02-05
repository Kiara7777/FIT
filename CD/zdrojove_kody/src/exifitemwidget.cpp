#include "exifitemwidget.h"
#include <QDebug>

ExifItemWidget::ExifItemWidget(QString exifName, QString exifNameData, QWidget *parent) : QWidget(parent)
{
    exifData = new QLineEdit;
    exifData->setText(exifNameData);
    exifData->setReadOnly(true);


    exifLavel = new QLabel(exifName);
    exifLavel->setBuddy(exifData);



    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(exifLavel);
    mainLayout->addWidget(exifData);
    setLayout(mainLayout);
}

QLineEdit *ExifItemWidget::getExifDataWidget()
{
    return exifData;
}

void ExifItemWidget::clearWidget()
{
    exifData->clear();
}
