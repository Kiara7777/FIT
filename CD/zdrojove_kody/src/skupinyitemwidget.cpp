#include "skupinyitemwidget.h"

SkupinyItemWidget::SkupinyItemWidget(QWidget *parent) : QWidget(parent)
{
    treeWidget = new QTreeWidget;
    treeWidget->setColumnCount(1);
    treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    treeWidget->header()->close();

    connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(setExifData(QTreeWidgetItem*,int)));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(treeWidget);
    setLayout(mainLayout);

    haveData = false;


}

void SkupinyItemWidget::createWidget()
{

    if (allFoto->getGroupSize() <= 0)
        return;

    haveData = true;

    treeWidget->clear();

    QTreeWidgetItem *topLevel = treeWidget->invisibleRootItem();

    int size = allFoto->getGroupSize();
    for(int i = 0; i < size; i++)
    {

        if (allFoto->getGroupFotoSize(i) <= 0)
            continue;

        QTreeWidgetItem *skupinyLevel = new QTreeWidgetItem(topLevel);
        skupinyLevel->setData(0, Qt::UserRole, i);

        if(allFoto->isNezarazeno(i))
            skupinyLevel->setText(0, "Nezařazeno");
        else
        {
            QString string = "Skupina " + QString::number(i);
            skupinyLevel->setText(0, string);
        }



        int foto = allFoto->getGroupFotoSize(i);
        for (int j = 0; j < foto; j++)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(skupinyLevel);
            item->setText(0, allFoto->getFotoFromGroup(i, j).getPath());
            item->setData(0, Qt::UserRole, j);

        }
    }
}

void SkupinyItemWidget::setExifPanel(ExifPanel *exifPanel)
{
    this->exifPanel = exifPanel;
    connect(this->exifPanel, SIGNAL(updateWidgets()), this, SLOT(updateWidget()));

}

void SkupinyItemWidget::setAllFoto(AllFoto *allFoto)
{
    this->allFoto = allFoto;
}

bool SkupinyItemWidget::isWithData()
{
    return haveData;
}

void SkupinyItemWidget::clearWidget()
{
    treeWidget->clear();
    haveData = false;
}

void SkupinyItemWidget::setExifData(QTreeWidgetItem *item, int column)
{

    int foto = item->data(column, Qt::UserRole).toInt();


    QTreeWidgetItem *itemHelp = item->parent();
    if (itemHelp) //foto
    {
        int skupina = itemHelp->data(column, Qt::UserRole).toInt();

        Foto fotoOBj = allFoto->getFotoFromGroup(skupina, foto);

        QString cesta = fotoOBj.getPath();
        QString velikost = fotoOBj.getVelikost();
        QString sirka = fotoOBj.getWidth();
        QString vyska = fotoOBj.getHeight();

        double lat = fotoOBj.getLatitude();
        double longi = fotoOBj.getLongitude();

        QDateTime dateTime = fotoOBj.getDateTime();

        exifPanel->setExifData(cesta, velikost, dateTime, sirka, vyska, lat, longi);
        exifPanel->setInfo(false, true, skupina, foto);
    }
    else
    {
        //je kliknuta skupina
        exifPanel->setInfo(true, false, foto, 0);
    }
}

void SkupinyItemWidget::updateWidget()
{
    if(haveData)
    {
        clearWidget();
        createWidget();
    }

}
