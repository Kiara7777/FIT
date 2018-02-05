#include "fotofile.h"

#include <QApplication>


/*
 * Zapise do souboru pozadovana data
*/
void FotoFile::writeData(QString name, QString size, QFile &file)
{
    QTextStream out(&file);

    out << name << "\t";
    out << size << "\t";
    //------------------EXIF----------------------//
    if (mexif->fotoExifData(name, out) == -1) //-1 nejsou tam exif data
    {
        out << "Exif data nenalezena\t";
    }
    //------------------EXIF----------------------//
     out << "\n";
}

/*
 * Vytvori nebo upravi soubor s informacemi o fotografiich
*/
int FotoFile::createFotoFile()
{
    mexif = new Mexif;

    pocetFoto = 0;

    progress = new QProgressDialog();
    progress->setWindowTitle("Načítání");

    progress->setLabelText("Příprava...");
    progress->setRange(0, 1000);
    progress->setModal(true);
    progress->setAttribute(Qt::WA_DeleteOnClose, true);

    progress->setValue(1);

    int pocet = getFileCount();
    if (pocet == -1)
    {
        delete progress;
        delete mexif;
        return 0;
    }

    progress->setMaximum(pocet);
    int poradi = 1;


//    //Vytvori slozku na ulozeni souboru, pokut neexistuje
//    if (!QDir("xskuto00IBP").exists())
//        QDir().mkdir("xskuto00IBP");

    QFile file(fileFoto);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
        return -1;

    QHash<QString, int> hashFile;
    bool isFile = false;

    //jestli soubor bude prazdny, tak se prohledava poprve, vynecha se cela dodatecna prace se souborem
    if(file.pos() == 0)
        isFile = false;
    else //soubor neni prazdny, musi se s nim pracovat, pripadne updatovat
    {
        file.seek(0); // ukayatel na pozici souboru, vratit na zacatek
        isFile = true;
        QTextStream in(&file);

        while(!in.atEnd()) // cely soubor ulozit do pameti
        {
            QString line = in.readLine();
            QStringList list = line.split('\t');
            QString size = list[1];
            hashFile.insert(list[0], size.toInt());
        }
    }

    QDirIterator it(directory, QDirIterator::Subdirectories);
    QString koncovka = "";
    progress->setLabelText("Skenování fotografií...");
    //dokut existuje nejaky jiny zaznam tak pokracuj
    while (it.hasNext())
    {
        // prejdi na dalsi zaznam, pro prvni iteraci to nepotrebujeme
        it.next();

        //je to soubor?
        if(it.fileInfo().isFile())
        {
            //ma to koncovku jakou to chceme?
            koncovka = it.fileInfo().suffix();
            if (koncovka == "JPG" || koncovka == "jpg")
            {
                pocetFoto++;
                progress->setValue(poradi++);
                qApp->processEvents();
                if(progress->wasCanceled())
                {
                    file.remove();
                    delete progress;
                    delete mexif;
                    return -1;
                }

                QString size = QString::number(it.fileInfo().size());
                QString name = it.filePath();
                //existuje uz soubor, kde se ukladalo?
                if(isFile) //existuje, neexistuje, je v souboru - neni na disku
                {
                    //existuje, dale se nezabirat OK
                    //neexistuje, vytahni data OK
                    //je v souboru, ale nenalezen pri prohledavani - smazat zaznam v souboru (to co zbude?) OK

                    if(hashFile.contains(name) && (size.toInt() == hashFile.value(name))) // zaznam existuje, pokracuj dale
                    {
                        hashFile.remove(name);
                        continue;
                    }
                    else // zaznam neexistuje, nebo velikost je jina (upraveny zaznam?), najit a ulozit data
                    {
                        writeData(name, size, file);

                    }

                }
                else //normalni cinost, nemusi se kontrolovat pritomnost jiz existujich zaznamu, jede to poprve
                {
                    writeData(name, size, file);
                }
            }
        }
    }


    //soubor uz existoval, v tomto bode, by v hashFile mely zustat pouze zaznamy, ktere se nenasly
    //smazat je ze souboru
    if(isFile && !hashFile.isEmpty())
    {
        file.seek(0);
        QTextStream in(&file);
        QStringList list;
        QString line;

        while (!in.atEnd())
        {
            line = in.readLine();
            QStringList items = line.split('\t');
            if (hashFile.contains(items[0])) // je tam zaznam - nebyl nalezen pri prohledavani - nejspis uz neexistuje
                ;
            else
                list << line;
        }

        file.resize(0);
        for (int i = 0; i < list.size(); i++)
        {
            in << list[i];
            in << "\n";
        }

    }

    file.close();

    delete progress;
    delete mexif;

    return 0;

}

void FotoFile::setDir(QString directory)
{
    this->directory = directory;

}

int FotoFile::getFileCount()
{
    int pocet = 0;
    QDirIterator it(directory, QDirIterator::Subdirectories);

    while(it.hasNext())
    {
        qApp->processEvents();
        if(progress->wasCanceled())
                return -1;
        it.next();
        if(it.fileInfo().isFile())
        {
            QString koncovka = it.fileInfo().suffix();
            if (koncovka == "JPG" || koncovka == "jpg")
                pocet++;
        }
    }

    return pocet;


}

int FotoFile::getFotoSize()
{
    return pocetFoto;

}
