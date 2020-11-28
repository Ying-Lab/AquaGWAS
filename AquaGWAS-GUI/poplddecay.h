#ifndef POPLDDECAY_H
#define POPLDDECAY_H

#include "tool.h"
#include <QStringList>
#include <QTextStream>
#include <QFileInfo>
#include <QFile>
#include <QApplication>

class PopLDdecay : public Tool
{
public:
    PopLDdecay();

    bool runLD(QString _genotype, QString out);
    bool makeGenotype(QString ped, QString map, QString out);
    bool plotLD(QString in, QString out);

    QString makeSingleKeepFile(QString src, QString fid);
    QStringList makeKeepFile(QString src);
};

#endif // POPLDDECAY_H
