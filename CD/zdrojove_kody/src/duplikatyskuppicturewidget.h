#ifndef DUPLIKATYSKUPPICTUREWIDGET_H
#define DUPLIKATYSKUPPICTUREWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsGridLayout>
#include <QGraphicsProxyWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QPushButton>
#include <QHBoxLayout>

#include "allfoto.h"
#include "fotowidget.h"
#include "exifpanel.h"

class DuplikatySkupPictureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DuplikatySkupPictureWidget(QWidget *parent = 0);

    void createWidget();
    void setAllFoto(AllFoto *allFoto);
    void setExifPanel(ExifPanel *exifPanel);

    bool isWithData();
    void clearWidget();

signals:

public slots:

private slots:
    void setGroupInfo(int index);
    void setGroupInfo2(int index);
    void removeFoto(bool click);
    void updateWidget();

private:
    void deleteGroups();


    QLabel *nameWindow;

    QComboBox *sk1;
    QLabel *fotosSK1;
    QLabel *fotosDuplSK1;
    QGraphicsView *viewSK1;
    QGraphicsScene *sceneSK1;

    QComboBox *sk2;
    QLabel *fotosSK2;
    QLabel *fotosDuplSK2;
    QGraphicsView *viewSK2;
    QGraphicsScene *sceneSK2;

    QFrame *line;
    QComboBox *deleteBox;
    QPushButton *deleteButton;

    AllFoto *allFoto;
    ExifPanel *exifPanel;

    bool haveData;


};

#endif // DUPLIKATYSKUPPICTUREWIDGET_H
