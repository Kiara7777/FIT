#include "allfoto.h"
#include <exiv2/exiv2.hpp>
#include <cassert>

AllFoto::AllFoto()
{
    pixmap = false;
    duplIN = false;
    duplSK = false;
}

AllFoto::~AllFoto()
{
}

/*
 * Prevede GPS souradnice z formatu ktery jsme dosatali z Exif knihovny.
 * Format: String: stupne/x minuty/y sekundy/z
 * Prevede to na desetiny stupne (Todo OVERIT PREKLAD, decimal degrees)
 * Pozor na smer, N a E je +, S a W je -
 */
double AllFoto::toDecimal(QString souradnice, QString smer)
{
    //rozdeli jednotlive casti (stupne, minuty, sekundy od sebe, jsou oddelene mezerou)
    QStringList list = souradnice.split(' ');

    double stupne = 0.0;
    double minuty = 0.0;
    double sekundy = 0.0;

    for (int i = 0; i < list.size(); i++)
    {
         //jednotliva cisla jsou od sebe oddelena /
        QStringList numbers = list[i].split('/');
        double d1 = numbers[0].toDouble();
        double d2 = numbers[1].toDouble();

        switch (i)
        {
        case 0:
            stupne = d1/d2;
            break;
        case 1:
            minuty = d1/d2;
            break;
        case 2:
            sekundy = d1/d2;
            break;
        default:
            break;
        }
    }


    //prevest na decimal degrees, dle vzorce st + m/60 + sek/3600
    double decimal = stupne + minuty/60 + sekundy/3600;

    //zajistit aby to melo spravne znamenko dle smeru N E +, S W -
    if (smer == "S" || smer == "W")
        decimal = -decimal;

    return decimal;
}

/*
 * Ziskany retezec prevede na jednotlive polozky a vytvori z nich fotografii
 */
Foto AllFoto::createFoto(QString exifData)
{
    //jednotlive zaznamz jsou od sebe oddeleny \t
     QStringList list = exifData.split("\t");

     //0 Path, 1 velikost, 2 date, 3 X, 4 Y, 5 Lat, 6 LatR, 7 Longi, 8 LongR
     //kdyz bude list mit jenom 3 prvky, tak tam nejsou exif data
     if (list.size() == 4)
     {
         Foto foto(list[0],list[1]);
         foto.setAllInfo(exifData);
         return foto;
     }

     QDateTime date = QDateTime::fromString(list[2], "yyyy:MM:dd HH:mm:ss");

     //nejsou tam GPS data
     if (list[5] == "NENI")
     {
        Foto foto(list[0], date, list[3], list[4], list[1]);
        foto.setAllInfo(exifData);
        return foto;
     }

     //jeste pred tim nez se vytvori fotografie tak se musi prevest a vypocitat GPS souradnice
     double lat = toDecimal(list[5], list[6]);
     double longi = toDecimal(list[7], list[8]);

     Foto foto(list[0], date, list[3], list[4], list[1], lat, longi);
     foto.setAllInfo(exifData);


     return foto;
}

/*
 * Otevre soubor s inforamcemi, rozdeli na radky a vytvorene fotografie ulozi do vektoru
 */
int AllFoto::createFotosItem(QString filename)
{

    allFoto.clear();
    skupiny.clear();
    pixmap = false;
    duplIN = false;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return -1;

    QTextStream in(&file);

    //vytvorene objekty fotografii
    while(!in.atEnd())
        allFoto.append(createFoto(in.readLine()));

    return 0;
}

/*
 * Dle zadanych parametru rozhodne jak vytvaret skupiny a zavola potrebne funkce
 */
void AllFoto::createGroups(int type, int typeSize)
{
    skupiny.clear();
    duplIN = false;

    groupType = type;
    intervalGroup = typeSize;

    if (type == 0) //DATUM
        createTimeFotoGroup(typeSize);
    else if(type == 1) //GPS
        createGPSFotoGroup(typeSize);

}

/*
 * Vytiskne vektor neroztridenych fotografii
 */
void AllFoto::printFotos()
{
    for(int i = 0; i < allFoto.size(); i++)
        qDebug() << allFoto[i].getPath();
}

/*
 * Vytiskne vsechny skupiny a fotografie v nich
 */
void AllFoto::printGroups()
{
    for(int i = 0; i < skupiny.size(); i++)
    {
        qDebug() << "Skupina " << i;
        for (int j = 0; j < skupiny[i].getSize(); j++)
            qDebug() << skupiny[i].getFoto(j).getPath();
    }
}

/*
 * Vytiskne skupiny a pomocny vektor k nim (pomocny vektor slouzi jako uloziste indexu fotografii z neroytridenych fotografii)
 * Pomocny vektor slouzi jako pomucka k vkladani pixmap do fotografii
 */
void AllFoto::printFotoIndex()
{
    for (int i = 0; i <skupiny.size(); i++)
    {
        qDebug() << "Skupina: " << i;
        for (int j = 0; j < skupiny[i].getHelpFotoSize(); j++)
            qDebug() << skupiny[i].getHelpFoto(j);
    }
}

/*
 * Zavola fukni ktera pro danou skupinu vziskne vnitrni duplicitni fotografi
 */
void AllFoto::printDuplIN()
{
    for(int i = 0; i < skupiny.size(); i++)
    {
        qDebug() << "Skupina " << i;
        skupiny[i].printDuplFoto();
    }
}

/*
 * Zavola fukni ktera pro danou skupinu vziskne vnitrni duplicitni skupiny
 */
void AllFoto::printDuplicitniSkupiny()
{
    for (int i = 0; i < skupiny.size(); i++)
    {
        qDebug() << "Skupina " << i;
        int pocetSkupin = skupiny[i].getDuplikatySkupinSize();
        for (int j = 0; j < pocetSkupin; j++)
        {
            int duplSkupina = skupiny[i].getDuplikatySkupin(j).getSkupina();
            qDebug() << " Ma dupl skupinu: " << duplSkupina;
            int duplcitniFota = skupiny[i].getDuplikatySkupin(j).getSize();
            for (int k = 0; k < duplcitniFota; k++)
            {
                int mainFoto = skupiny[i].getDuplikatySkupin(j).getItem(k).getMainFoto();
                qDebug() << "MAIN FOTO z 1. skupiny " << mainFoto;
                int duplikaty = skupiny[i].getDuplikatySkupin(j).getItem(k).getSize();
                for (int l = 0; l < duplikaty; l++)
                {
                    qDebug() <<  skupiny[i].getDuplikatySkupin(j).getItem(k).getFotoIndex(l);
                }
            }
        }
    }
}

/*
 * Vytiskne info o ulozenych pixmapach
 */
void AllFoto::printPixmapInfo()
{
    if (skupiny.size() != 0)
    {
        for (int i = 0; i < skupiny.size(); i++)
        {
            qDebug() << "Skupina: " << i;
            for (int j = 0; j < skupiny[i].getSize(); j++)
            {
                qDebug() << skupiny[i].getFoto(j).getPath() << " " << skupiny[i].getFoto(j).getPreview().height();
            }
        }
    }

    for(int i = 0; i < allFoto.size(); i++)
        qDebug() << allFoto[i].getPath() << " " << allFoto[i].getPreview().height();
}

/*
 * Vrati velikost vektoru ve kterem jsou ulozeny fotografie - pocet fotografii
 */
int AllFoto::getFotoSize()
{
    return allFoto.size();
}

/*
 * Vrati pocet skupin
 */
int AllFoto::getGroupSize()
{
    return skupiny.size();
}

/*
 * Vrati pocet fotografii v dane skupine
 */
int AllFoto::getGroupFotoSize(int group)
{
    return skupiny[group].getSize();
}

/*
 * Vrati fotografii ze zadaneho indexu ze zadane skupiny
 */
Foto AllFoto::getFotoFromGroup(int goup, int foto)
{
    return skupiny[goup].getFoto(foto);
}

/*
 * Vrati pocet vnitrnich duplikatu ze zadane skupiny
 */
int AllFoto::getGroupDuplINsize(int group)
{
    return skupiny[group].getDuplicitaINSize();
}

/*
 * Vrati dany vnitrni duplikat ze zadane skupiny
 */
DuplicitniFoto AllFoto::getDuplicitniFoto(int group, int duplicita)
{
    return skupiny[group].getDuplicitniFoto(duplicita);
}

bool AllFoto::haveDuplIN(int group)
{
    if (skupiny[group].getDuplicitaINSize())
        return true;
    else
        return false;
}

/*
 * Vrati pocet duplicitnich skupin z dane skupiny
 */
int AllFoto::getGoupDuplSkupSize(int group)
{
    return skupiny[group].getDuplikatySkupinSize();
}

/*
 * Ze zadane skupiny vrati duplicitni skupinu
 */
Duplicita AllFoto::getDuplicita(int group, int duplicita)
{
    return skupiny[group].getDuplikatySkupin(duplicita);
}

/*
 * Vrati informaci zda skupina ma duplicitni skupiny
 */
bool AllFoto::haveDuplSkup(int group)
{
    if (skupiny[group].getDuplikatySkupinSize())
        return true;
    else
        return false;
}

bool AllFoto::isPixmap()
{
    return pixmap;
}

void AllFoto::clearWidget()
{
    allFoto.clear();
    skupiny.clear();
    fotoDeath.clear();
    pixmap = false;
    duplIN = false;
    duplSK = false;
}

void AllFoto::remove()
{
    bool recreateDuplIN = false;
    bool recreateDuplSK = false;

    if (duplIN)
        recreateDuplIN = true;

    if(duplSK)
        recreateDuplSK = true;

    //jenom kdyz mame skupiny
    if (skupiny.size() > 0)
    {
        skupiny.clear();
        createGroups(groupType, intervalGroup);
    }

    if (recreateDuplIN)
        duplVSkup();

    if(recreateDuplSK)
        duplicitniSkupiny();

}

void AllFoto::removeFoto(int foto)
{
    fotoDeath.append(allFoto[foto].getPath());
    allFoto.removeAt(foto);
    remove();
}

void AllFoto::removeGroup(int group)
{
    std::vector<int> pomoc;
    for (int i = 0; i < skupiny[group].getHelpFotoSize(); i++)
    {
        //dostat indexy, ale pozor musim je menit od nejvetsiho do nejmensiho
        pomoc.push_back(skupiny[group].getHelpFoto(i));

    }

    std::sort(pomoc.begin(), pomoc.end(), std::greater<int>());

    for (unsigned int i = 0; i < pomoc.size(); i++)
    {
        fotoDeath.append(allFoto[pomoc[i]].getPath());
        allFoto.removeAt(pomoc[i]);
    }

    remove();
}

void AllFoto::removeFromGroup(int group, int foto)
{
    int fotoIndex = skupiny[group].getHelpFoto(foto);

    fotoDeath.append(allFoto[fotoIndex].getPath());
    allFoto.removeAt(fotoIndex);

    remove();


}

QString AllFoto::deathFoto(int index)
{
//    for (int i = 0; i < fotoDeath.size(); i++)
//    {
//        if (!QFile::remove(fotoDeath[i]))
//            return false;
//    }
    return fotoDeath[index];
}

int AllFoto::dethFotoSize()
{
    return fotoDeath.size();
}

bool AllFoto::isNezarazeno(int group)
{
    return skupiny[group].getNezarazeno();
}

void AllFoto::setNewPath(int group, int foto, QString newPath)
{
    allFoto[skupiny[group].getHelpFoto(foto)].setNewPath(newPath);
}

/*
 * Vrati fotografi na danem indexu
 */
Foto AllFoto::getFoto(int index)
{
    return allFoto[index];
}


/*
* Rozdeli fotogrrafie do skupin.
* Zatim rozdelovani podle datumu.
* Pokud skupina s danym datumem existuje, vloz fotografii do ni,
* pokud ne, vytvor danou skupinu a vloz foto do ni.
*/
void AllFoto::createTimeFotoGroup(int minut)
{

    //projet fotky a rodelit do skupiny podel datumu
    for (int i = 0; i < allFoto.size(); i++)
    {
        QDateTime dateTime = allFoto[i].getDateTime();
        QFileInfo info(allFoto[i].getPath());
        QString pathGroup = info.absolutePath();

        bool vlozeno = false; // zda dana fotografie je vlozena

        //defaultni skupina, skupina NEZARAZENO, bude to prvni skupina
        if (skupiny.size() == 0)
        {
            Skupina newSkupina = Skupina();
            newSkupina.setNezarazeno(true);
            skupiny.append(newSkupina);
        }

        //lze fotografii zaradit? ma exif medata?
        if (!allFoto[i].isOK())
        {
            skupiny[0].addFoto(allFoto[i], i);
            continue;
        }

        //zsjitit jestli existuje skupina z danym datumem
        //ano - vlozit fotku dovnitr
        //ne - vytvorit novou skupinu a vlozit fotku dovnitr
        //nezapomenou novou skupinu vlozit do seznamu skupin
        if (skupiny.size() == 1) //je tam jenom 1, a to pro neurcite
        {
            Skupina newSkupina(pathGroup, dateTime);
            newSkupina.addFoto(allFoto[i], i);
            newSkupina.setNezarazeno(false);
            newSkupina.addDate(dateTime.date());
            skupiny.append(newSkupina);
            vlozeno = true;
        }
        else // skupiny uz nejake jsou, projet a nadi nestli nejaka nema datum fotky
        {
            for (int j = 0; j < skupiny.size(); j++)
            {
               if (((skupiny[j].getPath() == pathGroup))) // je tam skupina s danou cestou
                {
                    //Zjistit datum a cas skupiny
                    //Pridat ke datumu a casu skupiny pozadovany pocet minut
                    //Porovnat cas modifikovany skupiny a fotky - prvni cas skupina a posledni (modifikovany)
                    // - vetsi nebo roven nez first ale mensi nebo roben nez last PATRI
                    // - mensi nez first, vetsi nez last NEPATRI
                    QDateTime firstDate = skupiny[j].getSkupDateTime();
                    QDateTime lastDate = skupiny[j].getLastSkupDateTime();

                    lastDate = lastDate.addSecs(minut*60); //pricitaji se sekundy, ale zadane jsou minuty - proto 60

                    if (dateTime >= firstDate && dateTime <= lastDate)
                    {
                        skupiny[j].addFoto(allFoto[i], i);
                        skupiny[j].setLastSkupDateTime(dateTime);
                        int size = skupiny[j].getDateSize();
                        bool ano = false;
                        for (int k = 0; k < size; k++)
                        {
                            QDate date = skupiny[j].getDate(k);
                            if (date == dateTime.date())
                            {
                                ano = true;
                                break;
                            }
                            else
                                ano = false;
                        }

                        if (!ano)
                            skupiny[j].addDate(dateTime.date());

                        vlozeno = true;
                        break;
                    }
                    else
                    {
                        vlozeno = false;
                    }
                }
            }
        }

        //skupina neexistuje, fotka stale nebzla vlozena
        if (!vlozeno)
        {
            Skupina newSkupina(pathGroup, dateTime);
            newSkupina.setNezarazeno(false);
            newSkupina.addFoto(allFoto[i], i);
            newSkupina.addDate(dateTime.date());
            skupiny.append(newSkupina);
        }

    }
}


/*
 * Funkce spocita vzdalenost mezi dvema GPS souradnicema
 * Vrati vzdalenst jako vysledek
 * http://mathforum.org/library/drmath/view/51879.html
 */
double AllFoto::haversineFormula(double lat1, double longi1, double lat2, double longi2)
{
    int R = 6371; //polomer Zeme, proc to nemuze byt koule... vse by bylo mnohem jednodussi
    //trigonometricke funkce potrebuji radiany
    double dlat = qDegreesToRadians(lat2 - lat1);
    double dlongi = qDegreesToRadians(longi2 - longi1);
    lat1 = qDegreesToRadians(lat1);
    lat2 = qDegreesToRadians(lat2);

    //(sin(dlat/2))^2 + cos(lat1) * cos(lat2) * (sin(dlon/2))^2
    double a = qPow(qSin(dlat / 2), 2) + qCos(lat1) * qCos(lat2) * qPow(qSin(dlongi / 2), 2);

    //2 * atan2(sqrt(a), sqrt(1-a))
    double c = 2 * qAtan2(qSqrt(a), qSqrt(1 - a));

    //d = R * c
    double d = R * c;

    return d;
}

/*
 * Rozdeleni fotografii do skupin
 * Rozdelovani podel GPS soiradnic
 * Pokud skupina s danym datumem existuje, vloz fotografii do ni,
 * pokud ne, vytvor danou skupinu a vloz foto do ni.
 * Praktickz to same co s datumem a casem. (TODO Popremyslet jak to optimaliyovat na jednu funkci)
 */
void AllFoto::createGPSFotoGroup(double km)
{
    //projet fotky a rodelit do skupiny podle vzdalenosti gps souradnic
    for (int i = 0; i < allFoto.size(); i++)
    {
        QDateTime dateTime = allFoto[i].getDateTime();
        double lat1 = allFoto[i].getLatitude();
        double longi1 = allFoto[i].getLongitude();
        QFileInfo info(allFoto[i].getPath());
        QString pathGroup = info.absolutePath();

        bool vlozeno = false; // zda dana fotografie je vlozena

        //defaultni skupina, skupina NEZARAZENO, bude to prvni skupina
        if (skupiny.size() == 0)
        {
            Skupina newSkupina = Skupina();
            newSkupina.setNezarazeno(true);
            skupiny.append(newSkupina);
        }

        //lze fotografii zaradit? ma exif medata? ma gps souradnice?
        if (!allFoto[i].isOK() || !allFoto[i].isGPS())
        {
            skupiny[0].addFoto(allFoto[i], i);
            continue;
        }

        //zsjitit jestli existuje skupina z danym datumem
        //ano - vlozit fotku dovnitr
        //ne - vytvorit novou skupinu a vlozit fotku dovnitr
        //seznam je prazdny, jako u ne
        //nezapomenou novou skupinu vlozit do seznamu skupin
        if (skupiny.size() == 1) //neni skupina
        {
            Skupina newSkupina(pathGroup, lat1, longi1, dateTime);
            newSkupina.setNezarazeno(false);
            newSkupina.addFoto(allFoto[i], i);
            newSkupina.addDate(dateTime.date());
            skupiny.append(newSkupina);
            vlozeno = true;
        }
        else // skupiny uz nejake jsou, projet a nadi nestli nejaka nema datum fotky
        {
             for (int j = 0; j < skupiny.size(); j++)
             {
                if (((skupiny[j].getPath() == pathGroup))) // je tam skupina s danou cestou
                {
                    //Zjistit gsp souradnice skupiny (posledni vlozena foto)
                    //Vypocitat vzdalenost od skupiny a fotografie
                    //-- vzdalenost mensi rovna zadane zvdalenosti... patri
                    //-- vzddalenost vetsi ... nepatri

                    double lat2 = skupiny[j].getLat();
                    double longi2 = skupiny[j].getLongi();

                    double d = haversineFormula(lat1, longi1, lat2, longi2);

                    if (d <= km)
                    {
                        skupiny[j].addFoto(allFoto[i], i);
                        skupiny[j].setLat(lat1);
                        skupiny[j].setLongi(longi1);

                        int size = skupiny[j].getDateSize();
                        bool ano = false;
                        for (int k = 0; k < size; k++)
                        {
                            QDate date = skupiny[j].getDate(k);
                            if (date == dateTime.date())
                            {
                                ano = true;
                                break;
                            }
                            else
                                ano = false;
                        }

                        if (!ano)
                            skupiny[j].addDate(dateTime.date());

                        vlozeno = true;
                        break;
                    }
                    else
                        vlozeno = false;
                    }
             }
        }

        //skupina neexistuje, fotka stale nebzla vlozena
        if (!vlozeno)
        {
            Skupina newSkupina(pathGroup, lat1, longi1, dateTime);
            newSkupina.setNezarazeno(false);
            newSkupina.addFoto(allFoto[i], i);
            newSkupina.addDate(dateTime.date());
            skupiny.append(newSkupina);
        }

    }

}

/*
 * Vyhleda duplicity ve skupinach. Vevnitr skupin.
 * Vysledkem bude vektor objektu, urcujicich duplicity.
 */
void AllFoto::duplVSkup()
{
    duplIN = true;

    //prohledat vsechny skupiny
    for (int sk = 0; sk < skupiny.size(); sk++)
    {
        //nezarazeno neprohlradavat
        if (skupiny[sk].getNezarazeno())
            continue;

        skupiny[sk].cleanDuplikIN();

        bool ano = false;
        QVector<int> bylo; //indikator pro fotografie, ktere uz maji duplikaty, abz se nestalo, ze se 1 skupinu duplikatu vytvori vice objektu

        //prohledat vsechny fotografie v dane skupine...
        for (int i = 0; i < skupiny[sk].getSize(); i++)
        {
            bool preskoc = false;
            //kontrola zda uz dana fotografie nebyla oznacena za duplikat
            for (int kontrola = 0; kontrola < bylo.size(); kontrola++)
            {
                if (i == bylo[kontrola]) // uz bylo, neprohledavat
                {
                    preskoc = true;
                    break;
                }
            }

            //bylo, dale nevyhledavat
            if (preskoc)
                continue;


            //Dostat potrebna data k fotografi 1
            Foto foto1 = skupiny[sk].getFoto(i);
            QString velikost1 = foto1.getVelikost();
            QDateTime dateTime1 = foto1.getDateTime();
            QString width1 = foto1.getWidth();
            QString height1 = foto1.getHeight();

            DuplicitniFoto duplikatyFota = DuplicitniFoto();
            bool vlozeno = false;

            //... a porovnat je se vsema dalsima fotkama (a vyhnout se porovnavani sama ze sebou)
            for (int j = i + 1; j < skupiny[sk].getSize(); j++)
            {
                Foto foto2 = skupiny[sk].getFoto(j);
                QString velikost2 = foto2.getVelikost();
                QDateTime dateTime2 = foto2.getDateTime();
                QString width2 = foto2.getWidth();
                QString height2 = foto2.getHeight();

                // porovnat fotografie, pokud vsechno souhlasi, tak fotografie jsou duplicitni, ulozit si je
                if (velikost1 == velikost2 && dateTime1 == dateTime2 &&
                        width1 == width2 && height1 == height2)
                {

                    //prvni foto, hlavni foto, foto od ktereho se ukladaji duplicity, tam ulozit jenom jednou
                    if (!vlozeno)
                    {
                        duplikatyFota.addMainFoto(i);
                        vlozeno = true;
                        ano = true;
                    }

                    //ulozit si, ze dana fotografie je v uz duplikatech
                    bylo.append(j);

                    //vlozit duplikaty fota
                    duplikatyFota.addFoto(j);
                }
            }

            if (ano)
            {
                skupiny[sk].addVnitrniDuplik(duplikatyFota);
                ano = false;
            }

        }
    }

}


/*
 * Vyhleda duplicitni skupiny.
 * Vysledek bude vektor duplicitnich skupin.
 */
void AllFoto::duplicitniSkupiny()
{
    duplSK = true;

    //vycisteni
    for (int i = 0; i < skupiny.size(); i++)
        skupiny[i].cleanDuplikSKUP();

    //prohledat vsechny skupiny...
    for (int sk1 = 0; sk1 < skupiny.size(); sk1++)
    {
        //neprohleadavat pro nezarazeno
        if(skupiny[sk1].getNezarazeno())
            continue;

        int sizeDateSK1 = skupiny[sk1].getDateSize();
        //.. a porovnat je s ostatnima skupinama
        for (int sk2 = sk1 + 1; sk2 < skupiny.size(); sk2++)
        {
            int sizeDateSK2 = skupiny[sk2].getDateSize();

            bool ano = false;

            //overit datumy, pokud alespon 1 datum souhlasi, tak se bude dana skupina prohledavat pro duplikaty
            for (int date1 = 0; date1 < sizeDateSK1; date1++)
            {
                QDate dateSK1 = skupiny[sk1].getDate(date1);
                for (int date2 = 0; date2 < sizeDateSK2; date2++)
                {
                    QDate dateSK2 = skupiny[sk2].getDate(date2);

                    //pokut souhasi, tak vyskoc, dale neprohledavej, a uloz si, ze to mas cele porovnat
                    if (dateSK1 == dateSK2)
                    {
                        ano = true;
                        break;
                    }
                }
                //dale uz nepotrebuju prohledavat, alespon jedno datum souhlasi
                if(ano)
                    break;
            }

            //pokut se nenasla alesno 1 shoda, tak dana skupina neni kandidatem na duplikat, prejdi na dalsi skupinu
            if(!ano)
                continue;

            //Proslo to, je velika pravdepodobnost, ze skupina bude duplicitni
            Duplicita duplikat = Duplicita(sk2);
            bool duplSkup = false;

            //prohledat jednotlive fotografie v obou skupinach, pokut se najde, tak si to ulozit
            for (int i = 0; i < skupiny[sk1].getSize(); i++)
            {
                Foto foto1 = skupiny[sk1].getFoto(i);
                QString velikost1 = foto1.getVelikost();
                QDateTime dateTime1 = foto1.getDateTime();
                QString width1 = foto1.getWidth();
                QString height1 = foto1.getHeight();

                DuplicitniFoto duplikatyFota = DuplicitniFoto();
                bool vlozeno = false;

                for (int j = 0; j < skupiny[sk2].getSize(); j++)
                {
                    Foto foto2 = skupiny[sk2].getFoto(j);
                    QString velikost2 = foto2.getVelikost();
                    QDateTime dateTime2 = foto2.getDateTime();
                    QString width2 = foto2.getWidth();
                    QString height2 = foto2.getHeight();

                    // porovnat fotografie, pokud vsechno souhlasi, tak fotografie jsou duplicitni, ulozit si je
                    if (velikost1 == velikost2 && dateTime1 == dateTime2 &&
                            width1 == width2 && height1 == height2)
                    {
                        //hlavni foto vlozeno nebylo
                        if (!vlozeno)
                        {
                            duplikatyFota.addMainFoto(i);
                            vlozeno = true;
                        }

                        duplikatyFota.addFoto(j);

                    }

                }

                //vsechny fotografie z druhe skupiny jsou prohledane, ulozit si vysledek
                if (vlozeno)
                {
                    duplSkup = true;
                    duplikat.add(duplikatyFota);

                }
            }

            //vsechny fotografie v druhe skupine prohledany, ulozit si je
            if (duplSkup)
                skupiny[sk1].addDuplikat(duplikat);
        }
    }

}

/*
 * Nastavi preview fotografiim.
 * POZOR - musi se testovat dvoji ukladani, spatny navrh
 */
void AllFoto::setFotosPicture()
{

    int pocetFotek = pocetFotoPic();
    int progressNumber = 0;
    QProgressDialog progress;
    progress.setLabelText("Nahrávání náhledů fotografií");
    progress.setAttribute(Qt::WA_DeleteOnClose, true);
    progress.setWindowModality(Qt::WindowModal);
    progress.setRange(0, pocetFotek);

    pixmap = true;
    bool cancel = false;


    //skupiny a az pak foto, nebo jenom foto
    if (skupiny.size() > 0) //skupiny jsou
    {
        for (int i = 0; i < skupiny.size(); i++)
        {
            for(int j = 0; j < skupiny[i].getSize(); j++)
            {
                progress.setValue(progressNumber++);
                if (progress.wasCanceled())
                {
                    cancel = true;
                    break;
                }

                QString file = skupiny[i].getFoto(j).getPath();
                QPixmap pic = findFotosPreview(file);

               //nejdriv do skupiny
                skupiny[i].setPreviewToFoto(j, pic);

                //pak do all
                int help = skupiny[i].getHelpFoto(j);
                allFoto[help].setPrewiev(pic);

            }

        }

    }
    else
    {
        for (int i = 0; i < allFoto.size(); i++)
        {
            progress.setValue(progressNumber++);
            if (progress.wasCanceled())
            {
                cancel = true;
                break; 
            }

            QString file = allFoto[i].getPath();
            QPixmap pic = findFotosPreview(file);

            allFoto[i].setPrewiev(pic);
        }

    }

    if (cancel)
        pixmap = false;


}

/*
 * Nalezne a vrati preview dane fotografie
 */
QPixmap AllFoto::findFotosPreview(QString file)
{
    QPixmap pic;

    try
    {
        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(file.toStdString());
        assert(image.get() != 0);
        image->readMetadata();

        Exiv2::PreviewManager loader(*image);
        Exiv2::PreviewPropertiesList list = loader.getPreviewProperties();

        if (list.size() != 0)
        {
            Exiv2::PreviewImage preview = loader.getPreviewImage(list[0]);

//            std::string name = "preview" + preview.extension();
//            QString names = QString::fromStdString(name);
//            preview.writeFile(name);

//            QImage im;
//            Debug() << im.load(names, preview.extension().c_str());
//            pic = QPixmap::fromImage(im);

            Exiv2::DataBuf data = preview.copy();

            //Zde vystupuje temnota, ktera pozira kotata - NESAHAT || PREST SMRTI
            //aka pole bytu a QByteArray + pretypovani, v data je to ulozene v poli bytu, coz je uint8_t
            QByteArray array = QByteArray::fromRawData(reinterpret_cast<const char*>(data.pData_), data.size_);

            pic.loadFromData(array, preview.extension().c_str());
        }
        else
           pic.load(file);
    }
    catch (Exiv2::Error& e)
    {
        qDebug() << "Problem pri cteni preview";
    }

    return pic;

}

int AllFoto::pocetFotoPic()
{
    int pocet = 0;
    if (skupiny.size() > 0)
    {
        for (int i = 0; i < skupiny.size(); i++)
            pocet += skupiny[i].getSize();
    }
    else
        pocet = allFoto.size();

    return pocet;

}
