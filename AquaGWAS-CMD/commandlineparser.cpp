#include "commandlineparser.h"

CommandLineParser::CommandLineParser() //构造函数的定义
{
    this->proc = new QProcess;  //proc为对象的一个成员指针 类型为 QProcess *
    this->initCmdLineOption();  //调用类的成员函数将对象初始化
    addFilesExecutePermission(this->toolpath);
    addFilesExecutePermission(this->scriptpath+"annovar/");
    addFilesExecutePermission(this->scriptpath+"poplddecay/");
}

CommandLineParser::~CommandLineParser() //析构函数的定义
{
    delete proc;
}
void CommandLineParser::addFilesExecutePermission(QString directory)
{
    QDir dir(directory);
    QStringList fileList = dir.entryList();
    for (auto item:fileList)
    {
        QProcess::execute("chmod", QStringList()<< "+x" << directory+item);
    }
}
void CommandLineParser::initCmdLineOption()
{
    this->addHelpOption();       //this实际上是那个对象的常量指针
    this->addVersionOption();   //这里实际上就是调用 继承自Qcommandlineparser类的两个成员函数
    this->assocCmdOp = new QCommandLineOption(QStringList()  //这里是初始化一个qcommandlineoption类的对象把括号里的参数传给它的构造函数
                            << "A" << "assoc", "Run association analysis");//括号里参数的顺序按照name，description，valueName，defaultValue的顺序
    this->pcaCmdOp = new QCommandLineOption(QStringList()
                               << "pca" << "pca", "Run component analysis");
    this->gwBase =new QCommandLineOption(QStringList()
                          << "gwBase" , "set Genomewideline base(Manhattan plot)","gwBase","5");
    this->gwExpo =new QCommandLineOption(QStringList()
                          << "gwExpo" , "set Genomewideline Expo(Manhattan plot)","gwExpo","-8");
    this->sgBase =new QCommandLineOption(QStringList()
                          << "sgBase" , "set Suggestiveline Expo(Manhattan plot)","sgBase","1");
    this->sgExpo =new QCommandLineOption(QStringList()
                          << "sgExpo" , "set Genomewideline Expo(Manhattan plot)","sgExpo","-5");
    this->thBase =new QCommandLineOption(QStringList()
                          << "thBase" , "set thresholdBase(anno)","thBase","1");
    this->thExpo =new QCommandLineOption(QStringList()
                          << "thExpo" , "set thresholdExpo(anno)","thExpo","-5");
    this->snp_pos =new QCommandLineOption(QStringList()
                          << "snp_pos" , "set snpPosFilepath(anno)","snp_pos");
    this->funcAnnoRef =new QCommandLineOption(QStringList()
                          << "funcAnnoRef" , "set funcAnnoRefFilepath(anno)","funcAnnoRef");
    this->var =new QCommandLineOption(QStringList()
                          << "var" , "set varFuncFilePath(anno)","var");
    this->exvar =new QCommandLineOption(QStringList()
                          << "exvar" , "set exVarFuncFilePath(anno)","exvar");

    this->funcAnnoCmdOp=new QCommandLineOption(QStringList()
                          << "funcAnno" , "Run functional Annotation(anno)");
    this->stepAnnoCmdOp= new QCommandLineOption(QStringList()
                    << "step" , "Run annostep(anno)");
    this->pvalFileCmdOp= new QCommandLineOption(QStringList()
                           << "pvalue" , "inputPvalueFile(anno)","pvalueFile");
    this->strucAnnoCmdOp = new QCommandLineOption(QStringList()
                               << "strucAnno" << "strucAnno(anno)", "Run structural Annotation");
    this->gffFileCmdOp = new QCommandLineOption(QStringList()
                               << "gff" << "gffFile", "gffFile(anno)", "gffFile", "");
    this->fastaFileCmdOp = new QCommandLineOption(QStringList()
                               << "fas" << "fastaFile", "fastaFile(anno)", "fastaFile", "");
    this->avinputCmdOp = new QCommandLineOption(QStringList()
                               << "avin"<< "avinput", "avinput(anno)", "avinput", "");

    this->pcaCmdOp = new QCommandLineOption(QStringList()
                               << "pca" << "pca", "Run component analysis");
    this->gcta_PCsCmdOp =  new QCommandLineOption(QStringList()
                                << "PCs", "PCs(gcta)", "PCs", "");
    this->gcta_threadsCmdOp =  new QCommandLineOption(QStringList()
                                << "threads", "threads(gcta)", "threads", "1");

    this->toolCmdOp = new QCommandLineOption(QStringList()
                            << "T" << "tool", "Set association tool", "toolSelector", "");
    this->modelCmdOp = new QCommandLineOption(QStringList()//这个命令必须设置,比如plink有linear和logistic,gemma有lmm和bslmm,emmax只有EMMA但也要设置
                            << "M" << "model", "Set association model", "modleSelector", "");
    this->outCmdOp = new QCommandLineOption(QStringList()
                            << "o" << "out", "Out path with file prefix", "outPath", "");
    this->phenoFileCmdOp = new QCommandLineOption(QStringList()
                            << "p" << "phenofile" , "Phenotype file", "phenoFile", "");
    this->genoFileCmdOp = new QCommandLineOption(QStringList()
                            << "g" << "genofile", "Genotype file", "gneoFile", "");
    this->mapFileCmdOp = new QCommandLineOption(QStringList()
                            << "m" << "mapfile", "Map file(same path as genotype file default)", "mapFile", "");
    this->covarFileCmdOp = new QCommandLineOption(QStringList()
                            << "c" << "covarfile", "Coavariate file", "covarFile", "");
    this->kinFileCmdOp = new QCommandLineOption(QStringList()
                            << "k" << "kinfile", "Kinship file", "kinFile", "");
    this->mafCmdOp = new QCommandLineOption(QStringList()
                            << "maf", "Minor allele frequency(MAF))", "maf", "");
    this->genoCmdOp = new QCommandLineOption(QStringList()
                            << "geno", "Missingness of SNPs", "geno", "");
    this->mindCmdOp = new QCommandLineOption(QStringList()
                            << "mind", "Missingness of individual", "mind", "");
    this->gemma_makeKinCmdOp = new QCommandLineOption(QStringList()
                            << "makekin_gemma", "是否要生成亲缘矩阵1为是0为否 默认为1Make kinship auto(gemma)", "makekin", "1");
    this->gemma_kinMatCmdOp = new QCommandLineOption(QStringList()
                            << "kinmatrix_gemma", "Kinship file type:1.centered relatedness matrix 2.standardized relatedness matrix默认为1(gemma)", "kinmatrix", "1");
    this->gemma_lmmTestCmdOp = new QCommandLineOption(QStringList()
                            << "lmmtest", "choose LMM Test:1-4(gemma)", "lmmtest", "");
    this->gemma_bslmmModelCmdOp = new QCommandLineOption(QStringList()
                            << "bslmmmodel", "choose BSLMM model:1-3(gemma)", "bslmmmodel", "");
    this->gemma_famCompleCmdOp =  new QCommandLineOption(QStringList()
                            << "famcomple", "是否要改造fam文件1为是0为否 默认为1 (gemma)", "famcomplete", "1"); //以上这部分主要是创建命令
                                                                                             //类似于QCommandLineOption op1("a")
    this->LD_CmdOp = new QCommandLineOption(QStringList()
                       << "L" << "LD", "run LD");    //调用方式 -L或--LD 无参数
    this->LD_analysisCmdOp = new QCommandLineOption(QStringList()
                          << "analysis" , "choose family(yes) or single(no) ","analysis_model",""); //调用方式--analysis yes/no 有参数
    this->ProjectNameCmdOp = new QCommandLineOption(QStringList()
                 << "name" , "set project name","ProjectName",""); //调用方式 --name pro1
    this->LD_plot = new QCommandLineOption(QStringList()
                 << "LDplot" , "LD分析完是否要画图","LD_plot",""); //调用方式 --LDplot yes/no
    // linrenhao
    this->emmax_makeKinCmdOp = new QCommandLineOption(QStringList()
                            <<  "makekin_emmax","Make kinship auto(emmax)", "makekin", "1");
    this->emmax_kinMatCmdOp = new QCommandLineOption(QStringList()
                            << "kinmatrix_emmax", "Kinship file type(emmax)", "kinmatrix", "BN");

    this->addOption(*assocCmdOp);
    this->addOption(*toolCmdOp);
    this->addOption(*modelCmdOp);       //这部分主要是添加选项命令到命令行中
    this->addOption(*outCmdOp);         //类似于QCommandLineParser parser; parser.addOption(op1);
    this->addOption(*phenoFileCmdOp);
    this->addOption(*genoFileCmdOp);
    this->addOption(*mapFileCmdOp);
    this->addOption(*covarFileCmdOp);
    this->addOption(*kinFileCmdOp);
    this->addOption(*mafCmdOp);
    this->addOption(*genoCmdOp);
    this->addOption(*mindCmdOp);
    this->addOption(*gemma_makeKinCmdOp);
    this->addOption(*gemma_kinMatCmdOp);
    this->addOption(*gemma_lmmTestCmdOp);
    this->addOption(*gemma_bslmmModelCmdOp);
    this->addOption(*gemma_famCompleCmdOp);
    this->addOption(*LD_CmdOp);
    this->addOption(*LD_analysisCmdOp);
    this->addOption(*ProjectNameCmdOp);
    this->addOption(*LD_plot);
    this->addOption(*emmax_makeKinCmdOp);
    this->addOption(*emmax_kinMatCmdOp);
    this->addOption(*gcta_PCsCmdOp);
   this->addOption(*gcta_threadsCmdOp );
   this->addOption(*strucAnnoCmdOp);
    this->addOption(*gffFileCmdOp);
   this->addOption(*fastaFileCmdOp);
   this->addOption(*avinputCmdOp);
    this->addOption(*pcaCmdOp);
    this->addOption(*funcAnnoCmdOp);
    this->addOption(*stepAnnoCmdOp);
    this->addOption(*pvalFileCmdOp);
    this->addOption(*thBase);
    this->addOption(*thExpo);
    this->addOption(*snp_pos);
    this->addOption(*funcAnnoRef);
    this->addOption(*var);
    this->addOption(*exvar);

    this->addOption(*gwBase);
    this->addOption(*gwExpo);
    this->addOption(*sgBase);
    this->addOption(*sgExpo);

}

void CommandLineParser::delCmdLineOption()
{
    delete assocCmdOp;
    delete toolCmdOp;
    delete modelCmdOp;
    delete outCmdOp;
    delete phenoFileCmdOp;
    delete genoFileCmdOp;
    delete mapFileCmdOp;
    delete covarFileCmdOp;
    delete kinFileCmdOp;
    delete mafCmdOp;
    delete genoCmdOp;
    delete mindCmdOp;
    delete gemma_makeKinCmdOp;
    delete gemma_kinMatCmdOp;
    delete gemma_lmmTestCmdOp;
    delete gemma_bslmmModelCmdOp;
    delete gemma_famCompleCmdOp;
    delete LD_CmdOp;
    delete LD_analysisCmdOp;
    delete ProjectNameCmdOp;
    delete LD_plot;
    //linrenhao
    delete emmax_kinMatCmdOp;
    delete emmax_makeKinCmdOp;
    //zizi
    delete gcta_PCsCmdOp;
    delete gcta_threadsCmdOp;
    delete strucAnnoCmdOp;
    delete pcaCmdOp;
    delete gffFileCmdOp;
    delete fastaFileCmdOp;
    delete avinputCmdOp;

     delete funcAnnoCmdOp;
     delete stepAnnoCmdOp;
    delete pvalFileCmdOp;

    delete thBase;
    delete thExpo;
    delete snp_pos;
    delete funcAnnoRef;
    delete var;
    delete exvar;
    delete gwBase;
    delete gwExpo;
    delete sgBase;
    delete gwExpo;
}

QHash<QString, QString> CommandLineParser::getArgsFromCmd()
{
    QHash<QString, QString> argHash;

    if (this->isSet(*assocCmdOp))  //检测用户是否设置了关联命令 若是则为true 执行if语句
    {   // Association analysis arguments.
        argHash["assoc"] = nullptr;

        // Necessary to set.
        if (!this->isSet(*toolCmdOp) || this->value(*toolCmdOp). isNull())//如果用户没有设置工具命令
        {                                                                  //或者设置了工具命令但没给参数指定用哪个工具
            throw invalid_argument("Invalid argument -- tool");            //就抛出异常
        }
        else
        {
            argHash["tool"] = this->value(*toolCmdOp);  //若设置正常，则取出用户选择的工具（实际上是个字符串）：plink or gemma or exmma······
        }

        if (!this->isSet(*modelCmdOp) || this->value(*modelCmdOp).isNull())
        {
            throw invalid_argument("Invalid argument -- model");
        }
        else
        { //获得模式,这个命令必须设置,比如plink有linear和logistic,gemma有lmm和bslmm,emmax只有EMMA
            argHash.insert("model", this->value(*modelCmdOp));
        }

        if (!this->isSet(*outCmdOp) || this->value(*outCmdOp).isNull())
        {
            throw invalid_argument("Invalid argument -- out");
        }
        else
        {
            argHash["out"] = this->value(*outCmdOp);
        }

        if (!this->isSet(*phenoFileCmdOp) || this->value(*phenoFileCmdOp).isNull())
        {
            throw invalid_argument("Invalid argument -- phenotype file");
        }
        else
        {
            argHash["phenoFile"] = this->value(*phenoFileCmdOp);
        }

        if (!this->isSet(*genoFileCmdOp) || this->value(*genoFileCmdOp).isNull())
        {
            throw invalid_argument("Invalid argument -- genotype file");
        }
        else
        {
            argHash["genoFile"] = this->value(*genoFileCmdOp);
        }
        if (!this->isSet(*ProjectNameCmdOp) || this->value(*ProjectNameCmdOp).isNull())
        {
            throw invalid_argument("Invalid argument --name");
        }
        else
        {
            argHash["ProjectName"] = this->value(*ProjectNameCmdOp);
        }
        // Allow to not set, but must have argument if set.
        if (this->isSet(*gwBase) && this->value(*gwBase).isNull())
        {
            throw invalid_argument("Invalid argument --gwBase");
        }
        else if (this->isSet(*gwBase) && !this->value(*gwBase).isNull())
        {
            argHash["gwBase"] = this->value(*gwBase);
        }
        if (this->isSet(*gwExpo) && this->value(*gwExpo).isNull())
        {
            throw invalid_argument("Invalid argument --gwExpo");
        }
        else if (this->isSet(*gwExpo) && !this->value(*gwExpo).isNull())
        {
            argHash["gwExpo"] = this->value(*gwExpo);
        }
        if (this->isSet(*sgBase) && this->value(*sgBase).isNull())
        {
            throw invalid_argument("Invalid argument --sgBase");
        }
        else if (this->isSet(*sgBase) && !this->value(*sgBase).isNull())
        {
            argHash["sgBase"] = this->value(*sgBase);
        }
        if (this->isSet(*sgExpo) && this->value(*sgExpo).isNull())
        {
            throw invalid_argument("Invalid argument --sgExpo");
        }
        else if (this->isSet(*sgExpo) && !this->value(*sgExpo).isNull())
        {
            argHash["sgExpo"] = this->value(*sgExpo);
        }

        if (this->isSet(*mapFileCmdOp) && this->value(*mapFileCmdOp).isNull()) //注意这里map和协变量文件的判断逻辑变了，因为这两个不是必须的，
        {                                                                      //即使用户没有设置这两个命令也不会抛出异常（map不是必须是因为可以去genotype相同目录下找，而不是文件不需要，covariate则是文件可以没有的非必须）
            throw invalid_argument("Invalid argument -- map file");
        }
        else if (this->isSet(*mapFileCmdOp) && !this->value(*mapFileCmdOp).isNull())
        {
            argHash["mapFile"] = this->value(*mapFileCmdOp);
        }

        if (this->isSet(*covarFileCmdOp) && this->value(*covarFileCmdOp).isNull())
        {
            throw invalid_argument("Invalid argument -- covariate file");
        }
        else if (this->isSet(*covarFileCmdOp) && !this->value(*covarFileCmdOp).isNull())
        {
            argHash["covarFile"] = this->value(*covarFileCmdOp);
        }

        if (this->isSet(*kinFileCmdOp) && this->value(*kinFileCmdOp).isNull())
        {
            throw invalid_argument("Invalid argument -- kinship file");
        }
        else if (this->isSet(*kinFileCmdOp) && !this->value(*kinFileCmdOp).isNull())
        {
            argHash["kinFile"] = this->value(*kinFileCmdOp);
        }

        if (this->isSet(*genoCmdOp) && this->value(*genoCmdOp).isNull())
        {
            throw invalid_argument("Invalid argument -- geno");
        }
        else if (this->isSet(*genoCmdOp) && !this->value(*genoCmdOp).isNull())
        {
            argHash["geno"] = this->value(*genoCmdOp);
        }

        if (this->isSet(*mindCmdOp) && this->value(*mindCmdOp).isNull())
        {
            throw invalid_argument("Invalid argument -- mind");
        }
        else if (this->isSet(*mindCmdOp) && !this->value(*mindCmdOp).isNull())
        {
            argHash["mind"] = this->value(*mindCmdOp);
        }

        if (this->isSet(*mafCmdOp) && this->value(*mafCmdOp).isNull())
        {
            throw invalid_argument("Invalid argument -- maf");
        }
        else if (this->isSet(*mafCmdOp) && !this->value(*mafCmdOp).isNull())
        {
            argHash["maf"] = this->value(*mafCmdOp);
        }

        // Gemma specific arguments.
        if (argHash["tool"] == "gemma")
        {
            if (this->value(*gemma_makeKinCmdOp)=='0'||this->value(*gemma_makeKinCmdOp)=='1') //不管用户有没有设置 不是0就是1
            {
                argHash["gemma_makekin"] = this->value(*gemma_makeKinCmdOp);//如果用户没有设置默认是1 有设置就按设置值来
            }
            else
            {
                throw invalid_argument("Invalid argument --makekin please input 0 or 1");
            }

            if (this->value(*gemma_kinMatCmdOp)=='1'||this->value(*gemma_kinMatCmdOp)=='2')//不管用户有没有设置不是1就是2
            {
                argHash["gemma_kinmat"] = this->value(*gemma_kinMatCmdOp);//获取亲缘矩阵类型,若用户没有设置就是默认为1,若用户设置了就用设置值                                                                   //因为这个选项是有默认值的所以不用判断有没有设置(当然后面完善的时候还是要判断下用户
            }
            else
            {
                throw invalid_argument("Invalid argument --kinmatrix please input 1 or 2");
            }

            if (this->isSet(*gemma_lmmTestCmdOp) && this->value(*gemma_lmmTestCmdOp).isNull())
            {
                throw invalid_argument("Invalid argument -- lmmtest");
            }
            else if (this->isSet(*gemma_lmmTestCmdOp) && !this->value(*gemma_lmmTestCmdOp).isNull())
            {
                argHash["lmmtest"] = this->value(*gemma_lmmTestCmdOp); //获取lmm的数字 lmm可选1-4
            }

            if (this->isSet(*gemma_bslmmModelCmdOp) && this->value(*gemma_bslmmModelCmdOp).isNull())
            {
                throw invalid_argument("Invalid argument -- bslmmmodel");
            }
            else if (this->isSet(*gemma_bslmmModelCmdOp) && !this->value(*gemma_bslmmModelCmdOp).isNull())
            {
                argHash["bslmmmodel"] = this->value(*gemma_bslmmModelCmdOp);//获取bslmm的数字 bslmm可选1-3
            }

            if (this->value(*gemma_famCompleCmdOp)=='0'||this->value(*gemma_famCompleCmdOp)=='1') //不管用户有没有设置 不是0就是1
            {
                argHash["famcomple"] = this->value(*gemma_famCompleCmdOp);//如果用户没有设置默认是1 有设置就按设置值来
            }
            else
            {
                throw invalid_argument("Invalid argument --famcomple please input 0 or 1");
            }
        }

        // Emmax specific arguments.
        if (argHash["tool"] == "emmax")                 //linrenhao
        {
            if (this->isSet(*emmax_makeKinCmdOp) && this->value(*emmax_makeKinCmdOp).isNull())
            {
                throw invalid_argument("Invalid argument -- emmax_makekin");
            }
            else if (this->isSet(*emmax_makeKinCmdOp) && !this->value(*emmax_makeKinCmdOp).isNull())
            {
                argHash["emmax_makekin"] = this->value(*emmax_makeKinCmdOp);
            }

            if (this->isSet(*emmax_kinMatCmdOp) && this->value(*emmax_kinMatCmdOp).isNull())
            {
                throw invalid_argument("Invalid argument -- emmax_kinmat");
            }
            else if (this->isSet(*emmax_kinMatCmdOp) && !this->value(*emmax_kinMatCmdOp).isNull())
            {
                argHash["emmax_kinmat"] = this->value(*emmax_kinMatCmdOp);
            }
        }

    }
    if (this->isSet(*LD_CmdOp))
    {
        argHash["LD_CmdOp"] = nullptr;//在哈希表中创建一个LD键值表明有LD这个命令存在
        //necessary to set 基因型 是否要分析  输出路径 检查这三个命令是否有设置以及有没有跟相应的参数
        if (!this->isSet(*LD_analysisCmdOp) || this->value(*LD_analysisCmdOp).isNull())
        {
            throw invalid_argument("Invalid argument --analysis");
        }
        else
        {
            argHash.insert("analysisCmdOp", this->value(*LD_analysisCmdOp));
        }
        if (!this->isSet(*outCmdOp) || this->value(*outCmdOp).isNull())
        {
            throw invalid_argument("Invalid argument --out");
        }
        else
        {
            argHash["out"] = this->value(*outCmdOp);
        }
        if (!this->isSet(*genoFileCmdOp) || this->value(*genoFileCmdOp).isNull())
        {
            throw invalid_argument("Invalid argument --genofile");
        }
        else
        {
            argHash["genoFile"] = this->value(*genoFileCmdOp);
        }

        if (!this->isSet(*ProjectNameCmdOp) || this->value(*ProjectNameCmdOp).isNull())
        {
            throw invalid_argument("Invalid argument --name");//必须要设置工程名
        }
        else
        {
            argHash["ProjectName"] = this->value(*ProjectNameCmdOp);
        }
        // Allow to not set, but must have argument if set.
        if (this->isSet(*mapFileCmdOp) && this->value(*mapFileCmdOp).isNull()) //注意这里map和协变量文件的判断逻辑变了，因为这两个不是必须的，
        {                                                                      //即使用户没有设置这两个命令也不会抛出异常（map不是必须是因为可以去genotype相同目录下找，而不是文件不需要，covariate则是文件可以没有的非必须）
            throw invalid_argument("Invalid argument -- map file");
        }
        else if (this->isSet(*mapFileCmdOp) && !this->value(*mapFileCmdOp).isNull())
        {
            argHash["mapFile"] = this->value(*mapFileCmdOp);
        }

        if (this->isSet(*LD_plot) && this->value(*LD_plot).isNull()) //注意这里map和协变量文件的判断逻辑变了，因为这两个不是必须的，
        {                                                                      //即使用户没有设置这两个命令也不会抛出异常（map不是必须是因为可以去genotype相同目录下找，而不是文件不需要，covariate则是文件可以没有的非必须）
            throw invalid_argument("Invalid argument --LDplot ");
        }
        else if (this->isSet(*LD_plot) && !this->value(*LD_plot).isNull())
        {
            argHash["LD_plot"] = this->value(*LD_plot);
        }
    }
    if (this->isSet(*pcaCmdOp))
      {
         argHash["pca"] = nullptr;
         // Necessary to set.


         if (!this->isSet(*ProjectNameCmdOp) || this->value(*ProjectNameCmdOp).isNull())
         {
             throw invalid_argument("Invalid argument --name");//必须要设置工程名
         }
         else
         {
             argHash["ProjectName"] = this->value(*ProjectNameCmdOp);
         }

         if (!this->isSet(*outCmdOp) || this->value(*outCmdOp).isNull())
         {
             throw invalid_argument("Invalid argument -- out");
         }
         else
         {
             argHash["out"] = this->value(*outCmdOp);
         }



         if (!this->isSet(*genoFileCmdOp) || this->value(*genoFileCmdOp).isNull())
         {
             throw invalid_argument("Invalid argument -- genotype file");
         }
         else
         {
             argHash["genoFile"] = this->value(*genoFileCmdOp);
         }

         if (this->isSet(*mapFileCmdOp) && this->value(*mapFileCmdOp).isNull())
         {
             throw invalid_argument("Invalid argument -- map file");
         }
         else if (this->isSet(*mapFileCmdOp) && !this->value(*mapFileCmdOp).isNull())
         {
             argHash["mapFile"] = this->value(*mapFileCmdOp);
         }

         if (this->isSet(*covarFileCmdOp) && this->value(*covarFileCmdOp).isNull())
         {
             throw invalid_argument("Invalid argument -- covariate file");
         }
         else if (this->isSet(*covarFileCmdOp) && !this->value(*covarFileCmdOp).isNull())
         {
             argHash["covarFile"] = this->value(*covarFileCmdOp);
         }


         // Allow to not set, but must have argument if set.
         if (this->isSet(*kinFileCmdOp) && this->value(*kinFileCmdOp).isNull())
         {
             throw invalid_argument("Invalid argument -- kinship file");
         }
         else if (this->isSet(*kinFileCmdOp) && !this->value(*kinFileCmdOp).isNull())
         {
             argHash["kinFile"] = this->value(*kinFileCmdOp);
         }

         if (this->isSet(*genoCmdOp) && this->value(*genoCmdOp).isNull())
         {
             throw invalid_argument("Invalid argument -- geno");
         }
         else if (this->isSet(*genoCmdOp) && !this->value(*genoCmdOp).isNull())
         {
             argHash["geno"] = this->value(*genoCmdOp);
         }

         if (this->isSet(*mindCmdOp) && this->value(*mindCmdOp).isNull())
         {
             throw invalid_argument("Invalid argument -- mind");
         }
         else if (this->isSet(*mindCmdOp) && !this->value(*mindCmdOp).isNull())
         {
             argHash["geno"] = this->value(*mindCmdOp);
         }

         if (this->isSet(*mafCmdOp) && this->value(*mafCmdOp).isNull())
         {
             throw invalid_argument("Invalid argument -- maf");
         }
         else if (this->isSet(*mafCmdOp) && !this->value(*mafCmdOp).isNull())
         {
             argHash["maf"] = this->value(*mafCmdOp);
         }


         if (this->isSet(*gcta_PCsCmdOp) && this->value(*gcta_PCsCmdOp).isNull())
         {
             throw invalid_argument("Invalid argument -- PCs");
         }
         else if (this->isSet(*gcta_PCsCmdOp) && !this->value(*gcta_PCsCmdOp).isNull())
         {
             argHash["PCs"] = this->value(*gcta_PCsCmdOp);
         }

         if (this->isSet(*gcta_threadsCmdOp) && this->value(*gcta_threadsCmdOp).isNull())
         {
             throw invalid_argument("Invalid argument -- threads");
         }
         else if (this->isSet(*gcta_threadsCmdOp) && !this->value(*gcta_threadsCmdOp).isNull())
         {
             argHash["threads"] = this->value(*gcta_threadsCmdOp);
         }
      }
    if (this->isSet(*stepAnnoCmdOp))
    {
        argHash["stepAnno"] = nullptr;

        if (!this->isSet(*pvalFileCmdOp) || this->value(*pvalFileCmdOp).isNull())
        {
            throw invalid_argument("Invalid argument -- pvalue");
        }
        else
        {
            argHash["pvalueFile"] = this->value(*pvalFileCmdOp);
        }
        if (!this->isSet(*thBase) || this->value(*thBase).isNull())
        {
            throw invalid_argument("Invalid argument --thBase");
        }
        else
        {
            argHash["thBase"] = this->value(*thBase);
        }
        if (!this->isSet(*thExpo) || this->value(*thExpo).isNull())
        {
            throw invalid_argument("Invalid argument --thExpo");
        }
        else
        {
            argHash["thExpo"] = this->value(*thExpo);
        }
        if (!this->isSet(*genoFileCmdOp) || this->value(*genoFileCmdOp).isNull())
        {
            throw invalid_argument("Invalid argument -g");
        }
        else
        {
            argHash["genoFile"] = this->value(*genoFileCmdOp);
        }
    }

    if (this->isSet(*strucAnnoCmdOp))
       {
           argHash["strucAnno"] = nullptr;
           if (this->isSet(*gffFileCmdOp) && this->value(*gffFileCmdOp).isNull())
           {
               throw invalid_argument("Invalid argument -- gffFile");
           }
           else if (this->isSet(*gffFileCmdOp) && !this->value(*gffFileCmdOp).isNull())
           {
               argHash["gffFile"] = this->value(*gffFileCmdOp);
           }

           if (this->isSet(*fastaFileCmdOp) && this->value(*fastaFileCmdOp).isNull())
           {
               throw invalid_argument("Invalid argument -- fastaFile");
           }
           else if (this->isSet(*fastaFileCmdOp) && !this->value(*fastaFileCmdOp).isNull())
           {
               argHash["fastaFile"] = this->value(*fastaFileCmdOp);
           }

           if (this->isSet(*avinputCmdOp) && this->value(*avinputCmdOp).isNull())
           {
               throw invalid_argument("Invalid argument -- avinput");
           }
           else if (this->isSet(*avinputCmdOp) && !this->value(*avinputCmdOp).isNull())
           {
               argHash["avinput"] = this->value(*avinputCmdOp);
           }
           if (!this->isSet(*outCmdOp) || this->value(*outCmdOp).isNull())
           {
               throw invalid_argument("Invalid argument -- out");
           }
           else
           {
               argHash["out"] = this->value(*outCmdOp);
           }

           if (!this->isSet(*ProjectNameCmdOp) || this->value(*ProjectNameCmdOp).isNull())
           {
               throw invalid_argument("Invalid argument --name");//必须要设置工程名
           }
           else
           {
               argHash["ProjectName"] = this->value(*ProjectNameCmdOp);
           }
       }

    if (this->isSet(*funcAnnoCmdOp))
    {
        argHash["funcAnno"] = nullptr;
        if (!this->isSet(*snp_pos) || this->value(*snp_pos).isNull())
        {
            throw invalid_argument("Invalid argument --snp_pos");
        }
        else
        {
            argHash["snpPosFilePath"] = this->value(*snp_pos);
        }
        if (!this->isSet(*funcAnnoRef) || this->value(*funcAnnoRef).isNull())
        {
            throw invalid_argument("Invalid argument --funcAnnoRef");
        }
        else
        {
            argHash["funcAnnoRefFilePath"] = this->value(*funcAnnoRef);
        }
        if (!this->isSet(*exvar) || this->value(*exvar).isNull())
        {
            throw invalid_argument("Invalid argument --exvar");
        }
        else
        {
            argHash["exVarFuncFilePath"] = this->value(*exvar);
        }
        if (!this->isSet(*var) || this->value(*var).isNull())
        {
            throw invalid_argument("Invalid argument --var");
        }
        else
        {
            argHash["varFuncFilePath"] = this->value(*var);
        }
        if (!this->isSet(*outCmdOp) || this->value(*outCmdOp).isNull())
        {
            throw invalid_argument("Invalid argument -- out");
        }
        else
        {
            argHash["out"] = this->value(*outCmdOp);
        }
        if (!this->isSet(*ProjectNameCmdOp) || this->value(*ProjectNameCmdOp).isNull())
        {
            throw invalid_argument("Invalid argument --name");
        }
        else
        {
            argHash["ProjectName"] = this->value(*ProjectNameCmdOp);
        }
    }

    return argHash;
}

void CommandLineParser::parseCommand()
{
    QHash<QString, QString> argHash = getArgsFromCmd();//从命令行中获取用户的命令并存储到arghash这个哈希表中

    if (argHash.find("assoc") != argHash.end()) //主要是判断哈希表能不能找到‘assoc’这个键值 若能则说明用户设置了关联命令，执行关联分析
    {                                           //find，在hash里面查找目标键值，返回的是迭代器（如果没接触过就理解为指向那个位置的指针），end也是返回一个迭代器，指向最后一个元素的后一个位置
        runGWAS(argHash);
    }
    if (argHash.find("pca") != argHash.end())
    {
        runPCA(argHash);
    }
    if (argHash.find("LD_CmdOp") != argHash.end())
    {
        on_ldRunPushButton_clicked(argHash);//虽然命令行没有点击,但这里命名跟界面里的函数统一
    }
    if (argHash.find("stepAnno") != argHash.end())
       {
           on_annoStepPushButton_clicked(argHash);
       }

    if (argHash.find("strucAnno") != argHash.end())
       {
           structuralAnnotation(argHash);
       }
    if (argHash.find("funcAnno") != argHash.end())
       {
           functionalAnnotation(argHash);
       }

}
void CommandLineParser::runPCA(QHash<QString, QString> argHash)
{

    callGctaPca(argHash);
}

bool CommandLineParser::callGctaPca(QHash<QString, QString> args)
{
    QString phenotype = args["phenoFile"];
    QString genotype = args["genoFile"];
    QString map = args["mapFile"];
    QString covar = args["covarFile"];
    QString kinship = args["kinFile"];
    QString out = args["out"];
    QString model = args["model"];
    QString maf = args["maf"];
    QString mind = args["mind"];
    QString geno = args["geno"];

    QFileInfo genoFileInfo(genotype);
    QString genoFileAbPath = genoFileInfo.absolutePath();
    QString genoFileBaseName = genoFileInfo.baseName();

    QString name = args["name"];
    QString PCs = args["PCs"];
    QString threads = args["threads"];
    if (genotype.isNull() || genotype.isEmpty())
    {
        throw runtime_error("A genotype file is necessary!   ");
        return false;
    }

    qApp->processEvents();



    try {
        QString phenotype = args["phenoFile"];
        QFileInfo genoFileInfo(genotype);
        QString genoFileAbPath = genoFileInfo.absolutePath();
        QString genoFileBaseName = genoFileInfo.baseName();
        QString map = args["mapFile"];
        QString out = args["out"];
        QString name = args["name"];
        QString PCs = args["PCs"];
        QString threads = args["threads"];
        //  binaryFile: Set a default path. Binary geno file with paht without suffix.
        QString binaryFile = genoFileAbPath+"/"+genoFileBaseName+"_tmp";

        bool transformFileFlag = false;


        Plink plink;
        if (isVcfFile(genotype)) // Transform "vcf" to "transpose"
        {
            if(!plink.vcf2binary(genotype, binaryFile, nullptr, nullptr, nullptr))
            {
                throw runtime_error("vcf to transpose file error");
            }

            transformFileFlag = true;
        }

        if (genotype.split(".")[genotype.split(".").length()-1] == "ped")  // Transform "plink" to "binary"
        {
            if (map.isNull())
            {
                map = genoFileAbPath+"/"+genoFileBaseName+".map";
            }
            if (!plink.plink2binary(genotype, map, binaryFile, nullptr, nullptr, nullptr))
            {
                throw runtime_error("plink to binary file error");
            }

            transformFileFlag = true;
        }

        if (genotype.split(".")[genotype.split(".").length()-1] == "tped")  // Transform "transpose" to "binary"
        {
            if (map.isNull())
            {
                map = genoFileAbPath+"/"+genoFileBaseName+".tfam";
            }
            if (!plink.transpose2binary(genotype, map, binaryFile, nullptr, nullptr, nullptr))
            {
                throw runtime_error("transpose to binary file error");
            }
            transformFileFlag = true;
        }

        if (transformFileFlag)
        {   // Run plink to transform file or filter data.

            int exitCode = this->proc->execute(this->toolpath+"plink", plink.getParamList());
            if (exitCode == -2)
            {
                throw runtime_error("process cannot be started");
            }
            if (exitCode == -1)
            {
                throw runtime_error("process crashes");
            }
            this->proc->close();
        }
        else
        {
            binaryFile = genoFileAbPath + "/" + genoFileBaseName;
        }

        // Make GRM
        Gcta gcta;
        if (gcta.makeGRM(binaryFile, binaryFile))
        {

            int exitCode = this->proc->execute(this->toolpath+"gcta64", gcta.getParamList());
            this->proc->waitForStarted();

            if (exitCode == -2)
            {
                throw runtime_error("process cannot be started");
            }
            if (exitCode == -1)
            {
                throw runtime_error("process crashes");
            }
            this->proc->close();
        }

        // Run PCA
        if (gcta.runPCA(binaryFile,PCs.toInt(), threads.toInt(), out+"/"+genoFileBaseName))
        {
            int exitCode = this->proc->execute(this->toolpath+"gcta64", gcta.getParamList());
            this->proc->waitForStarted();

            if (exitCode == -2)
            {
                throw runtime_error("process cannot be started");
            }
            if (exitCode == -1)
            {
                throw runtime_error("process crashes");
            }
            this->proc->close();
        }

        QFile file;
        if (transformFileFlag)
        {
            file.remove(binaryFile+".bed");
            file.remove(binaryFile+".bim");
            file.remove(binaryFile+".fam");
            file.remove(binaryFile+".nosex");
        }
        file.remove(binaryFile+".grm.bin");
        file.remove(binaryFile+".grm.id");
        file.remove(binaryFile+".grm.N.bin");

    } catch (runtime_error *re) {
                    qDebug() <<  re->what();
                    this->proc->close();
                    return false;
                }

    this->proc->close();

    //QString path = out;
    plotPca(args);
    return true;
}
bool CommandLineParser::plotPca(QHash<QString, QString> args)
{
    //,QString eigenvalFile,QString eigenvecFile
    bool runningFlag=false;
    if (runningFlag)
    {
        throw runtime_error("A project is running now.");
        return false;
    }

    QString out = args["out"];
    QString ProjectName = args["ProjectName"];
    //QString path = out;

    //QString outFile = this->workDirectory->getOutputDirectory() + "/" +
                      //this->workDirectory->getProjectName() + "_pca.png";

    QString eigenvalFile = out + "/hapmap1.eigenval";
    QString eigenvecFile = out + "/hapmap1.eigenvec";
    //QString eigenvalFile = "/home/yingwang/yzz/test/out/hapmap1.eigenval";
    //QString eigenvecFile = "/home/yingwang/yzz/test/out/hapmap1.eigenvec";

    QString outFile = out + "/"+ProjectName+"_pca.png";

    //QString outFile = "/home/yingwang/yzz/test/out/_pca.png";

    if (eigenvalFile.isEmpty() ||
        eigenvecFile.isEmpty() ||
        outFile.isEmpty())
    {
        throw runtime_error("A file is necessary! ");
        return false;
    }
    runningFlag = true;
    //ui->pcaPlotPushButton->setEnabled(false);
    qApp->processEvents();
   // QFuture<void> fu = QtConcurrent::run([&]()
   // {

      //  emit runningMsgWidgetAppendText(QDateTime::currentDateTime().toString() +
       //                                 "\nPlot PCA, \n");
        //QThread::msleep(10);
        QStringList param;
        // The sequence of param is not changeable
        param.clear();
        param.append(this->scriptpath+"pca/pca_plot.R");    // Can choose pca_plot.R or pca_ggplot.R
        param.append(eigenvalFile);
        param.append(eigenvecFile);
        param.append(outFile);

        // R in environment path is necessary.
        proc->start("Rscript", param);
        if(!proc->waitForStarted())
        {
            delete proc;
            proc=nullptr;
            return false;

        }
        proc->waitForFinished(-1);
       // emit runningMsgWidgetAppendText(QDateTime::currentDateTime().toString() +
        //                                "\nOK,\n" + outFile + "\n");
        //QThread::msleep(10);
        // Show plot
       // if (runningFlag && checkoutExistence(outFile))
       // {
          //  emit setGraphViewerGraphSig(QStringList() << outFile);
          //  QThread::msleep(10);
       // }
   // });
    //while (!fu.isFinished())
   // {
   //     qApp->processEvents(QEventLoop::AllEvents, 200);
   // }

    //ui->pcaPlotPushButton->setEnabled(true);
    qApp->processEvents();
    runningFlag = false;
}


void CommandLineParser::on_ldRunPushButton_clicked(QHash<QString, QString> argHash)
{//GUI界面代码 这个函数里有判断是否输入了基因型文件,但这个功能在cmd代码里的getArgsFromcmd函数里就有,所以这里不再判断
    if(argHash["analysisCmdOp"]=="yes"||argHash["analysisCmdOp"]=="YES")
    {
        //执行family
        runPopLDdecaybyFamily(argHash);
    }
    if(argHash["analysisCmdOp"]=="no"||argHash["analysisCmdOp"]=="NO")
    {
        //执行single
        runPopLDdecaySingle(argHash);
    }

}
void CommandLineParser::runPopLDdecaySingle(QHash<QString, QString> args)
{
    QString LD_plot = args["LD_plot"];
    QString genotype =args["genoFile"];
    QFileInfo genoFileInfo(genotype);
    QString genoFileAbPath = genoFileInfo.absolutePath();
    QString genoFileBaseName = genoFileInfo.baseName();
    QString map = args["mapFile"];
    QString out = args["out"];
    QString name = args["ProjectName"];
    //  binaryFile: Set a default path. Binary geno file with path without suffix.
    QString plinkFile = genoFileAbPath+"/"+genoFileBaseName+"_tmp";
    bool transformFileFlag = false;

    // Need binary files.  Every temp file and a "_tmp" after baseName, and will be deleted after gwas.
    Plink plink;//判断输入的基因文件的格式，都转化成plink格式
    if (isVcfFile(genotype)) // Transform "vcf" to "plink"
    {
        if(!plink.vcf2plink(genotype, plinkFile, nullptr, nullptr, nullptr))
        {
            throw -1;
        }

        transformFileFlag = true;
    }
    if (genotype.split(".")[genotype.split(".").length()-1] == "bed")  //Transform "binary" to "plink"
    {
        if (!plink.binary2plink(genoFileAbPath+"/"+genoFileBaseName, plinkFile, nullptr, nullptr, nullptr))
        {
            throw -1;
        }

        transformFileFlag = true;
    }

    if (genotype.split(".")[genotype.split(".").length()-1] == "tped")  // Transform "transpose" to "plink"
    {
        if (map.isNull())
        {
            map = genoFileAbPath+"/"+genoFileBaseName+".tfam";
        }
        if (!plink.transpose2plink(genotype, map, plinkFile, nullptr, nullptr, nullptr))
        {
            throw -1;
        }
        transformFileFlag = true;
    }
    if (transformFileFlag)//真正调用工具来转化格式
    {   // Run plink to transform file or filter data.
        int exitCode = this->proc->execute(this->toolpath+"plink", plink.getParamList());//这步就是最终调用plink工具
        if (exitCode == -2)                                                              //由toolpath来告知系统plink工具在哪
        {                                                                                //由getparamlist来提供plink命令后面一大串的命令参数
            throw runtime_error("process cannot be started when transformfile(LD single)");                       //用execute执行进程后，若有错误会返回错误代码-1或-2
        }
        if (exitCode == -1)
        {
            throw runtime_error("process crashes when transformfile(LD single)");
        }
    }
    else //说明输入就是plink格式文件不需要转化
    {
        plinkFile = genoFileAbPath + "/" + genoFileBaseName;
    }

    qDebug() << QDateTime::currentDateTime().toString() << "\nMake " <<plinkFile << ".genotype,\n";
    PopLDdecay popLDdecay;  //制作参数列表
    if (popLDdecay.makeGenotype(plinkFile+".ped", plinkFile+".map", plinkFile+".genotype"))
    {

        int exitCode = this->proc->execute(this->scriptpath+"poplddecay/plink2genotype",
                                           popLDdecay.getParamList());
        if (exitCode == -2)
        {
            throw runtime_error("process cannot be started when makeGenotype(LD single)");
        }
        if (exitCode == -1)
        {
            throw runtime_error("process crashes when makeGenotype(LD single)");
        }

        qDebug()<< QDateTime::currentDateTime().toString() << "\n" << plinkFile << ".genotype OK.\n";
    }
    else
    {
        qDebug()<<QDateTime::currentDateTime().toString()<<"\n" << plinkFile << ".genotype ERROR.\n";
        throw -1;
    }
    QFile file;// 移除临时变量
    if (plinkFile != genoFileAbPath + "/" + genoFileBaseName)
    {
        file.remove(plinkFile+".ped");
        file.remove(plinkFile+".map");
    }
    file.remove(plinkFile+".nosex");
    file.remove(plinkFile+".log");
    qDebug()<< QDateTime::currentDateTime().toString() << "\nRun LD,\n";
    if (popLDdecay.runLD(plinkFile+".genotype", out+"/"+name))//填充参数列表
    {

        int exitCode = this->proc->execute(this->toolpath+"PopLDdecay",
                                           popLDdecay.getParamList());
        if (exitCode == -2)
        {
            throw runtime_error("process cannot be started when runLD(LD single)");
        }
        if (exitCode == -1)
        {
            throw runtime_error("process crashes when when runLD(LD single)");
        }

  /*      if (checkoutExistence(out+"/"+name+".stat.gz"))
        {
            //out+"/"+name+".stat.gz"就是最终结果的路径
            emit setLineEditTextSig(ui->ldResultLineEdit, out+"/"+name+".stat.gz");
            QThread::msleep(10);
        }*/

        qDebug()<< QDateTime::currentDateTime().toString() <<  "\nLD OK.\n";
    }
    else
    {
        qDebug()<< QDateTime::currentDateTime().toString() << "\nLD ERROR.\n";
        throw -1;
    }
    file.remove(plinkFile+".genotype");
    if(LD_plot=="yes")
    {
        on_ldPlotPushButton_clicked(args, out+"/"+name+".stat.gz");
    }
    this->proc->close();

}
void CommandLineParser::runPopLDdecaybyFamily(QHash<QString, QString> args)
{
    QString LD_plot = args["LD_plot"];
    QString out = args["out"];
    QString name = args["ProjectName"];
    QString genotype =args["genoFile"];
    QString map = args["mapFile"];
    QFileInfo genoFileInfo(genotype);
    QString genoFileSuffix = genoFileInfo.suffix();
    QString genoFileBaseName = genoFileInfo.baseName();
    QString genoFileAbPath = genoFileInfo.absolutePath();
    QStringList keepFileList;
    Plink plink;
    PopLDdecay popLDdecay;
    if (isVcfFile(genotype)){} // ???界面这里学长没写完
    // Make .keep file.
    qDebug() << QDateTime::currentDateTime().toString() << "\nMake .keep file, \n" ;
    if (genoFileSuffix == "ped")
    {
        keepFileList = popLDdecay.makeKeepFile(genotype);
    }
    if (genoFileSuffix == "tped")
    {
        map = map.isNull() ? genoFileAbPath+"/"+genoFileBaseName+".tfam" : map;
        keepFileList = popLDdecay.makeKeepFile(map);
    }
    if (genoFileSuffix == "bed")
    {
        map = map.isNull() ? genoFileAbPath+"/"+genoFileBaseName+".fam" : map;
        keepFileList = popLDdecay.makeKeepFile(map);
    }
     qDebug() << QDateTime::currentDateTime().toString() << "\n.keep file OK.\n" ;
     bool isLD_OK = true;
     for (QString keepFile:keepFileList)
     {
         QFileInfo keepFileInfo(keepFile);
         QString keepFileBaseName = keepFileInfo.baseName();
         QString keepFileAbPath = keepFileInfo.absolutePath();
         qDebug() << QDateTime::currentDateTime().toString() << "\nMake "<<keepFileBaseName
                  <<".ped and "<<keepFileBaseName<<".map, \n" ;


         // Split ped and map file.
         if (genoFileSuffix == "ped")
         {
             map = map.isNull() ? genoFileAbPath+"/"+genoFileBaseName+".map" : map;
             plink.splitPlinkFile(genotype, map, keepFile,
                     genoFileAbPath+"/"+keepFileBaseName);
         }
         if (genoFileSuffix == "tped")
         {
             map = map.isNull() ? genoFileAbPath+"/"+genoFileBaseName+".tfam" : map;
             plink.splitTransposeFile(genotype, map, keepFile,
                     genoFileAbPath+"/"+keepFileBaseName);
         }
         if (genoFileSuffix == "bed")
         {
             plink.splitBinaryFile(genoFileAbPath+"/"+genoFileBaseName, keepFile, genoFileAbPath+"/"+keepFileBaseName);
         }

         int exitCode = this->proc->execute(this->toolpath+"plink", plink.getParamList());
         if (exitCode == -2)
         {
             throw runtime_error("process cannot be started when Split ped and map file(LD family)");
         }
         if (exitCode == -1)
         {
             throw runtime_error("process crashes when Split ped and map file(LD family)");
         }

          qDebug() << QDateTime::currentDateTime().toString() <<"\n"<< keepFileBaseName <<
                      ".ped and " << keepFileBaseName << ".map OK.\n";

         QFile file;
         file.remove(keepFile);
         qDebug() << QDateTime::currentDateTime().toString() <<
                     "\nMake "<<keepFileBaseName<<".genotype,\n";

         // Make .genotype file.
         if (popLDdecay.makeGenotype(genoFileAbPath+"/"+keepFileBaseName+".ped",
                                 genoFileAbPath+"/"+keepFileBaseName+".map",
                                 genoFileAbPath+"/"+keepFileBaseName+".genotype"))
         {
             qDebug()<< "paramlist:"<<popLDdecay.getParamList();

             int exitCode = this->proc->execute(this->scriptpath+"poplddecay/plink2genotype",
                                                popLDdecay.getParamList());
             if (exitCode == -2)
             {
                 throw runtime_error("process cannot be started when Make .genotype file(LD family)");
             }
             if (exitCode == -1)
             {
                 throw runtime_error("process crashes when Make .genotype file(LD family)");
             }

             qDebug() <<QDateTime::currentDateTime().toString()
                                             << "\n" << keepFileBaseName<<".genotype OK.\n";

         }
         else
         {
             qDebug() <<QDateTime::currentDateTime().toString() <<
                                         "\nMake "<<keepFileBaseName<<".genotype ERROR.\n";

             isLD_OK = false;
             throw -1;
         }

         file.remove(genoFileAbPath+"/"+keepFileBaseName+".ped");
         file.remove(genoFileAbPath+"/"+keepFileBaseName+".map");
         file.remove(genoFileAbPath+"/"+keepFileBaseName+".log");
         file.remove(genoFileAbPath+"/"+keepFileBaseName+".nosex");

         // Run LD.
         // keepFileName:fileAbPath+"/"+fileBaseName+"_"+fid+".keep"
         if (popLDdecay.runLD(genoFileAbPath+"/"+keepFileBaseName+".genotype",
                              out+"/"+name+"_"+keepFileBaseName.split("_")[keepFileBaseName.split("_").length()-1]))
         {
             qDebug() <<QDateTime::currentDateTime().toString() << "\nRun LD,\n";

             int exitCode = this->proc->execute(this->toolpath+"PopLDdecay",
                                                popLDdecay.getParamList());
             if (exitCode == -2)
             {
                 throw runtime_error("process cannot be started when Run LD.(LD family)");
             }
             if (exitCode == -1)
             {
                 throw runtime_error("process crashes when Run LD.(LD family)");
             }


//                ui->ldResultLineEdit->setText(out+"/"+name+"_"+keepFileBaseName.split("_")[keepFileBaseName.split("_").length()-1]+".stat.gz");
             qDebug() << QDateTime::currentDateTime().toString() << "\nLD OK. (FID: " <<
                 keepFileBaseName.split("_")[keepFileBaseName.split("_").length() - 1] << ")\n";

         }
         else
         {
            qDebug() << QDateTime::currentDateTime().toString() << "\nLD ERROR. (FID: " <<
                keepFileBaseName.split("_")[keepFileBaseName.split("_").length() - 1] << ")\n";
             isLD_OK = false;
             throw -1;
         }
         file.remove(genoFileAbPath+"/"+keepFileBaseName+".genotype");

         if(LD_plot=="yes")
         {                                  //其实这一大串keepFileBaseName.split("_")[keepFileBaseName.split("_").length()-1 代表一个数字
             on_ldPlotPushButton_clicked(args,  out+"/"+name+"_"+keepFileBaseName.split("_")[keepFileBaseName.split("_").length()-1]+".stat.gz");
         }
     }

}
void CommandLineParser::on_ldPlotPushButton_clicked(QHash<QString, QString> args,QString ldResultFile)
{
    QString out = args["out"];
    QString name = args["ProjectName"];

    if (ldResultFile.isNull() || ldResultFile.isEmpty())
    {
        return;
    }

    PopLDdecay popLDdecay;
    qDebug()<< "LD plot, \n" << ldResultFile << "\n";

    QFileInfo ldResultFileInfo(ldResultFile);
   QString abPath = ldResultFileInfo.absolutePath();
   QString baseName = ldResultFileInfo.baseName();

   if (popLDdecay.plotLD(ldResultFile,abPath+"/"+baseName+"_ld"))
    {
        int exitCode = this->proc->execute(this->scriptpath+"poplddecay/Plot_OnePop",
                                          popLDdecay.getParamList());
        if (exitCode == -2)
        {
            throw runtime_error("process cannot be started when plotLD");
        }
        if (exitCode == -1)
        {
            throw runtime_error("process crashes when when plotLD");
        }

        QStringList graphList(ldResultFile+"_ld.png");
        if (checkoutExistence(graphList[0]))
        {
            qDebug()<<"LD plot OK.\n\n" <<ldResultFile<<"_ld.png\n";
            qDebug()<< graphList;

        }
    }
}

bool CommandLineParser::checkoutExistence(QString filePath)
{
    QFile file(filePath);
    return file.exists();
}
void CommandLineParser::runGWAS(QHash<QString, QString> argHash)
{
    QStringList param;
    if (argHash["tool"] == "plink")
    {
        callPlinkGwas(argHash);
    }
    else if (argHash["tool"] == "gemma")
    {
        callGemmaGwas(argHash);
    }
    else if (argHash["tool"] == "emmax")
    {
        callEmmaxGwas(argHash);
    }
}

bool CommandLineParser::callPlinkGwas(QHash<QString, QString> args)
{
    QString phenotype = args["phenoFile"];
    QString genotype = args["genoFile"];
    QString map = args["mapFile"];
    QString covar = args["covarFile"];
    QString kinship = args["kinFile"];
    QString out = args["out"];
    QString model = args["model"];
    QString maf = args["maf"];
    QString mind = args["mind"];
    QString geno = args["geno"];
    QString ProjectName = args["ProjectName"];

    // Genotype file info.
    QFileInfo genoFileInfo = QFileInfo(genotype);  //用来解析路径 如genotype= /home/data/geno.vcf
    QString genoFileName = genoFileInfo.fileName();         // geno.vcf
    QString genoFileBaseName = genoFileInfo.baseName();     // geno
    QString genoFileSuffix = genoFileInfo.suffix();         // vcf
    QString genoFileAbPath = genoFileInfo.absolutePath();   //绝对路径 /home/data

    // Phenotype file info.
    QFileInfo pheFileInfo = QFileInfo(phenotype);
    QString pheFileBaseName = pheFileInfo.baseName();

    Plink plink;

    try {
        // Run GWAS
        if(!plink.runGWAS(phenotype, genotype, map, covar, kinship, model, maf, mind, geno, out+"/"+ProjectName+"_"+pheFileBaseName))//runGWAS函数会填写paramlist这个变量
        {
            throw runtime_error("plink.runGWAS() error");
        }
        int exitCode = this->proc->execute(this->toolpath+"plink", plink.getParamList());//这步就是最终调用plink工具
        if (exitCode == -2)                                                              //由toolpath来告知系统plink工具在哪
        {                                                                                //由getparamlist来提供plink命令后面一大串的命令参数
            throw runtime_error("process cannot be started");                       //用execute执行进程后，若有错误会返回错误代码-1或-2
        }
        if (exitCode == -1)
        {
            throw runtime_error("process crashes");
        }
//        this->proc->start(this->toolpath+"plink", plink.getParamList());
//        if (!this->proc->waitForStarted())
//        {
//            throw runtime_error("Start plink error");
//        }

//        if (!this->proc->waitForFinished(-1))
//        {
//            throw runtime_error("Finish plink error");
//        }
    } catch (runtime_error *re) {
        qDebug() <<  re->what();
        return false;
    }
    QFile file;
    file.remove(out+"/"+ProjectName+"_"+pheFileBaseName+".nosex");
    file.remove(out+"/"+ProjectName+"_"+pheFileBaseName+".log");
    this->proc->close();
     //plink分析后的结果文件,这里只是选择linear模型的结果来画图
    //至于logistic模型的结果，因为界面没跑成功过，不知道它结果的后缀是咋样的，还没搞
    QString resultfile = out+"/"+ProjectName+"_"+pheFileBaseName+".assoc."+model.toLower() ;

    on_drawManPushButton_clicked(args, resultfile);



    return true;

}

bool CommandLineParser::callGemmaGwas(QHash<QString, QString> args)
{   //从哈希表中取出各个命令的参数值
    QString phenotype = args["phenoFile"];
    QString genotype = args["genoFile"];
    QString map = args["mapFile"];
    QString covar = args["covarFile"];
    QString kinship = args["kinFile"];//亲缘矩阵文件 若用户没有输入亲缘矩阵 则这个值是空的
    QString out = args["out"];
    QString model = args["model"];
    QString maf = args["maf"];
    QString mind = args["mind"];
    QString geno = args["geno"];
    QString kinship_type = args["gemma_kinmat"];//这是亲缘矩阵的类型 1或2 用来-gk 1 或-gk 2 分别代表中心矩阵和标准矩阵
    QString lmmtest = args["lmmtest"];
    QString bslmmmodel =  args["bslmmmodel"];
    QString ProjectName = args["ProjectName"];
    // Specific arguments
    bool isFamCompletedAuto = args["famcomple"] == "0" ? 0 : 1;  //这个参数表示要不要改造fam文件，若用户输0表示不要 1表示要
    bool isMakeRelatedMatAuto = args["makekin"] == "0" ? 0 : 1; //这个参数表示要不要生成亲缘矩阵，若用户输0表示不要 1表示要

    // Genotype file info.
    QFileInfo genoFileInfo = QFileInfo(genotype); //比如输入的基因型文件参数为 ：home/data/hapmap.bed
    QString genoFileName = genoFileInfo.fileName();         // hapmap.bed
    QString genoFileBaseName = genoFileInfo.baseName();     // hapmap
    QString genoFileSuffix = genoFileInfo.suffix();         // bed
    QString genoFileAbPath = genoFileInfo.absolutePath();   // home/data

    QString binaryFile = genoFileAbPath+"/"+genoFileBaseName+"_tmp"; // home/data/hapmap_tmp临时文件的名字

    // Phenotype file info.
    QFileInfo pheFileInfo = QFileInfo(phenotype);
    QString pheFileBaseName = pheFileInfo.baseName();
    // Necessary to transform file ?
    bool transformFileFlag = false;
    bool filterDataFlag = false;

    // Need binary files.  Every temp file and a "_tmp" after baseName, and will be deleted after gwas.
    Plink plink;
    try {
    if (isVcfFile(genotype)) // 看下是不是vcf文件 若是就要把vcf转化成二进制格式
    {
        if(!plink.vcf2binary(genotype, binaryFile, maf, mind, geno))//该函数会将vcf转化二进制和进行质量控制的plink语法添加进参数列表
        {                                      //这里将binaryfile作为输出路径,因此生成的二进制文件跟输入的基因型文件在同个路径下

            return false;
        }

        transformFileFlag = true;
    }
    if (genotype.split(".")[genotype.split(".").length()-1] == "ped")  // Transform "plink" to "binary"
    {
        if (map.isNull())
        {
            map = genoFileAbPath+"/"+genoFileBaseName+".map";
        }
        if (!plink.plink2binary(genotype, map, binaryFile, maf, mind, geno))//gemma的基因型输入文件的格式必须是二进制那个
        {                                                                   //所以需要把各种基因型文件转换成二进制格式
            return false;
        }

        transformFileFlag = true;
    }

    if (genotype.split(".")[genotype.split(".").length()-1] == "tped")  // Transform "transpose" to "binary"
    {
        if (map.isNull())
        {
            map = genoFileAbPath+"/"+genoFileBaseName+".tfam";
        }
        if (!plink.transpose2binary(genotype, map, binaryFile, maf, mind, geno))
        {

            return false;
        }
        transformFileFlag = true;
    }

    if (genotype.split(".")[genotype.split(".").length()-1] == "bed")//若基因型输入的本来就是二进制文件就不需要转化
    {   // When don't set any QC param, it won't execute.             //若有质控要求就添加质控命令到列表 没有就算
        plink.filterBinaryFile(genoFileAbPath+"/"+genoFileBaseName, maf, mind, geno, binaryFile);
        filterDataFlag = true;
    }
    if (transformFileFlag || filterDataFlag)//要么有转化格式 要么有进行质控，负责就不能进行底层plink调用
    {   // Run plink to transform file or filter data. 这里正式开始调用底层plink工具转化格式的同时进行质控
       int exitCode = this->proc->execute(this->toolpath+"plink", plink.getParamList());
       if (exitCode == -2)                                                              //由toolpath来告知系统plink工具在哪
       {                                                                                //由getparamlist来提供plink命令后面一大串的命令参数
           throw runtime_error("process cannot be started when run QC and transform file by plink");                       //用execute执行进程后，若有错误会返回错误代码-1或-2
       }
       if (exitCode == -1)
       {
           throw runtime_error("process crashes when run QC and transform file by plink");
       }
     }
    }
    catch (runtime_error *re)
        {
            qDebug() <<  re->what();
            return false;
        }

    //前面是用plink转化格式和质量控制的部分 接下来轮到主角gemma
    Gemma gemma;

    if (isFamCompletedAuto && !gemma.phe_fam_Preparation(phenotype, binaryFile+".fam"))//改造fam文件
    {   // Replace "NA" to "-9", then complete .fam
        // .fam: FID IID PID MID Sex 1 Phe  (phenotype data to the 7th column of .fam)

        return false;
    }


    if (kinship.isNull() && isMakeRelatedMatAuto)//如果用户没有输入亲缘矩阵文件，同时要求生成亲缘矩阵，则就开始制作矩阵（实际上两个条件有一个就行了，比较鸡肋）
    {    qDebug()<< "kinship_type: " << kinship_type ;
         if (!gemma.makeKinship(binaryFile, genoFileBaseName+"_tmp", kinship_type))//这里生成亲缘矩阵（其实是填充生成亲缘矩阵命令参数列表，使后面直接调用gemma生成矩阵）
         {                                                              //需要注意这里生成的矩阵它的名字带有后缀_tmp但它的位置不一定和所用的二进制文件在一起,而是在当前终端的路径下
           //  this->resetWindow();                                    //比如在整个工程build文件下的output里
             return false;  // Make kinship failed.
         }
         int exitCode= this->proc->execute(this->toolpath+"gemma", gemma.getParamList());//正式开始底层调用gemma来生成矩阵
 /*      if (!this->process->waitForStarted())
         {
             QMessageBox::information(nullptr, "Error", "Start gemma with error when  make kinship!   ");
             this->resetWindow();
             return false;
         }
         //this->runningMsgWidget->setTitle("Making " + genoFileBaseName+"_tmp" + ".cXX.txt");
         this->runningMsgWidget->setTitle("Making kinship");
         if (!this->process->waitForFinished(-1))
         {
             QMessageBox::information(nullptr, "Error", "Exit gemma with error when  make kinship!   ");
             this->resetWindow();
             return false;
         }
*/
         try {
                 if (exitCode == -2)
                 {
                     throw runtime_error("Start gemma with error when  make kinship!   ");
                 }
                 if (exitCode == -1)
                 {
                     throw runtime_error("process crashes when  make kinship! ");
                 }

             }
         catch (runtime_error *re)
         {
             qDebug() <<  re->what();
             return false;
         }
         this->proc->close(); //调用gemma生成矩阵结束

         //this->runningMsgWidget->setTitle("Kinship is made");
         //kinship = genoFileAbPath + "/output/" + genoFileBaseName + ".cXX.txt";    // Attention

         if (kinship_type == "1")//这里因为生成了矩阵，前面用户没有输入矩阵导致kinship是空的，所以要对接新生成的矩阵
         {
             kinship = QDir::currentPath()  + "/output/" + genoFileBaseName+"_tmp" + ".cXX.txt"; //新生成的矩阵位于当前路径下output文件里
         }
         else
         {
             kinship = QDir::currentPath() + "/output/" + genoFileBaseName+"_tmp" + ".sXX.txt";
         }
         qDebug()<< "kinship:"<<kinship; //在命令行里看看矩阵文件的路径对不对
    }

    if (gemma.runGWAS(binaryFile, phenotype, covar, kinship,
                     ProjectName+"_"+pheFileBaseName, model, lmmtest, bslmmmodel))//往参数列表里填充gemma的各种命令
    {   qDebug()<< "this->toolpath+gemma:"<<this->toolpath+"gemma" ;
        qDebug()<< "paramlist:"<<gemma.getParamList();
        int exitCode= this->proc->execute(this->toolpath+"gemma", gemma.getParamList());//正式调用gemma工具跑
        try {
                if (exitCode == -2)
                {
                    throw runtime_error("Start gemma with error when run GWAS! ");
                }
                if (exitCode == -1)
                {
                    throw runtime_error("process crashes when run GWAS!");
                }

            }
        catch (runtime_error *re)
        {
            qDebug() <<  re->what();
            return false;
        }

        // Running message to display message.
/*        if (!this->process->waitForStarted())
        {
            QMessageBox::information(nullptr, "Error", "Start gemma with error when run GWAS! ");
            this->resetWindow();
            return false;
        }
        this->runningMsgWidget->setTitle("Gemma(" + pheFileBaseName+ "): " + name+" is running...");
        if (!this->process->waitForFinished(-1))
        {
            QMessageBox::information(nullptr, "Error", "Exit gemma with error when run GWAS! ");
            this->resetWindow();
            return false;
        }
*/
        this->proc->close();

        QFile file;
        // delete intermidiate file.把过渡的二进制文件以及矩阵文件都删除 只保留最终结果
        file.remove(binaryFile+".bed");
        file.remove(binaryFile+".bim");
        file.remove(binaryFile+".fam");
        file.remove(binaryFile+".log");
        file.remove(binaryFile+".nosex");
        if (kinship_type == "1")
        {
          //  file.remove(QDir::currentPath() + "/output/"+genoFileBaseName+"_tmp.cXX.txt");
        }
        else
        {
          //  file.remove(QDir::currentPath() + "/output/"+genoFileBaseName+"_tmp.sXX.txt");
        }
        file.remove(QDir::currentPath() + "/output/"+genoFileBaseName+"_tmp.log.txt");


        QDir dir;   // gemma output in the execution file dir by default.
        QDir objDir(out+"/output");
        // We move it to the work dir.
        int i = 0;
        while(objDir.exists())
        {   // It will be wrong when object dir existed.
            i++;
        //objDir是为i准备的,如果路径已经存在就更改路径重新检测是否还存在,如果不更改路径,i就会无限加下去
            objDir.setPath(out+"/output"+QString::number(i));
        }
 // rename可以更改一个已经存在的文件的路径,相当于把一个文件移动到新地方去,但要注意新地方必须已经存在
 //如最终移动到 /desktop/out/output这个路径下,则/desktop/out这个必须要先存在,rename只会在out文件夹里新建一个output文件夹
        dir.rename(QDir::currentPath() + "/output", out+"/output"+(i==0?"":QString::number(i)));

//        ui->gwasResultLineEdit->setText(out+"/output"+(i==0?"":QString::number(i))+"/"+name+"_"+pheFileBaseName);

        if(model=="LMM")//注意！BSLMM模型的结果不能画图 只有LMM才可以！
        {
            //gemma分析后的结果文件
            QString resultfile = out+"/output"+(i==0?"":QString::number(i))+ "/"+ProjectName+"_"+pheFileBaseName+".assoc.txt";

            on_drawManPushButton_clicked(args, resultfile);

        }



  }


    return true;
}

bool CommandLineParser::callEmmaxGwas(QHash<QString, QString> args)
{
    QString phenotype = args["phenoFile"];
    QString genotype = args["genoFile"];
    QString map = args["mapFile"];
    QString covar = args["covarFile"];
    QString kinship = args["kinFile"];
    QString out = args["out"];
    QString model = args["model"];      // 实际没用上,因为emmax只有EMMA这一个模式,但为了和界面以及gemma plink统一,
                                        //输入命令的时候也统一为其设置EMMA模式
    QString maf = args["maf"];
    QString mind = args["mind"];
    QString geno = args["geno"];

    QString ProjectName = args["ProjectName"];


//    Emmax Specific arguments
//    QString makekin = args["emmax_makekin"];
    QString kinmat = args["emmax_kinmat"];
//    bool isFamCompletedAuto = args["famcomple"] == "0" ? 0 : 1;
//    bool isMakeRelatedMatAuto = args["makekin"] == "0" ? 0 : 1;


    // Genotype file info.
    QFileInfo genoFileInfo = QFileInfo(genotype);
    QString genoFileName = genoFileInfo.fileName();         //
    QString genoFileBaseName = genoFileInfo.baseName();     //
    QString genoFileSuffix = genoFileInfo.suffix();         //
    QString genoFileAbPath = genoFileInfo.absolutePath();
    QString genoFileCMD = genoFileAbPath+"/"+genoFileBaseName;

    QString transposeFile =genoFileAbPath+"/"+genoFileBaseName+"_tmp";    // can not understand?

    // Phenotype file info.
    QFileInfo pheFileInfo = QFileInfo(phenotype);
    QString pheFileBaseName = pheFileInfo.baseName();

    // Necessary to transform file ?
    bool transformFileFlag = false;
    bool filterDataFlag = false;


    try {//输入的基因型文件都要转化成tped格式来用
        Plink plink;
        if (isVcfFile(genotype)) // Transform "vcf" to "transpose"
        {
            if(!plink.vcf2transpose(genotype, transposeFile, maf, mind, geno))
            {
                return false;
            }
            fileReader->completeTfamFromPheno(phenotype, transposeFile+".tfam");
            transformFileFlag = true;
        }
        if (genotype.split(".")[genotype.split(".").length()-1] == "ped")  // Transform "plink" to "transpose"
        {
            if (map.isNull())
            {
                map = genoFileAbPath+"/"+genoFileBaseName+".map";
            }

            if (!plink.plink2transpose(genotype, map, transposeFile, maf, mind, geno))
            {
                return false;
            }
            transformFileFlag = true;
        }
        if (genotype.split(".")[genotype.split(".").length()-1] == "bed")  // Transform "binary" to "transpose"
        {
            if (!plink.binary2transpose(genoFileAbPath+"/"+genoFileBaseName,
                                        transposeFile, maf, mind, geno))
            {
                return false;
            }
            transformFileFlag = true;
        }

        if (genotype.split(".")[genotype.split(".").length()-1] == "tped")  // Transform "binary" to "transpose"
        {
            if (map.isNull())
            {
                map = genoFileAbPath + "/" + genoFileBaseName + ".tfam";
            }
            plink.filterTransposeFile(genotype, map, maf, mind, geno, transposeFile);
            filterDataFlag = true;
        }

        if (transformFileFlag || filterDataFlag)
        {
            int exitCode = this->proc->execute(this->toolpath+"plink", plink.getParamList());
            if (exitCode == -1)
            {
                throw runtime_error("process crashes");
            }
            if (exitCode == -2)
            {
                throw runtime_error("process cannot be started");
            }
            //   i think this place still need to add something!!!
        }

    } catch (runtime_error *re) {
        qDebug() <<  re->what();
        return false;
    }

    try {
        Emmax emmax;
        if (kinship.isNull())        //a little error     linrenhao
           {
             if (!emmax.makeKinship(transposeFile, kinmat))
             {
                 throw runtime_error("Make kinship failed");
             }
             int exitCode = this->proc->execute(this->toolpath+"emmax-kin", emmax.getParamList());
             if (exitCode == -1)
             {
                 throw runtime_error("process crashes");
             }
             //this->runningMsgWidget->setTitle("Making " + genoFileBaseName + ".hBN.kinf");
             if (exitCode == -2)
             {
                 throw runtime_error("process cannot be started");
             }
             if (kinmat == "BN")
             {
                 kinship = genoFileAbPath + "/" + genoFileBaseName+"_tmp" + ".hBN.kinf";
             }
             else if(kinmat == "IBS")
             {
                kinship = genoFileAbPath + "/" + genoFileBaseName+"_tmp" + ".hIBS.kinf";
             }
        }

//linrenhao
        if (emmax.runGWAS(transposeFile, phenotype, covar, kinship, out+"/"+ProjectName+"_"+pheFileBaseName, kinmat))
        {
            QStringList paralist = emmax.getParamList();
//            int exitCode = this->proc->execute(this->toolpath+"emmax-kin",
//                QStringList() << "-v" << "-h" << "-d" << "10"
//                              << "/home/dengchao/Desktop/hapmap1/hapmap");
            int exitCode = this->proc->execute(this->toolpath+"emmax", emmax.getParamList());
            if (exitCode == -1)
            {
                throw runtime_error("process crashes");
            }
            //this->runningMsgWidget->setTitle("Making " + genoFileBaseName + ".hBN.kinf");
            if (exitCode == -2)
            {
                throw runtime_error("process cannot be started");
            }

            QFile file;
            // delete intermidiate file.
            file.remove(genoFileAbPath+"/"+genoFileBaseName+"_tmp.map");
            file.remove(genoFileAbPath+"/"+genoFileBaseName+"_tmp.tped");
            file.remove(genoFileAbPath+"/"+genoFileBaseName+"_tmp.tfam");
            file.remove(genoFileAbPath+"/"+genoFileBaseName+"_tmp.log");
            file.remove(genoFileAbPath+"/"+genoFileBaseName+"_tmp.nosex");
            if (kinmat == "BN")
            {
                file.remove(genoFileAbPath+"/"+genoFileBaseName+"_tmp.hBN.kinf");
            }
            else
            {
                file.remove(genoFileAbPath+"/"+genoFileBaseName+"_tmp.hIBS.kinf");
            }
        }


    } catch (runtime_error *re) {
        qDebug() <<  re->what();
        return false;
    }
    this->proc->close();

    //要找到emmax的输出目录
    QString newdir = out+"/"+ProjectName+"_"+pheFileBaseName;
    QString resultfile = newdir +".ps" ;
    on_drawManPushButton_clicked(args,resultfile);
    return true;
}


//linrenhao new
void CommandLineParser::on_drawManPushButton_clicked(QHash<QString, QString> args, QString gwasResulFile)
{

    try {
        if (gwasResulFile.isEmpty())
            {   // Gwas result file is necessary.

                throw runtime_error("Gwas result file is empty.");
            }

            QFileInfo pvalueFileInfo(gwasResulFile);
            QString pvalueFileAbPath = pvalueFileInfo.absolutePath();
            QString pvalueFileBaseName = pvalueFileInfo.baseName();
            QString pvalueFileSuffix = pvalueFileInfo.suffix();
            QString pvalueFileComSuffix = pvalueFileInfo.completeSuffix();



            // Transform gwas result file type to input file type of qqman.
            QStringList qqmanFile = makeQQManInputFile(gwasResulFile); //   path/name.gemma_wald
            QStringList Man_outList;
            QStringList QQ_outList;
            if (qqmanFile.isEmpty())
            {   // makeQQManInputFile error.


                throw runtime_error("Error makeQQManFile error.");

            }


            for (auto item:qqmanFile)
            {   // Multiple result, multiple output plot, append to list.
                Man_outList.append(pvalueFileAbPath+"/"+args["ProjectName"]+"_"+item.split(".")[item.split(".").length()-1]+"_man.png");
                QQ_outList.append(pvalueFileAbPath+"/"+args["ProjectName"]+"_"+item.split(".")[item.split(".").length()-1]+"_qq.png");
            }


            if (!this->drawManhattan(args, qqmanFile, Man_outList))
            {   // drawManhattan error

                throw runtime_error("Draw manhattan plot ERROR.\n");
            }

            if (!this->drawQQplot(qqmanFile, QQ_outList))
            {

               throw runtime_error("Draw QQ plot Error.\n");
            }




            QFile file;
            for (auto item:qqmanFile)
            {   // Remove intermediate file.
                if (item == gwasResulFile)
                {
                    continue;
                }
                file.remove(item);
            }
        } catch (runtime_error *re) {
        qDebug() <<  re->what();
    }
}

QStringList CommandLineParser::makeQQManInputFile(QString pvalueFile)
{
    QStringList qqmanInFileList;
    if (pvalueFile.isNull() || pvalueFile.isEmpty())
    {
        return qqmanInFileList;
    }

    QFileInfo pvalueFileInfo(pvalueFile);
    QString pvalueFileAbPath = pvalueFileInfo.absolutePath();
    QString pvalueFileBaseName = pvalueFileInfo.baseName();
    QString pvalueFileSuffix = pvalueFileInfo.suffix();
    QString pvalueFileComSuffix = pvalueFileInfo.completeSuffix();

    if ( pvalueFileComSuffix == "assoc.linear" || pvalueFileComSuffix == "assoc.logistic")
    {   // Plink output file don't need to be transformed.
        qqmanInFileList.append(pvalueFile);
        return qqmanInFileList;
    }

    QFile gwasResultFile(pvalueFile);
    QTextStream gwasResultFileStream(&gwasResultFile);
    if (!gwasResultFile.open(QIODevice::ReadOnly))
    {
//        QMessageBox::information(nullptr, "Error", "Open gwasResultFile error.  ");
        return qqmanInFileList;
    }

    /* Script(one line):
     *  sed 's/:/\t/g; s/chr//g' 222_filter1_phe1_BN.ps
     *  |perl -alne '{if(/^0/){print "19\tchr0:$F[1]\t$F[0]\t$F[1]\t$F[3]"}
     *  else{print "$F[0]\tchr$F[0]:$F[1]\t$F[0]\t$F[1]\t$F[3]"}}'
     *  |sort -k 1,1n -k4,4n |perl -alne '{$num=shift @F;
     *  $line=join "\t",@F; print $line}'
     *  |sed '1 i\SNP\tCHR\tBP\tP' > phe1_emmax_table
     */
    if (pvalueFileSuffix == "ps")
    {   // Emmax output file.
        /* From: .ps:
         *      [SNP ID(CHR:BP)], [beta], [p-value] (header, don't exist in file)
         *      chr0:39616  0.7571908167    0.2146455451
         * To:
         *      SNP CHR BP  P (Header is necessary)
         */
        QFile qqmanInputFile(pvalueFileAbPath+"/"+pvalueFileBaseName+"."+"emmax");
        QTextStream qqmanInputFileStream(&qqmanInputFile);
        qqmanInputFile.open(QIODevice::ReadWrite);
        qqmanInputFileStream << "SNP\tCHR\tBP\tP" << endl; // Write header
        while (!gwasResultFileStream.atEnd())
        {   // Read data line by line.
            QStringList curLine = gwasResultFileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
            QString SNP = curLine[0];

            if (SNP.split(":").length() < 2)
            {
//                QMessageBox::information(nullptr, "Error", ".ps file format error(maybe without chr).   ");
                return qqmanInFileList;
            }
            QString CHR = SNP.split(":")[0].remove(0, 3); // e.g. remove "chr" in "chr12", to get "12"
            QString BP = SNP.split(":")[1];
            QString P = curLine[curLine.length()-1];
            qqmanInputFileStream << SNP << "\t" << CHR << "\t" << BP << "\t" << P << endl;
        }
        qqmanInFileList.append(qqmanInputFile.fileName());
        qqmanInputFile.close();
        gwasResultFile.close();
        return qqmanInFileList;
    }

    /* Script (one line):
     *  perl -alne '{print "$F[1]\t$F[-1]"}'
     *  phe2_pc2_lmm.assoc.txt |sed 's/:/\t/g;
     *  s/chr//g' |perl -alne '{if(/^0/){print
     *  "chr0:$F[1]\t19\t$F[1]\t$F[2]"}
     *  else{print "chr$F[0]:$F[1]\t$F[0]\t$F[1]\t$F[2]"}}'
     *  |sort -k 2,2n -k3,3n |sed '1d' |sed '1 i\SNP\tCHR\tBP\tP'
     *  > phe2_pc2_gemma_lmm
     */
    if (pvalueFileComSuffix == "assoc.txt")
    {   // Gemma LMM
        /* From:
         *  chr	rs	ps	n_miss	allele1	allele0	af	beta	se	logl_H1	l_remle	l_mle	p_wald	p_lrt	p_score
         * To:
         *  SNP CHR BP  P (Header is necessary)
         */
        QStringList header = gwasResultFileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);    // Read header, we don't need it in .qqman file.
        int num = (header.length() != 12)  ? (header.length()-13+1) : 1;  // Multi(3) p-values in file when chosed all tested.
        for (int i = 1; i <= num; i++)
        {
            gwasResultFileStream.seek(0);       // Back to begin of the file.
            gwasResultFileStream.readLine();    // Read the header in the first line and strip it.
            QFile qqmanInputFile(pvalueFileAbPath+"/"+pvalueFileBaseName+"."+"gemma"
                                 +"_"+header[header.length()-i].split("_")[1]);    // out_name.gemma_wald
            QTextStream qqmanInputFileStream(&qqmanInputFile);
            qqmanInputFile.open(QIODevice::ReadWrite);
            qqmanInputFileStream << "SNP\tCHR\tBP\tP" << endl; // Write header
            while (!gwasResultFileStream.atEnd())
            {
                QStringList curLine = gwasResultFileStream.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
                QString SNP = curLine[1];
                QString CHR = curLine[0];
                QString BP = curLine[2];
                QString P = curLine[curLine.length()-i];
                qqmanInputFileStream << SNP << "\t" << CHR << "\t" << BP << "\t" << P << endl;
            }
            qqmanInFileList.append(qqmanInputFile.fileName());
            qqmanInputFile.close();
        }
        gwasResultFile.close();
        return qqmanInFileList;
    }

    return qqmanInFileList;
}

bool CommandLineParser::drawManhattan(QHash<QString, QString> args, QStringList data, QStringList out)
{
    if (data.isEmpty() || out.isEmpty() || data.size() != out.size())
    {
        return false;
    }

    int gwBase = args["gwBase"].toInt();
    int gwExpo = args["gwExpo"].toInt();
    int sgBase = args["sgBase"].toInt();
    int sgExpo = args["sgExpo"].toInt();


//     Threshold value
//    int gwBase =  5;
//    int gwExpo = -8;
//    int sgBase = 1;
//    int sgExpo = -5;

    QStringList param;


    //  i < data.size()&& runningFlag
    for (int i = 0; i < data.size() ; i++)
    {
        // The sequence of param is not changeable
        param.clear();
        param.append(this->scriptpath+"qqman/plot.R");
        param.append("manhattan");
#ifdef QT_NO_DEBUG
        param.append("debugno");    // Help to set path of manhattan.R and qq.R
#else
        param.append(this->scriptpath+"qqman/manhattan.R");
#endif
        param.append(data[i]);
        param.append(out[i]);
        param.append(QString::number(gwBase)+'e'+QString::number(gwExpo));
        param.append(QString::number(sgBase)+'e'+QString::number(sgExpo));


        //R in environment path is necessary.
        proc->start("Rscript", param);
        if (!proc->waitForStarted())
        {
            delete proc;
            proc = nullptr;
            return false;
        }
        proc->waitForFinished(-1);


//        proc->close();
//        delete proc;
//        proc = nullptr;



//        // R in environment path is necessary.
//        if (!runExTool("Rscript", param))
//        {
//            return false;
//        }
    }
    // Show plot

//    if (this->runningFlag && checkoutExistence(out[0]))
//    {
//        emit setGraphViewerGraphSig(out);
//        QThread::msleep(10);
//    }

    return true;
}

bool CommandLineParser::drawQQplot(QStringList data, QStringList out)
{
    if (data.isEmpty() || out.isEmpty() || data.size() != out.size())
    {
        return false;
    }

    QStringList param;



    for (int i = 0; i < data.size(); i++)
    {
        // The sequence of param is not changeable
        param.clear();
        param.append(this->scriptpath+"qqman/plot.R");
        param.append("qqplot");
#ifdef QT_NO_DEBUG
        param.append("debugno");    // Help to set path of manhattan.R and qq.R
#else
        param.append(this->scriptpath + "qqman/qq.R");
#endif
        param.append(data[i]);
        param.append(out[i]);
        //R in environment path is necessary.
        proc->start("Rscript", param);
        if (!proc->waitForStarted())
        {
            delete proc;
            proc = nullptr;
            return false;
        }
        proc->waitForFinished(-1);

        proc->close();
        delete proc;
        proc = nullptr;
        }
    return true;
}
//annotation part
//zz
bool CommandLineParser::structuralAnnotation(QHash<QString, QString> args)
{

    bool runningFlag=false;
    if (runningFlag)
    {
        throw runtime_error("A project is running now.");
        return false;
    }
    runningFlag = true;
    QString gffFile = args["gffFile"];
    QString fastaFile = args["fastaFile"];
    //QString gffFile = ui->gffFileLineEdit->text();
    //QString fastaFile = ui->fastaFileLineEdit->text();
    if (gffFile.isNull() || gffFile.isEmpty() || fastaFile.isNull() || fastaFile.isEmpty())
    {
        throw runtime_error("A file is necessary! ");
        return false;
    }
    try
    {
        //QFuture<void> fu = QtConcurrent::run([&]()
        //{
            //ui->strucAnnoRunPushButton->setEnabled(false);
            qApp->processEvents();

            QFileInfo gffFileInfo(gffFile);
            QString gffFileBaseName = gffFileInfo.baseName();
            QString gffFileCompBaseName = gffFileInfo.completeBaseName();
            QString gffFileSuffix = gffFileInfo.suffix();
            QString gffFileAbPath = gffFileInfo.absolutePath();

            QString gtfFile =gffFileAbPath+"/"+gffFileCompBaseName+".gtf";

            // gffTogtf
            Annovar annovar;
            if (annovar.gffTogtf(gffFile, gtfFile))
            {
                int exitCode = this->proc->execute(this->toolpath+"gffread", annovar.getParamList());


                if (exitCode == -2)
                {
                   throw runtime_error("process cannot be started");
                }
                if (exitCode == -1)
                {
                   throw runtime_error("process crashes");
                }
                this->proc->close();
            }


            // gtfToGenePred
            QString refGeneFile = gffFileAbPath+"/"+gffFileBaseName+"_refGene.txt";
            if (annovar.gtfToGenePred(gtfFile, refGeneFile))
            {
                //throw runtime_error("gtfToGenePred error");
                int exitCode = this->proc->execute(this->toolpath+"gtfToGenePred", annovar.getParamList());
                this->proc->waitForStarted();

                if (exitCode == -2)
                {
                   throw runtime_error("process cannot be started");
                }
                if (exitCode == -1)
                {
                   throw runtime_error("process crashes");
                }
                this->proc->close();
            }

            QString outFastaFile = gffFileAbPath + "/" + gffFileBaseName + "_refGeneMrna.fa";
            if (annovar.retrieveSeqFromFasta(refGeneFile, fastaFile, outFastaFile))
            {
                //throw runtime_error("retrieveSeqFromFasta error");
                int exitCode = this->proc->execute(this->scriptpath+"annovar/retrieve_seq_from_fasta", annovar.getParamList());
                this->proc->waitForStarted();

                if (exitCode == -2)
                {
                   throw runtime_error("process cannot be started");
                }
                if (exitCode == -1)
                {
                   throw runtime_error("process crashes");
                }
                this->proc->close();
            }




            // annotation
            //QString out = this->workDirectory->getOutputDirectory();
            QString out = args["out"];
            //QString name = this->workDirectory->getProjectName();
            QString projectname = args["ProjectName"];
            QString outFile = out + "/" + projectname + "_" + gffFileBaseName;
            QString avinput = args["avinput"];
            //QString avinput = ui->avinFileLineEdit->text();
            QString refGeneDir = gffFileAbPath;
            QString refGenePrefix = gffFileBaseName;

            // table_annovar

            // annotate_variation
            if (annovar.annotateVariation(avinput, refGeneDir, refGenePrefix, outFile))
            {
                //throw runtime_error("annotateVariation error");
                int exitCode = this->proc->execute(this->scriptpath+"annovar/annotate_variation", annovar.getParamList());


                if (exitCode == -2)
                {
                   throw runtime_error("process cannot be started");
                }
                if (exitCode == -1)
                {
                   throw runtime_error("process crashes");
                }
                this->proc->close();
            }





    } catch (runtime_error *re) {
        qDebug() <<  re->what();
        this->proc->close();
        return false;
    }

this->proc->close();

    //this->resetWindow();
    return true;
    runningFlag = false;
}
bool CommandLineParser::functionalAnnotation(QHash<QString, QString> args)
{
    QString snpPosFilePath = args["snpPosFilePath"];
    QString funcAnnoRefFilePath =args["funcAnnoRefFilePath"];
    QString varFuncFilePath =args["varFuncFilePath"];
    QString exVarFuncFilePath=args["exVarFuncFilePath"];
    try {
            QString out = args["out"];
            QString name = args["ProjectName"];

            if (snpPosFilePath.isEmpty())
            {
                throw runtime_error("Error,A position of SNP file is necessary.");

            }
            if (funcAnnoRefFilePath.isEmpty())
            {
                throw runtime_error("Error,A annotation base file is necessary.");

            }
            if (varFuncFilePath.isEmpty())
            {
                throw runtime_error( "Error,A .variant_function file is necessary.");

            }
            if (exVarFuncFilePath.isEmpty())
            {
                throw runtime_error( "Error, A .exonic_variant_function file is necessary.");
            }

            QFileInfo snpPosFileInfo(snpPosFilePath);
            QString snpPosFileAbPath = snpPosFileInfo.absolutePath();
            QString snpPosFileBaseName = snpPosFileInfo.baseName();

            QString exonicPosFile = snpPosFileAbPath + "/exonic_pos";
            QString nonExonicPosFile = snpPosFileAbPath + "/non_exonic_pos";
            QString funcAnnoResult = out + "/" + name +"_func_anno";
            FuncAnnotator funcAnnotator;

    //        QRegExp ncbiFuncAnnoSuffixRegExp(".*.ncbi.csv");
    //        QRegExp ensemFuncAnnoSuffixRegExp(".*.ensem.csv");


              // Run functional annotation in another thread;
                qDebug()<< QDateTime::currentDateTime().toString() <<
                                                "\nComplete exonic SNP infomation,\n";
                QThread::msleep(10);
                if (!funcAnnotator.complExoSnpInfo(snpPosFilePath, exVarFuncFilePath, exonicPosFile))
                {
                    throw -1;
                }

                 qDebug()<< QDateTime::currentDateTime().toString() <<
                                                "\nOK\n\n" << QDateTime::currentDateTime().toString() <<
                                                "\nComplete non-exonic SNP infomation,\n";
                QThread::msleep(10);
                if (!funcAnnotator.complNonExoSnpInfo(exonicPosFile, snpPosFilePath, varFuncFilePath, nonExonicPosFile))
                {
                    throw -1;
                }

                 qDebug()<< QDateTime::currentDateTime().toString() <<
                                                "\nComplete functional annotation infomation,\n";
                QThread::msleep(10);
                if (!funcAnnotator.complFuncAnnoInfo(exonicPosFile, nonExonicPosFile, funcAnnoRefFilePath, funcAnnoResult))
                {
                    throw -1;
                }

                qDebug()<< QDateTime::currentDateTime().toString() <<
                                                "\nFunctional annotation OK\n"<<
                                                "\n" <<funcAnnoResult << "\n";
                QThread::msleep(10);


        } catch (runtime_error *re)
    {
        qDebug() <<  re->what();
        return false;
    }

    qApp->processEvents();

}
bool CommandLineParser::on_annoStepPushButton_clicked(QHash<QString, QString> args)
{

        qApp->processEvents();

        //从哈希表中取出各个命令的参数值
        QString phenotype = args["phenoFile"];
        QString map = args["mapFile"];
        QString covar = args["covarFile"];
        QString kinship = args["kinFile"];//亲缘矩阵文件 若用户没有输入亲缘矩阵 则这个值是空的
        QString out = args["out"];
        QString model = args["model"];
        QString maf = args["maf"];
        QString mind = args["mind"];
        QString geno = args["geno"];
        QString kinship_type = args["gemma_kinmat"];//这是亲缘矩阵的类型 1或2 用来-gk 1 或-gk 2 分别代表中心矩阵和标准矩阵
        QString lmmtest = args["lmmtest"];
        QString bslmmmodel =  args["bslmmmodel"];
        QString ProjectName = args["ProjectName"];

        QString vcfFile = args["genoFile"];

        QFileInfo vcfFileInfo(vcfFile);
        QString vcfFileAbPath = vcfFileInfo.absolutePath();
        QString vcfFileBaseName = vcfFileInfo.baseName();

        QString avinputFilePath = vcfFileAbPath + "/" + vcfFileBaseName + ".avinput";   // For input of structural annotaion
        QString snpPosFilePath = vcfFileAbPath + "/" + vcfFileBaseName + "_SNPpos";     // For input of functional annotation

        try {
            QString pvalFile = args["pvalueFile"];    // p-value file(the first column is SNP_ID and the last column is p-value)
            if (pvalFile.isNull() || pvalFile.isEmpty())
            {
                throw runtime_error("Error,A p-value file is necessary.");
                throw -1;
            }

            QFileInfo pvalFileInfo(pvalFile);
            QString pvalFileAbPath = pvalFileInfo.absolutePath();
            QString pvalFileBaseName = pvalFileInfo.baseName();

            QString thBase = args["thBase"];    // Threshold base number.
            QString thExpo = args["thExpo"];    // Threshold exponent.

            if (thBase.isEmpty() || thExpo.isEmpty())
            {
                throw runtime_error("Error,Please set the threshold.");
                throw -1;
            }

            QString sigSnpFile = pvalFileAbPath + "/" + pvalFileBaseName + "_sig";   // to save SNP after filter.

            QStringList snpIDList;
            FuncAnnotator funcAnnotator;

            qDebug()<< QDateTime::currentDateTime().toString()<<"\nFilter SNP below threshold,\n" ;

            QThread::msleep(10);
            if (!funcAnnotator.filterSNP(pvalFile, thBase, thExpo, sigSnpFile))
            {
                throw -1;
            }

            qDebug()<< QDateTime::currentDateTime().toString() <<"\nFilter SNP below threshold OK\n";
            QThread::msleep(10);

            qDebug()<<QDateTime::currentDateTime().toString() << "\nMake .avinput and get snp position,\n";
            QThread::msleep(10);

            if (!fileReader->makeAvinputAndSnpposFile(vcfFile, sigSnpFile, avinputFilePath, snpPosFilePath))
            {
                throw -1;
            }

             qDebug()<< QDateTime::currentDateTime().toString() <<
                                            "\nMake .avinput and get snp position OK.\n";

            if ( !checkoutExistence(snpPosFilePath) || !checkoutExistence(avinputFilePath))
            {
               throw runtime_error("snpPosFilePath or avinputFilePath don't exist ");
            }


        } catch (runtime_error *re)
        {
            qDebug() <<  re->what();
            return false;
        }

        qApp->processEvents();
        return 1;
}
bool CommandLineParser::isVcfFile(QString file) // Just consider filename.
{
    if (file.isNull() || file.isEmpty())
    {
        return false;
    }

    QFileInfo fileInfo = QFileInfo(file);
    QStringList fileNameList = fileInfo.fileName().split(".");         // demo.vcf.gz

    for (QString item : fileNameList)
    {
        if (item == "vcf")
        {
            return true;
        }
    }

    return false;
}
