#ifndef FUNCTIONALANNOTATOR_H
#define FUNCTIONALANNOTATOR_H

#include <QDebug>
#include <QString>
#include <QtMath>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QProcess>
//#include <QApplication>
#include <QMap>
#include <QPair>
#include <QSet>
#include <iostream>
#include "filereader.h"

// Functional annotator
class FuncAnnotator
{
public:
    FuncAnnotator();
    bool filterSNP(QString const pvalFilePath, QString const thBase,
                   QString const thExpo, QString const outFilePath);
    bool extractPos(QString const pvalFilePath, QString const mapFilePath, QString const outFilePath);
    bool complExoSnpInfo(QString const snpPosFilePath, QString const exVarFuncFilePath, QString const outFilePath);
    bool complNonExoSnpInfo(QString const exonicPosFilePath, QString const snpPosFilePath,
                            QString const varFuncFilePath, QString const outFilePath);
    bool complFuncAnnoInfo(QString const exonicPosFilePath, QString nonExonicPosFilePath,
                           QString baseFilePath, QString outFilePath);
    QSet<QString> getTransIDFromGtf(QString gtfFilePath);
    QSet<QString> getTransIDFromGff(QString gffFilePath);
    QMap<QString, QString> getProteinIDandTransIDFromGtf(QString gtfFilePath);
    QMap<QString, QString> getProteinIDandTransIDFromGff(QString gffFilePath);
    bool makeBaseFromEnsembl(QString gtfOrGffFilePath, QString ensemblBase, QString outFilePath);
    bool makeBaseFromNcbi(QString gtfOrGffFilePath, QString ncbiBase, QString outFilePath);
};

#endif // FUNCTIONALANNOTATOR_H
