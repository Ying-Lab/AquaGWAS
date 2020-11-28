#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QFileDialog>
#include <QAbstractItemView>
#include <QProcess>
#include <QDebug>
#include <QList>
#include <QThread>
#include <QMap>
#include <QButtonGroup>
#include <QDateTime>
#include <QThread>
#include <QtConcurrent>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include "mtoolbutton.h"
#include "filereader.h"
#include "workdirectory.h"
#include "phenoselector.h"
#include "plink.h"
#include "emmax.h"
#include "gemma.h"
#include "gcta.h"
#include "poplddecay.h"
#include "annovar.h"
#include "runningmsgwidget.h"
#include "gemmaparamwidget.h"
#include "emmaxparamwidget.h"
#include "qualityctrlwidget.h"
#include "graphviewer.h"
#include "funcannotator.h"
#include "process.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void terminateProcess();
    void runningMsgWidgetAppendText(QString text);
    void setLineEditTextSig(QLineEdit *, QString);
    void setButtonEnabledSig(QAbstractButton *, bool);
    void setGraphViewerGraphSig(QStringList plot);
    void resetWindowSig();
    void setMsgBoxSig(const QString &title, const QString &text);

private slots:
    // for FileReader
    void on_pheFileToolButton_clicked();
    void on_genoFileToolButton_clicked();
    void on_mapFileToolButton_clicked();
    void on_covarFileToolButton_clicked();
    void on_kinFileToolButton_clicked();
    //  right click
    void on_pheFileToolButton_closeFileSig();
    void on_genoFileToolButton_closeFileSig();
    void on_mapFileToolButton_closeFileSig();
    void on_kinFileToolButton_closeFileSig();
    void on_covarFileToolButton_closeFileSig();
    // for WorkDirectory
    void on_outdirBrowButton_clicked();
    void on_projectNameLineEdit_textChanged(const QString &text);
    void on_projectNameLineEdit_editingFinished();
    void on_outdirLineEdit_editingFinished();
    // for PhenoSelector
    void on_excludeAllPhenoButton_clicked();
    void on_selectAllPhenoButton_clicked();
    void on_selectPhenoButton_clicked();
    void on_excludePhenoButton_clicked();
    // for run pushButton
    void on_runGwasButton_clicked();
    // Close running widget
    void on_closeRunningWidget();
    // For tool and model selector
    void on_toolComboBox_currentTextChanged(const QString &tool);   
    // Detail params.
    void on_detailPushButton_clicked();   
    // Plot QQ_Manhattan
    void on_drawManPushButton_clicked();
    void on_drawQQPushButton_clicked();
    void on_qqmanGwasReultBrowButton_clicked();
    // Linkage disequilibrium
    void on_ldRunPushButton_clicked();
//    void on_ldPlotPushButton_clicked();
    // Graph viewer
    void graphViewer_clicked_slot();    // on_*** is implicit slot function
    // Structural annotation and Functional annotation
    void on_refGeneFileBrowButton_clicked();
    void on_refSeqFileBrowButton_clicked();
    void on_avinFileBrowButton_clicked();   
    void on_funcAnnoRefFileBrowButton_clicked();
    void on_snpPosFileFileBrowButton_clicked();
    void on_annoPvalBrowButton_clicked();
    void on_annotationRunButton_clicked();
    void on_annoStepPushButton_clicked();
    // PCA
    void on_pcaRunPushButton_clicked();
    // Quality control
    void on_qualCtrlDetailPushButton_clicked();
    // For my Process
    void on_outMessageReady(QString text);
    void on_errMessageReady(QString text);
    // To set LineEdit text in another thread
    void on_setLineEditText(QLineEdit *lineEdit, QString text);
    void on_setButtonEnabled(QAbstractButton *Button, bool boolean);
    void on_setGraphViewerGraph(QStringList plot);
    // Reset Windw SLOT
    void on_resetWindowSig();
    // Set message box
    void on_setMsgBoxSig(const QString &title, const QString &text);
    // for generate cmdline Button
   void cmdGWASButton_clicked();
   void pca_ld_cmdButton_clicked();
   void annotationCmdButton_clicked();

private:
    Ui::MainWindow *ui;
    volatile bool runningFlag = false;   // A flag t judge there running project.
    void closeEvent(QCloseEvent *event) override;
    //  Tool and script path;
#ifdef QT_NO_DEBUG
    QString toolpath = QCoreApplication::applicationDirPath() + "/tools/";    // Realease
    QString scriptpath = QCoreApplication::applicationDirPath() + "/script/";
#else
    QString toolpath = "/home/zhi/Desktop/AquaGWAS-master/code/tools/";
    QString scriptpath = "/home/zhi/Desktop/AquaGWAS-master/code/script/";
#endif
    // Basic association parameters.
    FileReader *fileReader;             // Read file, and save the directory.
    WorkDirectory *workDirectory;       // Set and save the module name and the output directory.
    PhenoSelector *phenoSelector;       // Select objective phenotype to analysis.
    // for display running information.
    RunningMsgWidget *runningMsgWidget;
    // To set detail paramters
    GemmaParamWidget *gemmaParamWidget;
    EmmaxParamWidget *emmaxParamWidget;
    // Graph Viewer
    GraphViewer * graphViewer;
    // LD by family.
    QButtonGroup *ldByFamGroupButton;
    // Quality Control parameters more.
    QualityCtrlWidget *qualityControl;

    // Running process(External process)
    QList<QFuture<void>> concurRunningList;

    // Usual functions.
    bool isVcfFile(QString const file);
    void resetWindow(void);
    bool callPlinkGwas(QString phenotype, QString genotype, QString map,
                       QString covar, QString kinship, QString out, QString name);
    bool callEmmaxGwas(QString phenotype, QString genotype, QString map,
                       QString covar, QString kinship, QString out, QString name);
    bool callGemmaGwas(QString phenotype, QString genotype, QString map,
                       QString covar, QString kinship, QString out, QString name);
    bool makePheFile(QString const phenotype, QString const selectedPheno);
    bool pValCorrect(QString pvalFile, bool header, QString correctType, QString outFile);
    QString refreshMessage(QString curMsg, QString newMsg);
    bool drawManhattan(QStringList data, QStringList out);
    bool drawQQplot(QStringList data, QStringList out);
    bool ldPlot(QStringList ldResultFileList);
    QStringList makeQQManInputFile(QString pvalueFile);
    void runPopLDdecaySingle(void);
    void runPopLDdecaybyFamily(void);
    bool runExTool(QString tool, QStringList param);
    // Checkout file existence.
    bool checkoutExistence(QString filePath);
    void structuralAnnotation(QString avinputFilePath, QString refGeneFilePath,
                              QString refFastaFilePath, QString outFilePath);
    void functionalAnnotation(QString snpPosFilePath, QString varFuncFilePath,
                              QString exVarFuncFilePath, QString funcAnnoBase);
    void addFilesExecutePermission(QString dir);
};
#endif // MAINWINDOW_H
