#include "duplicita.h"


Duplicita::Duplicita(const Duplicita &d)
{
    skupina = d.skupina;

    for (int i = 0; i < d.foto.size(); i++)
        foto.append(d.foto[i]);

}

void Duplicita::add(DuplicitniFoto foto)
{
    this->foto.append(foto);

}

int Duplicita::getSize()
{
    return foto.size();
}

DuplicitniFoto Duplicita::getItem(int index)
{
    return foto[index];

}

int Duplicita::getSkupina()
{
    return skupina;
}

