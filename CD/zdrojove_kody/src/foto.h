#ifndef FOTO_H
#define FOTO_H

#include <QString>
#include <QDateTime>
#include <QDebug>
#include <QPixmap>

class Foto
{
    static int pocet;

    QString fotoPath;
    bool ok;
    QDateTime creationDate;
    QString width; //X
    QString height; //Y
    QString velikost;
    double lat;
    double longi;
    bool gps;

    QPixmap preview;
    bool pixmap;

    QString allInfo;

public:
    //KONSTRUKTORY
    Foto(){pocet++;}
    Foto(QString fotoPath, QString velikost);
    Foto(QString fotoPath, QDateTime creationDate, QString width,
         QString height, QString velikost);
    Foto(QString fotoPath, QDateTime creationDate, QString width,
         QString height, QString velikost, double lat, double longi);

    //KOPIROVACI KONSTRUKTOR
    Foto(const Foto &f);

    //DESTRUKTOR
    ~Foto(){pocet--;}

    //STATICKE TRIDNI METODA
    static int getPocet();

    //METODY

    void setPrewiev(QPixmap preview);
    QPixmap getPreview();

    QDateTime getDateTime();

    QString getPath();
    QString getWidth();
    QString getHeight();
    QString getVelikost();
    double getLatitude();
    double getLongitude();
    bool isOK();
    bool isGPS();

    void setAllInfo(QString info);
    QString getAllInfo();

    void setNewPath(QString newPath);

};

#endif // FOTO_H
