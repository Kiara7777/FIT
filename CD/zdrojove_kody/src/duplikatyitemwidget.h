#ifndef DUPLIKATYITEMWIDGET_H
#define DUPLIKATYITEMWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>

#include "exifpanel.h"
#include "allfoto.h"

class DuplikatyItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DuplikatyItemWidget(QWidget *parent = 0);

    void createWidget();

    void setExifPanel(ExifPanel *exifPanel);
    void setAllFoto(AllFoto *allFoto);

    bool isWithData();
    void clearWidget();

signals:

public slots:

private slots:
    void setExifData(QTreeWidgetItem *item, int column);
    void updateWidget();

private:
    QTreeWidget *treeWidget;

    ExifPanel *exifPanel;
    AllFoto *allFoto;

    bool haveData;

};

#endif // DUPLIKATYITEMWIDGET_H
