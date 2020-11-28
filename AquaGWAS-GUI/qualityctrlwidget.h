#ifndef QUALITYCTRLWIDGET_H
#define QUALITYCTRLWIDGET_H

#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class QualityCtrlWidget;
}

class QualityCtrlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QualityCtrlWidget(QWidget *parent = nullptr);
    ~QualityCtrlWidget();

    bool isLinkageFilterNeeded();
    void getLinkageFilterType(QString &winSize, QString &stepLen, QString &r2Threshold);

private slots:
    void on_noLinkageFilterRadioButton_clicked();
    void on_yesLinkageFilterRadioButton_clicked();

private:
    Ui::QualityCtrlWidget *ui;

    QButtonGroup *linkageFileterBtnGroup;
};

#endif // QUALITYCTRLWIDGET_H
