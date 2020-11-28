#ifndef PLINK_H
#define PLINK_H

#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QStringList>
#include <QProcess>
#include <QMessageBox>
#include <QFile>
#include "assoctool.h"

class Plink : public AssocTool
{
public:
    Plink();

/*  !!! NOTICE:
 *
 * Any paht of binary genotype file didn't contain the suffix.
 *
*/
    // File transform
    bool vcf2transpose(QString vcfFile, QString out, QString maf, QString mind, QString geno);
    bool vcf2binary(QString vcfFile, QString out, QString maf, QString mind, QString geno);
    bool vcf2plink(QString vcfFile, QString out, QString maf, QString mind, QString geno);
    bool plink2transpose(QString pedFile, QString mapFile, QString out,
                         QString maf, QString mind, QString geno);
    bool plink2binary(QString pedFile, QString mapFile, QString out,
                      QString maf, QString mind, QString geno);
    bool transpose2binary(QString tpedFile, QString tfamFile, QString out,
                          QString maf, QString mind, QString geno);
    bool transpose2plink(QString tpedFile, QString tfamFile, QString out,
                          QString maf, QString mind, QString geno);
    bool binary2transpose(QString binaryFile, QString out,
                          QString maf, QString mind, QString geno);
    bool binary2plink(QString binaryFile, QString out,
                      QString maf, QString mind, QString geno);
\
    // Filter data(Quality control)
    void filterVcfFile(QString genotype, QString maf, QString mind, QString geno, QString out);
    void filterBinaryFile(QString genotype, QString maf, QString mind, QString geno, QString out);
    void filterPlinkFile(QString genotype, QString map, QString maf, QString mind, QString geno, QString out);
    void filterTransposeFile(QString genotype, QString map, QString maf, QString mind, QString geno, QString out);
    void filterData(QString genotype, QString map, QString maf, QString mind, QString geno, QString out);
    void linkageFilter(QString genotype, QString map,
                       QString winSize, QString stepLen, QString r2Threshold, QString out);
    void extractBySnpNameFile(QString genotype, QString map, QString snpIDFile, QString out);

    // Split genotype(and map) file, all of them generate map/ped file.
    void splitVcfFile(QString vcfFile, QString keepFile, QString out);
    void splitPlinkFile(QString ped, QString map, QString keepFile, QString out);
    void splitBinaryFile(QString binaryFile, QString keepFile, QString out);
    void splitTransposeFile(QString tped, QString tfam, QString keepFile, QString out);

    // Run
    bool runGWAS(QString phenotype, QString genotype, QString map, QString covariate, QString kinship,
                 QString model, QString maf, QString mind, QString geno,QString out);

private:

    bool isVcfFile(QString file);
};

#endif // PLINK_H
