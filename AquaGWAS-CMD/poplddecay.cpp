#include "poplddecay.h"

PopLDdecay::PopLDdecay()
{

}

bool PopLDdecay::makeGenotype(QString ped, QString map, QString out)
{
    if (ped.isNull() || map.isNull() || out.isNull())
    {
        return false;
    }
    /*
     * perl ~/software/PopLDdecay-master/bin/mis/plink2genotype.pl -inPED 222_filter1.ped
     *  -inMAP 222_filter1.map -outGenotype 222_filter1.genotype
     */
    this->paramlist.clear();
    this->paramlist.append("-inPED");
    this->paramlist.append(ped);
    this->paramlist.append("-inMAP");
    this->paramlist.append(map);
    this->paramlist.append("-outGenotype");
    this->paramlist.append(out);

    return true;
}

bool PopLDdecay::runLD(QString _genotype, QString out)
{
    if (_genotype.isNull() || out.isNull())
    {
        return false;
    }
     // PopLDdecay -InGenotype 222_filter1.genotype -OutStat 222_filter1_LD
    this->paramlist.clear();

    this->paramlist.append("-InGenotype");
    this->paramlist.append(_genotype);
    this->paramlist.append("-OutStat");
    this->paramlist.append(out);

    return true;
}



bool PopLDdecay::plotLD(QString in, QString out)
{
    if (in.isNull() || out.isNull())
    {
        return false;
    }

    /*
     * perl ~/software/PopLDdecay-master/bin/Plot_OnePop.pl
     * -inFile 222_filter1_LD.stat.gz -output 222_filter1_LD_fig
     */
    this->paramlist.clear();
    this->paramlist.append("-inFile");
    this->paramlist.append(in);
    this->paramlist.append("-output");
    this->paramlist.append(out);

    return true;
}

QString PopLDdecay::makeSingleKeepFile(QString src, QString fid)
{
    if (src.isNull() || fid.isNull())
    {
        return QString::Null();
    }

    QFile file(src);
    QFileInfo fileInfo(file);
    QString fileBaseName = fileInfo.baseName();
    QString fileAbPath = fileInfo.absolutePath();

    QTextStream tfileStream(&file);

    if (!file.open(QIODevice::ReadOnly))
    {
        return QString::Null();
    }

    QFile keepFile(fileAbPath+"/"+fileBaseName+"_"+fid+".keep");
    QTextStream keepFileStream(&keepFile);
    keepFile.open(QIODevice::Append);
    while (!tfileStream.atEnd())
    {
        QStringList curLine = tfileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
        if (fid == curLine[0])
        {
            keepFileStream << curLine[0] << "\t" << curLine[1] << "\n";
        }
        qApp->processEvents(); // Avoid no responding of MainWindow.
    }

    keepFile.close();

    return keepFile.fileName();
}

/**
 * @brief PopLDdecay::makeKeepFile
 *      make keep file for every family according to the first 2 columns of src.
 * @param src: .ped, .tfam or .fam.
 * @return  keepList.
 */
QStringList PopLDdecay::makeKeepFile(QString src)
{
    QStringList keepList;
    if (src.isNull())
    {
        return keepList;
    }

    QStringList fidList;
    QFile file(src);
    QFileInfo fileInfo(file);
    QString fileBaseName = fileInfo.baseName();
    QString fileAbPath = fileInfo.absolutePath();
    QTextStream fileStream(&file);

    QFile keepFile;
    QTextStream keepFileStream(&keepFile);
    if (!file.open(QIODevice::ReadOnly))
    {
        return keepList;
    }

    while (!fileStream.atEnd())
    {
        QStringList curLine = fileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
        keepFile.setFileName(fileAbPath+"/"+fileBaseName+"_"+curLine[0]+".keep");
        QTextStream keepFileStream(&keepFile);
        if (fidList.indexOf(curLine[0]) == -1)
        {   // The first appearence of the FID.
            keepFile.remove();
            fidList.append(curLine[0]);
            keepList.append(fileAbPath+"/"+fileBaseName+"_"+curLine[0]+".keep");
        }
        keepFile.open(QIODevice::Append);
        keepFileStream << curLine[0] << "\t" << curLine[1] << "\n";
        qApp->processEvents();  // Avoid no responding of MainWindow.
    }

    for (auto item:keepList)
    {
        QFile file(item);
        file.close();
    }
    return keepList;
}
