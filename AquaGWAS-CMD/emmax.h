#ifndef EMMAX_H
#define EMMAX_H

#include <QString>
#include <QMap>
#include "assoctool.h"

class Emmax : public AssocTool
{
public:
    Emmax();
    bool runGWAS(QString genotype, QString phenotype, QString covariate, QString kinship,
                 QString out, QString kinmat);
    bool makeKinship(QString genotype, QString kinmat);
};

#endif // EMMAX_H
