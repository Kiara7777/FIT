#ifndef MEXIF_H
#define MEXIF_H

#include <exiv2/exiv2.hpp>
#include <cassert>
#include <QDebug>
#include <QString>
#include <QTextStream>


class Mexif
{
public:
    Mexif();
    QString fotoExifKey(QString exifKey, Exiv2::ExifData &exifData);
    int fotoExifData(QString filePath, QTextStream &out);
};

#endif // MEXIF_H
