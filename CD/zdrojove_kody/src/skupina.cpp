#include "skupina.h"

//TRIDNI PROMENNE
int Skupina::pocetSkupin = 0;

//KONSTRUKTORY
Skupina::Skupina()
{
    QDateTime datime = QDateTime();
    this->path = "";
    this->skupDateTime = datime;
    this->lastSkupDateTime = datime;
    this->lat = 0;
    this->longi = 0;
    pocetSkupin++;
    pocet = 0;

}

Skupina::Skupina(QString path, QDateTime skupDateTime)
{
    this->path = path;
    this->skupDateTime = skupDateTime;
    this->lastSkupDateTime = skupDateTime;
    this->lat = 0;
    this->longi = 0;
    pocetSkupin++;
    pocet = 0;
}

Skupina::Skupina(QString path, double lat, double longi, QDateTime skupDateTime)
{
    this->path = path;
    this->skupDateTime = skupDateTime;
    this->lastSkupDateTime = skupDateTime;
    this->lat = lat;
    this->longi = longi;
    pocetSkupin++;
    pocet = 0;
}

//KOPIROVACI KONSTRUKTOR
Skupina::Skupina(const Skupina &s)
{
    this->path = s.path;
    this->skupDateTime = s.skupDateTime;
    this->lastSkupDateTime = s.lastSkupDateTime;
    this->lat = s.lat;
    this->longi = s.longi;
    pocetSkupin++;
    pocet = s.pocet;
    for (int i = 0; i < s.skupinaFoto.size(); i++)
        this->skupinaFoto.append(s.skupinaFoto[i]);

    for (int i = 0; i < s.vnitrniDuplikaty.size(); i++)
        this->vnitrniDuplikaty.append(s.vnitrniDuplikaty[i]);

    for(int i = 0; i < s.date.size(); i++)
        this->date.append(s.date[i]);

    for (int i = 0; i < s.duplikaty.size(); i++)
        this->duplikaty.append(s.duplikaty[i]);

    for (int i = 0; i < s.helpFoto.size(); i++)
        this->helpFoto.append(s.helpFoto[i]);

    nezarazeno = s.nezarazeno;

}

//TRIDNI METODA
/*
 * Vrati pocet skupin, kontrola pro kopirovaci konstruktor
 */
int Skupina::getPocetSkupin()
{
    return pocetSkupin;
}



//METODY

void Skupina::cleanDuplikIN()
{
    vnitrniDuplikaty.clear();

}

void Skupina::cleanDuplikSKUP()
{
    duplikaty.clear();
}

/*
 * Vrati aktualni datum a cas skupiny
 */
QDateTime Skupina::getSkupDateTime()
{
    return skupDateTime;
}

/*
 * Vrati datum a cas posledne pridane fotogrfaie
 */
QDateTime Skupina::getLastSkupDateTime()
{
    return lastSkupDateTime;
}

/*
 * Vrati objekt fotografie na danem indexu
 */
Foto Skupina::getFoto(int index)
{
    return skupinaFoto[index];
}

/*
 * Nastavi datum a cas posledne pridane fotografie
 */
void Skupina::setLastSkupDateTime(QDateTime skupDateTime)
{
    this->lastSkupDateTime = skupDateTime;
}

/*
 * Nastavi latitude/zemepisna sirka posledne pridane fotografie
 */
void Skupina::setLat(double lat)
{
    this->lat = lat;
}

/*
 * Nastavi longitude/zemepisna delka posledne pridane fotografie
 */
void Skupina::setLongi(double longi)
{
    this->longi = longi;
}

void Skupina::setPreviewToFoto(int index, QPixmap map)
{
    skupinaFoto[index].setPrewiev(map);
}

/*
 * Do vektoru fotografii prida zadanou fotografii
 */
void Skupina::addFoto(Foto foto, int index)
{
    skupinaFoto.append(foto);
    pocet++;

    helpFoto.append(index);
}

/*
 * Vrati pocet fotografii ve skupine
 */
int Skupina::getSize()
{
    return skupinaFoto.size();
}

/*
 * Vrati cestu ke skupine fotografii
 */
QString Skupina::getPath()
{
    return path;
}

/*
 * Vrati zemepisnou sirku skupiny
 */
double Skupina::getLat()
{
    return lat;
}

/*
 * Vrati zemepisnou delku skupiny
 */
double Skupina::getLongi()
{
    return longi;
}

int Skupina::getDuplicitaINSize()
{
    return vnitrniDuplikaty.size();
}

DuplicitniFoto Skupina::getDuplicitniFoto(int index)
{
    return vnitrniDuplikaty[index];
}

int Skupina::getDuplikatySkupinSize()
{
    return duplikaty.size();
}

Duplicita Skupina::getDuplikatySkupin(int duplikat)
{
    return duplikaty[duplikat];
}

void Skupina::printDuplFoto()
{
    for (int i = 0; i < vnitrniDuplikaty.size(); i++)
    {
        int main = vnitrniDuplikaty[i].getMainFoto();
        qDebug() << "MAIN: " << skupinaFoto[main].getPath();

        for (int j = 0; j < vnitrniDuplikaty[i].getSize(); j++)
        {
            int foto = vnitrniDuplikaty[i].getFotoIndex(j);
            qDebug() << skupinaFoto[foto].getPath();
        }
    }
}

/*
 * Vytiskne cestu k ulozenym fotografiim ve skupine
 */
void Skupina::printFoto()
{
    for(int i = 0; i < skupinaFoto.size(); i++)
        qDebug() << skupinaFoto[i].getPath();
}

/*
 * Vytiskne datumy ulozenych fotografii
 */
void Skupina::printDate()
{
    for(int i = 0; i < date.size(); i++)
        qDebug() << date[i];
}

int Skupina::getHelpFotoSize()
{
    return helpFoto.size();
}

int Skupina::getHelpFoto(int index)
{
    return helpFoto[index];
}

void Skupina::setNezarazeno(bool neza)
{
    nezarazeno = neza;

}

bool Skupina::getNezarazeno()
{
    return nezarazeno;

}

/*
 * Vrati pocet ruznych datumu fotografii, ktere jsou ulozene ve skupine
 */
int Skupina::getDateSize()
{
    return date.size();
}

/*
 * Prida datum do vektoru datumu ulozenych fotografii
 */
void Skupina::addDate(QDate date)
{
    this->date.append(date);
}

void Skupina::addVnitrniDuplik(DuplicitniFoto duplikaty)
{
    vnitrniDuplikaty.append(duplikaty);
}


void Skupina::addDuplikat(Duplicita duplikat)
{
    duplikaty.append(duplikat);
}

/*
 * Vrati datum na danem indexu
 */
QDate Skupina::getDate(int index)
{
    return date[index];
}

