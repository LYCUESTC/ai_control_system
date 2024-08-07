#include "GraphicsView.h"

GraphicsView::GraphicsView(QWidget*parent)
	: QGraphicsView(parent)
    , isDrawROI(true)
{
    m_scene = new QGraphicsScene();
    m_PixmapItem = new QGraphicsPixmapItem();
    this->setScene(m_scene);
    m_scene->addItem(m_PixmapItem);

    setMouseTracking(true);  //为false时需要点击下鼠标才触发鼠标移动事件

    m_scaledValue = 1; //缩放倍数

    //以鼠标为中心进行缩放
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    m_drawROI = new DrawROI(this);

    m_pOptMenu = new QMenu(this);
    m_pDelAction = new QAction("del", this);
    connect(m_pDelAction, &QAction::triggered, m_drawROI, &DrawROI::DeleteCurrentROI);
    m_pSaveAction = new QAction("save", this);
    connect(m_pSaveAction, &QAction::triggered, m_drawROI, &DrawROI::saveAllROIImage);

    m_pOptMenu->addAction(m_pDelAction);
    m_pOptMenu->addAction(m_pSaveAction);

    m_graphicsViewTool = new GraphicsViewTool(this, m_scene, m_PixmapItem);
    m_drawROI->isDrawMultipleROIs(false);  //是否允许画多个矩形
}

GraphicsView::~GraphicsView()
{
    delete m_scene;
}
void GraphicsView::clear_all()
{
    m_drawROI->DeleteAllROIS();
}
std::map<int, QRect> GraphicsView::getRoi()
{
    return m_drawROI->getRoiRect();
}

QImage GraphicsView::GetImage()
{
    return m_image;
}

void GraphicsView::SetPix(QPixmap pix)
{
    this->pix = pix;
}

qreal GraphicsView::GetScaledValue()
{
    return m_scaledValue;
}

//鼠标按下事件
void GraphicsView::mousePressEvent(QMouseEvent* event)
{
    //界面宽高为this->width(), this->height()，不是this->viewport()->width()和this->viewport()->height()，但需要很多测试验证
    qreal viewWidth = this->viewport()->width(); //859
    qreal viewHeight = this->viewport()->height(); //517
    qreal fviewWidth = this->width(); //882
    qreal fviewHeight = this->height(); //519
    qreal pviewWidth  = m_graphicsViewTool->GetCurrentPix().width(); //778
    qreal pviewHeight = m_graphicsViewTool->GetCurrentPix().height(); //519

    //鼠标左键按下
    if (event->button() == Qt::LeftButton) {
        //视图坐标
        QPoint nViewPoint = event->pos();
        //视图坐标小数类型
        QPointF fViewPoint = event->pos();  //QPointF转QPoint  QPoint = QPointF.toPoint();
        //场景坐标
        QPointF fScenePoint = this->mapToScene(nViewPoint);
        //图形项坐标
        QGraphicsItem* pItem = this->scene()->itemAt(fScenePoint, QTransform());
        
        if (NULL != pItem)
        {
            QPointF fItemPoint = pItem->mapFromScene(fScenePoint);   //获取图形项坐标
            //图元转视图
            QPointF fViewPoint1 = this->mapFromScene(fItemPoint.x(), fItemPoint.y());
            int a = 0;
        }

        m_drawROI->changeROIBegin(nViewPoint);
    }

    //子类重写父类的mousePressEvent事件，在执行mousePressEvent的时候只会执行子类重写的功能，如果想要用到父类的其他功能需要调用
    //父类mousePressEvent按下事件
    QGraphicsView::mousePressEvent(event);

    
}

void GraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        //矩形绘制结束
        m_drawROI->changeROIEnd(event->pos());
    }

    QGraphicsView::mouseReleaseEvent(event);

}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        //isDoubleClick = true;

    }

    QGraphicsView::mouseDoubleClickEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    //鼠标按住左键移动
    if (event->buttons() & Qt::LeftButton) {
        //绘制矩形时鼠标移动处理
        m_drawROI->changeROIInProgress(event->pos());
    }
    //设置鼠标形状
    m_drawROI->changeMouseShape(event->pos());
    
    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::keyPressEvent(QKeyEvent* event)
{
    //直接用父类的键盘按下事件所有功能
    QGraphicsView::keyPressEvent(event);
    if (event->key() == Qt::Key_Delete)
    {
        m_drawROI->DeleteCurrentROI();
        this->viewport()->update();
    }
}

//窗口变化、显隐、重新布局、调用update/repaint(重画)、窗口第一次显示、窗口组件被遮挡又显示等触发paintEvent
void GraphicsView::paintEvent(QPaintEvent* event)
{
    //调用父类绘画功能
    QGraphicsView::paintEvent(event);

    //画roi
    m_drawROI->drawROI();
    
}

/**
 * @brief RectView::paintEvent
 *     双缓冲绘图，原理是在拖动过程中先把原来的图形复制到tempPix里面并在tempPix里面画，
 * 我们此时看到的就是在tempPix里的图形。只在鼠标释放的时候才在pix绘一次。
 */
//void GraphicsView::paintEvent(QPaintEvent* event)
//{
//    
//    QPainter painter(this->viewport());
//    if (isDrawing)
//    {
//        tempPix = pix;
//        QPainter pp(&tempPix);
//        pp.drawLine(lasetPoint, endPoint);
//        painter.drawPixmap(0, 0, tempPix);
//
//    }
//    else
//    {
//        QPainter pp(&pix);
//        pp.drawLine(lasetPoint, endPoint);
//        painter.drawPixmap(0, 0, pix);
//    }
//
//    QGraphicsView::paintEvent(event);
//  
//}

//界面大小发生变化会调用该事件
void GraphicsView::resizeEvent(QResizeEvent* event)
{
    m_graphicsViewTool->SetPixMiddle(m_image);
    m_drawROI->SetBackPix(m_graphicsViewTool->GetCurrentPix());
}

void GraphicsView::wheelEvent(QWheelEvent* ev)
{
    //放大图元
    m_graphicsViewTool->ScaleImage(ev);
    //图元放大，在图元上画的矩形不会放大，下面方法是让画的矩形和图元同比例放大，这里的画框是用QPainter方式，画出的东西不会随QGraphicsItem图元改变而改变
    //如果在图元上依然用QGraphicsItem图元来画框，那他们会一起改变，自动同比例放大缩小，因为都是属于QGraphics系列
    m_drawROI->ScaleROIS(ev, m_graphicsViewTool->GetScale());
}

void GraphicsView::contextMenuEvent(QContextMenuEvent* ev)
{
    QPoint mousePos = ev->pos();

    //QRect.contains被给的点在矩形内部返回true
    //场景坐标
    QPointF fScenePoint = this->mapToScene(mousePos);
    //图形项坐标
    QGraphicsItem* pItem = this->scene()->itemAt(fScenePoint, QTransform());

    if (NULL != pItem)
    {
        QPointF fItemPoint = pItem->mapFromScene(fScenePoint);   //获取图形项坐标
        QPoint itemPoint(fItemPoint.x(), fItemPoint.y());

        for (int i = 0; i < m_drawROI->getRoiRect().size(); i++)
        {
            if (m_drawROI->getRoiRect()[i].contains(itemPoint))
            {
                m_pOptMenu->exec(QCursor::pos());
                break;
            }
        }
    }
    ev->accept();
}

void GraphicsView::rotate()
{
    // boundingRect得到图像轮廓的最小正矩形， minAreaRect得到图像轮廓的最小斜矩形

    //QPointF centerPos = m_PixmapItem->boundingRect().center(); //获取图像中心坐标
    //m_PixmapItem->setTransformOriginPoint(centerPos.x(), centerPos.y()); //设置QGraphicsPixmapItem旋转点，旋转QGraphicsPixmapItem不是图像，图像放在QGraphicsPixmapItem里面
    //m_PixmapItem->setRotation(90); //设置QGraphicsPixmapItem旋转度数

    //第二种方法就是直接用opencv旋转图像

    //图像旋转后会跑到view的左边或者右边，下面代码在setPixMap后加setSceneRect保证图像在view中心旋转
    //m_PixmapItem->setPixmap(QPixmap::fromImage(image).scaled(s, Qt::KeepAspectRatio));
    //m_Scene->setSceneRect(m_PixmapItem->boundingRect()); //Image center rotation
}

void GraphicsView::setBackImage(QImage img)
{
    m_image = img;
    int width = img.width();
    int height = img.height();

    QSize s = QSize(width * 0.5, height*0.5);//this->size();
    //保持纵横比后图像在视图中心，但获取m_PixmapItem坐标依然是(0, 0)，注意这是图像和scene的左上角坐标，如果想要视图坐标需要转化
    QPixmap pix = QPixmap::fromImage(m_image).scaled(s, Qt::KeepAspectRatio);
    m_PixmapItem->setPixmap(pix);

    //QGraphicsRectItem* item = new QGraphicsRectItem();
    //item->setRect(0, 0, 80, 80);
    //item->setFlag(QGraphicsItem::ItemIsMovable);
    //m_scene->addItem(item);
}

//void GraphicsView::drawRect1()
//{
//    QGraphicsItem* item = drawControl->createDrawObject(Polygons::rect);
//    drawControl->draw_Rect(QRect(0, 0, 100, 100), color::blue, item);
//
//}
