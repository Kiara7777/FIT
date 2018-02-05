#ifndef SKUPINA_H
#define SKUPINA_H

#include "foto.h"
#include <QString>
#include <QVector>
#include <QTextStream>
#include "duplicita.h"


class Skupina
{
    static int pocetSkupin;

    int pocet;
    QDateTime skupDateTime;
    QDateTime lastSkupDateTime;
    QVector<Foto> skupinaFoto;
    QVector<QDate> date;
    QString path;
    double lat;
    double longi;

    QVector<Duplicita> duplikaty;
    QVector<DuplicitniFoto> vnitrniDuplikaty;

    QVector<int> helpFoto;

    bool nezarazeno;

public:
    //KONSTRUKTORY
    Skupina();
    Skupina(QString path, QDateTime skupDateTime);
    Skupina(QString path, double lat, double longi, QDateTime skupDateTime);

    //KOPIROVACI KONSTRUKTOR
    Skupina(const Skupina &s);

    //DESTRUKTOR
    ~Skupina(){pocetSkupin--;}

    //TRIDNI METODY
    static int getPocetSkupin();

    //METODY
    void cleanDuplikIN();
    void cleanDuplikSKUP();

    void addFoto(Foto foto, int index);
    void addDuplikat(Duplicita duplikat);
    void addDate(QDate date);
    void addVnitrniDuplik(DuplicitniFoto duplikaty);

    QDateTime getSkupDateTime();
    QDateTime getLastSkupDateTime();
    QString getPath();
    double getLat();
    double getLongi();

    Foto getFoto(int index);
    int getSize();

    int getDateSize();
    QDate getDate(int index);

    int getDuplicitaINSize();
    DuplicitniFoto getDuplicitniFoto(int index);

    int getDuplikatySkupinSize();
    Duplicita getDuplikatySkupin(int duplikat);

    void setLastSkupDateTime(QDateTime skupDateTime);
    void setLat(double lat);
    void setLongi(double longi);

    void setPreviewToFoto(int index, QPixmap map);

    void printDuplFoto();
    void printFoto();
    void printDate();

    int getHelpFotoSize();
    int getHelpFoto(int index);

    void setNezarazeno(bool neza);
    bool getNezarazeno();

};

#endif // SKUPINA_H
