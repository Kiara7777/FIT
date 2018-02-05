#ifndef SKUPINYITEMWIDGET_H
#define SKUPINYITEMWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>

#include "fotoitemwidget.h"
#include "foto.h"
#include "skupina.h"
#include "allfoto.h"

class SkupinyItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SkupinyItemWidget(QWidget *parent = 0);

    void createWidget();

    void setExifPanel(ExifPanel *exifPanel);
    void setAllFoto(AllFoto *allFoto);

    bool isWithData();
    void clearWidget();

private slots:
    void setExifData(QTreeWidgetItem *item, int column);
    void updateWidget();

private:

    QTreeWidget *treeWidget;

    ExifPanel *exifPanel;
    AllFoto *allFoto;

    bool haveData;
};

#endif // SKUPINYITEMWIDGET_H
