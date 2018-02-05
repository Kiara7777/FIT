#include "foto.h"

//TRIDNI PROMENNA
int Foto::pocet = 0;

//KONSTRUKTORY
Foto::Foto(QString fotoPath, QString velikost)
{
    this->fotoPath = fotoPath;
    this->creationDate = QDateTime();
    this->width = "";
    this->height = "";
    this->velikost = velikost;
    this->ok = false;
    this->lat = 0;
    this->longi = 0;
    this->gps = false;
    pocet++;
}

Foto::Foto(QString fotoPath, QDateTime creationDate, QString width, QString height, QString velikost)
{
    pocet++;
    this->fotoPath = fotoPath;
    this->creationDate = creationDate;
    this->width = width;
    this->height = height;
    this->velikost = velikost;
    this->ok = true;
    this->lat = 0;
    this->longi = 0;
    this->gps = false;

}

Foto::Foto(QString fotoPath, QDateTime creationDate, QString width,
     QString height, QString velikost, double lat, double longi)
{
    pocet++;
    this->fotoPath = fotoPath;
    this->creationDate = creationDate;
    this->width = width;
    this->height = height;
    this->velikost = velikost;
    this->ok = true;
    this->lat = lat;
    this->longi = longi;
    this->gps = true;
}

//KOPIROVACI KONSTRUKTOR
Foto::Foto(const Foto &f)
{
    fotoPath = f.fotoPath;
    creationDate = f.creationDate;
    width = f.width;
    height = f.height;
    velikost = f.velikost;
    ok = f.ok;
    lat = f.lat;
    longi = f.longi;
    gps = f.gps;
    preview = f.preview;

    allInfo = f.allInfo;
}

//TRIDNI METODA
int Foto::getPocet()
{
    return pocet;
}

void Foto::setPrewiev(QPixmap preview)
{
    QPixmap map = preview.scaled(120, 80);
    this->preview = map;
}

QPixmap Foto::getPreview()
{
    return preview;

}


//METODY
QDateTime Foto::getDateTime()
{
    return creationDate;
}

QString Foto::getPath()
{
    return fotoPath;
}

QString Foto::getWidth()
{
    return width;
}

QString Foto::getHeight()
{
    return height;
}

QString Foto::getVelikost()
{
    return velikost;
}

double Foto::getLatitude()
{
    return lat;
}

double Foto::getLongitude()
{
    return longi;
}

bool Foto::isOK()
{
    return ok;
}

bool Foto::isGPS()
{
    return gps;
}

void Foto::setAllInfo(QString info)
{
    allInfo = info;
}

QString Foto::getAllInfo()
{
    return allInfo;
}

void Foto::setNewPath(QString newPath)
{
    QString old = allInfo;
    QString newInfo = newPath + "\t";

    QStringList list = old.split("\t");

    //prvni je stara cesta - nechci
    //posledni je bordel - nechci
    for (int i = 1; i < list.size()-1; i++)
    {
        newInfo += list[i];
        newInfo += "\t";
    }

    newInfo += "\n";

    allInfo = newInfo;
    fotoPath = newPath;

}

