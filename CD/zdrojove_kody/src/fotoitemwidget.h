#ifndef FOTOITEMWIDGET_H
#define FOTOITEMWIDGET_H

#include <QWidget>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "foto.h"
#include "exifpanel.h"
#include <QListWidget>
#include <QVBoxLayout>
#include <QDateTime>
#include "allfoto.h"



class FotoItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FotoItemWidget(QWidget *parent = 0);

    void createWidget();

    void setExifPanel(ExifPanel *exifPanel);
    void setAllFoto(AllFoto *allFoto);

    bool isWithData();
    void clearWidget();

signals:

public slots:

private slots:
    void setExifData(QListWidgetItem *item);
    void updateWidget();

private:
    QListWidget *listWidget;
    int id;

    ExifPanel *exifPanel;
    AllFoto *allFoto;

    bool haveData;


};

#endif // FOTOITEMWIDGET_H
