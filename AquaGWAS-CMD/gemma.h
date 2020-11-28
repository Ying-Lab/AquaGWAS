#ifndef GEMMA_H
#define GEMMA_H

#include <QStringList>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDebug>
#include <assoctool.h>

class Gemma : public AssocTool
{
public:
    Gemma();
    bool runGWAS(QString binGenoBaseName, QString phenotype, QString covariate, QString kinship, QString out,
                 QString model, QString lmmtest, QString bslmmmodel);
    bool makeKinship(QString binGenoBaseName, QString out, QString kinship_type);
    bool phe_fam_Preparation(QString phe, QString fam);
};

#endif // GEMMA_H
