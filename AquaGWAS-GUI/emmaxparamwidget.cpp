#include "emmaxparamwidget.h"
#include "ui_emmaxparamwidget.h"

EmmaxParamWidget::EmmaxParamWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmmaxParamWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("Emmax settings");

    // Create button group and set exclusive.
    kinAutoBtnGroup = new QButtonGroup;
    kinMatrixBtnGroup = new QButtonGroup;
    correctBtnGroup = new QButtonGroup;
    kinAutoBtnGroup->addButton(ui->yesKinRadioButton);
    kinAutoBtnGroup->addButton(ui->noKinRadioButton);
    kinAutoBtnGroup->setExclusive(true);
    kinMatrixBtnGroup->addButton(ui->bnRadioButton);
    kinMatrixBtnGroup->addButton(ui->ibsRadioButton);
    kinMatrixBtnGroup->setExclusive(true);
    correctBtnGroup->addButton(ui->noCorrectRadioButton);
    correctBtnGroup->addButton(ui->stepDownCorrectRadioButton);
    correctBtnGroup->addButton(ui->fdrCorrectRadioButton);
    correctBtnGroup->addButton(ui->bonfCorrectRadioButton);
    correctBtnGroup->setExclusive(true);

    // Set default paramters.
    ui->yesKinRadioButton->setChecked(true);
    ui->bnRadioButton->setChecked(true);
    ui->noCorrectRadioButton->setChecked(true);
}

EmmaxParamWidget::~EmmaxParamWidget()
{
    delete ui;
    delete kinAutoBtnGroup;
    delete kinMatrixBtnGroup;
}

/**
 * @brief EmmaxParamWidget::isMakeKinAuto
 * @return
 */
bool EmmaxParamWidget::isMakeKinAuto(void)
{
    if (!ui->yesKinRadioButton->isChecked())
    {
        return false;
    }
    return true;
}

bool EmmaxParamWidget::isBNkinMatrix(void)
{
    if (!ui->bnRadioButton->isChecked())
    {
        return false;
    }
    return true;
}

bool EmmaxParamWidget::isIBSkinMatrix(void)
{
    if (!ui->ibsRadioButton->isChecked())
    {
        return false;
    }
    return true;
}

/**
 * @brief EmmaxParamWidget::getCurrentParam
 * @return A QStringList contain the values in the widget.
 *      QMap retParam: "makekin", "kinmatrix"
 *
 */
QMap<QString, QString> EmmaxParamWidget::getCurrentParam(void)
{
//    QStringList ret;
    QMap<QString, QString> retParam;

    if (this->isMakeKinAuto())
    {
        retParam.insert("makekin", "yes");
    }
    else
    {
        retParam.insert("makekin", "no");
    }

    if (this->isBNkinMatrix())
    {
        retParam.insert("kinmatrix", "BN");
    }
    else
    {
        retParam.insert("kinmatrix", "IBS");
    }

    return retParam;
}

/**
 * @brief EmmaxParamWidget::on_noKinRadioButton_clicked
 *          Disable radioButton to choose type of kinship matrix.
 */
void EmmaxParamWidget::on_noKinRadioButton_clicked()
{
    ui->ibsRadioButton->setEnabled(false);
    ui->bnRadioButton->setEnabled(false);
}

/**
 * @brief EmmaxParamWidget::on_yesKinRadioButton_clicked
 *          Enable radioButton to choose type of kinship matrix.
 */
void EmmaxParamWidget::on_yesKinRadioButton_clicked()
{
    ui->ibsRadioButton->setEnabled(true);
    ui->bnRadioButton->setEnabled(true);
}

QString EmmaxParamWidget::getCorrectionType()
{
    if (ui->noCorrectRadioButton->isChecked())
    {
        return nullptr;
    }
    else if (ui->stepDownCorrectRadioButton->isChecked())
    {
        return "holm";
    }
    else if (ui->fdrCorrectRadioButton->isChecked())
    {
        return "BH";
    }
    else if (ui->bonfCorrectRadioButton)
    {
        return "bonferroni";
    }
    else
    {
        return nullptr;
    }
}
