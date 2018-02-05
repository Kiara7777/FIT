#include "mexif.h"

Mexif::Mexif()
{

}

/*
* V Exiv datech nalezne zadany klic a vrati jeho
* hodnotu jako QString.
* Pokud zadany klic neexistuje, nebo chybi, tak se
* vrati retezec NENI
*/
QString Mexif::fotoExifKey(QString exifKey, Exiv2::ExifData &exifData)
{
    //vytvoreni klice ktery chceme hledat
    Exiv2::ExifKey key(exifKey.toStdString());

    //iterator, ktery ho bude hledat, vrati prvni moznou pozici
    Exiv2::ExifData::iterator i = exifData.findKey(key);

    std::string a = "";
    QString qstr = "";

    if (i == exifData.end())
    qstr = "NENI";
    else
    {
        a = i->value().toString();
        qstr = QString::fromStdString(a);
    }

    qstr = qstr + "\t";
    return qstr;
}

/*
 * Vytahne z fotografie pozadovane exif data a ulozi je do souboru.
 * Datum vytvoreni, sirka, vyska, model fotoaparatu
 */
int Mexif::fotoExifData(QString filePath, QTextStream &out)
{
    try
    {
        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filePath.toStdString());
        assert(image.get() != 0);
        image->readMetadata();

        Exiv2::ExifData &exifData = image->exifData();
        //test zda obsahuje Exifmetadata, dale fotku neprohledavat, pokut tam nejsou
        if(exifData.empty())
            return -1;

        out << fotoExifKey("Exif.Photo.DateTimeOriginal", exifData);
        out << fotoExifKey("Exif.Photo.PixelXDimension", exifData);
        out << fotoExifKey("Exif.Photo.PixelYDimension", exifData);
        out << fotoExifKey("Exif.GPSInfo.GPSLatitude", exifData);
        out << fotoExifKey("Exif.GPSInfo.GPSLatitudeRef", exifData);
        out << fotoExifKey("Exif.GPSInfo.GPSLongitude", exifData);
        out << fotoExifKey("Exif.GPSInfo.GPSLongitudeRef", exifData);

    }
    catch (Exiv2::Error& e)
    {
        qDebug() << "Neco se stalo '" << e.what() << "'\n";
    }

    return 0;
}
