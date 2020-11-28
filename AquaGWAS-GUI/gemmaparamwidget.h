#ifndef GEMMAPARAMWIDGET_H
#define GEMMAPARAMWIDGET_H

#include <QWidget>
#include <QButtonGroup>
#include <QStringList>
#include <QMap>

namespace Ui {
class GemmaParamWidget;
}

class GemmaParamWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GemmaParamWidget(QWidget *parent = nullptr);
    ~GemmaParamWidget();

    bool isMakeRelatedMatAuto(void);
    bool isCentRelatedMat(void);
    bool isStdRelatedMat(void);
    bool isWaldTest(void);
    bool isLikelihoodRatioTest(void);
    bool isScoreTest(void);
    bool isAllTest(void);
    bool isStdLinearBSLMM(void);
    bool isRidgeRegreBSLMM(void);
    bool isProbitBSLMM(void);
    bool isFamCompletedAuto(void);
    QMap<QString, QString> getCurrentParam(void);
    QString getCorrectionType(void);

    void setLmmParamEnabled(bool boolean);
    void setBslmmParamEnabled(bool boolean);

private slots:
    void on_noKinRadioButton_clicked();
    void on_yesKinRadioButton_clicked();

private:
    Ui::GemmaParamWidget *ui;

    QButtonGroup *kinAutoBtnGroup;
    QButtonGroup *kinMatrixBtnGroup;
    QButtonGroup *lmmTestBtnGroup;
    QButtonGroup *bslmmModelBtnGroup;
    QButtonGroup *famCompleteBtnGroup;
    QButtonGroup *correctBtnGroup;
};

#endif // GEMMAPARAMWIDGET_H
