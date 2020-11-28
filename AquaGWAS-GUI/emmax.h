#ifndef EMMAX_H
#define EMMAX_H

#include <QString>
#include <QMap>
#include <QMessageBox>
#include "assoctool.h"

class Emmax : public AssocTool
{
public:
    Emmax();

    bool runGWAS(QString genotype, QString phenotype, QString covariate, QString kinship,
                 QString out, QMap<QString, QString> moreParam);
    bool makeKinship(QString genotype, QMap<QString, QString> moreParam);
};

#endif // EMMAX_H
