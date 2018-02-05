#ifndef FOTOPICTUREWIDGET_H
#define FOTOPICTUREWIDGET_H

#include <QFrame>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsGridLayout>
#include <QGraphicsProxyWidget>
#include <QVBoxLayout>
#include <QList>

#include "fotowidget.h"
#include "allfoto.h"
#include "exifpanel.h"

class FotoPictureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FotoPictureWidget(QWidget *parent = 0);

    void createVseWidget();
    void createGroupWidget();

    void setExifPanel(ExifPanel *exifPanel);
    void setAllFoto(AllFoto *allFoto);

    void setDupl(bool dupl);

    bool isWithData();

    bool isInsideGroup();

    void clearWidget();

signals:

public slots:
    void recreateGroups(bool click);

private slots:
    void setExifData(FotoWidget *fotoWidget);
    void setExifDataGroup(FotoWidget *fotoWidget);
    void createFotoFromGroup(int gr);
    void createDuplINFoto(int gr);
    void updateWidget();

private:

    QGraphicsView *view;
    QGraphicsScene *scene;

    AllFoto *allFoto;
    ExifPanel *exifPanel;

    //pomocny list pro zotaveni zobrazovani skupin
    QList<QGraphicsItem *> listSK;
    QList<QGraphicsWidget *> help;

    QList<FotoWidget *> listWidget;
    QList<FotoWidget *> listWidgetSK;



    bool dupl;

    bool haveData;

    bool isInside;

    bool free;




};

#endif // FOTOPICTUREWIDGET_H
