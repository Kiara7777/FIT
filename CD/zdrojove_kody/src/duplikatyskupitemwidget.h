#ifndef DUPLIKATYSKUPITEMWIDGET_H
#define DUPLIKATYSKUPITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QListWidget>
#include <QGridLayout>
#include <QFrame>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "allfoto.h"
#include "exifpanel.h"

class DuplikatySkupItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DuplikatySkupItemWidget(QWidget *parent = 0);

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

    QComboBox *sk2;
    QLabel *fotosSK2;
    QLabel *fotosDuplSK2;


    QListWidget *listWidgetSK1;
    QListWidget *listWidgetSK2;

    QFrame *line;
    QComboBox *deleteBox;
    QPushButton *deleteButton;


    AllFoto *allFoto;
    ExifPanel *exifPanel;

    bool haveData;
};

#endif // DUPLIKATYSKUPITEMWIDGET_H
