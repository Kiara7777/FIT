#include "exifdateitemwidget.h"



ExifDateItemWidget::ExifDateItemWidget(QString exifName, QString exifNameData, QWidget *parent) : QWidget(parent)
{
    exifData = new QDateTimeEdit;
    exifData->setReadOnly(true);
    exifData->clear();


    exifLavel = new QLabel(exifName);
    exifLavel->setMinimumSize(exifLavel->sizeHint());
    exifLavel->setBuddy(exifData);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(exifLavel);
    mainLayout->addWidget(exifData);

    setLayout(mainLayout);

}

QDateTimeEdit *ExifDateItemWidget::getExifDataWidget()
{
    return exifData;

}

void ExifDateItemWidget::clearWidget()
{
    exifData->clear();

}
