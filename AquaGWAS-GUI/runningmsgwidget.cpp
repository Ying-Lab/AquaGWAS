#include "runningmsgwidget.h"
#include "ui_runningmsgwidget.h"

RunningMsgWidget::RunningMsgWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RunningMsgWidget)
{
    ui->setupUi(this);
    this->installEventFilter(this);
    this->setTitle("Running Messages");
}

RunningMsgWidget::~RunningMsgWidget()
{
    delete ui;
}

/**
 * @brief RunningMsgWidget::setText
 *          (Will clear original data)
 * @param text
 */
void RunningMsgWidget::setText(QString text)
{
    if (!this->isVisible())
    {
        this->show();
    }
    ui->textBrowser->setText(text);
    ui->textBrowser->moveCursor(QTextCursor::End);  // Move cursor to end.
}

/**
 * @brief RunningMsgWidget::appendText
 *          (Will not clear original text)
 * @param text: new text
 */
void RunningMsgWidget::appendText(QString text)
{
    if (!this->isVisible())
    {
        this->show();
    }
    ui->textBrowser->append(text);
    ui->textBrowser->moveCursor(QTextCursor::End);
}

void RunningMsgWidget::clearText(void)
{
    if (this->isVisible())
    {
        this->hide();
    }
    ui->textBrowser->clear();
}

void RunningMsgWidget::setTitle(QString title)
{
    this->setWindowTitle(title);
}

/**
 * @brief RunningMsgWidget::getText
 * @return current text in widget.
 */
QString RunningMsgWidget::getText(void)
{
    return ui->textBrowser->toPlainText();
}

/**
 * @brief RunningMsgWidget::closeEvent
 *          overrider closeEvent function.
 * @param event
 */
void RunningMsgWidget::closeEvent(QCloseEvent *event)
{
    if (this->isVisible())
    {
        emit this->closeSignal();
    }
    event->ignore();
}

void RunningMsgWidget::on_appendText(QString text)
{
    this->appendText(text);
}
