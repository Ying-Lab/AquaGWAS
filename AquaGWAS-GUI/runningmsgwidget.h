#ifndef RUNNINGMSGWIDGET_H
#define RUNNINGMSGWIDGET_H

#include <QDebug>
#include <QWidget>
#include <QCloseEvent>
#include <QTextCursor>
#include <QMessageBox>

namespace Ui {
class RunningMsgWidget;
}

class RunningMsgWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RunningMsgWidget(QWidget *parent = nullptr);
    ~RunningMsgWidget();
    void setText(QString text);
    void appendText(QString text);
    void clearText(void);
    void setTitle(QString title);
    void refreshLastLine(QString line);
    QString getText(void);


private:
    Ui::RunningMsgWidget *ui;
    // To stop process.
    void closeEvent(QCloseEvent *event) override;
//    bool eventFilter(QObject *object, QEvent *event);

signals :
    void closeSignal(void);

protected slots:
    void on_appendText(QString text);
};

#endif // RUNNINGMSGWIDGET_H
