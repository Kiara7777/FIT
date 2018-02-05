#include "exifpanel.h"
#include <QDebug>

ExifPanel::ExifPanel(QWidget *parent) : QWidget(parent)
{
    exifName = new ExifItemWidget("Jméno:");
    exifSize = new ExifItemWidget("Velikost souboru:");
    exifWidth = new ExifItemWidget("Šířka:");
    exifHeight= new ExifItemWidget("Výška:");
    exifGPSLat = new ExifItemWidget("Zeměpisná šířka:");
    exifGPSLong = new ExifItemWidget("Zeměpisná délka:");


//    dateLabel = new QLabel(tr("Datum vytvoření:"));
//    dateLabel->setMinimumSize(dateLabel->sizeHint());
//    dateTime = new QDateTimeEdit();
//    dateLabel->setBuddy(dateTime);

//    QHBoxLayout *dateLayout = new QHBoxLayout;
//    dateLayout->addWidget(dateLabel);
//    dateLayout->addWidget(dateTime);

    exifDate = new ExifDateItemWidget("Datum vytvoření:");

    zpet = new QPushButton("&Zpět");
    zpet->setEnabled(false);

    smazat = new QPushButton("&Smazat");
    connect(smazat, SIGNAL(clicked(bool)), this, SLOT(removeFoto(bool)));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(zpet);
    buttonLayout->addWidget(smazat);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(exifName);
    layout->addWidget(exifSize);
//    layout->addLayout(dateLayout);
    layout->addWidget(exifDate);
    layout->addWidget(exifWidth);
    layout->addWidget(exifHeight);
    layout->addWidget(exifGPSLat);
    layout->addWidget(exifGPSLong);
    layout->addStretch(1);
    layout->addLayout(buttonLayout);

    setLayout(layout);

    group = false;
    isInGroup = false;
    groupIndex = 0;
    fotoIndex = 0;


}

void ExifPanel::setAllFoto(AllFoto *allFoto)
{
    this->allFoto = allFoto;
}

void ExifPanel::setExifData(QString path, QString size, QDateTime date, QString width, QString height, double lat, double longi)
{
    QFile file(path);
    QFileInfo info(file);
    QString name = info.fileName();

    exifName->getExifDataWidget()->setText(name);
    exifSize->getExifDataWidget()->setText(size);
    exifDate->getExifDataWidget()->setDateTime(date);
    exifWidth->getExifDataWidget()->setText(width);
    exifHeight->getExifDataWidget()->setText(height);
    exifGPSLat->getExifDataWidget()->setText(QString::number(lat, 'f', 10));
    exifGPSLong->getExifDataWidget()->setText(QString::number(longi, 'f', 10));
}

QPushButton *ExifPanel::zpetButton()
{
    return zpet;

}

void ExifPanel::setButtonZpet(bool zapnout)
{
    zpet->setEnabled(zapnout);
}

void ExifPanel::setInfo(bool group, bool isInGroup, int groupIndex, int fotoIndex)
{
    this->group = group;
    this->groupIndex = groupIndex;
    this->isInGroup = isInGroup;
    this->fotoIndex = fotoIndex;
}

void ExifPanel::printInfo()
{
    if (group) //je to skupina
        qDebug() << "Group: TRUE: " << groupIndex;
    else //uvnitr nebo volne
    {
        if (isInGroup) //uvnitr
            qDebug() << "InGroup: " << groupIndex << " foto: " << fotoIndex;
        else //volne
            qDebug() << "Volne: " << fotoIndex;
    }
}

void ExifPanel::clearWidget()
{
    zpet->setEnabled(false);
    exifName->clearWidget();
    exifSize->clearWidget();
    exifDate->clearWidget();
    exifHeight->clearWidget();
    exifWidth->clearWidget();
    exifGPSLat->clearWidget();
    exifGPSLong->clearWidget();

    group = false;
    isInGroup = false;
    groupIndex = 0;
    fotoIndex = 0;
}

void ExifPanel::removeFoto(bool click)
{
    if (group) //je to skupina
    {
        allFoto->removeGroup(groupIndex);
    }
    else //uvnitr nebo volne
    {
        if (isInGroup) //uvnitr
        {
            allFoto->removeFromGroup(groupIndex, fotoIndex);
        }
        else //volne
        {
            allFoto->removeFoto(fotoIndex);
        }
    }

    zpet->setEnabled(false);

    emit updateWidgets();

}
