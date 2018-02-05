#ifndef ALLFOTO_H
#define ALLFOTO_H

#include <QVector>
#include <QFile>
#include <QFileInfo>
#include <QtMath>
#include <QImage>
#include <QProgressDialog>

#include "foto.h"
#include "skupina.h"
#include "duplicita.h"

#include <algorithm>
#include <functional>
#include <vector>


/*
 * Slouzi jako pomocny objekt pomoci ktereho se prenaseji fotografie
 * Ma v sobe ulozene fotografie
 */
class AllFoto
{
public:
    AllFoto();
    ~AllFoto();

    double toDecimal(QString souradnice, QString smer);
    int createFotosItem(QString filename);
    void createGroups(int type, int typeSize);
    void duplVSkup();
    void duplicitniSkupiny();

    void setFotosPicture();

    void printFotos();
    void printGroups();
    void printFotoIndex();
    void printDuplIN();
    void printDuplicitniSkupiny();
    void printPixmapInfo();

    Foto getFoto(int index);
    int getFotoSize();

    int getGroupSize();
    int getGroupFotoSize(int group);
    Foto getFotoFromGroup(int goup, int foto);

    int getGroupDuplINsize(int group);
    DuplicitniFoto getDuplicitniFoto(int group, int duplicita);
    bool haveDuplIN(int group);

    int getGoupDuplSkupSize(int group);
    Duplicita getDuplicita(int group, int duplicita);
    bool haveDuplSkup(int group);

    bool isPixmap();

    void clearWidget();

    void remove();
    void removeFoto(int foto);
    void removeGroup(int group);
    void removeFromGroup(int group, int foto);

    QString deathFoto(int index);
    int dethFotoSize();
    bool isNezarazeno(int group);

    void setNewPath(int group, int foto, QString newPath);

private:
    Foto createFoto(QString exifData);
    void createTimeFotoGroup(int minut);
    double haversineFormula(double lat1, double longi1, double lat2, double longi2);
    void createGPSFotoGroup(double km);
    QPixmap findFotosPreview(QString file);

    int pocetFotoPic();

    QVector<Foto> allFoto;
    QVector<Skupina> skupiny;

    QVector<QString> fotoDeath;

    bool pixmap;

    QProgressDialog *progress;

    int groupType;
    int intervalGroup;
    bool duplIN;
    bool duplSK;
};

#endif // ALLFOTO_H
