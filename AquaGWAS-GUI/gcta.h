#ifndef GCTA_H
#define GCTA_H

#include "tool.h"
#include <QStringList>

class Gcta : public Tool
{
public:
    Gcta();
    bool makeGRM(QString genoBaseName, QString out);
    bool runPCA(QString grmBaseName, int PCs, int threads, QString out);
};

#endif // GCTA_H
