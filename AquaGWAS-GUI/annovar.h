#ifndef ANNOVAR_H
#define ANNOVAR_H

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include "tool.h"
#include "filereader.h"

class Annovar : public Tool
{
public:
    Annovar();
    bool gffTogtf(QString gffFile, QString outFile);
    bool gtfToGenePred(QString gtfFile, QString outFile);
    bool retrieveSeqFromFasta(QString refGeneFile, QString seqFile, QString outFile);
    bool tableAnnovar(QString avinput, QString refGeneDir, QString refGenePrefix, QString out);
    bool annotateVariation(QString avinput, QString refGeneDir, QString refGenePrefix, QString out);
    bool filterSNP(QString const pvalFilePath, QString const thBase,
                        QString const thExpo, QStringList  &snpID);
    bool vcf2avinput(QString vcf, QStringList snpIDList, QString avinput);
};

#endif // ANNOVAR_H
