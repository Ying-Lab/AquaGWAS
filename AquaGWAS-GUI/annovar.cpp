#include "annovar.h"

Annovar::Annovar()
{

}

bool Annovar::gffTogtf(QString gffFile, QString outFile)
{
    if (gffFile.isNull() || outFile.isNull())
    {
        return false;
    }

    //  gffread Hdhv3.changeID.gff3 -T -o Hdhv3.changeID.gtf
    this->paramlist.clear();
    this->paramlist.append(gffFile);
    this->paramlist.append("-T");
    this->paramlist.append("-o");
    this->paramlist.append(outFile);

    return true;
}

bool Annovar::gtfToGenePred(QString gtfFile, QString outFile)
{
    if (gtfFile.isNull() || outFile.isNull())
    {
        return false;
    }

    // gtfToGenePred -genePredExt Hdhv3.changeID.gtf Hdhv3_refGene.txt
    this->paramlist.clear();
    this->paramlist.append("-genePredExt");
    this->paramlist.append(gtfFile);
    this->paramlist.append(outFile);

    return true;
}

/**
 * @brief Annovar::retrieveSeqFromFasta
 * @param refGeneFile
 * @param seqFile       .fasta file.
 * @param outFile
 * @return
 */
bool Annovar::retrieveSeqFromFasta(QString refGeneFile, QString seqFile, QString outFile)
{
    if (refGeneFile.isNull() || seqFile.isNull() || outFile.isNull())
    {
        return false;
    }

    /*
       perl retrieve_seq_from_fasta.pl --format refGene --seqfile hdhdb/Hdhv3.changeID.fa
       hdhdb/Hdhv3_refGene.txt --outfile hdhdb/Hdhv3_refGeneMrna.fa
    */
    this->paramlist.clear();
    this->paramlist.append("--format");
    this->paramlist.append("refGene");
    this->paramlist.append("--seqfile");
    this->paramlist.append(seqFile);
    this->paramlist.append(refGeneFile);
    this->paramlist.append("--outfile");
    this->paramlist.append(outFile);

    return true;
}

bool Annovar::tableAnnovar(QString avinput, QString refGeneDir, QString refGenePrefix, QString out)
{
    if (avinput.isNull() || refGeneDir.isNull() || refGenePrefix.isNull() || out.isNull())
    {
        return false;
    }

    QFile file;
    file.setFileName(refGeneDir + "/" + refGenePrefix + "_refGeneMrna.fa");
    if (!file.exists())
    {
        return false;
    }
    file.setFileName(refGeneDir + "/" + refGenePrefix + "_refGene.txt");

    if (!file.exists())
    {
        return false;
    }

    // perl ../table_annovar.pl  test.avinput sp/ --buildver SP --outfile myanno --protocol refGene --operation g
    this->paramlist.clear();
    this->paramlist.append(avinput);
    this->paramlist.append(refGeneDir);
    this->paramlist.append("--buildver");
    this->paramlist.append(refGenePrefix);
    this->paramlist.append("--outfile");
    this->paramlist.append(out);
    this->paramlist.append("--protocol");
    this->paramlist.append("refGene");
    this->paramlist.append("--operation");
    this->paramlist.append("g");

    return true;
}

bool Annovar::annotateVariation(QString avinput, QString refGeneDir, QString refGenePrefix, QString out)
{
    if (avinput.isNull() || refGeneDir.isNull() || refGenePrefix.isNull() || out.isNull())
    {
        return false;
    }

    QFile file;
    file.setFileName(refGeneDir + "/" + refGenePrefix + "_refGeneMrna.fa");
    if (!file.exists())
    {
        return false;
    }
    file.setFileName(refGeneDir + "/" + refGenePrefix + "_refGene.txt");

    if (!file.exists())
    {
        return false;
    }

    // ../annotate_variation -out hdhdb/anno -build Hdhv3 input_for_annovar hdhdb
    this->paramlist.clear();
    this->paramlist.append("--out");
    this->paramlist.append(out);
    this->paramlist.append("--build");
    this->paramlist.append(refGenePrefix);
    this->paramlist.append(avinput);
    this->paramlist.append(refGeneDir);

    return true;
}

/**
 * @brief FunctionalAnnotator::filterSNP
 *      select SNP whose p-value bigger than threshold to out file.
 *      the threhold is -log(thBase * 10 ^ thExpo)
 * @param pvalFile      a file the last column is p value.(a header is necessary)
 * @param thBase        base of threshold
 * @param thExpo        exponent of threshold
 * @param snpID         save the SNP ID after filter
 * @return              fail(false) or success(true).
 */
bool Annovar::filterSNP(QString const pvalFilePath, QString const thBase,
                              QString const thExpo, QStringList &snpID)
{
    if (pvalFilePath.isNull() || thBase.isNull() || thExpo.isNull())
    {
        return false;
    }

    int thresholdBase = thBase.toInt();
    int thresholdExpo = thExpo.toInt();
    float threshold = -log10f(thresholdBase*pow10f(thresholdExpo));

    QFile pvalFile(pvalFilePath);

    if (!pvalFile.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QFileInfo pvalueFileInfo(pvalFilePath);
    QString pvalueFileAbPath = pvalueFileInfo.absolutePath();
    QString pvalueFileBaseName = pvalueFileInfo.baseName();
    QString pvalueFileSuffix = pvalueFileInfo.suffix();
    QString pvalueFileComSuffix = pvalueFileInfo.completeSuffix();

    int snpIDIndex;
    if ( pvalueFileComSuffix == "assoc.linear" || pvalueFileComSuffix == "assoc.logistic")
    {   // plink
        /* .assoc.linear
         * .assoc.logistic:
         *      CHR SNP BP A1 TEST NMISS OR STAT P
         */
        snpIDIndex = 1;
    }

    if (pvalueFileSuffix == "ps")
    {   // Emmax output file.
        /*.ps:
         *      [SNP ID(CHR:BP)], [beta], [p-value] (header, don't exist in file)
         *      chr0:39616  0.7571908167    0.2146455451
         */
        snpIDIndex = 0;
    }

    QTextStream pvalFileStream(&pvalFile);

    while (!pvalFileStream.atEnd())
    {
        QString curLine = pvalFileStream.readLine();
        QStringList curLineList = curLine.replace("NA", "-9").split(QRegExp("\\s+"), QString::SkipEmptyParts);
        if (-log10f(curLineList.constLast().toFloat()) >= threshold)
        {
            snpID.append(curLineList[snpIDIndex]);
        }
    }
    pvalFile.close();

    return true;
}

bool Annovar::vcf2avinput(QString vcf, QStringList snpIDList, QString avinput)
{
    if (vcf.isNull() || avinput.isNull() || snpIDList.isEmpty())
    {
        return false;
    }

    QFile vcfFile(vcf);
    QFile avinputFile(avinput);
    if (!vcfFile.open(QIODevice::ReadOnly) ||
        !avinputFile.open(QIODevice::WriteOnly))
    {
        return false;
    }

    FileReader fileReader;
    // traverse map file, extract SNP.
    QTextStream vcfFileStream(&vcfFile);
    QTextStream avinputFileStream(&avinputFile);
    vcfFileStream.readLine();   // Discard the header of vcf file.
    while (!vcfFileStream.atEnd())
    {
        QString curLine = vcfFileStream.readLine();
        if (curLine[0] == '#')
        {
            continue;
        }
        QStringList curLineList = curLine.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        if (snpIDList.indexOf(curLineList[2]) != -1)
        {
            avinputFileStream << (fileReader.isNumber(curLineList[0]) ? "chr"+curLineList[0] : curLineList[0])
                    << "\t" << curLineList[1] << "\t" << curLineList[1]
                    << "\t" << curLineList[3] << "\t" << curLineList[4] << endl;
        }
    }

    return true;
}
