#ifndef GRAPHVIEWER_H
#define GRAPHVIEWER_H

#include <QWidget>
#include <QPixmap>
#include <QGraphicsScene>
#include <QFileInfo>
#include <QMouseEvent>
#include <QDebug>

namespace Ui {
class GraphViewer;
}

class GraphViewer : public QWidget
{
    Q_OBJECT

public:
    explicit GraphViewer(QWidget *parent = nullptr);
    ~GraphViewer();

    void setGraph(QStringList graph);
    void setImgList(QStringList imgList);
    QStringList getImgList(void);
    QString getCurrentImg(void);

private:
    void loopChangeGraphInList(void);

protected slots:
    void mouseDoubleClickEvent(QMouseEvent *e) override;

signals:
    void doubleClicked();

private:
    Ui::GraphViewer *ui;
    QImage *image;
    QGraphicsScene *scene;
    QGraphicsView* view;
    QStringList imgList;
    QString curImage;
    void showEvent(QShowEvent *) override;
};

#endif // GRAPHVIEWER_H
