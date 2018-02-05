#ifndef EXIFPANEL_H
#define EXIFPANEL_H

#include <QWidget>
#include <QDateTimeEdit>
#include <QDateTime>
#include <QVBoxLayout>
#include <QListWidget>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QPushButton>

#include "exifitemwidget.h"
#include "exifdateitemwidget.h"
#include "allfoto.h"

class ExifPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ExifPanel(QWidget *parent = 0);

    void setAllFoto(AllFoto *allFoto);

    void setExifData(QString path, QString size, QDateTime date, QString width, QString height, double lat, double longi);

    QPushButton *zpetButton();
    void setButtonZpet(bool zapnout);

    void setInfo(bool group, bool isInGroup, int groupIndex, int fotoIndex);
    void printInfo();

    void clearWidget();

signals:
    void updateWidgets();
//public slots:
//    void setExifData(QListWidgetItem *item);

public slots:
    void removeFoto(bool click);

private:
    AllFoto *allFoto;

    ExifItemWidget *exifName;
    ExifItemWidget *exifSize;
    ExifItemWidget *exifWidth;
    ExifItemWidget *exifHeight;
    ExifItemWidget *exifGPSLat;
    ExifItemWidget *exifGPSLong;

//    QLabel *dateLabel;
//    QDateTimeEdit *dateTime;

    ExifDateItemWidget *exifDate;

    QPushButton *zpet;
    QPushButton *smazat;

    bool group;
    bool isInGroup;
    int groupIndex;
    int fotoIndex;
};

#endif // EXIFPANEL_H
