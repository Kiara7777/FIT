#include "mainwindow.h"
#include "fotofile.h"
#include "skupinydialog.h"
#include <QApplication>
#include <QDebug>

//KONSTRUKTOR
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Správce fotografií"));
    zobrazitFoto = false;
    zmena = false;
    firstTime = true;
    stavProgramu = VSE;

    createActions();
    createMenu();

    setMenus();

    createAll();
}

//DESTRUKTOR
MainWindow::~MainWindow()
{
    delete allFoto;
    delete exifPanelWidget;

    delete fotoItemsWidget;
    delete skupinzItemsWidget;
    delete duplINItemsWidget;
    delete duplSkupinyWidget;

    delete fotoPictureWidget;
    delete skupinyPictureWidget;
    delete duplINPictureWidget;
    delete duplSkupPictureWidget;

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (zmena)
    {
        int respond = QMessageBox::warning(this, tr("Správce fotografií"),
                                           tr("Byly provedeny změny v rozložení fotografií.\n"
                                              "Chcete uložit tyto změny uložit?"),
                                           QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (respond == QMessageBox::Yes)//ulozit
        {
            bool ok = exportFotos();
            if (!ok)
            {
                msg.showMessage(tr("Změny se nepodařilo uložit.\nZkuste to prosím později."));
                event->ignore();
            }
            else
            {
                event->accept();
            }
        }
        else if (respond == QMessageBox::No)//neukladat, zahodit zmeny
            event->accept();
        else if (respond == QMessageBox::Cancel) //kaslu na to, chci dale robit
            event->ignore();
    }
    else
    {
        event->accept();
    }

}

void MainWindow::findFotos()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Importovat fotografie z adresáře"), ".", QFileDialog::ShowDirsOnly);

    if(!dir.isEmpty())
    {
        FotoFile foto("foto.txt");
        foto.setDir(dir);

        int ok = foto.createFotoFile();
        if (ok != 0)
        {
            msg.setWindowTitle(tr("CHYBA"));
            msg.setModal(true);
            msg.showMessage(tr("Soubor s informacemi o fotografiích se nepodařilo vytvořit"));
        }
        else
        {
            openLast->setEnabled(true);
            deleteFileAction->setEnabled(true);

            skupinyMenu->setEnabled(true);
            zobrazitMenu->setEnabled(true);

            zobrazitSeznam->setEnabled(false);
            zobrazitFotografie->setEnabled(true);

            allFoto->createFotosItem("foto.txt");

            stavProgramu = VSE;

            createWidgets();
            displayWidgets();

        }
    }
}

void MainWindow::openLastFile()
{

    skupinyMenu->setEnabled(true);
    zobrazitMenu->setEnabled(true);

    zobrazitSeznam->setEnabled(false);
    zobrazitFotografie->setEnabled(true);

    allFoto->createFotosItem("foto.txt");

    stavProgramu = VSE;

    createWidgets();
    displayWidgets();
}

void MainWindow::deleteFileFotos()
{
    QString string = "foto.txt";
    QFile::remove(string);

    QMessageBox box;
    box.setWindowTitle(tr("Soubor smazán"));
    box.setText(tr("Soubor s informacemi o fotografiích smazán"));
    box.exec();

    cleanAll();
}

bool MainWindow::exportFotos()
{

    //ziskat adresar do ktereho se ulozi skupiny
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    ".",
                                                    QFileDialog::ShowDirsOnly);

    QProgressDialog progress(this);
    progress.setWindowTitle("Ukládání");

    int pocet = allFoto->getFotoSize() * 2;
    pocet += allFoto->dethFotoSize();
    int caut = 0;
    progress.setRange(0, pocet);
    progress.setModal(true);
    progress.setAttribute(Qt::WA_DeleteOnClose, true);
    progress.setCancelButton(0);
    progress.setMinimumDuration(0);
    progress.show();

    if (dir.isEmpty()) //nic se nevratilo, ABORT!!!!
        return false;

   repaint();

    progress.setLabelText("Mazání fotografií");
    for (int i = 0; i < allFoto->dethFotoSize(); i++)
    {
        if (!QFile::remove(allFoto->deathFoto(i)))
        {
            progress.close();
            return false;
        }

        progress.setValue(++caut);

    }


    progress.setLabelText("Tvorba skupin a přesun fotografií");

    //vytvorit adresare podel skupin nasackovat do nich fotografie
    QDir saveDir(dir);
    for (int i = 0; i < allFoto->getGroupSize(); i++)
    {
        QString name;

        if (allFoto->isNezarazeno(i))
            name = "Nezarazeno";
        else
            name = "Skupina" + QString::number(i);

        saveDir.mkdir(name);

        QDir noveDir(saveDir.absoluteFilePath(name));

        int fotoSize = allFoto->getGroupFotoSize(i);
        //presunout fotografie
        for (int j = 0; j < fotoSize; j++)
        {
            QString fotoPath = allFoto->getFotoFromGroup(i, j).getPath();
            QFileInfo fileInfo(fotoPath);
            QString fotoName = fileInfo.fileName();

            progress.setValue(++caut);


           noveDir.rename(fotoPath, noveDir.absoluteFilePath(fotoName));

           //aktualizovat info, kde je nove ulozeni fotografie
           allFoto->setNewPath(i, j, noveDir.absoluteFilePath(fotoName));
        }

    }

    progress.setLabelText("Aktualizace souboru");

    //aktualizovat soubor s informacemi
    QFile file("foto.txt");
    if (!file.open(QIODevice::Truncate | QIODevice::Text | QIODevice::WriteOnly))
        return false;

    QTextStream out(&file);

    for (int i = 0; i < allFoto->getFotoSize(); i++)
    {
        out << allFoto->getFoto(i).getAllInfo();
        progress.setValue(++caut);

    }

    file.close();


    zmena = false;
    cleanAll();
    firstTime = false;

    return true;
}

void MainWindow::createGroups()
{
    SkupinyDialog dialog(this);
    int ok = dialog.exec();

    if(ok == QDialog::Accepted)
    {
        int type = dialog.getType();
        QString typeNumber = dialog.getTypeSize();
        int number = typeNumber.toInt();

        allFoto->createGroups(type, number);


        exportAction->setEnabled(true);
        duplikatyMenu->setEnabled(true);
        zobrazitSkupiny->setEnabled(true);


        stavProgramu = SKUPINY;
        zmena = true;

        createWidgets();
        displayWidgets();
    }
}

void MainWindow::findDuplicatsInGrous()
{
    allFoto->duplVSkup();

    zobrayitDuplikatyIN->setEnabled(true);

    stavProgramu = DUPLICITYIN;

    createWidgets();
    displayWidgets();
}

void MainWindow::findDuplicats()
{
    allFoto->duplicitniSkupiny();

    zobrazitDuplicitniSkupiny->setEnabled(true);

    stavProgramu = DUPLICITYSKUPIN;

    createWidgets();
    displayWidgets();
}

void MainWindow::about()
{

}

void MainWindow::createSeznam()
{
    zobrazitFoto = false;

    createWidgets();
    displayWidgets();

    zobrazitFotografie->setEnabled(true);
    zobrazitSeznam->setEnabled(false);

}

void MainWindow::createImageFotos()
{
    if(!allFoto->isPixmap()) // vytvorit to pouze jednou
    {
        allFoto->setFotosPicture();
    }

    if(!allFoto->isPixmap()) //zmackl se cancel
        return;

    zobrazitFoto = true;

    zobrazitFotografie->setEnabled(false);
    zobrazitSeznam->setEnabled(true);

    createWidgets();
    displayWidgets();

}

void MainWindow::createCentralFotoItemWidget()
{
    if (allFoto->getFotoSize() > 0)
        setWindowTitle(tr("Správce fotografií - nerozdělené fotografie"));

    stavProgramu = VSE;
    createWidgets();

    if (zobrazitFoto)
        createCentralFotoPictureWidget();
    else
    {
        if(centralWidget())
        {
            centralWidget()->layout()->removeWidget(exifPanelWidget);
            centralWidget()->setParent(0);
        }
        centralSeznamVseWidget->layout()->addWidget(exifPanelWidget);
        setCentralWidget(centralSeznamVseWidget);
    }

}

void MainWindow::createCentralGroupItemWidget()
{
    setWindowTitle(tr("Správce fotografií - skupiny fotografií"));
    stavProgramu = SKUPINY;
    createWidgets();

    if (zobrazitFoto)
        createCentralGroupPictureWidget();
    else
    {
        if(centralWidget())
        {
            centralWidget()->layout()->removeWidget(exifPanelWidget);
            centralWidget()->setParent(0);
        }
        centralSeznamSkupinyWidget->layout()->addWidget(exifPanelWidget);
        setCentralWidget(centralSeznamSkupinyWidget);
    }

}

void MainWindow::createCentralDuplikItemWidget()
{
    setWindowTitle(tr("Správce fotografií - vnitřní duplikáty"));
    stavProgramu = DUPLICITYIN;
    createWidgets();

    if(zobrazitFoto)
        createCentralDuplikPictureWidget();
    else
    {
        if(centralWidget())
        {
            centralWidget()->layout()->removeWidget(exifPanelWidget);
            centralWidget()->setParent(0);
        }
        centralSeznamDuplINWidget->layout()->addWidget(exifPanelWidget);
        setCentralWidget(centralSeznamDuplINWidget);
    }
}

void MainWindow::createCentralDuplSkupItemWidget()
{
    setWindowTitle(tr("Správce fotografií - duplicitní skupiny"));
    stavProgramu = DUPLICITYSKUPIN;
    createWidgets();
    if (zobrazitFoto)
        createCentralDuplSkupPictureWidget();
    else
    {
        if (centralWidget())
        {
            centralWidget()->setParent(0);
        }

        setCentralWidget(duplSkupinyWidget);
    }


}

void MainWindow::createCentralFotoPictureWidget()
{
    if (allFoto->getFotoSize() > 0)
        setWindowTitle(tr("Správce fotografií - nerozdělené fotografie"));

    stavProgramu = VSE;
    createWidgets();

    reCreateGroups();

    if(centralWidget())
    {
        centralWidget()->layout()->removeWidget(exifPanelWidget);
        centralWidget()->setParent(0);
    }
    centralFotoVseWidget->layout()->addWidget(exifPanelWidget);
    setCentralWidget(centralFotoVseWidget);

}

void MainWindow::createCentralGroupPictureWidget()
{
    setWindowTitle(tr("Správce fotografií - skupiny fotografií"));

    stavProgramu = SKUPINY;
    createWidgets();

    reCreateGroups();

    if(centralWidget())
    {
        centralWidget()->layout()->removeWidget(exifPanelWidget);
        centralWidget()->setParent(0);
    }
    centralFotoSkupinyWidget->layout()->addWidget(exifPanelWidget);
    setCentralWidget(centralFotoSkupinyWidget);

}

void MainWindow::createCentralDuplikPictureWidget()
{
    setWindowTitle(tr("Správce fotografií - vnitřní duplikáty"));

    stavProgramu = DUPLICITYIN;
    createWidgets();

    reCreateGroups();

    if(centralWidget())
    {
        centralWidget()->layout()->removeWidget(exifPanelWidget);
        centralWidget()->setParent(0);
    }

    centralFotoDuplINWidget->layout()->addWidget(exifPanelWidget);
    setCentralWidget(centralFotoDuplINWidget);

}

void MainWindow::createCentralDuplSkupPictureWidget()
{
    setWindowTitle(tr("Správce fotografií - duplicitní skupiny"));

    stavProgramu = DUPLICITYSKUPIN;
    createWidgets();

    if (centralWidget())
    {
        centralWidget()->setParent(0);
    }

    setCentralWidget(duplSkupPictureWidget);


}

/*
 * Vytvori vsechny akce, ktere jsou v Menu okna
 */
void MainWindow::createActions()
{
    //AKCE Z FOTOMENU
    //Vyhledat foto
    findFotoAction = new QAction(tr("&Import"), this);
    findFotoAction->setShortcut(tr("Ctrl+F"));
    findFotoAction->setStatusTip(tr("Vyhledá fotografie"));
    connect(findFotoAction, SIGNAL(triggered()), this, SLOT(findFotos()));

    //otevre posledni vyhledani
    openLast = new QAction(tr("&Otevřít"));
    openLast->setShortcut(tr("Ctrl+O"));
    openLast->setStatusTip(tr("Otevře posledně vytvořený soubor s informacemi"));
    connect(openLast, SIGNAL(triggered()), this, SLOT(openLastFile()));

    //Smaze soubor s informacemi o fotografiich
    deleteFileAction = new QAction(tr("&Smazat soubor"), this);
    deleteFileAction->setShortcut(tr("Ctrl+D"));
    deleteFileAction->setStatusTip(tr("Smaže soubor s informacemi"));
    connect(deleteFileAction, SIGNAL(triggered()), this, SLOT(deleteFileFotos()));

    //Exportuje vysledky
    exportAction = new QAction(tr("&Exportovat fotografie"), this);
    exportAction->setShortcut(tr("Ctrl+E"));
    exportAction->setStatusTip(tr("Exportuje nastavení fotografií"));
    connect(exportAction, SIGNAL(triggered()), this, SLOT(exportFotos()));

    //Ukonci program
    exitAction = new QAction(tr("&Konec"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Ukončí aplikaci"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    //AKCE Z ZOBRAZIT
    zobrazitSeznam = new QAction(tr("&Zobrazit jako seznam"), this);
    zobrazitSeznam->setStatusTip(tr("Zobrazí fotografie jako seznam"));
    connect(zobrazitSeznam, SIGNAL(triggered()), this, SLOT(createSeznam()));

    zobrazitFotografie = new QAction(tr("&Zobrazit náhledy"), this);
    zobrazitFotografie->setStatusTip(tr("Zobrazí fotografie jako obrazky"));
    connect(zobrazitFotografie, SIGNAL(triggered()), this, SLOT(createImageFotos()));

    zobrazitVse = new QAction(tr("&Zobrazit všechny fotografie"), this);
    zobrazitVse->setStatusTip(tr("Zobrazí fotografie nerozřazené do skupin"));
    connect(zobrazitVse, SIGNAL(triggered()), this, SLOT(createCentralFotoItemWidget()));

    zobrazitSkupiny = new QAction(tr("&Zobrazit skupiny fotografii"), this);
    zobrazitSkupiny->setStatusTip(tr("Zobrazí fotografie rozřazené do skupin"));
    connect(zobrazitSkupiny, SIGNAL(triggered()), this, SLOT(createCentralGroupItemWidget()));

    zobrayitDuplikatyIN = new QAction(tr("&Zobrazit duplikaty ve skupinách"), this);
    zobrayitDuplikatyIN->setStatusTip(tr("Zobrazí duplicitní fotografie vevnitř skupin"));
    connect(zobrayitDuplikatyIN, SIGNAL(triggered()), this, SLOT(createCentralDuplikItemWidget()));

    zobrazitDuplicitniSkupiny = new QAction(tr("&Zobrazit duplicitní skupiny"), this);
    zobrazitDuplicitniSkupiny->setStatusTip(tr("Umožní zobrazit duplicitní skupiny"));
    connect(zobrazitDuplicitniSkupiny, SIGNAL(triggered()), this, SLOT(createCentralDuplSkupItemWidget()));




    //AKCE Z SKUPIN
    findSkupinyAction = new QAction(tr("&Vyhledat skupiny"), this);
    findSkupinyAction->setShortcut(tr("Ctrl+S"));
    findSkupinyAction->setStatusTip(tr("Roztřídí fotografie do skupin"));
    connect(findSkupinyAction, SIGNAL(triggered()), this, SLOT(createGroups()));

    //AKCE Z DUPLIKATU
    findDuplikatySkupinAction = new QAction(tr("&Vyhledat duplitáky skupin"), this);
    findDuplikatySkupinAction->setShortcut(tr("Ctrl+R"));
    findDuplikatySkupinAction->setStatusTip(tr("Vyhledá duplicitní skupiny"));
    connect(findDuplikatySkupinAction, SIGNAL(triggered()), this, SLOT(findDuplicats()));


    findDuplikatyVnitrniAction = new QAction(tr("&Vyhledat duplitáky ve skupinách"), this);
    findDuplikatyVnitrniAction->setStatusTip(tr("Vyhledá duplicitní fotografie vevnitř skupin"));
    connect(findDuplikatyVnitrniAction, SIGNAL(triggered()), this, SLOT(findDuplicatsInGrous()));

    //AKCE Z O PROGRAMU
    //O programu
    aboutAction = new QAction(tr("&O Programu"), this);
    aboutAction->setStatusTip(tr("Zobrazi informace o programu"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    //O Qt
    aboutQtAction = new QAction(tr("&O Qt"), this);
    aboutQtAction->setStatusTip(tr("Zobrazi informace o Qt"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

}

/*
 * Vytvori menu programu
 */
void MainWindow::createMenu()
{
    programMenu = menuBar()->addMenu(tr("&Soubor"));
    programMenu->addAction(findFotoAction);
    programMenu->addAction(openLast);
    programMenu->addAction(deleteFileAction);
    programMenu->addAction(exportAction);
    programMenu->addSeparator();
    programMenu->addAction(exitAction);

    skupinyMenu = menuBar()->addMenu(tr("&Skupiny"));
    skupinyMenu->addAction(findSkupinyAction);

    duplikatyMenu = menuBar()->addMenu(tr("&Duplicity"));
    duplikatyMenu->addAction(findDuplikatyVnitrniAction);
    duplikatyMenu->addAction(findDuplikatySkupinAction);

    zobrazitMenu = menuBar()->addMenu(tr("&Zobrazit"));
    zobrazitMenu->addAction(zobrazitSeznam);
    zobrazitMenu->addAction(zobrazitFotografie);
    zobrazitMenu->addAction(zobrazitVse);
    zobrazitMenu->addAction(zobrazitSkupiny);
    zobrazitMenu->addAction(zobrayitDuplikatyIN);
    zobrazitMenu->addAction(zobrazitDuplicitniSkupiny);


    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void MainWindow::createAll()
{
    allFoto = new AllFoto;
    exifPanelWidget = new ExifPanel;
    exifPanelWidget->setAllFoto(allFoto);

    inicialSeznamWidgets();
    icicialPictureWidgets();



}

/*
 * Inicalizuje vytvori widgety pro zobrazovani fotografii jako seznamu
 */
void MainWindow::inicialSeznamWidgets()
{
    //seznam pro zobrazeni nerozrazenych fotografii
    fotoItemsWidget = new FotoItemWidget;
    fotoItemsWidget->setAllFoto(allFoto);
    fotoItemsWidget->setExifPanel(exifPanelWidget);

    QHBoxLayout *layoutSeznamVse = new QHBoxLayout;
    layoutSeznamVse->addWidget(fotoItemsWidget);
    layoutSeznamVse->addWidget(exifPanelWidget);
    layoutSeznamVse->setStretchFactor(fotoItemsWidget, 6);
    layoutSeznamVse->setContentsMargins(0,0,0,0);

    centralSeznamVseWidget = new QWidget;
    centralSeznamVseWidget->setLayout(layoutSeznamVse);
    setCentralWidget(centralSeznamVseWidget);

    //seznam pro skupiny
    skupinzItemsWidget = new SkupinyItemWidget;
    skupinzItemsWidget->setAllFoto(allFoto);
    skupinzItemsWidget->setExifPanel(exifPanelWidget);

    QHBoxLayout *layoutSeznamSkupiny = new QHBoxLayout;
    layoutSeznamSkupiny->addWidget(skupinzItemsWidget);
    layoutSeznamSkupiny->setStretchFactor(skupinzItemsWidget, 6);
    layoutSeznamSkupiny->setContentsMargins(0,0,0,0);

    centralSeznamSkupinyWidget = new QWidget;
    centralSeznamSkupinyWidget->setLayout(layoutSeznamSkupiny);

    //seznam pro duplikaty
    duplINItemsWidget = new DuplikatyItemWidget;
    duplINItemsWidget->setAllFoto(allFoto);
    duplINItemsWidget->setExifPanel(exifPanelWidget);

    QHBoxLayout *layoutSeznamDuplIN = new QHBoxLayout;
    layoutSeznamDuplIN->addWidget(duplINItemsWidget);
    layoutSeznamDuplIN->setStretchFactor(duplINItemsWidget, 6);
    layoutSeznamDuplIN->setContentsMargins(0,0,0,0);

    centralSeznamDuplINWidget = new QWidget;
    centralSeznamDuplINWidget->setLayout(layoutSeznamDuplIN);

    //widget pro duplikaty skupin
    duplSkupinyWidget = new DuplikatySkupItemWidget;
    duplSkupinyWidget->setAllFoto(allFoto);
    duplSkupinyWidget->setExifPanel(exifPanelWidget);

}

/*
 * Inicalizuje vytvori widgety pro zobrazovani fotografii pomoci preview fotografii
 */
void MainWindow::icicialPictureWidgets()
{
    //zobrazeni nerozdelenych fotografii
    fotoPictureWidget = new FotoPictureWidget;
    fotoPictureWidget->setAllFoto(allFoto);
    fotoPictureWidget->setExifPanel(exifPanelWidget);

    QHBoxLayout *layoutFotoVseWidget = new QHBoxLayout;
    layoutFotoVseWidget->addWidget(fotoPictureWidget);
    layoutFotoVseWidget->setStretchFactor(fotoPictureWidget, 6);
    layoutFotoVseWidget->setContentsMargins(0,0,0,0);

    centralFotoVseWidget = new QWidget;
    centralFotoVseWidget->setLayout(layoutFotoVseWidget);

    //Zobrazeni skupin fotografi pomoci preview
    skupinyPictureWidget = new FotoPictureWidget;
    skupinyPictureWidget->setAllFoto(allFoto);
    skupinyPictureWidget->setExifPanel(exifPanelWidget);

    QHBoxLayout *layoutFotoSkupinyWidget = new QHBoxLayout;
    layoutFotoSkupinyWidget->addWidget(skupinyPictureWidget);
    layoutFotoSkupinyWidget->setStretchFactor(skupinyPictureWidget, 6);
    layoutFotoSkupinyWidget->setContentsMargins(0,0,0,0);

    centralFotoSkupinyWidget = new QWidget;
    centralFotoSkupinyWidget->setLayout(layoutFotoSkupinyWidget);

    //Zobrazeni duplikatu ve skupinach pomoci preview
    duplINPictureWidget = new FotoPictureWidget;
    duplINPictureWidget->setDupl(true);
    duplINPictureWidget->setAllFoto(allFoto);
    duplINPictureWidget->setExifPanel(exifPanelWidget);

    QHBoxLayout *layoutFotoDuplINWidget = new QHBoxLayout;
    layoutFotoDuplINWidget->addWidget(duplINPictureWidget);
    layoutFotoDuplINWidget->setStretchFactor(duplINPictureWidget, 6);
    layoutFotoDuplINWidget->setContentsMargins(0,0,0,0);

    centralFotoDuplINWidget = new QWidget;
    centralFotoDuplINWidget->setLayout(layoutFotoDuplINWidget);

    //okno pro zobrazovani duplicitnich skupin
    duplSkupPictureWidget = new DuplikatySkupPictureWidget;
    duplSkupPictureWidget->setAllFoto(allFoto);
    duplSkupPictureWidget->setExifPanel(exifPanelWidget);
}

void MainWindow::setMenus()
{
    //Overeni zda program jede prvni krat - AKA soubor nemuye existovat
    QFileInfo file("foto.txt");
    if (file.exists() && file.isFile())
        firstTime = false;
    else
        firstTime = true;

    //nastaveni menu, jestli program jede prvni krat, nektere tlacitka nebudou aktivni
    if (firstTime)
    {
        openLast->setEnabled(false);
        deleteFileAction->setEnabled(false);
    }

    exportAction->setEnabled(false);

    skupinyMenu->setEnabled(false);
    zobrazitMenu->setEnabled(false);
    duplikatyMenu->setEnabled(false);

    zobrayitDuplikatyIN->setEnabled(false);
    zobrazitDuplicitniSkupiny->setEnabled(false);
    zobrazitSkupiny->setEnabled(false);

}

void MainWindow::createWidgets()
{
    if (zobrazitFoto)
    {
        switch (stavProgramu) {
        case VSE:
            if (!fotoPictureWidget->isWithData())
                fotoPictureWidget->createVseWidget();
            break;
        case SKUPINY:
            if (!skupinyPictureWidget->isWithData())
                skupinyPictureWidget->createGroupWidget();
            break;
        case DUPLICITYIN:
            if (!duplINPictureWidget->isWithData())
                duplINPictureWidget->createGroupWidget();
            break;
        case DUPLICITYSKUPIN:
            if (!duplSkupPictureWidget->isWithData())
                duplSkupPictureWidget->createWidget();
            break;
        default: //k tomuhle tady by se to nidky nemelo dostat
            if (!fotoPictureWidget->isWithData())
                fotoPictureWidget->createVseWidget();
            stavProgramu = VSE;
            break;

        }
    }
    else
    {
        switch (stavProgramu) {
        case VSE:
            if (!fotoItemsWidget->isWithData())
                fotoItemsWidget->createWidget();
            break;
        case SKUPINY:
            if (!skupinzItemsWidget->isWithData())
                skupinzItemsWidget->createWidget();
            break;
        case DUPLICITYIN:
            if (!duplINItemsWidget->isWithData())
                duplINItemsWidget->createWidget();
            break;
        case DUPLICITYSKUPIN:
            if (!duplSkupinyWidget->isWithData())
                duplSkupinyWidget->createWidget();
            break;
        default:
            if (!fotoItemsWidget->isWithData())
                fotoItemsWidget->createWidget();
            stavProgramu = VSE;
            break;

        }

    }

}

void MainWindow::displayWidgets()
{
    if (zobrazitFoto)
    {
        switch (stavProgramu) {
        case VSE:
            createCentralFotoItemWidget();
            break;
        case SKUPINY:
            createCentralGroupItemWidget();
            break;
        case DUPLICITYIN:
            createCentralDuplikPictureWidget();
            break;
        case DUPLICITYSKUPIN:
            createCentralDuplSkupPictureWidget();
            break;
        default: //k tomuhle tady by se to nidky nemelo dostat
            createCentralFotoItemWidget();
            stavProgramu = VSE;
            break;

        }
    }
    else
    {
        switch (stavProgramu) {
        case VSE:
            createCentralFotoItemWidget();
            break;
        case SKUPINY:
            createCentralGroupItemWidget();
            break;
        case DUPLICITYIN:
            createCentralDuplikItemWidget();
            break;
        case DUPLICITYSKUPIN:
            createCentralDuplSkupItemWidget();
            break;
        default:
            createCentralFotoItemWidget();
            stavProgramu = VSE;
            break;

        }

    }

}

void MainWindow::reCreateGroups()
{
    if(skupinyPictureWidget->isInsideGroup())
        skupinyPictureWidget->recreateGroups(true);

    if(duplINPictureWidget->isInsideGroup())
        duplINPictureWidget->recreateGroups(true);
}

/*
 * Pri smazani souboru nastavi program do puvodniho stavu
 */
void MainWindow::cleanAll()
{
    stavProgramu = VSE;
    firstTime = true;
    setWindowTitle("Správce fotografií");
    zmena = false;
    zobrazitFoto = false;

    //nastavit menu
    setMenus();

    fotoItemsWidget->clearWidget();
    skupinzItemsWidget->clearWidget();
    duplINItemsWidget->clearWidget();
    duplSkupinyWidget->clearWidget();

    fotoPictureWidget->clearWidget();
    skupinyPictureWidget->clearWidget();
    duplINPictureWidget->clearWidget();
    duplSkupPictureWidget->clearWidget();

    allFoto->clearWidget();
    exifPanelWidget->clearWidget();

    displayWidgets();


}
