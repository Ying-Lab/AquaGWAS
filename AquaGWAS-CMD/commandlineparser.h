#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <QObject>
#include <QProcess>
#include <QFileInfo>
#include <QException>
#include <QHash>
#include <QThread>
#include "plink.h"
#include "gemma.h"
#include "emmax.h"
#include "gcta.h"
#include "annovar.h"
#include "funcannotator.h"
#include "filereader.h"
#include <iostream>
#include <QDir>
#include <QDateTime>
#include "poplddecay.h"
using namespace std;

class CommandLineParser :  public QCommandLineParser  //类的定义 继承自qcommandlineparser类
{
public:
    CommandLineParser();  //构造函数
    ~CommandLineParser(); //析构函数
    void parseCommand();
    void delCmdLineOption();
    void initCmdLineOption();
    void addFilesExecutePermission(QString directory);// add permission
    void runGWAS(QHash<QString, QString> args);
    void runPCA(QHash<QString,QString > args);        //yangzizi
    bool callGctaPca(QHash<QString, QString> args);    //yangzizi
    bool plotPca(QHash<QString, QString> args);        //yangzizi

    bool callPlinkGwas(QHash<QString, QString> args);
    bool callGemmaGwas(QHash<QString, QString> args);
    bool callEmmaxGwas(QHash<QString, QString> args);
    QHash<QString, QString> getArgsFromCmd();
    bool isVcfFile(QString file);
    void on_ldRunPushButton_clicked(QHash<QString, QString> argHash);
    void runPopLDdecaySingle(QHash<QString, QString> argHash);
    void runPopLDdecaybyFamily(QHash<QString, QString> argHash);
    void on_ldPlotPushButton_clicked(QHash<QString, QString> args,QString ldResultFile);
    bool checkoutExistence(QString filePath);


    //LINRENHAO
    void on_drawManPushButton_clicked(QHash<QString, QString> args, QString gwasResulFile);
    QStringList makeQQManInputFile(QString pvalueFile);
    bool drawManhattan(QHash<QString, QString> args, QStringList data, QStringList out);
    bool drawQQplot(QStringList data, QStringList out);
    void on_drawQQPushButton_clicked(QString gwasResulFile);

    //annotation part
    //zz
    bool structuralAnnotation(QHash<QString, QString> args);
    bool on_annoStepPushButton_clicked(QHash<QString, QString> args);
    bool functionalAnnotation(QHash<QString, QString> args);

private:
    FileReader *fileReader;
    // Association analysis arguments.
    QCommandLineOption *assocCmdOp; //关联分析命令
    QCommandLineOption *toolCmdOp;
    QCommandLineOption *modelCmdOp; //选择什么模型 如gemma有LMM和BSLMM模型
    QCommandLineOption *outCmdOp;
    QCommandLineOption *phenoFileCmdOp;
    QCommandLineOption *genoFileCmdOp;
    QCommandLineOption *mapFileCmdOp;
    QCommandLineOption *covarFileCmdOp;
    QCommandLineOption *kinFileCmdOp;//亲缘矩阵文件（如果有的话）
    QCommandLineOption *mafCmdOp;
    QCommandLineOption *genoCmdOp;
    QCommandLineOption *mindCmdOp;

    // gemma detail parameters
    QCommandLineOption *gemma_makeKinCmdOp;//表示要不要生成亲缘矩阵
    QCommandLineOption *gemma_kinMatCmdOp;//选择亲缘矩阵的类型，有1和2两个选项分别对应 -gk 1 -gh 2
    QCommandLineOption *gemma_lmmTestCmdOp; // LMM模型的参数 lmm可以选1-4，一般选1 Wald检验，4为全选
    QCommandLineOption *gemma_bslmmModelCmdOp;//BSLMM模型的参数 如 -bslmm 1   -bslmm 2
    QCommandLineOption *gemma_famCompleCmdOp; //该选项表示要不要改造fam文件
    // LD detail parameters
    QCommandLineOption *LD_CmdOp;//LD分析命令
    QCommandLineOption *LD_analysisCmdOp;//选择family还是single对应界面里yes/no
    QCommandLineOption *ProjectNameCmdOp;//对应GUI界面里的项目名称pro1
    QCommandLineOption *LD_plot;//对应GUI界面里plot的按钮
    // emmax detail parameters              // linrenhao
    QCommandLineOption *emmax_makeKinCmdOp;
    QCommandLineOption *emmax_kinMatCmdOp;
    //Man/QQ plot parameters
    QCommandLineOption *gwBase ;
    QCommandLineOption *gwExpo ;
    QCommandLineOption *sgBase ;
    QCommandLineOption *sgExpo ;

    // gcta detail parameters
   QCommandLineOption *gcta_threadsCmdOp;
   QCommandLineOption *gcta_PCsCmdOp;
   QCommandLineOption *pcaCmdOp;
   //structure and functional ANNO detail parameters
   QCommandLineOption *strucAnnoCmdOp;
   QCommandLineOption *gffFileCmdOp;
   QCommandLineOption *fastaFileCmdOp;
   QCommandLineOption *avinputCmdOp;
   QCommandLineOption *funcAnnoCmdOp;
   QCommandLineOption *stepAnnoCmdOp;
   QCommandLineOption *pvalFileCmdOp;
   QCommandLineOption *thBase ;
   QCommandLineOption *thExpo ;
   QCommandLineOption *snp_pos ;
   QCommandLineOption *funcAnnoRef;
   QCommandLineOption *var;
   QCommandLineOption *exvar;


    // !!! TAKE CARE !!!
#ifdef QT_NO_DEBUG
    QString toolpath = QCoreApplication::applicationDirPath() + "/tools/";    // Realease
    QString scriptpath = QCoreApplication::applicationDirPath() + "/script/";
#else
    QString toolpath = "/home/zhi/Desktop/H_G_CMD_new/code/tools/";  //!!attention! 这里最后一个要加个"/"不然跑不起来
    QString scriptpath = "/home/zhi/Desktop/H_G_CMD_new/code/script/";
#endif
    QProcess *proc;

};

#endif // COMMANDLINEPARSER_H
