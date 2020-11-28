#include "gemma.h"

Gemma::Gemma()
{
    this->model << "LMM" << "BSLMM";
    this->paramlist.clear();
}

/**
 * @brief Gemma::makeKinship
 * @param binGenoBaseName:binary file, without suffix, need .bed/.fam/.bim in the same path.
 * @param out: a file base name.
 * @return
 */
bool Gemma:: makeKinship(QString binGenoBaseName, QString out, QString kinship_type)
{  //生成矩阵需要相应的二进制文件，而整个gwas过程中，真正使用的二进制文件都是代码生成的带有tmp字样的临时二进制文件
   //无论输入的是vcf等非二进制格式还是bed二进制格式，最终都生成例如：hapmap_tmp.bed hapmap_tmp.fam hapmap_tmp.bim这样的二进制文件
   //所以传入的binGenoBaseName为hapmap_tmp 这样的字符串 以此来选中所有相同命名的二进制文件
    if (binGenoBaseName.isNull() || out.isNull())
    {
        return false;
    }

    // gemma -bfile 222_filter1 -gk 1 -o 222_filter1
    this->paramlist.clear();
    this->paramlist.append("-bfile");
    this->paramlist.append(binGenoBaseName);

    this->paramlist.append("-gk");
    this->paramlist.append(kinship_type);
    this->paramlist.append("-o");
    this->paramlist.append(out);   //这里生成的矩阵的文件名就是类似 hapmap_tmp


    return true;
}

/**
 * @brief Gemma::runGWAS
 * @param binGenoBaseName: the prefix of binary file.
 * @param phenotype
 * @param covariate
 * @param kinship
 * @param out
 * @param model
 * @return
 */
bool Gemma::runGWAS(QString binGenoBaseName, QString phenotype, QString covariate, QString kinship,
                    QString out, QString model, QString lmmtest ,QString bslmmmodel)
{
    this->paramlist.clear();            // Clear paramlist before set parameter.
    if (binGenoBaseName.isNull() || phenotype.isNull() || kinship.isNull() || model.isNull())
    {
        if (phenotype.isNull())
        {
            return false;
        }
        if (binGenoBaseName.isNull())
        {
            return false;
        }
        return false;
    }

    this->paramlist.append("-bfile");
    this->paramlist.append(binGenoBaseName);

    if (model == "LMM"||model == "lmm")
    {   // gemma -bfile 222_filter1 -k 222_filter1.cXX.txt -lmm 1 -n 2 -o 222_filter1
        this->paramlist.append("-k");
        this->paramlist.append(kinship);
        this->paramlist.append("-lmm");
        this->paramlist.append(lmmtest);   // wald:1   likelihood:2    score:3     all:4
        this->paramlist.append("-n");
        this->paramlist.append("2");
        if (!covariate.isNull())    // Can't for BSLMM
        {
//            this->paramlist.append("-c");
//            this->paramlist.append(covariate);
        }
    }

    if (model == "BSLMM"||model == "bslmm")
    {  // gemma -bfile 222_filter1 -bslmm 1 -n 2 -o tmp
        this->paramlist.append("-bslmm");
        this->paramlist.append(bslmmmodel);
        this->paramlist.append("-n");
        this->paramlist.append("2");
    }

    this->paramlist.append("-o");
    this->paramlist.append(out);

    return true;
}

bool Gemma::phe_fam_Preparation(QString phe, QString fam)
{   // Replace "NA" to "-9", then complete .fam
    // .fam: FID IID PID MID Sex 1 Phe  (phenotype data to the 7th column of .fam)
    if (phe.isNull() || fam.isNull())
    {
        return false;
    }

    QFile pheFile(phe);
    QFile famFile(fam);
    QFile tmpFamFile(fam+".tmp");  // A temp file to save the final data of fam file.


    QFileInfo famFileInfo(fam), tmpFamFileInfo(tmpFamFile);
    QString famFileName = famFileInfo.fileName();
    QString famFileAbPath = famFileInfo.absolutePath();
    QString tmpFamFileAbFilePath = tmpFamFileInfo.absoluteFilePath();   // /path/basename.suffix

    QTextStream tmpFamStream(&tmpFamFile);
    QTextStream pheStream(&pheFile);
    QTextStream famStream(&famFile);

    if (!pheFile.open(QIODevice::ReadOnly) ||
        !famFile.open(QIODevice::ReadOnly) ||
        !tmpFamFile.open(QIODevice::ReadWrite))
    {   // Open file error.
        return false;
    }
    while (!pheStream.atEnd() && !famStream.atEnd())
    {   // Read file line by line respectively.
        QString famCurLine = famStream.readLine();
        QString pheCurLine = pheStream.readLine();
        // Replace "NA" to "-9"
        QStringList pheCurLineList = pheCurLine.replace("NA", "-9").split(QRegExp("\\s+"), QString::SkipEmptyParts);
       // Split by space(s)
        QStringList famCurLineList = famCurLine.split(QRegExp("\\s+"), QString::SkipEmptyParts);

        while (famCurLineList.length() > 5)
        {   // Only hold the first 5 columns.(FID IIF PID MID Sex)
            famCurLineList.removeLast();
        }

        // .fam: FID IID PID MID Sex 1 Phe  (phenotype data to the 7th column of .fam)
        tmpFamStream << famCurLineList.join("\t") << "\t1\t"  << pheCurLineList[pheCurLineList.length()-1] << "\n";
                                                    // \t1\t就是填充一列的1 \t是tab
        qApp->processEvents();  // Avoid no responding of MainWindow.
    }

    if (!pheStream.atEnd() || !famStream.atEnd())
    {   // Not end synchronously.
        return false;
    }
    // Close file finally.
    pheFile.close();
    famFile.close();
    tmpFamFile.close();

    famFile.remove();// remove original file.
    // Rename new file to original file name.
    if (!tmpFamFile.rename(tmpFamFileAbFilePath, famFileAbPath+"/"+famFileName))
    {
        return false;
    }
    return true;
}
