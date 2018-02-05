#include "fotopicturewidget.h"

#include <QFileInfo>
#include <QPalette>

FotoPictureWidget::FotoPictureWidget(QWidget *parent) : QWidget(parent)
{
    view = new QGraphicsView;
    scene = new QGraphicsScene();

    view->setScene(scene);


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(view);
    setLayout(mainLayout);

    dupl = false;
    haveData = false;
    isInside = false;
    free = false;
}

void FotoPictureWidget::createVseWidget()
{

    if (allFoto->getFotoSize() <= 0)
        return;

    haveData = true;
    free = true;

    isInside = false;
    scene->clear();
    view->update();

    QGraphicsGridLayout *layout = new QGraphicsGridLayout;
    int row = 0;
    int col = 0;

    for (int i = 0; i < allFoto->getFotoSize(); i++)
    {
        Foto foto = allFoto->getFoto(i);
        QFileInfo fi(foto.getPath());

        FotoWidget *fotoWidget = new FotoWidget(foto.getPreview(), fi.fileName(), i);
        listWidget.append(fotoWidget);

        connect(fotoWidget, SIGNAL(clicked(FotoWidget*)), this, SLOT (setExifData(FotoWidget*)));

        QGraphicsWidget *labelP = scene->addWidget(fotoWidget);

        if (col > 6)//max 7 na radek
        {
            col = 0;
            row++;
        }

        layout->addItem(labelP, row, col);

        col++;
    }

    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(layout);
    scene->addItem(form);

    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);


}

void FotoPictureWidget::createGroupWidget()
{
    if (allFoto->getGroupSize() <= 0)
        return;

    scene->clear();
    view->update();
    listWidgetSK.clear();

    haveData = true;
    isInside = false;

    QGraphicsGridLayout *layout = new QGraphicsGridLayout;
    int row = 0;
    int col = 0;

    for (int i = 0; i < allFoto->getGroupSize(); i++)
    {

        if (allFoto->getGroupFotoSize(i) <= 0)
            continue;


        if(dupl)
        {
            if (!allFoto->haveDuplIN(i))
                continue;
        }
        Foto foto = allFoto->getFotoFromGroup(i, 0);

        QString name;

        if(allFoto->isNezarazeno(i))
            name = "Nezařazeno";
        else
            name = "Skupina " + QString::number(i);

        FotoWidget *fotoWidget = new FotoWidget(foto.getPreview(), name, i, i, false);
        listWidgetSK.append(fotoWidget);
        fotoWidget->setItsGroup(true);

        if (dupl)
            connect(fotoWidget, SIGNAL(doubleClicked(int)), this, SLOT(createDuplINFoto(int)));
        else
            connect(fotoWidget, SIGNAL(doubleClicked(int)), this, SLOT (createFotoFromGroup(int)));

        connect(fotoWidget, SIGNAL(clicked(FotoWidget*)), this, SLOT(setExifDataGroup(FotoWidget*)));

        QGraphicsWidget *labelP = scene->addWidget(fotoWidget);

        if (col > 6)//max 7 na radek
        {
            col = 0;
            row++;
        }

        layout->addItem(labelP, row, col);

        col++;
    }

    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(layout);
    scene->addItem(form);


    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);

}

void FotoPictureWidget::setExifPanel(ExifPanel *exifPanel)
{
    this->exifPanel = exifPanel;

    connect(exifPanel->zpetButton(), SIGNAL(clicked(bool)), this, SLOT(recreateGroups(bool)));
    connect(this->exifPanel, SIGNAL(updateWidgets()), this, SLOT(updateWidget()));

}

void FotoPictureWidget::setAllFoto(AllFoto *allFoto)
{
    this->allFoto = allFoto;

}

void FotoPictureWidget::setDupl(bool dupl)
{
    this->dupl = dupl;

}

bool FotoPictureWidget::isWithData()
{
    return haveData;
}

bool FotoPictureWidget::isInsideGroup()
{
    return isInside;
}

void FotoPictureWidget::setExifData(FotoWidget *fotoWidget)
{
    for (int i = 0; i < listWidget.size(); i++)
        listWidget[i]->restoreBorder();

    fotoWidget->setStyleSheet("#myObject { border: 3px solid #00cc00; }");

    int fotoIndex = fotoWidget->getIndex();

    Foto fotoOBj;


    //je ze skupiny
    if (fotoWidget->isFromGroup())
    {
        fotoOBj = allFoto->getFotoFromGroup(fotoWidget->getGroup(), fotoIndex);
        exifPanel->setInfo(false, true, fotoWidget->getGroup(), fotoIndex);
    }
    else
    {
        fotoOBj = allFoto->getFoto(fotoIndex);
        exifPanel->setInfo(false, false, 0, fotoIndex);
    }

    QString cesta = fotoOBj.getPath();
    QString velikost = fotoOBj.getVelikost();
    QString sirka = fotoOBj.getWidth();
    QString vyska = fotoOBj.getHeight();

    double lat = fotoOBj.getLatitude();
    double longi = fotoOBj.getLongitude();

    QDateTime dateTime = fotoOBj.getDateTime();

    exifPanel->setExifData(cesta, velikost, dateTime, sirka, vyska, lat, longi);

}

void FotoPictureWidget::setExifDataGroup(FotoWidget *fotoWidget)
{
    for (int i = 0; i < listWidgetSK.size(); i++)
        listWidgetSK.at(i)->restoreBorder();

    fotoWidget->setStyleSheet("#myObject { border: 3px solid blue; }");

    int skup = fotoWidget->getGroup();
    exifPanel->setInfo(true, false, skup, 0);

}

/*
 * Vytvori widgetz pro fotografie uvnitr
 */
void FotoPictureWidget::createFotoFromGroup(int gr)
{
    listWidget.clear();

    listSK = scene->items();
    isInside = true;

    for (int i = 0; i < listSK.size(); i++)
    {
         listSK.at(i)->hide();
    }

    int group = gr;

    QGraphicsGridLayout *layout = new QGraphicsGridLayout;
    int row = 0;
    int col = 0;

    for (int i = 0; i < allFoto->getGroupFotoSize(group); i++)
    {
        Foto foto = allFoto->getFotoFromGroup(group, i);
        QFileInfo fi(foto.getPath());

        FotoWidget *fotoWidgetM = new FotoWidget(foto.getPreview(), fi.fileName(), i, group, true);
        listWidget.append(fotoWidgetM);

        connect(fotoWidgetM, SIGNAL(clicked(FotoWidget*)), this, SLOT (setExifData(FotoWidget*)));

        QGraphicsWidget *labelP = scene->addWidget(fotoWidgetM);


        if (col > 6)//max 7 na radek
        {
            col = 0;
            row++;
        }

        layout->addItem(labelP, row, col);

        col++;
    }

    QGraphicsWidget *form = new QGraphicsWidget;
    help.append(form);
    form->setLayout(layout);
    scene->addItem(form);



    exifPanel->setButtonZpet(true);

}

void FotoPictureWidget::recreateGroups(bool click)
{
    if (!free)
    {
        listWidget.clear();
    }

    exifPanel->setButtonZpet(false);
    isInside = false;

    for (int i = 0; i < help.size(); i++)
    {
        delete help.at(i);
    }

    help.clear();


    for (int i = 0; i < listSK.size(); i++)
    {
        listSK.at(i)->show();
    }

    listSK.clear();

    for (int i = 0; i < listWidgetSK.size(); i++)
        listWidgetSK.at(i)->restoreBorder();


}

/*
 * Zobrazit duplicitni fotografie
 */

void FotoPictureWidget::createDuplINFoto(int gr)
{
    listSK.clear();
    help.clear();
    listWidget.clear();
    listSK = scene->items();
    isInside = true;

    for (int i = 0; i < listSK.size(); i++)
    {
         listSK.at(i)->hide();
    }

    int group = gr;

    QGraphicsGridLayout *layout = new QGraphicsGridLayout;
    int row = 0;
    int col = 0;

    for (int i = 0; i < allFoto->getGroupDuplINsize(group); i++)
    {
        DuplicitniFoto duplicitni = allFoto->getDuplicitniFoto(group, i);

        //Ziskat hlavni foto a vlozit
        int mainFoto = duplicitni.getMainFoto();

        //Vytvorit hlavni foto
        Foto foto = allFoto->getFotoFromGroup(group, mainFoto);
        QFileInfo fi(foto.getPath());
        FotoWidget *fotoWidgetM = new FotoWidget(foto.getPreview(), fi.fileName(), mainFoto, group, true);
        listWidget.append(fotoWidgetM);
        connect(fotoWidgetM, SIGNAL(clicked(FotoWidget*)), this, SLOT (setExifData(FotoWidget*)));
        QGraphicsWidget *labelM = scene->addWidget(fotoWidgetM);

        //vlozit hlavni foto
        if (col > 6)//max 7 na radek
        {
            col = 0;
            row++;
        }

        layout->addItem(labelM, row, col);

        col++;

        //vytvorit vedlejsi duplickaty
        for (int j = 0; j < duplicitni.getSize(); j++)
        {
            int duplFoto = duplicitni.getFotoIndex(j);

            //vytvorit duplicitni fota
            Foto fotoD = allFoto->getFotoFromGroup(group, duplFoto);
            QFileInfo fiD(fotoD.getPath());
            FotoWidget *fotoWidgetD = new FotoWidget(fotoD.getPreview(), fiD.fileName(), duplFoto, group, true);
            listWidget.append(fotoWidgetD);
            connect(fotoWidgetD, SIGNAL(clicked(FotoWidget*)), this, SLOT (setExifData(FotoWidget*)));
            QGraphicsWidget *labelD = scene->addWidget(fotoWidgetD);

            //vlozit duplicitni foto
            if (col > 6)//max 7 na radek
            {
                col = 0;
                row++;
            }

            layout->addItem(labelD, row, col);

            col++;
        }
    }

    //vlozit vsechno
    QGraphicsWidget *form = new QGraphicsWidget;
    help.append(form);
    form->setLayout(layout);
    scene->addItem(form);

    exifPanel->setButtonZpet(true);

}

void FotoPictureWidget::updateWidget()
{
    if(haveData)
    {
        if (listWidgetSK.size() > 0) // je to skupinovy widget
        {
            listWidgetSK.clear();
            isInside = false;
            help.clear();
            listSK.clear();
            createGroupWidget();
        }
        else if (listWidget.size() > 0) // je to vse widget
        {
            listWidget.clear();
            createVseWidget();
        }
    }


}

void FotoPictureWidget::clearWidget()
{
    scene->clear();
    view->update();

    haveData = false;
    isInside = false;

    listSK.clear();
    listWidget.clear();
    listWidgetSK.clear();



}
