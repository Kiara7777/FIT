#include "duplikatyitemwidget.h"

DuplikatyItemWidget::DuplikatyItemWidget(QWidget *parent) : QWidget(parent)
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

void DuplikatyItemWidget::createWidget()
{

    if (allFoto->getGroupSize() <= 0)
        return;

    haveData = true;

    treeWidget->clear();

    QTreeWidgetItem *topLevel = treeWidget->invisibleRootItem();

    int size = allFoto->getGroupSize();
    for(int i = 0; i < size; i++)
    {
        if (!allFoto->haveDuplIN(i))
            continue;

        QTreeWidgetItem *skupinyLevel = new QTreeWidgetItem(topLevel);
        skupinyLevel->setData(0, Qt::UserRole, i);

        if (i == 0)
            skupinyLevel->setText(0, "Nezařazeno");
        else
        {
            QString string = "Skupina " + QString::number(i);
            skupinyLevel->setText(0, string);
        }

        int duplicity = allFoto->getGroupDuplINsize(i);
        for (int j = 0; j < duplicity; j++)
        {
            DuplicitniFoto duplicitniFoto = allFoto->getDuplicitniFoto(i, j);
            int mainFoto = duplicitniFoto.getMainFoto();
            QTreeWidgetItem *item = new QTreeWidgetItem(skupinyLevel);
            item->setText(0, allFoto->getFotoFromGroup(i, mainFoto).getPath());
            item->setData(0, Qt::UserRole, mainFoto);

            int duplikatyMainFota = duplicitniFoto.getSize();
            for (int k = 0; k < duplikatyMainFota; k++)
            {
                int foto = duplicitniFoto.getFotoIndex(k);
                QTreeWidgetItem *itemFoto = new QTreeWidgetItem(item);
                itemFoto->setText(0, allFoto->getFotoFromGroup(i, foto).getPath());
                itemFoto->setData(0, Qt::UserRole, foto);


            }

        }
    }

}

void DuplikatyItemWidget::setExifPanel(ExifPanel *exifPanel)
{
     this->exifPanel = exifPanel;
     connect(this->exifPanel, SIGNAL(updateWidgets()), this, SLOT(updateWidget()));
}

void DuplikatyItemWidget::setAllFoto(AllFoto *allFoto)
{
    this->allFoto = allFoto;
}

bool DuplikatyItemWidget::isWithData()
{
    return haveData;
}

void DuplikatyItemWidget::clearWidget()
{
    treeWidget->clear();
    haveData = false;
}

void DuplikatyItemWidget::setExifData(QTreeWidgetItem *item, int column)
{
    QTreeWidgetItem *itemHelp = item->parent();
    int skupina;

    if (itemHelp)// ma to rodice, zjistit, jestli rodic ma rodice
    {
        int foto = item->data(column, Qt::UserRole).toInt();
        QTreeWidgetItem *itemParent = itemHelp->parent();
        if(itemParent)
        {
            //jestli itemHelp ma rodice, tak item je duplikat, itemHelp je main foto, a itemParent je skupina
          skupina = itemParent->data(column, Qt::UserRole).toInt();
        }
        else
        {
            //itemHelp nema rodice, item je main foto, itemHelp je skupina
            skupina = itemHelp->data(column, Qt::UserRole).toInt();
        }

        //nastavit info, pouzije se pro mazani
        exifPanel->setInfo(false, true, skupina, foto);

        Foto fotoOBj = allFoto->getFotoFromGroup(skupina, foto);

        QString cesta = fotoOBj.getPath();
        QString velikost = fotoOBj.getVelikost();
        QString sirka = fotoOBj.getWidth();
        QString vyska = fotoOBj.getHeight();

        double lat = fotoOBj.getLatitude();
        double longi = fotoOBj.getLongitude();

        QDateTime dateTime = fotoOBj.getDateTime();

        exifPanel->setExifData(cesta, velikost, dateTime, sirka, vyska, lat, longi);
    }
    else
    {
        //jestli to nema rodice, tak je to jenom skupina
        skupina = item->data(column, Qt::UserRole).toInt();
        exifPanel->setInfo(true, false, skupina, 0);
   }

}

void DuplikatyItemWidget::updateWidget()
{
    if(haveData)
    {
        clearWidget();
        createWidget();
    }
}
