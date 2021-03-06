#include "duplikatyskuppicturewidget.h"

DuplikatySkupPictureWidget::DuplikatySkupPictureWidget(QWidget *parent) : QWidget(parent)
{

    QGridLayout *widget = new QGridLayout;

    nameWindow = new QLabel(tr("Duplicitní skupiny"));

    sk1 = new QComboBox;
    fotosSK1 = new QLabel(tr("Celkový počet fotografií: "));
    fotosDuplSK1 = new QLabel(tr("Celkový počet duplicitních fotografií se skupinou x: "));
    viewSK1 = new QGraphicsView;
    sceneSK1 = new QGraphicsScene();
    viewSK1->setScene(sceneSK1);


    sk2 = new QComboBox;
    sk2->setEnabled(false);
    fotosSK2 = new QLabel(tr("Celkový počet fotografií: "));
    fotosSK2->setEnabled(false);
    fotosDuplSK2 = new QLabel(tr("Celkový počet duplicitních fotografií se skupinou x: "));
    fotosDuplSK2->setEnabled(false);
    viewSK2 = new QGraphicsView;
    sceneSK2 = new QGraphicsScene();

    viewSK2->setScene(sceneSK2);
    viewSK2->setEnabled(false);

    line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    QHBoxLayout *deleteWidget = new QHBoxLayout;

    deleteBox = new QComboBox;
    deleteButton = new QPushButton("Smazat skupinu");
    connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(removeFoto(bool)));

    deleteWidget->addStretch(1);
    deleteWidget->addWidget(deleteBox);
    deleteWidget->addWidget(deleteButton);


    widget->addWidget(nameWindow, 0, 0, 1, 2, Qt::AlignCenter);
    widget->addWidget(sk1, 1, 0);
    widget->addWidget(sk2, 1, 1);
    widget->addWidget(fotosSK1, 2, 0);
    widget->addWidget(fotosSK2, 2, 1);
    widget->addWidget(fotosDuplSK1, 3, 0);
    widget->addWidget(fotosDuplSK2, 3, 1);
    widget->addWidget(viewSK1, 4, 0);
    widget->addWidget(viewSK2, 4, 1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(widget);
    //mainLayout->addWidget(line);
    mainLayout->addLayout(deleteWidget);


    setLayout(mainLayout);

    haveData = false;

    viewSK1->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    viewSK2->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    connect(sk1, SIGNAL(currentIndexChanged(int)), this, SLOT(setGroupInfo(int)));
    connect(sk2, SIGNAL(currentIndexChanged(int)), this, SLOT(setGroupInfo2(int)));

}

void DuplikatySkupPictureWidget::createWidget()
{
    bool jeTam = false;

    deleteGroups();


    if (allFoto->getGroupSize() <= 0)
        return;

    haveData = true;
    //Prochazet vsechny skupiny
    //jestli skupina ma duplikaty, tak ji vlozit do sk1 comboboxu
    //kdyz se dana skupina zobrazi na comboBoxu - vyslat signal na update dalsich coponent

    int size = allFoto->getGroupSize();
    int index = 0;
    for(int i = 0; i < size; i++) //prochazet vsechny skupiny
    {
        if (allFoto->haveDuplSkup(i))
        {
            QString string = "Skupina " + QString::number(i);
            sk1->addItem(string);
            sk1->setItemData(index, i, Qt::UserRole);
            index++;
            jeTam = true;
        }
    }

    if (jeTam)
        emit sk1->currentIndexChanged(0);

}

void DuplikatySkupPictureWidget::setAllFoto(AllFoto *allFoto)
{
    this->allFoto = allFoto;

}

void DuplikatySkupPictureWidget::setExifPanel(ExifPanel *exifPanel)
{
    this->exifPanel = exifPanel;
    connect(this->exifPanel, SIGNAL(updateWidgets()), this, SLOT(updateWidget()));
}

bool DuplikatySkupPictureWidget::isWithData()
{
    return haveData;
}

void DuplikatySkupPictureWidget::clearWidget()
{
    sk1->clear();
    sk2->clear();
    sk2->setEnabled(false);
    fotosSK1->setText("Celkový počet fotografií: ");
    fotosSK2->setText("Celkový počet fotografií: ");
    fotosSK2->setEnabled(false);
    fotosDuplSK1->setText("Celkový počet duplicitních fotografií se skupinou x: ");
    fotosDuplSK2->setText("Celkový počet duplicitních fotografií se skupinou x: ");
    fotosDuplSK2->setEnabled(false);
    sceneSK1->clear();
    sceneSK2->clear();
    viewSK1->update();
    viewSK2->update();
    viewSK2->setEnabled(false);
    deleteBox->clear();

    haveData = false;
}

void DuplikatySkupPictureWidget::setGroupInfo(int index)
{
    sk2->clear();

    //Vypsat pocet celkovych fotek ve skupine
    //Vypdat pocet duplikatu k dane skupine k dane skupine

    //zobrazeni skupiny v comboBoxu
    //zobrazeni kolik dana skupina ma fotografii

    //index je cislo polozky zacina od 0

    int skupinaSK1 = sk1->itemData(index, Qt::UserRole).toInt(); // zjistil index/poradi vybrane skupiny

    //nastavim kolim ma celkovy pocet fotografii v sobe
    int pocetFotoSK1 = allFoto->getGroupFotoSize(skupinaSK1);
    fotosSK1->setText("Celkový počet fotografií: " + QString::number(pocetFotoSK1));

    //zjistit jake skupiny jsou k dane vybrane skupine duplicitni a vlozit je do druheho comboboxu
    int pocetDuplSkup = allFoto->getGoupDuplSkupSize(skupinaSK1);
    int indexSK2 = 0;
    for (int i = 0; i < pocetDuplSkup; i++)
    {
        sk2->setEnabled(true);
        fotosSK2->setEnabled(true);
        viewSK2->setEnabled(true);
        int skupinaSK2 = allFoto->getDuplicita(skupinaSK1, i).getSkupina();
        sk2->addItem("Skupina " + QString::number(skupinaSK2));
        sk2->setItemData(indexSK2, skupinaSK2, Qt::UserRole);
        indexSK2++;

    }


    emit sk2->currentIndexChanged(0);

}

void DuplikatySkupPictureWidget::setGroupInfo2(int index)
{
    int pocetForSK1 = 0;
    int pocetForSK2 = 0;

    //vycistit pole pro zobrazovani forografii
    sceneSK1->clear();
    sceneSK2->clear();
    viewSK1->update();
    viewSK2->update();

    //zjistit, ktera skupina je vybrana v druhem comoboxu, a zobrazit pocet fotografii ktere ma
    int skupinaSK2 = sk2->itemData(index, Qt::UserRole).toInt();
    int pocetFotoSK2 = allFoto->getGroupFotoSize(skupinaSK2);
    fotosSK2->setText("Celkový počet fotografií: " + QString::number(pocetFotoSK2));

    //zjistit danou skupinu co je v combo boxu 1, a zjistit pocet jejich duplicitnich skupin
    int SK1Index = sk1->currentIndex();
    int skupinaSK1 = sk1->itemData(SK1Index, Qt::UserRole).toInt();
    int pocetDuplSkup = allFoto->getGoupDuplSkupSize(skupinaSK1);

    QGraphicsGridLayout *layoutSK1 = new QGraphicsGridLayout;
    int rowSK1 = 0;
    int colSK1 = 0;

    QGraphicsGridLayout *layoutSK2 = new QGraphicsGridLayout;
    int rowSK2 = 0;
    int colSK2 = 0;

    //Prochazet duplicitni skupiny pro skupinu z combo boxu 1
    for (int i = 0; i < pocetDuplSkup; i++)
    {
        //dostat imdex duplicitni skupiny, aby se mohla zobrazit spravne duplicitni fotografie
        int SK2Help = allFoto->getDuplicita(skupinaSK1, i).getSkupina();
        if (SK2Help == skupinaSK2) //ano je to TA duplicitni skupina, vztahnou MAIN FOTO
        {
            //prochazet jednotlive duplikaty mezi skupinou SK1 a SK2, vytahnout nejdriv fotografie
            //ktere byly urcene jako duplicitni ve SK1 - main foto
            QVector<int> duplHelp;
            int duplSK1Size = allFoto->getDuplicita(skupinaSK1, i).getSize();
            for (int j = 0; j < duplSK1Size; j++)
            {
                int mainFoto = allFoto->getDuplicita(skupinaSK1, i).getItem(j).getMainFoto();
                Foto fotoM = allFoto->getFotoFromGroup(skupinaSK1, mainFoto);
                QFileInfo fiM(fotoM.getPath());
                pocetForSK2++;

                FotoWidget *fotoWidgetM = new FotoWidget(fotoM.getPreview(), fiM.fileName(), mainFoto, skupinaSK1, false);
                QGraphicsWidget *labelM = sceneSK1->addWidget(fotoWidgetM);

                //vlozit hlavni foto
                if (colSK1 > 3)//max 4 na radek
                {
                    colSK1 = 0;
                    rowSK1++;
                }
                layoutSK1->addItem(labelM, rowSK1, colSK1);
                colSK1++;

                //ktere fotografie z SK2 jsou duplicitni k SK1 - algoritmus a ukladani je ale tadz trochu blem takze
                //se musi ukladat jake indexy uz byly aby se to hromadne neopakovalo
                int duplSK2 = allFoto->getDuplicita(skupinaSK1, i).getItem(j).getSize();
                for (int k = 0; k < duplSK2; k++)
                {
                    int SK2Foto = allFoto->getDuplicita(skupinaSK1, i).getItem(j).getFotoIndex(k);
                    bool ne = true;

                    //test jestli uz dana fotografie tam nebyla vlozena
                    for (int l = 0; l < duplHelp.size(); l++)
                    {
                        if (duplHelp[l] == SK2Foto)
                        {
                            ne = false;
                            break;
                        }
                        else
                            ne = true;
                    }

                    if (ne)
                    {
                        Foto fotoD = allFoto->getFotoFromGroup(skupinaSK2, SK2Foto);
                        QFileInfo fiD(fotoD.getPath());

                        FotoWidget *fotoWidgetD = new FotoWidget(fotoD.getPreview(), fiD.fileName(), SK2Foto, skupinaSK2, false);
                        QGraphicsWidget *labelD = sceneSK2->addWidget(fotoWidgetD);
                        pocetForSK1++;

                        //vlozit hlavni foto
                        if (colSK2 > 3)//max 4 na radek
                        {
                            colSK2 = 0;
                            rowSK2++;
                        }
                        layoutSK2->addItem(labelD, rowSK2, colSK2);
                        colSK2++;

                        duplHelp.append(SK2Foto);
                    }
                }
            }
        }
    }


    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(layoutSK1);
    sceneSK1->addItem(form);

    QGraphicsWidget *form2 = new QGraphicsWidget;
    form2->setLayout(layoutSK2);
    sceneSK2->addItem(form2);

    fotosDuplSK2->setEnabled(true);

    fotosDuplSK1->setText("Celkový počet duplicitních fotografií se skupinou " +
                          QString::number(skupinaSK2) + " : " + QString::number(pocetForSK2));
    fotosDuplSK2->setText("Celkový počet duplicitních fotografií se skupinou " +
                          QString::number(skupinaSK1) + " : " + QString::number(pocetForSK1));

}

void DuplikatySkupPictureWidget::removeFoto(bool click)
{
    //ziskat akrualne zobrazenou skupinu a zavolat exifpanel
    int index = deleteBox->currentData(Qt::UserRole).toInt();
    exifPanel->setInfo(true, false, index, 0);
    exifPanel->removeFoto(true);
}

void DuplikatySkupPictureWidget::updateWidget()
{
    if(haveData)
    {
        clearWidget();
        createWidget();
    }
}

void DuplikatySkupPictureWidget::deleteGroups()
{
    std::vector<int> skupiny;
    for (int i = 0; i < allFoto->getGroupSize(); i++)
    {
        //jestli ma duplicitni skupiny, tak si to ulozis
        if (allFoto->haveDuplSkup(i))
        {
            bool muzu = true;

            for (unsigned int k = 0; k < skupiny.size(); k++)
            {
                if (skupiny[k] == i)
                {
                    muzu = false;
                    break;
                }
                else
                    muzu = true;
            }

            if (muzu)
                skupiny.push_back(i);

            //jake jsou jeji vnitrni duplikaty
            for (int j = 0; j < allFoto->getGoupDuplSkupSize(i); j++)
            {
                int skupina = allFoto->getDuplicita(i, j).getSkupina();
                bool je = false;
                for (unsigned int k = 0; k < skupiny.size(); k++)
                {
                    //jestli tam uz jednou skupina je, tak ji tam nebudu davat zase
                    if (skupina == skupiny[k])
                    {
                        je = true;
                        break;
                    }
                }

                //neni tam, muze se vlozit
                if(!je)
                {
                    skupiny.push_back(skupina);
                }
            }
        }
    }

    std::sort(skupiny.begin(), skupiny.end());

    for (unsigned int i = 0; i < skupiny.size(); i++)
    {
        deleteBox->addItem("Skupina: " + QString::number(skupiny[i]));
        deleteBox->setItemData(i, skupiny[i], Qt::UserRole);
    }

}
