#include "plink.h"

Plink::Plink()
{
    this->model << "Linear" << "Logistic";  //model: ["Linear", "Logistic"]
    this->paramlist.clear();  //清空参数列表
}

bool Plink::vcf2plink(QString vcfFile, QString out, QString maf, QString mind, QString geno)
{
    if (vcfFile.isNull() || out.isNull())
    {
        return false;
    }

    //plink --vcf vcfFile [--maf maf] [--mind mind] [--geno geno] --recode --out out

    this->paramlist.clear();            // Clear paramlist before set parameter.
    this->paramlist.append("--vcf");
    this->paramlist.append(vcfFile);

    if (!maf.isNull())
    {
        this->paramlist.append("--maf");
        this->paramlist.append(maf);
    }
    if (!mind.isNull())
    {
        this->paramlist.append("--mind");
        this->paramlist.append(mind);
    }
    if (!geno.isNull())
    {
        this->paramlist.append("--geno");
        this->paramlist.append(geno);
    }

    this->paramlist.append("--recode");
    this->paramlist.append("--out");
    this->paramlist.append(out);
    //this->paramlist.append("--noweb");

    return true;
}


bool Plink::vcf2binary(QString vcfFile, QString out, QString maf, QString mind, QString geno)
{
    if (vcfFile.isNull() || out.isNull())
    {
        return false;
    }

    // plink --vcf vcfFile [--maf maf] [--mind mind] [--geno geno] --make-bed --out out
    this->paramlist.clear();            // Clear paramlist before set parameter.
    this->paramlist.append("--vcf");
    this->paramlist.append(vcfFile);

    if (!maf.isNull())
    {
        this->paramlist.append("--maf");
        this->paramlist.append(maf);
    }
    if (!mind.isNull())
    {
        this->paramlist.append("--mind");
        this->paramlist.append(mind);
    }
    if (!geno.isNull())
    {
        this->paramlist.append("--geno");
        this->paramlist.append(geno);
    }

    this->paramlist.append("--make-bed");
    this->paramlist.append("--out");
    this->paramlist.append(out);

    return true;
}

bool Plink::vcf2transpose(QString vcfFile, QString out, QString maf, QString mind, QString geno)
{
    if (vcfFile.isNull() || out.isNull())
    {
        return false;
    }

    // plink --vcf [--maf maf] [--mind mind] [--geno geno] --recode12 --ouput-missing-genotype 0 --transpose --out out
    this->paramlist.clear();            // Clear paramlist before set parameter.
    this->paramlist.append("--vcf");
    this->paramlist.append(vcfFile);

    if (!maf.isNull())
    {
        this->paramlist.append("--maf");
        this->paramlist.append(maf);
    }
    if (!mind.isNull())
    {
        this->paramlist.append("--mind");
        this->paramlist.append(mind);
    }
    if (!geno.isNull())
    {
        this->paramlist.append("--geno");
        this->paramlist.append(geno);
    }

    this->paramlist.append("--recode12");
    this->paramlist.append("--output-missing-genotype");
    this->paramlist.append("0");
    this->paramlist.append("--transpose");
    this->paramlist.append("--out");
    this->paramlist.append(out);

    return true;
}

bool Plink::plink2transpose(QString pedFile, QString mapFile, QString out, QString maf, QString mind, QString geno)
{
    if (pedFile.isNull() || mapFile.isNull() || out.isNull())
    {
        return false;
    }

    // plink --ped pedFile --map mapFile [--maf maf] [--mind mind] [--geno geno] --recode12 --transpose --out out
    this->paramlist.clear();            // Clear paramlist before set parameter.
    this->paramlist.append("--ped");
    this->paramlist.append(pedFile);
    this->paramlist.append("--map");
    this->paramlist.append(mapFile);

    if (!maf.isNull())
    {
        this->paramlist.append("--maf");
        this->paramlist.append(maf);
    }
    if (!mind.isNull())
    {
        this->paramlist.append("--mind");
        this->paramlist.append(mind);
    }

    if (!geno.isNull())
    {
        this->paramlist.append("--geno");
        this->paramlist.append(geno);
    }

    this->paramlist.append("--recode12");
    this->paramlist.append("--output-missing-genotype");
    this->paramlist.append("0");
    this->paramlist.append("--transpose");
    this->paramlist.append("--out");
    this->paramlist.append(out);

    return true;
}

bool Plink::plink2binary(QString pedFile, QString mapFile, QString out, QString maf, QString mind, QString geno)
{
    if (pedFile.isNull() || mapFile.isNull() || out.isNull())
    {
        return false;
    }

    // plink --ped .ped --map .map [--maf maf] [--mind mind] [--geno geno] --make-bed --out out
    this->paramlist.clear();
    this->paramlist.append("--ped");
    this->paramlist.append(pedFile);
    this->paramlist.append("--map");
    this->paramlist.append(mapFile);

    if (!maf.isNull())
    {
        this->paramlist.append("--maf");
        this->paramlist.append(maf);
    }
    if (!mind.isNull())
    {
        this->paramlist.append("--mind");
        this->paramlist.append(mind);
    }
    if (!geno.isNull())
    {
        this->paramlist.append("--geno");
        this->paramlist.append(geno);
    }

    this->paramlist.append("--make-bed");
    this->paramlist.append("--out");
    this->paramlist.append(out);
    //this->paramlist.append("--noweb");

    return true;
}

bool Plink::transpose2binary(QString tpedFile, QString tfamFile, QString out, QString maf, QString mind, QString geno)
{
    if (tpedFile.isNull() || tfamFile.isNull() || out.isNull())
    {
        return false;
    }

    // plink --tped .tped --tfam .tfam [--maf maf] [--mind mind] [--geno geno] --make-bed --out out
    this->paramlist.clear();
    this->paramlist.append("--tped");
    this->paramlist.append(tpedFile);
    this->paramlist.append("--tfam");
    this->paramlist.append(tfamFile);

    if (!maf.isNull())
    {
        this->paramlist.append("--maf");
        this->paramlist.append(maf);
    }
    if (!mind.isNull())
    {
        this->paramlist.append("--mind");
        this->paramlist.append(mind);
    }
    if (!geno.isNull())
    {
        this->paramlist.append("--geno");
        this->paramlist.append(geno);
    }

    this->paramlist.append("--make-bed");
    this->paramlist.append("--out");
    this->paramlist.append(out);
    //this->paramlist.append("--noweb");

    return true;
}

bool Plink::transpose2plink(QString tpedFile, QString tfamFile, QString out, QString maf, QString mind, QString geno)
{
    if (tpedFile.isNull() || tfamFile.isNull() || out.isNull())
    {
        return false;
    }

    // plink --tped .tped --tfam .tfam [--maf maf] [--mind mind] [--geno geno] --recode --out out
    this->paramlist.clear();
    this->paramlist.append("--tped");
    this->paramlist.append(tpedFile);
    this->paramlist.append("--tfam");
    this->paramlist.append(tfamFile);

    if (!maf.isNull())
    {
        this->paramlist.append("--maf");
        this->paramlist.append(maf);
    }
    if (!mind.isNull())
    {
        this->paramlist.append("--mind");
        this->paramlist.append(mind);
    }
    if (!geno.isNull())
    {
        this->paramlist.append("--geno");
        this->paramlist.append(geno);
    }

    this->paramlist.append("--recode");
    this->paramlist.append("--out");
    this->paramlist.append(out);

    return true;
}

bool Plink::binary2transpose(QString binaryFile, QString out, QString maf, QString mind, QString geno)
{
    if (binaryFile.isNull() || out.isNull())
    {
        return false;
    }

    QFile bedFile(binaryFile+".bed");
    QFile bimFile(binaryFile+".bim");
    QFile famFile(binaryFile+".fam");

    if (!bedFile.exists() || !bimFile.exists() || !famFile.exists())
    {
        return false;
    }

    // plink --bfile binaryFile [--maf maf] [--geno ms] --recode12 --transpose --out out
    this->paramlist.clear();            // Clear paramlist before set parameter.
    this->paramlist.append("--bfile");
    this->paramlist.append(binaryFile);

    if (!maf.isNull())
    {
        this->paramlist.append("--maf");
        this->paramlist.append(maf);
    }
    if (!mind.isNull())
    {
        this->paramlist.append("--mind");
        this->paramlist.append(mind);
    }
    if (!geno.isNull())
    {
        this->paramlist.append("--geno");
        this->paramlist.append(geno);
    }

    this->paramlist.append("--recode12");
    this->paramlist.append("--output-missing-genotype");
    this->paramlist.append("0");
    this->paramlist.append("--transpose");
    this->paramlist.append("--out");
    this->paramlist.append(out);

    return true;
}

bool Plink::binary2plink(QString binaryFile, QString out, QString maf, QString mind, QString geno)
{
    if (binaryFile.isNull() || out.isNull())
    {
        return false;
    }

    QFile bedFile(binaryFile+".bed");
    QFile bimFile(binaryFile+".bim");
    QFile famFile(binaryFile+".fam");

    if (!bedFile.exists() || !bimFile.exists() || !famFile.exists())
    {
        return false;
    }

    // plink --bfile binaryFile [--maf maf] [--geno ms] --recode --out out
    this->paramlist.clear();            // Clear paramlist before set parameter.
    this->paramlist.append("--bfile");
    this->paramlist.append(binaryFile);

    if (!maf.isNull())
    {
        this->paramlist.append("--maf");
        this->paramlist.append(maf);
    }
    if (!mind.isNull())
    {
        this->paramlist.append("--mind");
        this->paramlist.append(mind);
    }
    if (!geno.isNull())
    {
        this->paramlist.append("--geno");
        this->paramlist.append(geno);
    }

    this->paramlist.append("--recode");
    this->paramlist.append("--out");
    this->paramlist.append(out);

    return true;
}

bool Plink::runGWAS(QString phenotype, QString genotype, QString map, QString covariate, QString kinship,
                    QString model, QString maf, QString mind, QString geno,QString out)
{
    if (phenotype.isNull() || genotype.isNull() || model.isNull())
    {
        if (phenotype.isNull())
        {
            return false;
        }
        if (genotype.isNull())
        {
            return false;
        }
        return false;
    }

    // Genotype file info.
    QFileInfo genoFileInfo = QFileInfo(genotype);
    QString genoFileName = genoFileInfo.fileName();         // demo.vcf.gz
    QString genoFileBaseName = genoFileInfo.baseName();     // geno
    QString genoFileSuffix = genoFileInfo.suffix();         //
    QString genoFileAbPath = genoFileInfo.absolutePath();

    this->paramlist.clear();            // Clear paramlist before set parameter.
    if (isVcfFile(genotype)) //判断命令行里输入的 -g 后面的参数是不是vcf文件 如可以-g /home/hapmap.bed也可以-g /home/hapmap.vcf
    {                         //若是vcf文件 需要进行处理
        this->paramlist.append("--vcf"); //这里往参数列表里加的"--vcf"命令正是底层plink工具用来处理vcf文件的命令
        this->paramlist.append(genotype);
    }
    if (genotype.split(".")[genotype.split(".").length()-1] == "ped")//看下输入的-g后面的基因参数是不是ped文件
    {
        if (map.isNull())      //这里允许ped和map文件处于不同文件路径，但要是用户的输入参数里没有map文件，
                                //就自动创建map文件参数，其路径默认为和ped文件相同，所以如果此时ped文件夹下确实没有map文件的话，软件就会报错
        {                       //基因型文件必须要同时有ped和map文件
            map = genoFileAbPath+"/"+genoFileBaseName+".map";
        }
        this->paramlist.append("--ped");
        this->paramlist.append(genotype);
        this->paramlist.append("--map");
        this->paramlist.append(map);
    }

    if (genotype.split(".")[genotype.split(".").length()-1] == "tped")
    {
        if (map.isNull())
        {
            map = genoFileAbPath+"/"+genoFileBaseName+".tfam"; //同上一样的操作
        }
        this->paramlist.append("--tped");
        this->paramlist.append(genotype);
        this->paramlist.append("--tfam");
        this->paramlist.append(map);
    }

    if (genotype.split(".")[genotype.split(".").length()-1] == "bed")//检测输入的基因型是否是bed文件
    {
        this->paramlist.append("--bfile");  //--bfile为plink工具处理二进制文件的命令
        this->paramlist.append(genoFileAbPath+"/"+genoFileBaseName);//plink处理二进制文件不需要在文件名上加后缀
    }                                                              //如 plink --bfile hapmap ·······

    this->paramlist.append("--pheno");
    this->paramlist.append(phenotype);

    if (!covariate.isNull())       //协变量文件可有可无，若有就加入参数列表命令 若没有就算了
    {
        this->paramlist.append("--covar");
        this->paramlist.append(covariate);
    }
    if (!kinship.isNull())
    {   //

    }

    if (!maf.isNull())
    {
        this->paramlist.append("--maf");
        this->paramlist.append(maf);
    }
    if (!mind.isNull())
    {
        this->paramlist.append("--mind");
        this->paramlist.append(mind);
    }
    if (!geno.isNull())
    {
        this->paramlist.append("--geno");
    }

    this->paramlist.append("--"+model.toLower());
//    if (model == "Linear")
//    {
//        this->paramlist.append("--linear");
//    }
//    if (model == "Logistic")
//    {
//        this->paramlist.append("--logistic");
//    }

    this->paramlist.append("--allow-no-sex");
    this->paramlist.append("--out");
    this->paramlist.append(out);

    return true;
}

void Plink::filterVcfFile(QString genotype, QString maf, QString mind, QString geno, QString out)
{
    this->paramlist.clear();
    this->paramlist.append("--vcf");
    this->paramlist.append(genotype);

    if (!maf.isNull())
    {
        this->paramlist.append("--maf");
        this->paramlist.append(maf);
    }
    if (!mind.isNull())
    {
        this->paramlist.append("--mind");
        this->paramlist.append(mind);
    }
    if (!geno.isNull())
    {
        this->paramlist.append("--geno");
        this->paramlist.append(geno);
    }

    this->paramlist.append("--recode");
    this->paramlist.append("vcf");
    this->paramlist.append("--out");
    this->paramlist.append(out);
}

void Plink::filterPlinkFile(QString genotype, QString map, QString maf, QString mind, QString geno, QString out)
{
    this->paramlist.clear();
    this->paramlist.append("--ped");
    this->paramlist.append(genotype);
    this->paramlist.append("--map");
    this->paramlist.append(map);

    if (!maf.isNull())
    {
        this->paramlist.append("--maf");
        this->paramlist.append(maf);
    }
    if (!mind.isNull())
    {
        this->paramlist.append("--mind");
        this->paramlist.append(mind);
    }
    if (!geno.isNull())
    {
        this->paramlist.append("--geno");
        this->paramlist.append(geno);
    }

    this->paramlist.append("--recode");
    this->paramlist.append("--out");
    this->paramlist.append(out);
}

void Plink::filterBinaryFile(QString genotype, QString maf, QString mind, QString geno, QString out)
{
    this->paramlist.clear();
    this->paramlist.append("--bfile");
    this->paramlist.append(genotype);

    if (!maf.isNull())
    {
        this->paramlist.append("--maf");
        this->paramlist.append(maf);
    }
    if (!mind.isNull())
    {
        this->paramlist.append("--mind");
        this->paramlist.append(mind);
    }
    if (!geno.isNull())
    {
        this->paramlist.append("--geno");
        this->paramlist.append(geno);
    }

    this->paramlist.append("--make-bed");
    this->paramlist.append("--out");
    this->paramlist.append(out);
}

void Plink::filterTransposeFile(QString genotype, QString map, QString maf, QString mind, QString geno, QString out)
{
    this->paramlist.clear();
    this->paramlist.append("--tped");
    this->paramlist.append(genotype);
    this->paramlist.append("--tfam");
    this->paramlist.append(map);

    if (!maf.isNull())
    {
        this->paramlist.append("--maf");
        this->paramlist.append(maf);
    }
    if (!mind.isNull())
    {
        this->paramlist.append("--mind");
        this->paramlist.append(mind);
    }
    if (!geno.isNull())
    {
        this->paramlist.append("--geno");
        this->paramlist.append(geno);
    }

    this->paramlist.append("--recode12");
    this->paramlist.append("--output-missing-genotype");
    this->paramlist.append("0");
    this->paramlist.append("--transpose");
    this->paramlist.append("--out");
    this->paramlist.append(out);
}

void Plink::filterData(QString genotype, QString map, QString maf, QString mind, QString geno, QString out)
{
    if (genotype.isNull() || map.isNull())
    {
        return;
    }

    QFileInfo genoFileInfo(geno);
    QString genoFileBaseName = genoFileInfo.baseName();
    QString genoFileAbPath = genoFileInfo.absolutePath();
    QString genoFileSuffix = genoFileInfo.suffix();

    if (this->isVcfFile(genotype))
    {
        this->filterVcfFile(genotype, maf, mind, geno, out);
    }

    if (genoFileSuffix == "ped")
    {
        this->filterPlinkFile(genotype, map, maf, mind, geno, out);
    }

    if (genoFileSuffix == "tped")
    {
        this->filterPlinkFile(genotype, map, maf, mind, geno, out);
    }

    if (genoFileSuffix == "bed")
    {
        this->filterBinaryFile(genoFileAbPath+"/"+genoFileBaseName, maf, mind, geno, out);    this->paramlist.append("--recode12");
        this->paramlist.append("--output-missing-genotype");
        this->paramlist.append("0");
        this->paramlist.append("--transpose");
        this->paramlist.append("--out");
        this->paramlist.append(out);
    }
}

bool Plink::isVcfFile(QString file) // Just consider filename.
{
    if (file.isNull() || file.isEmpty())
    {
        return false;
    }

    QFileInfo fileInfo = QFileInfo(file);
    QStringList fileNameList = fileInfo.fileName().split(".");   // 把文件名用. 分开 存到列表中
                                               //如“/tmp/archive.tar.gz” filename为：archive.tar.gz
    for (QString item : fileNameList)           //则分开后列表中就是 'archive' 'tar' 'gz'
    {                               //在列表中循环查找 如果有vcf这串字 就说明是vcf文件（即找到后缀为vcf，当然你文件名字里不要用vcf命名）
        if (item == "vcf")
        {
            return true;
        }
    }

    return false;
}

void Plink::splitVcfFile(QString vcfFile, QString keepFile, QString out)
{
    if (vcfFile.isNull() ||  keepFile.isNull())
    {
        return;
    }
    this->paramlist.clear();
    this->paramlist.append("--vcf");
    this->paramlist.append(vcfFile);
    this->paramlist.append("--keep");
    this->paramlist.append(keepFile);
    this->paramlist.append("--recode");
    this->paramlist.append("--out");
    this->paramlist.append(out);
}

void Plink::splitPlinkFile(QString ped, QString map, QString keepFile, QString out)
{
    if (ped.isNull() || map.isNull() ||  keepFile.isNull())
    {
        return;
    }
    this->paramlist.clear();
    this->paramlist.append("--ped");
    this->paramlist.append(ped);
    this->paramlist.append("--map");
    this->paramlist.append(map);
    this->paramlist.append("--keep");
    this->paramlist.append(keepFile);
    this->paramlist.append("--recode");
    this->paramlist.append("--out");
    this->paramlist.append(out);
}

void Plink::splitBinaryFile(QString binaryFile, QString keepFile, QString out)
{
    if (binaryFile.isNull() ||  keepFile.isNull())
    {
        return;
    }

    QFile file;
    if (!file.exists(binaryFile+".bed") || !file.exists(binaryFile+".bim") || !file.exists(binaryFile+".fam"))
    {
        return;
    }

    this->paramlist.clear();
    this->paramlist.append("--bfile");
    this->paramlist.append(binaryFile);
    this->paramlist.append("--keep");
    this->paramlist.append(keepFile);
    this->paramlist.append("--recode");
    this->paramlist.append("--out");
    this->paramlist.append(out);
}

void Plink::splitTransposeFile(QString tped, QString tfam, QString keepFile, QString out)
{
    if (tped.isNull() || tfam.isNull() ||  keepFile.isNull())
    {
        return;
    }
    this->paramlist.clear();
    this->paramlist.append("--tped");
    this->paramlist.append(tped);
    this->paramlist.append("--tfam");
    this->paramlist.append(tfam);
    this->paramlist.append("--keep");
    this->paramlist.append(keepFile);
    this->paramlist.append("--recode");
    this->paramlist.append("--out");
    this->paramlist.append(out);
}
