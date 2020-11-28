#include "funcannotator.h"

FuncAnnotator::FuncAnnotator()
{

}

/**
 * @brief FunctionalAnnotator::filterSNP
 *      select SNP whose p-value bigger than threshold to out file.
 *      the threhold is -log(thBase * 10 ^ thExpo)
 * @param pvalFile      a header is necessary, the last column is p-value
 * @param thBase        base of threshold
 * @param thExpo        exponent of threshold
 * @param out           a file to store SNP which is selected.(Two column: SNP_ID P-VAL)
 * @return              fail(false) or success(true).
 */
bool FuncAnnotator::filterSNP(QString const pvalFilePath, QString const thBase,
                              QString const thExpo, QString const outFilePath)
{
    if (pvalFilePath.isNull() || thBase.isNull() || thExpo.isNull() || outFilePath.isNull())
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

    if (pvalueFileComSuffix == "assoc.txt")
    {   // Gemma LMM
        /* .assoc.txt:
         *  chr rs ps n_miss allele1 allele0 af beta se logl_H1 l_remle l_mle p_wald p_lrt p_score
         */
        snpIDIndex = 1;
    }

    int thresholdBase = thBase.toInt();
    int thresholdExpo = thExpo.toInt();
    float threshold = -log10f(thresholdBase*pow10f(thresholdExpo));

    QFile pvalFile(pvalFilePath);
    QFile outFile(outFilePath);

    if (!pvalFile.open(QIODevice::ReadOnly) ||
        !outFile.open(QIODevice::ReadWrite))
    {
        return false;
    }
    QTextStream pvalFileStream(&pvalFile);
    QTextStream outFileStream(&outFile);

    pvalFileStream.readLine();  // Discard the header in the first line.
    while (!pvalFileStream.atEnd())
    {
        QString curLine = pvalFileStream.readLine();
        QStringList curLineList = curLine.replace("NA", "-9").split(QRegExp("\\s+"), QString::SkipEmptyParts);
        if (-log10f(curLineList.constLast().toFloat()) >= threshold)
        {
            outFileStream << curLineList[snpIDIndex] << "\t"
                          << curLineList.constLast() << endl;
        }
    }
    pvalFile.close();
    outFile.close();

    return true;
}

/**
 * @brief extractPos
 *      Extrac position of SNP and save to outFile.(reserve p-value)
 * @param pvalFilePath  p-value file (Two column: SNP_ID P-VAL)
 * @param mapFilePath   map file(There position info of SNP)
 * @param outFilePath   output file(SNP_ID, p-value, CHR, BP)
 * @return
 */
bool FuncAnnotator::extractPos(QString const pvalFilePath, QString const mapFilePath, QString const outFilePath)
{
    if (pvalFilePath.isNull() || mapFilePath.isNull() || outFilePath.isNull())
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

    QFile mapFile(mapFilePath);
    QFile outFile(outFilePath);
    if (!mapFile.open(QIODevice::ReadOnly) ||
        !outFile.open(QIODevice::WriteOnly))
    {
        return false;
    }
    FileReader fileReader;
    // traverse map file, extract SNP.
    QTextStream mapFileStream(&mapFile);
    QTextStream outFileStream(&outFile);
    while (!mapFile.atEnd())
    {
        QStringList curLine = mapFileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
        if (snpIDMap.find(curLine[1]) != snpIDMap.end())
        {
            outFileStream << snpIDMap[curLine[1]] << "\t"
                          << (fileReader.isNumber(curLine[0]) ? "chr"+curLine[0] : curLine[0])
                          << "\t" << curLine[3] << endl;
        }
    }

     return true;
}

/**
 * @brief FuncAnnotator::complExoSnpInfo
 *      Add gene ID and mutation type to SNP in exonic
 *      (Except the annotatin is unknown of SNP in .exonic_variant_function)
 * @param snpPosFilePath        :SNP_ID P-VALUE CHR BP
 * @param exValFuncFilePath     :.exonic_variant_function
 * @param outFilePath           :snpPosFile(SNP_ID P-VALUE CHR BP), gene_ID, mutation_type, other info
 * @return
 */
bool FuncAnnotator::complExoSnpInfo(QString const snpPosFilePath, QString const exVarFuncFilePath, QString const outFilePath)
{
    if (snpPosFilePath.isNull() ||
        exVarFuncFilePath.isNull() ||
        outFilePath.isNull())
    {
        return false;
    }

    ///////////////////////////////////////
    ////////    Read snp position  ////////
    ///////////////////////////////////////
    QFile snpPosFile(snpPosFilePath);
    if (!snpPosFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QTextStream snpPosFileStream(&snpPosFile);
    QMap<QString, QStringList> snpPosInfoMap;       // Save snp position and info.
    while (!snpPosFileStream.atEnd())
    {
        QStringList curLine = snpPosFileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
        QString chr_bp = curLine[2] + "_" + curLine[3];
        snpPosInfoMap.insert(chr_bp, curLine);
    }

    /////////////////////////////////////////////////////
    ////////    Capture exonic by snp position.  ////////
    /////////////////////////////////////////////////////
    QFile exValFuncFile(exVarFuncFilePath);
    QFile outFile(outFilePath);
    if (!exValFuncFile.open(QIODevice::ReadOnly) ||
        !outFile.open(QIODevice::WriteOnly))
    {
        return false;
    }
    QTextStream outFileStream(&outFile);
    QTextStream exValFuncFileStream(&exValFuncFile);
    while (!exValFuncFileStream.atEnd())
    {
        QStringList curLine = exValFuncFileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
        QString chr_bp = curLine[4] + "_" + curLine[5];
        if (snpPosInfoMap.find(chr_bp) == snpPosInfoMap.end())
        {
            continue;
        }
        // aminoAcidChange: Amino acid changes
        //     contains the gene name, the transcript identifier and the sequence change in the corresponding transcript
        QString aminoAcidChange = curLine[3];
        int commaCounter = aminoAcidChange.count(',');
        if (commaCounter == 1)
        {
            QRegExp regExp("([^:]*):(.*),");    //  Obj data: "HDH_G00730:HDH_T00730:exon1:c.A132G:p.T44T,"
            int pos = regExp.indexIn(aminoAcidChange);
            if (pos == -1)
            {   // Match string error.
                return false;
            }
            QStringList list = regExp.capturedTexts();
            QString subStr_1 = regExp.cap(1);  // "HDH_G00730"
            QString subStr_2 = regExp.cap(2);  // "HDH_T00730:exon1:c.A132G:p.T44T,"
            outFileStream << snpPosInfoMap[chr_bp].join("\t") << "\t" << subStr_1 << "\t"
                          << curLine[1] << "\t" << subStr_2 << endl;
        }
        else if (commaCounter == 2)
        {   // Assume there are two annotation.()
            QStringList aminoAcidChangeList = aminoAcidChange.split(",", QString::SkipEmptyParts);

            // The first annotation
            QRegExp regExp("([^:]*):(.*)");    //  Obj data: "HDH_G00730:HDH_T00730:exon1:c.A132G:p.T44T"
            int pos = regExp.indexIn(aminoAcidChangeList[0]);
            if (pos == -1)
            {   // Match string error.
                return false;
            }
            QStringList list = regExp.capturedTexts();
            QString subStr_1 = regExp.cap(1);  // "HDH_G00730"
            QString subStr_2 = regExp.cap(2);  // "HDH_T00730:exon1:c.A132G:p.T44T,"

            // $hash1{$tmp}=[$1, $F[1], $2];
            // $hash1{$tmp}[0]\t$hash1{$tmp}[1]\t$_\t$hash1{$tmp}[2]
            outFileStream << snpPosInfoMap[chr_bp].join("\t") << "\t" << subStr_1 << "\t"
                          << curLine[1] << "\t" << subStr_2 << endl;

            //  The second annotation
            pos = regExp.indexIn(aminoAcidChangeList[1]);
            if (pos == -1)
            {   // Match string error.
                return false;
            }
            list = regExp.capturedTexts();
            subStr_1 = regExp.cap(1);  // "HDH_G00730"
            subStr_2 = regExp.cap(2);  // "HDH_T00730:exon1:c.A132G:p.T44T,"
            // $hash2{$tmp}[0]\t$hash2{$tmp}[1]\t$_\t$hash2{$tmp}[2]
            // snp2ndInfoMap.insert(chr_bp, QStringList()<<subStr_1<<curLine[1]<<subStr_2);
            outFileStream  << snpPosInfoMap[chr_bp].join("\t") << "\t" << subStr_1
                           << "\t" << curLine[1] << "\t" << subStr_2 << endl;
        }
    }

    return true;
}

/**
 * @brief FuncAnnotator::complNonExoSnpInfo
 *          Add the gene ID and region before the SNP loci located outside of the gene exon,
 *      and add the SNP relative to the gene information in the last column.
 * @param exonicPosFilePath     :gene_ID, mutation_type, snpPosFile(SNP_ID P-VALUE CHR BP),other info
 * @param snpPosFilePath        :SNP_ID P-VALUE CHR BP
 * @param varFuncFilePath       :.variant_function
 * @param outFilePath           :snpPosFile:(SNP_ID, P-VALUE, CHR, BP), gene_ID, mutation_type, distance
 * @return
 */
bool FuncAnnotator::complNonExoSnpInfo(QString const exonicPosFilePath, QString const snpPosFilePath,
                                       QString const varFuncFilePath, QString const outFilePath)
{   // Wait for reconstruct.
    if (exonicPosFilePath.isNull() ||
        snpPosFilePath.isNull() ||
        varFuncFilePath.isNull() ||
        outFilePath.isNull())
    {
        return false;
    }

    ////////////////////////////////////////////
    //////////    Read exonic snp ID  //////////
    ////////////////////////////////////////////
    QFile exonicPosFile(exonicPosFilePath);
    if (!exonicPosFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QTextStream exonicPosFileStream(&exonicPosFile);
    QSet<QString> exonicSnpSet;       // Use "chr_bp" represent SNP.
    while (!exonicPosFileStream.atEnd())
    {
        QStringList curLine = exonicPosFileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
        exonicSnpSet.insert(curLine[2]+"_"+curLine[3]);
    }

    ///////////////////////////////////////
    ////////    Read snp position  ////////
    ///////////////////////////////////////
    QFile snpPosFile(snpPosFilePath);
    if (!snpPosFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QTextStream snpPosFileStream(&snpPosFile);
    QMap<QString, QStringList> snpPosInfoMap;       // Save snp position and info.
    while(!snpPosFileStream.atEnd())
    {
        QStringList curLine = snpPosFileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
        QString chr_bp = curLine[2]+"_"+curLine[3];
        if (exonicSnpSet.find(chr_bp) != exonicSnpSet.end())
        {   // Exclude snp in exonic. Use "chr_bp"
            continue;
        }
        snpPosInfoMap.insert(chr_bp, curLine);
    }

    /////////////////////////////////////////////////////
    ////////    Capture exonic by snp position.  ////////
    /////////////////////////////////////////////////////
    QFile varFuncFile(varFuncFilePath);
    QFile outFile(outFilePath);
    if (!varFuncFile.open(QIODevice::ReadOnly) ||
        !outFile.open(QIODevice::WriteOnly))
    {
        return false;
    }
    QTextStream varFuncFileStream(&varFuncFile);
    QTextStream outFileStream(&outFile);
    while(!varFuncFileStream.atEnd())
    {
        QStringList curLine = varFuncFileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
//        if (curLine[0] == "intergenic")
//        {   // Not to deal with.
//            continue;
//        }
        QString chr_bp = curLine[2]+"_"+curLine[3];
        if (snpPosInfoMap.find(chr_bp) == snpPosInfoMap.end())
        {   // Not to deal with.
            continue;
        }

        int commaCounter = curLine[1].count(',');
        int semiCounter = curLine[1].count(';');

        if (commaCounter < 1 && semiCounter < 1)
        {//curLine[0]:intronic  curLine[1]:HDH_G00724
            // gene: Gene_ID(distance) ---> Gene_ID, distance
            QStringList geneAnno = curLine[1].split(QRegExp("\\(|\\)"), QString::SkipEmptyParts);
            outFileStream << snpPosInfoMap[chr_bp].join("\t") << "\t"
                          << geneAnno[0] << "\t" << curLine[0] << "\t"
                          << (geneAnno.length() < 2 ? "" : geneAnno[1]) << endl;
        }
        else if (commaCounter == 1 && semiCounter == 1)
        {//curLine[0]:upstream;downstream   curLine[1]:HDH_G00991,HDH_G00992(dist=242);HDH_G00993(dist=838)
            QStringList gene = curLine[1].split(";", QString::SkipEmptyParts);
            QStringList gene_1st = gene[0].split(",", QString::SkipEmptyParts);
            QStringList gene_2nd = gene[1].split(",", QString::SkipEmptyParts);
            QStringList snpRegion = curLine[0].split(";", QString::SkipEmptyParts);
            if (gene_1st.length() < 2)
            {   // A;B,C
                QStringList geneAnno = gene_1st[0].split(QRegExp("\\(|\\)"), QString::SkipEmptyParts);
                outFileStream << snpPosInfoMap[chr_bp].join("\t") << "\t"
                              << geneAnno[0] << "\t" << snpRegion[0] << "\t"
                              << (geneAnno.length() < 2 ? "" : geneAnno[1]) << endl;
                geneAnno = gene_2nd[0].split(QRegExp("\\(|\\)"), QString::SkipEmptyParts);
                outFileStream << snpPosInfoMap[chr_bp].join("\t") << "\t"
                              << geneAnno[0] << "\t" << snpRegion[1] << "\t"
                              << (geneAnno.length() < 2 ? "" : geneAnno[1]) << endl;
                geneAnno = gene_2nd[1].split(QRegExp("\\(|\\)"), QString::SkipEmptyParts);
                outFileStream << snpPosInfoMap[chr_bp].join("\t") << "\t"
                              << geneAnno[0] << "\t" << snpRegion[1] << "\t"
                              << (geneAnno.length() < 2 ? "" : geneAnno[1]) << endl;
            }
            else if (gene_2nd.length() < 2)
            {   // A,B;C
                QStringList geneAnno = gene_1st[0].split(QRegExp("\\(|\\)"), QString::SkipEmptyParts);
                outFileStream << snpPosInfoMap[chr_bp].join("\t") << "\t"
                              << geneAnno[0] << "\t" << snpRegion[0] << "\t"
                              << (geneAnno.length() < 2 ? "" : geneAnno[1]) << endl;
                geneAnno = gene_1st[1].split(QRegExp("\\(|\\)"), QString::SkipEmptyParts);
                outFileStream << snpPosInfoMap[chr_bp].join("\t") << "\t"
                              << geneAnno[0] << "\t" << snpRegion[0] << "\t"
                              << (geneAnno.length() < 2 ? "" : geneAnno[1]) << endl;
                geneAnno = gene_2nd[0].split(QRegExp("\\(|\\)"), QString::SkipEmptyParts);
                outFileStream << snpPosInfoMap[chr_bp].join("\t") << "\t"
                              << geneAnno[0] << "\t" << snpRegion[1] << "\t"
                              << (geneAnno.length() < 2 ? "" : geneAnno[1]) << endl;
            }
        }
        else if (commaCounter == 1)
        {   // curLine[0]:intergenic    curLine[1]:HDH_G00724(dist=1008),HDH_G00725(dist=39868)
            // also can be: upstream	HDH_G00718,HDH_G00719(dist=775)	Contig678	47982	47982	G	C
            QStringList geneAnno = curLine[1].split(",", QString::SkipEmptyParts);
            QStringList gene_1st = geneAnno[0].split(QRegExp("\\(|\\)"), QString::SkipEmptyParts);
            outFileStream << snpPosInfoMap[chr_bp].join("\t") << "\t"
                          << gene_1st[0] << "\t" << curLine[0] << "\t"
                          << (gene_1st.length() < 2 ? "" : gene_1st[1]) << endl;
            QStringList gene_2nd = geneAnno[1].split(QRegExp("\\(|\\)"), QString::SkipEmptyParts);
            outFileStream << snpPosInfoMap[chr_bp].join("\t") << "\t"
                          << gene_2nd[0] << "\t" << curLine[0] << "\t"
                          // consider upstream or downstream without dist ---> judge geneAnno.length()
                          << (gene_2nd.length() < 2 ? "" : gene_2nd[1]) << endl;
        }
        else if (semiCounter == 1)
        {   // curLine[0]:upstream;downstream     curLine[1]:HDH_G00745(dist=401);HDH_G00744(dist=386)
            QStringList gene = curLine[1].split(";", QString::SkipEmptyParts);
            QStringList snpRegion = curLine[0].split(";", QString::SkipEmptyParts);
            QStringList geneAnno = gene[0].split(QRegExp("\\(|\\)"), QString::SkipEmptyParts);
            outFileStream << snpPosInfoMap[chr_bp].join("\t") << "\t"
                          << geneAnno[0] << "\t" << snpRegion[0] << "\t"
                          << (geneAnno.length() < 2 ? "" : geneAnno[1]) << endl;
            geneAnno = gene[1].split(QRegExp("\\(|\\)"), QString::SkipEmptyParts);
            outFileStream << snpPosInfoMap[chr_bp].join("\t") << "\t"
                          << geneAnno[0] << "\t" << snpRegion[1] << "\t"
                          << (geneAnno.length() < 2 ? "" : geneAnno[1]) << endl;
        }
        else
        {

        }
    }

    return true;
}

/**
 * @brief FuncAnnotator::complFuncAnnoInfo
 *          Add functional annotation information according to the functional annotation database
 * @param exonicPosFilePath     :snpPosFile(SNP_ID P-VALUE CHR BP),gene_ID, mutation_type, other info
 * @param nonExonicPosFile      :snpPosFile(SNP_ID P-VALUE CHR BP), gene_ID, mutation_type, distance
 * @param baseFilePath          :annotation database file.
 * @param outFilePath           :annotation result file.
 * @return
 */
bool FuncAnnotator::complFuncAnnoInfo(QString const exonicPosFilePath, QString nonExonicPosFilePath,
                                      QString baseFilePath, QString outFilePath)
{
    if (exonicPosFilePath.isNull() ||
        nonExonicPosFilePath.isNull() ||
        baseFilePath.isNull() ||
        outFilePath.isNull())
    {
        return false;
    }

    QMap<QString, QString> geneIDMap;       // Save both exonicPos and nonExonicPos.
    QFile baseFile(baseFilePath);
    QFile outFile(outFilePath);
    if  (!baseFile.open(QIODevice::ReadOnly) ||
         !outFile.open(QIODevice::WriteOnly))
    {
        return false;
    }
    QTextStream baseFileStream(&baseFile);

    if (baseFilePath.contains("ncbi.csv"))
    {
        QStringList headerList = baseFileStream.readLine().remove(0, 1).split(',');      // Read the header line
        int geneIDIndex = headerList.indexOf("Locus");
        int geneDescripIndex = headerList.indexOf("Protein Name");
        if (geneIDIndex < 0 || geneDescripIndex < 0)
        {
            return false;
        }
        while (!baseFileStream.atEnd())
        {
            QString curLine = baseFileStream.readLine();
            QStringList curLineList = curLine.replace("\"", "").split(",");
            geneIDMap.insert(curLineList[geneIDIndex], curLineList[geneDescripIndex]);
        }
    }
    else if (baseFilePath.contains("ensem.csv"))
    {
        QStringList headerList = baseFileStream.readLine().split(',');      // Read the header line
        int geneIDIndex = headerList.indexOf("Gene stable ID");
        int geneDescripIndex = headerList.indexOf("Gene description");
        if (geneIDIndex < 0 || geneDescripIndex < 0)
        {
            return false;
        }

        while (!baseFileStream.atEnd())
        {
            QStringList curLineList = baseFileStream.readLine().split(',');
            geneIDMap.insert(curLineList[geneIDIndex], curLineList[geneDescripIndex]);
        }
    }
    else    // The first column is Gene_id, the other column is description. seperate by '\t'
    {
        while (!baseFileStream.atEnd())
        {
            QStringList curLine = baseFileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
            //        QString id = curLine[0].replace('T', 'G');      // Transcript ID to Gene ID.
            QString id = curLine[0];
            curLine.removeFirst();
            geneIDMap.insert(id, curLine.join("\t"));
        }
    }

    QFile exonicPosFile(exonicPosFilePath);
    QFile nonExonicPosFile(nonExonicPosFilePath);
    if (!nonExonicPosFile.open(QIODevice::ReadOnly) ||
        !exonicPosFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QTextStream exonicPosFileStream(&exonicPosFile);
    QTextStream nonExonicPosFileStream(&nonExonicPosFile);
    QTextStream outFileStream(&outFile);

    while (!exonicPosFileStream.atEnd())
    {
        QStringList curLineList = exonicPosFileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
        if (curLineList.length() == 6)
        {
            outFileStream << curLineList.join("\t") << "\t" << "unknown\t"
                          << geneIDMap[curLineList[4]] << endl;
//            geneIDMap.insert(curLineList[4], QStringList() << curLineList << "unknown");
        }
        else
        {
            outFileStream << curLineList.join("\t") << "\t"
                          << geneIDMap[curLineList[4]] << endl;
//            geneIDMap.insert(curLineList[4], curLineList);
        }
    }
    while (!nonExonicPosFileStream.atEnd())
    {
        QStringList curLineList = nonExonicPosFileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
        if (curLineList.length() == 6)
        {
            outFileStream << curLineList.join("\t") << "\t" << "unknown\t"
                          << geneIDMap[curLineList[4]] << endl;
//            geneIDMap.insert(curLineList[4], QStringList() << curLineList << "unknown");
        }
        else
        {
            outFileStream << curLineList.join("\t") << "\t"
                          << geneIDMap[curLineList[4]] << endl;
//            geneIDMap.insert(curLineList[4], curLineList);
        }
    }

    return true;
}

/*
bool FuncAnnotator::complFuncAnnoInfo(QString const exonicPosFilePath, QString nonExonicPosFilePath,
                                      QString baseFilePath, QString outFilePath)
{
    if (exonicPosFilePath.isNull() ||
        nonExonicPosFilePath.isNull() ||
        baseFilePath.isNull() ||
        outFilePath.isNull())
    {
        return false;
    }

    QFile exonicPosFile(exonicPosFilePath);
    QFile nonExonicPosFile(nonExonicPosFilePath);
    if (!nonExonicPosFile.open(QIODevice::ReadOnly) ||
        !exonicPosFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QTextStream exonicPosFileStream(&exonicPosFile);
    QTextStream nonExonicPosFileStream(&nonExonicPosFile);
    // Can't use map here, because of duplication.
    QMap<QString, QStringList> geneIDMap;       // Save both exonicPos and nonExonicPos.
    while (!exonicPosFileStream.atEnd())
    {
        QStringList curLineList = exonicPosFileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
        if (curLineList.length() == 6)
        {
            geneIDMap.insert(curLineList[4], QStringList() << curLineList << "unknown");
        }
        else
        {
            geneIDMap.insert(curLineList[4], curLineList);
        }
    }
    while (!nonExonicPosFileStream.atEnd())
    {
        QStringList curLineList = nonExonicPosFileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
        if (curLineList.length() == 6)
        {
            geneIDMap.insert(curLineList[4], QStringList() << curLineList << "unknown");
        }
        else
        {
            geneIDMap.insert(curLineList[4], curLineList);
        }
    }

    QFile baseFile(baseFilePath);
    QFile outFile(outFilePath);
    if  (!baseFile.open(QIODevice::ReadOnly) ||
         !outFile.open(QIODevice::WriteOnly))
    {
        return false;
    }
    QTextStream baseFileStream(&baseFile);
    QTextStream outFileStream(&outFile);

    if (baseFilePath.contains("ncbi.csv"))
    {
        QStringList headerList = baseFileStream.readLine().remove(0, 1).split(',');      // Read the header line
        int geneIDIndex = headerList.indexOf("Locus");
        int geneDescripIndex = headerList.indexOf("Protein Name");
        if (geneIDIndex < 0 || geneDescripIndex < 0)
        {
            return false;
        }
        while (!baseFileStream.atEnd())
        {
            QString curLine = baseFileStream.readLine();
            QStringList curLineList = curLine.replace("\"", "").split(",");
            qDebug() << curLineList.join("\t");
            if (geneIDMap.find(curLineList[geneIDIndex]) == geneIDMap.end())
            {
                continue;
            }
            outFileStream << geneIDMap[curLineList[geneIDIndex]].join("\t")
                          << "\t" << curLineList[geneDescripIndex] << endl;
        }
    }
    else if (baseFilePath.contains("ensem.csv"))
    {
        QStringList headerList = baseFileStream.readLine().split(',');      // Read the header line
        int geneIDIndex = headerList.indexOf("Gene stable ID");
        int geneDescripIndex = headerList.indexOf("Gene description");
        if (geneIDIndex < 0 || geneDescripIndex < 0)
        {
            return false;
        }

        while (!baseFileStream.atEnd())
        {
            QStringList curLineList = baseFileStream.readLine().split(',');

            if (geneIDMap.find(curLineList[geneIDIndex]) == geneIDMap.end())
            {
                continue;
            }

            outFileStream << geneIDMap[curLineList[geneIDIndex]].join("\t")
                          << "\t" << curLineList[geneDescripIndex] << endl;
        }
    }
    else    // The first column is Gene_id, the other column is description. seperate by '\t'
    {
        while (!baseFileStream.atEnd())
        {
            QStringList curLine = baseFileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
    //        QString id = curLine[0].replace('T', 'G');      // Transcript ID to Gene ID.
            QString id = curLine[0];
            if (geneIDMap.find(id) == geneIDMap.end())
            {
                continue;
            }
            curLine.removeFirst();
            outFileStream << geneIDMap[id].join("\t") << "\t" << curLine.join("\t") << endl;
        }
    }

    return true;
}
*/

/**
 * @brief FuncAnnotator::makeBaseFromGtfAndNcbi
 * @param gtfOrGffFilePath
 * @param ensemblBase
 *          Database file download from ensembl, seperate by comma.
 *          Include a column with header "Transcript stable ID".
 * @param out
 * @return
 */
bool FuncAnnotator::makeBaseFromEnsembl(QString gtfOrGffFilePath, QString ensemblBase, QString outFilePath)
{
    if (gtfOrGffFilePath.isNull() || ensemblBase.isNull() || outFilePath.isNull())
    {
        return false;
    }

    QFile gtfOrGffFile(gtfOrGffFilePath);
    QFile ensemblBaseFile(ensemblBase);
    QFile outFile(outFilePath);
    if (!gtfOrGffFile.open(QIODevice::ReadOnly) ||
        !ensemblBaseFile.open(QIODevice::ReadOnly) ||
        !outFile.open(QIODevice::WriteOnly))
    {
        return false;
    }
    QTextStream gtfOrGffFileStream(&gtfOrGffFile);
    QTextStream ensemblBaseFileStream(&ensemblBaseFile);
    QTextStream outFileStream(&outFile);

    QFileInfo fileInfo(gtfOrGffFile);
    QString suffix = fileInfo.suffix();

    QSet<QString> transcriptID;
    if (suffix.toLower() == "gff" || suffix.toLower() == "gff3")
    {
        transcriptID = this->getTransIDFromGff(gtfOrGffFilePath);
    }
    else if (suffix.toLower() == "gtf" || suffix.toLower() == "gtf3")
    {
        transcriptID = this->getTransIDFromGtf(gtfOrGffFilePath);
    }

    QStringList headerList = ensemblBaseFileStream.readLine().split(',');      // Read the header line
    int transIDIndex = headerList.indexOf("Transcript stable ID");
    int geneDescripIndex = headerList.indexOf("Gene description");
    if (transIDIndex < 0 || geneDescripIndex < 0)
    {
        return false;
    }

    while (!ensemblBaseFileStream.atEnd())
    {
        QStringList curLineList = ensemblBaseFileStream.readLine().split(',');

        if (transcriptID.find(curLineList[transIDIndex]) == transcriptID.end())
        {
            continue;
        }

        outFileStream << curLineList[transIDIndex] << "\t" << curLineList[geneDescripIndex] << endl;
    }

    return true;
}

/**
 * @brief makeBaseFromGtfAndNcbi
 * @param gtfFilePath
 * @param ncbiBase
 *          Database file download from ncbi, seperate by comma.
 *          Include a column with header "Protein stable ID".
 * @param outFilePath   Two columns(Transcript_ID   Protein_ID)
 * @return
 */
bool FuncAnnotator::makeBaseFromNcbi(QString gtfOrGffFilePath, QString ncbiBase, QString outFilePath)
{
    if (gtfOrGffFilePath.isNull() || ncbiBase.isNull() || outFilePath.isNull())
    {
        return false;
    }

    QFile gtfOrGffFile(gtfOrGffFilePath);
    QFile ncbiBaseFile(ncbiBase);
    QFile outFile(outFilePath);
    if (!gtfOrGffFile.open(QIODevice::ReadOnly) ||
        !ncbiBaseFile.open(QIODevice::ReadOnly) ||
        !outFile.open(QIODevice::WriteOnly))
    {
        return false;
    }
    QTextStream gtfOrGffFileStream(&gtfOrGffFile);
    QTextStream ncbiBaseFileStream(&ncbiBaseFile);
    QTextStream outFileStream(&outFile);

    QFileInfo fileInfo(gtfOrGffFile);
    QString suffix = fileInfo.suffix();
    QMap<QString, QString> proteinIDandTransIDMap;
    if (suffix.toLower() == "gff" || suffix.toLower() == "gff3")
    {
        proteinIDandTransIDMap = this->getProteinIDandTransIDFromGff(gtfOrGffFilePath);
    }
    else if (suffix.toLower() == "gtf" || suffix.toLower() == "gtf3")
    {
        proteinIDandTransIDMap = this->getProteinIDandTransIDFromGtf(gtfOrGffFilePath);
    }
    proteinIDandTransIDMap = this->getProteinIDandTransIDFromGtf(gtfOrGffFilePath);

    QStringList headerList = ncbiBaseFileStream.readLine().remove(0, 1).split(',');      // Read the header line
    int proteinIDIndex = headerList.indexOf("Protein product");
    int geneDescripIndex = headerList.indexOf("Protein Name");
    if (proteinIDIndex < 0 || geneDescripIndex < 0)
    {
        return false;
    }

    while (!ncbiBaseFileStream.atEnd())
    {
        QStringList curLineList = ncbiBaseFileStream.readLine().split(',');

        curLineList[proteinIDIndex].remove(0, 1);
        curLineList[proteinIDIndex].remove(curLineList[proteinIDIndex].length(), 1);

        if (proteinIDandTransIDMap.find(curLineList[proteinIDIndex]) == proteinIDandTransIDMap.end())
        {
            continue;
        }
        outFileStream << curLineList[proteinIDIndex]
                      << proteinIDandTransIDMap[curLineList[proteinIDIndex]]
                      << endl;
    }

    return true;
}

/**
 * @brief FuncAnnotator::getTransIDFromGtf
 * @param gtfFilePath
 *          The last column contains multiple attribute, seperate by ';',
 *        each of the attribute is the form of (key "value").
 *          transcript_id "ID", spell of "transcript_id" need to to be same.
 * @return
 */
QSet<QString> FuncAnnotator::getTransIDFromGtf(QString gtfFilePath)
{
    QSet<QString> transIDSet;
    if (gtfFilePath.isNull())
    {
        return transIDSet;
    }

    QFile gtfFile(gtfFilePath);
    if (!gtfFile.open(QIODevice::ReadOnly))
    {
        qDebug() << gtfFile.errorString();
        return transIDSet;
    }
    QTextStream gtfFileStream(&gtfFile);

    while (!gtfFileStream.atEnd())
    {
        QString curLine = gtfFileStream.readLine();
        if (curLine[0] == '#')
        {   // comment line begin with a symbol '#'
            continue;   // Read next line.
        }
        QStringList curLineInfoList = curLine.split('\t').last().split(';');

        QRegExp regExp("transcript_id \"(.*)\"");
        for (auto item : curLineInfoList)
        {
            int pos = regExp.indexIn(item);
            if (pos < 0)
            {
                continue;
            }
            transIDSet.insert(regExp.cap(1));
            break;  // Only one transcript_id need to be found.
        }
    }

    return transIDSet;
}

/**
 * @brief FuncAnnotator::getTransIDFromGff
 * @param gffFilePath
 *          The last column contains multiple attribute, seperate by ';',
 *        each of the attribute is the form of (key="value").
 *          transcript_id=ID, spell of "transcript_id" need to to be same.
 * @return
 */
QSet<QString> FuncAnnotator::getTransIDFromGff(QString gffFilePath)
{
    QSet<QString> transIDSet;
    if (gffFilePath.isNull())
    {
        return transIDSet;
    }

    QFile gffFile(gffFilePath);
    if (!gffFile.open(QIODevice::ReadOnly))
    {
        qDebug() << gffFile.errorString();
        return transIDSet;
    }
    QTextStream gffFileStream(&gffFile);

    while (!gffFileStream.atEnd())
    {
        QString curLine = gffFileStream.readLine();
        if (curLine[0] == '#')
        {   // comment line begin with a symbol '#'
            continue;   // Read next line.
        }
        QStringList curLineInfoList = curLine.split('\t').last().split(';');

        QRegExp regExp("transcript_id=(.*)");
        for (auto item : curLineInfoList)
        {
            int pos = regExp.indexIn(item);
            if (pos < 0)
            {
                continue;
            }
            transIDSet.insert(regExp.cap(1));
            break;  // Only one transcript_id need to be found.
        }
    }

    return transIDSet;
}

/**
 * @brief FuncAnnotator::getProteinIDandTransIDFromGtf
 * @param gtfFilePath
 *          The last column contains multiple attribute, seperate by ';',
 *        each of the attribute is the form of (key "value").
 *          protein_id "ID", spell of "protein_id" need to to be same.
 * @return  proteinIDtransIDMap: key proteinID, value transcriptID.
 */
QMap<QString, QString> FuncAnnotator::getProteinIDandTransIDFromGtf(QString gtfFilePath)
{
    QMap<QString, QString> proteinIDtransIDMap;
    if (gtfFilePath.isNull())
    {
        return proteinIDtransIDMap;
    }

    QFile gtfFile(gtfFilePath);
    if (!gtfFile.open(QIODevice::ReadOnly))
    {
        qDebug() << gtfFile.errorString();
        return proteinIDtransIDMap;
    }
    QTextStream gtfFileStream(&gtfFile);

    while (!gtfFileStream.atEnd())
    {
        QString curLine = gtfFileStream.readLine();
        if (curLine[0] == '#')
        {   // comment line begin with a symbol '#'
            continue;   // Read next line.
        }
        QStringList curLineInfoList = curLine.split('\t').last().split(';');

        QRegExp proteinID_regExp("protein_id \"(.*)\"");
        QRegExp transID_regExp("transcript_id \"(.*)\"");

        // Identify the attribute lookup situation and temporarily store the ID
        int proteinIDPos = -1, transcriptIDPos = -1;
        QString proteinID, transID;

        for (auto item : curLineInfoList)
        {   // curLineInfoList: Each element has an attribute
            // 	gene_id "ENSSSAG00000000039"
            //  ...
            //  transcript_id "ENSSSAT00000000039";
            //  ...
            //  protein_id "ENSSSAP00000000015";
            //  ...
            if (proteinIDPos >= 0 && transcriptIDPos >= 0)
            {   // transcript_id and protein_id has been found.
                break;
            }

            if (transcriptIDPos < 0)
            {
                transcriptIDPos = transID_regExp.indexIn(item);
                transID = transID_regExp.cap(1);
            }
            if (proteinIDPos < 0)
            {
                proteinIDPos = proteinID_regExp.indexIn(item);
                proteinID = proteinID_regExp.cap(1);
            }
        }
        if (proteinIDPos >= 0 && transcriptIDPos >= 0)
        {   // Both have transcript_id and protein_id.
            proteinIDtransIDMap.insert(proteinID, transID);
        }
        // Clear flag.
        proteinIDPos = -1, transcriptIDPos = -1;
        transID.clear();
        proteinID.clear();
    }

    return proteinIDtransIDMap;
}

/**
 * @brief FuncAnnotator::getProteinIDandTransIDFromGff
 * @param gffFilePath
 *          The last column contains multiple attribute, seperate by ';',
 *        each of the attribute is the form of (key="value").
 *          protein_id=ID, spell of "protein_id" need to to be same.
 * @return  proteinIDtransIDMap: key proteinID, value transcriptID.
 */
QMap<QString, QString> FuncAnnotator::getProteinIDandTransIDFromGff(QString gffFilePath)
{
    QMap<QString, QString> proteinIDtransIDMap;
    if (gffFilePath.isNull())
    {
        return proteinIDtransIDMap;
    }

    QFile gffFile(gffFilePath);
    if (!gffFile.open(QIODevice::ReadOnly))
    {
        qDebug() << gffFile.errorString();
        return proteinIDtransIDMap;
    }
    QTextStream gffFileStream(&gffFile);

    while (!gffFileStream.atEnd())
    {
        QString curLine = gffFileStream.readLine();
        if (curLine[0] == '#')
        {   // comment line begin with a symbol '#'
            continue;   // Read next line.
        }
        QStringList curLineInfoList = curLine.split('\t').last().split(';');

        QRegExp proteinID_regExp("protein_id=(.*)");
        QRegExp transID_regExp("transcript_id=(.*)");

        // Identify the attribute lookup situation and temporarily store the ID
        int proteinIDPos = -1, transcriptIDPos = -1;
        QString proteinID, transID;

        for (auto item : curLineInfoList)
        {   // curLineInfoList: Each element has an attribute
            // 	gene_id "ENSSSAG00000000039"
            //  ...
            //  transcript_id "ENSSSAT00000000039";
            //  ...
            //  protein_id "ENSSSAP00000000015";
            //  ...
            if (proteinIDPos >= 0 && transcriptIDPos >= 0)
            {   // transcript_id and protein_id has been found.
                break;
            }

            if (transcriptIDPos < 0)
            {
                transcriptIDPos = transID_regExp.indexIn(item);
                transID = transID_regExp.cap(1);
            }
            if (proteinIDPos < 0)
            {
                proteinIDPos = proteinID_regExp.indexIn(item);
                proteinID = proteinID_regExp.cap(1);
            }
        }
        if (proteinIDPos >= 0 && transcriptIDPos >= 0)
        {   // Both have transcript_id and protein_id.
            proteinIDtransIDMap.insert(proteinID, transID);
        }
        // Clear flag.
        proteinIDPos = -1, transcriptIDPos = -1;
        transID.clear();
        proteinID.clear();
    }

    return proteinIDtransIDMap;
}
