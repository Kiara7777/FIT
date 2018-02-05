#include "fotoitemwidget.h"
#include <QApplication>
#include <QDebug>

FotoItemWidget::FotoItemWidget(QWidget *parent) : QWidget(parent)
{
    listWidget = new QListWidget;

    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(setExifData(QListWidgetItem*)));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(listWidget);
    setLayout(mainLayout);

    haveData = false;

}


void FotoItemWidget::createWidget()
{

    if (allFoto->getFotoSize() <= 0)
        return;

    listWidget->clear();
    haveData = true;
    id = -1;

    int size = allFoto->getFotoSize();

    for (int i = 0; i < size; i++)
    {
        QListWidgetItem *item = new QListWidgetItem(listWidget);
        item->setData(Qt::DisplayRole, allFoto->getFoto(i).getPath());
        item->setData(Qt::UserRole, i);
    }



}

void FotoItemWidget::setExifPanel(ExifPanel *exifPanel)
{
    this->exifPanel = exifPanel;
    connect(this->exifPanel, SIGNAL(updateWidgets()), this, SLOT(updateWidget()));

}

void FotoItemWidget::setAllFoto(AllFoto *allFoto)
{
    this->allFoto = allFoto;
}

bool FotoItemWidget::isWithData()
{
    return haveData;
}

void FotoItemWidget::clearWidget()
{
    listWidget->clear();
    haveData = false;
}

void FotoItemWidget::setExifData(QListWidgetItem *item)
{
    int foto = item->data(Qt::UserRole).toInt();
    Foto fotoOBj = allFoto->getFoto(foto);

    QString cesta = fotoOBj.getPath();
    QString velikost = fotoOBj.getVelikost();
    QString sirka = fotoOBj.getWidth();
    QString vyska = fotoOBj.getHeight();

    double lat = fotoOBj.getLatitude();
    double longi = fotoOBj.getLongitude();

    QDateTime dateTime = fotoOBj.getDateTime();

    exifPanel->setExifData(cesta, velikost, dateTime, sirka, vyska, lat, longi);
    exifPanel->setInfo(false, false, 0, foto);
}

void FotoItemWidget::updateWidget()
{
    //updatovat jenom kdyz ma data, jinak je to nanic
    if (haveData)
    {
        listWidget->clear();
        createWidget();
    }

}
