#ifndef DUPLICITA_H
#define DUPLICITA_H

#include "foto.h"
#include "duplicitnifoto.h"

class Duplicita
{
    int skupina; //index skupiny, ktera je duplicitni
    QVector<DuplicitniFoto> foto; //objekty, ktere urcuji duplicitni fotogeafie (jeji indexy)

public:
    Duplicita(){}
    Duplicita(int skup) : skupina(skup){}

    //KOPIROVACI KONSTRUKTOR
    Duplicita(const Duplicita &d);

    void add(DuplicitniFoto foto);
    int getSize();
    DuplicitniFoto getItem(int index);
    int getSkupina();

};

#endif // DUPLICITA_H
