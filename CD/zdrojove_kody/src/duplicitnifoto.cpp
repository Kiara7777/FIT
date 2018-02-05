#include "duplicitnifoto.h"

DuplicitniFoto::DuplicitniFoto(const DuplicitniFoto &d)
{
    foto = d.foto;

    for (int i = 0; i < d.index.size(); i++)
        index.append(d.index[i]);

}

void DuplicitniFoto::addMainFoto(int indexFoto)
{
    foto = indexFoto;

}

void DuplicitniFoto::addFoto(int indexFoto)
{
    index.append(indexFoto);

}

int DuplicitniFoto::getSize()
{
    return index.size();
}

int DuplicitniFoto::getFotoIndex(int indexFoto)
{
    return index[indexFoto];
}

int DuplicitniFoto::getMainFoto()
{
    return foto;
}
