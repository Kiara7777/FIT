#ifndef DUPLICITNIFOTO_H
#define DUPLICITNIFOTO_H

#include <QVector>

class DuplicitniFoto
{
    int foto; //index fotky, ke ktere jsou ve vectoru ulozene duplicity
    QVector<int> index; // indexy na ukladani co je duplicitni

public:
    DuplicitniFoto(){}
    DuplicitniFoto(int f) : foto(f){}

    DuplicitniFoto(const DuplicitniFoto &d);

    void addMainFoto(int indexFoto);
    void addFoto(int indexFoto);
    int getSize();
    int getFotoIndex(int indexFoto);
    int getMainFoto();
};

#endif // DUPLICITNIFOTO_H
