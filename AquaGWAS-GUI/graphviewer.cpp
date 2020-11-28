#include "graphviewer.h"
#include "ui_graphviewer.h"

GraphViewer::GraphViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphViewer)
{
    ui->setupUi(this);

//    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    ui->graphicsView->setCursor(Qt::PointingHandCursor);
//    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

//    this->zoomDelta = 0.1;  // Intiate data.
//    this->scaleData = 1.0;
//    this->translateSpeed = 1.0;
//    this->translateButton = Qt::LeftButton;

//    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);
//    setFixedSize(this->width(),this->height());

    image = new QImage;
    scene = new QGraphicsScene;
}

GraphViewer::~GraphViewer()
{
    delete ui;
    delete image;
    delete scene;
}

/**
 * @brief GraphViewer::loopChangeGraphInList
 *          Slider show graph in list.
 */
void GraphViewer::loopChangeGraphInList(void)
{
    int index = imgList.indexOf(this->getCurrentImg());
    if (imgList.length() <= 1 || index == -1)
    {
        return;
    }
    QString graph = imgList[(index+1)%imgList.length()];
    if(image->load(graph))
    {
        scene->addPixmap(QPixmap::fromImage(*image));
        ui->graphicsView->setScene(scene);
        ui->graphicsView->resize(image->width() + 10, image->height() + 10);
        this->resize(image->width() + 32, image->height() + 32);

        QFileInfo graphInfo(graph);
        QString baseName = graphInfo.baseName();
        this->setWindowTitle(baseName);
        ui->graphicsView->show();

        this->curImage = graph;
    }
}

/**
 * @brief GraphViewer::setGraph
 *          Set graphs which will be show.
 * @param graph
 */
void GraphViewer::setGraph(QStringList graph)
{
    if(image->load(graph[0]))
    {
        delete scene;
        scene = new QGraphicsScene;
        scene->addPixmap(QPixmap::fromImage(*image));
        ui->graphicsView->setScene(scene);
        ui->graphicsView->resize(image->width() + 10, image->height() + 10);
        this->resize(image->width() + 32, image->height() + 32);
        QFileInfo graphInfo(graph[0]);
        QString baseName = graphInfo.baseName();
        this->setWindowTitle(baseName);
        ui->graphicsView->show();

        this->imgList.clear();
        imgList.append(graph);
        curImage = graph[0];
    }
}

/**
 * @brief GraphViewer::showEvent        override showEvent function
 */
void GraphViewer::showEvent(QShowEvent *)
{
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
}

/**
 * @brief GraphViewer::mouseDoubleClickEvent        override mouseDoubleClickEvent function
 * @param e
 */
void GraphViewer::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
    {
        this->loopChangeGraphInList();
        emit doubleClicked();
    }
}

/**
 * @brief GraphViewer::getImgList
 * @return  current image list
 */
QStringList GraphViewer::getImgList(void)
{
    return this->imgList;
}

/**
 * @brief GraphViewer::getCurrentImg
 * @return current image being show
 */
QString GraphViewer::getCurrentImg(void)
{
    return curImage;
}

/**
 * @brief GraphViewer::setImgList   (will clear orginal list)
 * @param imgList
 */
void GraphViewer::setImgList(QStringList imgList)
{
    this->imgList.clear();
    this->imgList.append(imgList);
}

//void GraphViewer::mouseMoveEvent(QMouseEvent *event)
//{
//    if (this->isMouseTranslate){
//        QPointF mouseDelta = ui->graphicsView->mapToScene(event->pos()) - ui->graphicsView->mapToScene(this->lastMousePos);
//        translate(mouseDelta);
//    }

//    this->lastMousePos = event->pos();
//}

//void GraphViewer::translate(QPointF delta)
//{
//    delta *= this->scaleData;
//    delta *= this->translateSpeed;

//    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//    QPoint newCenter(ui->graphicsView->viewport()->rect().width() / 2 - delta.x(),
//                     ui->graphicsView->viewport()->rect().height() / 2 - delta.y());
//    ui->graphicsView->centerOn(ui->graphicsView->mapToScene(newCenter));

//    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
//}

//void GraphViewer::mousePressEvent(QMouseEvent *event)
//{
//    if (event->button() == this->translateButton) {
//        // 当光标底下没有 item 时才能移动
//        QPointF point = ui->graphicsView->mapToScene(event->pos());
//        if (ui->graphicsView->scene()->itemAt(point, ui->graphicsView->transform()) == NULL)  {
//            this->isMouseTranslate = true;
//            this->lastMousePos = event->pos();
//        }
//    }
//}

//void GraphViewer::mouseReleaseEvent(QMouseEvent *event)
//{
//    if (event->button() == this->translateButton)
//    {
//        this->isMouseTranslate = false;
//    }
//}

//void GraphViewer::zoom(float scaleFactor)
//{
//    qreal factor = ui->graphicsView->transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
//    if (factor < 0.07 || factor > 100)
//        return;

//    ui->graphicsView->scale(scaleFactor, scaleFactor);
//    this->scaleData *= scaleFactor;
//}

//void GraphViewer::zoomIn()
//{
//    zoom(1 + this->zoomDelta);
//}

//void GraphViewer::zoomOut()
//{
//    zoom(1 - zoomDelta);
//}

//void GraphViewer::wheelEvent(QWheelEvent *event)
//{
//    QPoint scrollAmount = event->angleDelta();
//    scrollAmount.y() > 0 ? zoomIn() : zoomOut();
//}
