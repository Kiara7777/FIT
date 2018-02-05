#ifndef FOTOFILE_H
#define FOTOFILE_H

#include "mexif.h"

#include <QFile>
#include <QDir>
#include <QString>
#include <QHash>
#include <QStringList>
#include <QTextStream>
#include <QDirIterator>
#include <QProgressDialog>

class FotoFile
{
public:
    FotoFile(QString filename): fileFoto(filename){}

    int createFotoFile();
    void setDir(QString directory);
    int getFileCount();
    int getFotoSize();

private:
    void writeData(QString name, QString size, QFile &file);

    QString fileFoto;
    QString directory;

    QProgressDialog *progress;

    Mexif *mexif;

    int pocetFoto;
};

#endif // FOTOFILE_H
