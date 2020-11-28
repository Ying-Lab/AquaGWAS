#ifndef MTOOLBUTTON_H
#define MTOOLBUTTON_H

#include <QObject>
#include <QToolButton>
#include <QAction>
#include <QList>
#include <QMenu>
#include <QAction>

class MToolButton : public QToolButton
{

    Q_OBJECT
public:
    explicit MToolButton(QWidget *parent = nullptr);
    void setShowMenuFlag(bool b);
    bool isShowMenuOK(void);

private:
    QMenu *menu;
    QAction *closeAction;
    bool showMenuFlag;

protected slots:
    void showMenu(const QPoint &point);
    void on_closeAction(void);
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void rightClicked();
    void doubleClicked();
    void closeFileSig();
    void showMenuSig();
};

#endif // MTOOLBUTTON_H
