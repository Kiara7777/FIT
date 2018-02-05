#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QErrorMessage>
#include <QFileInfo>

#include "exifpanel.h"
#include "allfoto.h"

#include "fotoitemwidget.h"
#include "skupinyitemwidget.h"
#include "duplikatyitemwidget.h"
#include "duplikatyskupitemwidget.h"

#include "fotopicturewidget.h"
#include "duplikatyskuppicturewidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void findFotos();
    void openLastFile();
    void deleteFileFotos();
    bool exportFotos();

    void createGroups();

    void findDuplicatsInGrous();
    void findDuplicats();


    void about();

    void createSeznam();
    void createImageFotos();

    void createCentralFotoItemWidget();
    void createCentralGroupItemWidget();
    void createCentralDuplikItemWidget();
    void createCentralDuplSkupItemWidget();

    void createCentralFotoPictureWidget();
    void createCentralGroupPictureWidget();
    void createCentralDuplikPictureWidget();
    void createCentralDuplSkupPictureWidget();

private:

    void createActions();
    void createMenu();
    void createAll();
    int createFotos();

    void inicialSeznamWidgets();
    void icicialPictureWidgets();

    void setMenus();

    void createWidgets();
    void displayWidgets();
    void reCreateGroups();

    void cleanAll();



    //MENU PROGRAMU
    QMenu *programMenu;
    QMenu *zobrazitMenu;
    QMenu *skupinyMenu;
    QMenu *duplikatyMenu;
    QMenu *helpMenu;

    //AKCE Z PROGRAM MENU
    QAction *findFotoAction;
    QAction *openLast;
    QAction *deleteFileAction;
    QAction *exportAction;
    QAction *exitAction;

    //AKCE Z ZOBRAZIT MENU
    QAction *zobrazitSeznam;
    QAction *zobrazitFotografie;
    QAction *zobrazitVse;
    QAction *zobrazitSkupiny;
    QAction *zobrayitDuplikatyIN;
    QAction *zobrazitDuplicitniSkupiny;

    //AKCE Z SKUPINY MENU
    QAction *findSkupinyAction;

    //AKCE Z DUPLIKATY MENU
    QAction *findDuplikatyVnitrniAction;
    QAction *findDuplikatySkupinAction;

    //AKCE Z HELP MENU
    QAction *aboutAction;
    QAction *aboutQtAction;

    QString fileFoto;

    //Vyhledani souboru

    //Pro errory
    QErrorMessage msg;

    //CENTRALNI WIDGET

    //specialni objekt - Uchovava vsechny fotografie a predava se mezi widgety
    AllFoto *allFoto;
    ExifPanel *exifPanelWidget;

    //////////////////////SEZNAMY/////////////////////////////
    //Centralni widgety pro seznamy
    FotoItemWidget *fotoItemsWidget;
    SkupinyItemWidget *skupinzItemsWidget;
    DuplikatyItemWidget *duplINItemsWidget;
    DuplikatySkupItemWidget *duplSkupinyWidget;

    //widgety do kterych se ukladaji centralinni zobrayovani
    QWidget *centralSeznamVseWidget;
    QWidget *centralSeznamSkupinyWidget;
    QWidget *centralSeznamDuplINWidget;

    ////////////////////////FOTOGRAFIE///////////////////////////
    //centralni widgety pro fotografie
    FotoPictureWidget *fotoPictureWidget;
    FotoPictureWidget *skupinyPictureWidget;
    FotoPictureWidget *duplINPictureWidget;
    DuplikatySkupPictureWidget *duplSkupPictureWidget;


    QWidget *centralFotoVseWidget;
    QWidget *centralFotoSkupinyWidget;
    QWidget *centralFotoDuplINWidget;

    bool zobrazitFoto;
    bool firstTime;
    bool zmena;

    enum Stav {VSE, SKUPINY, DUPLICITYIN, DUPLICITYSKUPIN};

    Stav stavProgramu;



};

#endif // MAINWINDOW_H
