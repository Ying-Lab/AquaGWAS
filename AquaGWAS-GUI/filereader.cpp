#include "filereader.h"

FileReader::FileReader()
{

}

void FileReader::setPhenotypeFile(QString const dir)
{
    this->phenotypeFile = dir;
}

QString FileReader::getPhenotypeFile(void)
{
    return this->phenotypeFile;
}

void FileReader::setGenotypeFile(QString const dir)
{
    this->genotypeFile = dir;
}

QString FileReader::getGenotypeFile(void)
{
    return this->genotypeFile;
}

void FileReader::setMapFile(QString const dir)
{
    this->mapFile = dir;
}

QString FileReader::getMapFile(void)
{
    return this->mapFile;
}

void FileReader::setCovariateFile(QString const dir)
{
    this->covariateFile = dir;
}

QString FileReader::getCovariateFile(void)
{
    return this->covariateFile;
}

void FileReader::setKinshipFile(QString const dir)
{
    this->kinshipFile = dir;
}

QString FileReader::getKinshipFile(void)
{
    return this->kinshipFile;
}

/**
 * @brief FileReader::getFIDList
 *          get family ID in src file.
 * @param src
 * @param col: the No.col column is the fid in src file.
 * @return
 */
QStringList FileReader::getFIDList(QString const src, int col)
{
    QStringList fidList;
    if (src.isNull() || col < 1)
    {   // The path is not correct or the objective column is wrong.
        return fidList; // return empty list.
    }
    QFile file(src);
    QTextStream fileStream(&file);
    if (!file.open(QIODevice::ReadOnly))
    {   // Open file error.
        return fidList;
    }
    while (!fileStream.atEnd())
    {   // Read a line and split it by space(s).
        QStringList curLine = fileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
        if (col > curLine.length())
        {   // The objective column bigger than length of current line.
            return fidList; // return empty list.
        }
        if (fidList.indexOf(curLine[col-1]) == -1)
        {   // Append to FID list if not in FID list.
            fidList.append(curLine[col-1]);
        }
        qApp->processEvents();  // Prevent "no responding" of MainWindow.
    }
    return fidList;
}

/**
 * @brief transformCovariateFile
 * @param srcCovar                      FID IID 1 covar_1 covar_2 ... covar_n
 * @param desCovar                      1 covar_1 covar_2 ... covar_n
 */
bool FileReader::transformCovariateFile(QString srcCovar, QString desCovar)
{
    if (srcCovar.isNull() || desCovar.isNull())
    {
        return false;
    }

    QFile srcCovarFile(srcCovar);
    QFile desCovarFile(desCovar);

    QTextStream srcCovarFileStream(&srcCovarFile);
    QTextStream desCovarFileStream(&desCovarFile);

    if (!srcCovarFile.open(QIODevice::ReadOnly) ||
        !desCovarFile.open(QIODevice::ReadWrite))
    {   // Open file error.
        return false;
    }
    while (!srcCovarFileStream.atEnd())
    {   // Read file line by line respectively.
        QString srcCovarCurLine = srcCovarFileStream.readLine();
        // Replace "NA" to "-9"
        QStringList srcCovarCurLineList =
            srcCovarCurLine/*.replace("NA", "-9")*/.split(QRegExp("\\s+"),
                                    QString::SkipEmptyParts);
        srcCovarCurLineList.removeFirst();
        srcCovarCurLineList.removeFirst();

        desCovarFileStream << srcCovarCurLineList.join("\t") << endl;
    }
    // Close file finally.
    srcCovarFile.close();
    desCovarFile.close();

    return true;
}

/**
 * @brief FileReader::makeAvinputAndSnpposFile
 * @param vcfFilePath           Extrac position of SNP and save to outFile.(reserve p-value) and make avinput file.
 * @param pvalFilePath          pvalFilePath  p-value file (Two column: SNP_ID P-VAL)
 * @param avinputFilePath
 * @param snpPosFilePath        snpPosFilePath   output file(SNP_ID, p-value, CHR, BP)
 * @return
 */
bool FileReader::makeAvinputAndSnpposFile(QString vcfFilePath, QString pvalFilePath,
                              QString avinputFilePath, QString snpPosFilePath)
{
    if (vcfFilePath.isNull() || pvalFilePath.isNull() || avinputFilePath.isNull() || snpPosFilePath.isNull())
    {
        return false;
    }

    // Get the SNP which need to extract and save to snpID(QSet).
    QFile pvalFile(pvalFilePath);
    if (!pvalFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QTextStream pvalFileStream(&pvalFile);
    QMap<QString, QString> snpIDMap;    // SNPs need to extract.
    while (!pvalFileStream.atEnd())
    {
        QStringList curLine = pvalFileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
        snpIDMap.insert(curLine[0], curLine[0]+"\t"+curLine[1]);   // cut the SNP_ID and p-value
    }

    QFile vcfFile(vcfFilePath);
    QFile avinputFile(avinputFilePath);
    QFile snpPosFile(snpPosFilePath);
    if (!vcfFile.open(QIODevice::ReadOnly) ||
        !avinputFile.open(QIODevice::WriteOnly) ||
        !snpPosFile.open(QIODevice::WriteOnly))
    {
        return false;
    }

    // traverse map file, extract SNP.
    QTextStream vcfFileStream(&vcfFile);
    QTextStream avinputFileStream(&avinputFile);
    QTextStream snpPosFileStream(&snpPosFile);
    while (!vcfFile.atEnd())
    {
        QString curLine = vcfFileStream.readLine();
        if (curLine[0] == '#')
        {
            continue;
        }
        QStringList curLineList = curLine.split(QRegExp("\\s+"), QString::SkipEmptyParts);

        QString snpID = curLineList[2];
        if (snpID == '.')
        {
            snpID = (isNumber(curLineList[0]) ? "chr"+curLineList[0] : curLineList[0]) +":"+curLineList[1];
        }
        if (snpIDMap.find(snpID) != snpIDMap.end())
        {
            snpPosFileStream << snpIDMap[snpID] << "\t"
                             << (isNumber(curLineList[0]) ? "chr"+curLineList[0] : curLineList[0])
                             << "\t" << curLineList[1] << endl;
            avinputFileStream << (isNumber(curLineList[0]) ? "chr"+curLineList[0] : curLineList[0]) << "\t"
                              << curLineList[1] << "\t" << curLineList[1] << "\t"
                              << curLineList[3] << "\t" << curLineList[4] << endl;
        }
    }

    return true;
}

bool FileReader::isNumber(QString str)
{
    for (auto i:str)
    {
        if (i<'0' || i > '9')
        {
            return false;
        }
    }
    return true;
}
