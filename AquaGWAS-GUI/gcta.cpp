#include "gcta.h"

Gcta::Gcta()
{

}

/**
 * @brief Gcta::makeGRM     Set practical parameters of GCTA to make .grm file.
 * @param binGenoBaseName   binary genotype file base name(without suffix)
 * @param out               out file path
 * @return
 */
bool Gcta::makeGRM(QString binGenoBaseName, QString out)
{
    if (binGenoBaseName.isNull() || out.isNull())
    {
        return false;
    }
    this->paramlist.clear();

    // gcta64 --bfile 222_filter1_rec12 --make-grm --out 222_filter1_rec12
    this->paramlist.append("--bfile");
    this->paramlist.append(binGenoBaseName);
    this->paramlist.append("--make-grm");
    this->paramlist.append("--out");
    this->paramlist.append(out);

    return true;
}

bool Gcta::runPCA(QString grmBaseName, int PCs, int threads, QString out)
{
    if (grmBaseName.isNull() || out.isNull() || PCs < 1 || threads < 1)
    {
        return false;
    }
    // gcta64 --grm 222_filter1_rec12 --pca 4 --out 222_filter1_rec12 --threads 56;
    this->paramlist.clear();

    this->paramlist.append("--grm");
    this->paramlist.append(grmBaseName);
    this->paramlist.append("--pca");
    this->paramlist.append(QString::number(PCs));
    this->paramlist.append("--out");
    this->paramlist.append(out);
    this->paramlist.append("--threads");
    this->paramlist.append(QString::number(threads));

    return true;
}
