#ifndef EMMAXPARAMWIDGET_H
#define EMMAXPARAMWIDGET_H

#include <QWidget>
#include <QButtonGroup>
#include <QStringList>
#include <QMap>

namespace Ui {
class EmmaxParamWidget;
}

class EmmaxParamWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EmmaxParamWidget(QWidget *parent = nullptr);
    ~EmmaxParamWidget();

    bool isMakeKinAuto(void);
    bool isBNkinMatrix(void);
    bool isIBSkinMatrix(void);
    QMap<QString, QString> getCurrentParam(void);
    QString getCorrectionType(void);

private slots:
    void on_noKinRadioButton_clicked();
    void on_yesKinRadioButton_clicked();

private:
    Ui::EmmaxParamWidget *ui;

    QButtonGroup *kinAutoBtnGroup;
    QButtonGroup *kinMatrixBtnGroup;
    QButtonGroup *correctBtnGroup;
};

#endif // EMMAXPARAMWIDGET_H
