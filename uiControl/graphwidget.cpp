#include "graphwidget.h"
#include "edge.h"
#include "node.h"

#include <math.h>

#include <QKeyEvent>
#include <QRandomGenerator>

//! [0]
GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent), timerId(0)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-200, -200, 400, 400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(1), qreal(1));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Machine Map"));

    Node *stNode = new Node(this);
    Node *spaNode = new Node(this);

    Node *sut1Node = new Node(this);
    Node *sut2Node = new Node(this);

    Node *aa1Node = new Node(this);
    Node *aa2Node = new Node(this);

    Node *lutNode = new Node(this);

    Node *lpaNode = new Node(this);

    scene->addItem(stNode);
    scene->addItem(spaNode);
    scene->addItem(sut1Node);
    scene->addItem(sut2Node);
    scene->addItem(aa1Node);
    scene->addItem(aa2Node);
    scene->addItem(lutNode);
    scene->addItem(lpaNode);

    scene->addItem(new Edge(stNode, spaNode));
    scene->addItem(new Edge(spaNode, sut1Node));
    scene->addItem(new Edge(spaNode, sut2Node));
    scene->addItem(new Edge(sut1Node, aa1Node));
    scene->addItem(new Edge(sut2Node, aa2Node));
    scene->addItem(new Edge(aa1Node, lutNode));
    scene->addItem(new Edge(aa2Node, lutNode));
    scene->addItem(new Edge(lutNode, lpaNode));

    stNode->setPos(100, -150);
    spaNode->setPos(100, -100);
    sut1Node->setPos(70, -50);
    sut2Node->setPos(130, -50);
    aa1Node->setPos(70, 0);
    aa2Node->setPos(130, 0);
    lutNode->setPos(100, 50);
    lpaNode->setPos(100, 100);
}
//! [1]

//! [2]
void GraphWidget::itemMoved()
{
//    if (!timerId)
//        timerId = startTimer(2000);
}
//! [2]

//! [3]
void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        centerNode->moveBy(0, -20);
        break;
    case Qt::Key_Down:
        centerNode->moveBy(0, 20);
        break;
    case Qt::Key_Left:
        centerNode->moveBy(-20, 0);
        break;
    case Qt::Key_Right:
        centerNode->moveBy(20, 0);
        break;
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        shuffle();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}
//! [3]

//! [4]
void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

   QList<Node *> nodes;
   foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
   }

    foreach (Node *node, nodes){
        node->updateState();
    }

//    bool itemsMoved = false;
//    foreach (Node *node, nodes) {
//        if (node->advancePosition())
//            itemsMoved = true;
//    }

//    if (!itemsMoved) {
//        killTimer(timerId);
//        timerId = 0;
//    }
}
//! [4]

#if QT_CONFIG(wheelevent)
//! [5]
void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}
//! [5]
#endif

//! [6]
void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);

    // Text
    QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
                    sceneRect.width() - 4, sceneRect.height() - 4);
    QString message(tr(""));

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(10);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2, 2), message);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);

    //Sensor Pickarm
    QRectF spaRect(-25, -150, 30, 30);
    painter->drawRect(spaRect);
    painter->drawText(QRectF(spaRect.left(), spaRect.top() - 15,
                             spaRect.width() + 15, spaRect.height() -4), tr("SPA"));

    //Sensor Tray
    QRectF stRect(-75, -150, 30, 30);
    painter->drawRect(stRect);
    painter->drawText(QRectF(stRect.left(), stRect.top() - 15,
                             stRect.width() + 15, stRect.height() -4), tr("ST"));

    //Lens Pickarm
    QRectF lpaRect(-25, 150, 30, 30);
    painter->drawRect(lpaRect);
    painter->drawText(QRectF(lpaRect.left(), lpaRect.top() - 15,
                             lpaRect.width() + 15, lpaRect.height() -4), tr("LPA"));
    //Lens Tray
    QRectF ltRect(-75, 150, 30, 30);
    painter->drawRect(ltRect);
    painter->drawText(QRectF(ltRect.left(), ltRect.top() - 15,
                             ltRect.width() + 15, ltRect.height() -4), tr("LT"));

    //SUT 1
    QRectF sut1Rect(-150, -100, 30, 30);
    painter->drawRect(sut1Rect);
    painter->drawText(QRectF(sut1Rect.left(), sut1Rect.top() - 15,
                             sut1Rect.width() + 15, sut1Rect.height() -4), tr("SUT_1"));
    //SUT 2
    QRectF sut2Rect(-50, -100, 30, 30);
    painter->drawRect(sut2Rect);
    painter->drawText(QRectF(sut2Rect.left(), sut2Rect.top()-15,
                             sut2Rect.width() + 15, sut2Rect.height() - 4), tr("SUT_2"));

    //LUT
    QRectF lutRect(-25, 100, 30, 30);
    painter->drawRect(lutRect);
    painter->drawText(QRectF(lutRect.left(), lutRect.top()-15,
                             lutRect.width() + 15, lutRect.height() - 4), tr("LUT"));
    //AA 1
    QRect aa1LensRect(-125, 50, 30, 30);
    painter->drawRect(aa1LensRect);
    painter->drawText(QRectF(aa1LensRect.left(), aa1LensRect.top()-15,
                             aa1LensRect.width() + 40, aa1LensRect.height() - 4), tr("Lens"));

    QRect aa1SensorRect(-185, 50, 30, 30);
    painter->drawRect(aa1SensorRect);
    painter->drawText(QRectF(aa1SensorRect.left(), aa1SensorRect.top()-15,
                             aa1SensorRect.width() + 40, aa1SensorRect.height() - 4), tr("Sensor"));

    QRect aa1DispenseRect(-125, 0, 30, 30);
    painter->drawRect(aa1DispenseRect);
    painter->drawText(QRectF(aa1DispenseRect.left(), aa1DispenseRect.top() - 15,
                             aa1DispenseRect.width() + 40, aa1DispenseRect.height() - 4), tr("Disp"));

    QRect aa1UVRect(-185, 0, 30, 30);
    painter->drawRect(aa1UVRect);
    painter->drawText(QRectF(aa1UVRect.left(), aa1UVRect.top() - 15,
                             aa1UVRect.width() + 40, aa1UVRect.height() - 4), tr("UV"));

    QRect camera1Rect(-150, -50, 30, 30);
    painter->drawRect(camera1Rect);
    painter->drawText(QRectF(camera1Rect.left(), camera1Rect.top() - 15,
                             camera1Rect.width() + 40, camera1Rect.height() - 4), tr("Product"));

    //AA 2
    QRect aa2LensRect(-25, 50, 30, 30);
    painter->drawRect(aa2LensRect);
    painter->drawText(QRectF(aa2LensRect.left(), aa2LensRect.top()-15,
                             aa2LensRect.width() + 40, aa2LensRect.height() - 4), tr("Lens"));

    QRect aa2SensorRect(-75, 50, 30, 30);
    painter->drawRect(aa2SensorRect);
    painter->drawText(QRectF(aa2SensorRect.left(), aa2SensorRect.top()-15,
                             aa2SensorRect.width() + 40, aa2SensorRect.height() - 4), tr("Sensor"));

    QRect aa2DispRect(-25, 0, 30, 30);
    painter->drawRect(aa2DispRect);
    painter->drawText(QRectF(aa2DispRect.left(), aa2DispRect.top()-15,
                             aa2DispRect.width() + 40, aa2DispRect.height() - 4), tr("Disp"));

    QRect aa2UVRect(-75, 0, 30, 30);
    painter->drawRect(aa2UVRect);
    painter->drawText(QRectF(aa2UVRect.left(), aa2UVRect.top() - 15,
                             aa2UVRect.width() + 40, aa2UVRect.height() - 4), tr("UV"));

    QRect camera2Rect(-50, -50, 30, 30);
    painter->drawRect(camera2Rect);
    painter->drawText(QRectF(camera2Rect.left(), camera2Rect.top() - 15,
                             camera2Rect.width() + 40, camera2Rect.height() - 4), tr("Product"));

    painter->drawText(QRectF(50, -150, 40, 20), tr("ST"));
    painter->drawText(QRectF(60, -100, 30, 20), tr("SPA"));
    painter->drawText(QRectF(20, -50, 40, 20), tr("SUT_1"));
    painter->drawText(QRectF(150, -50, 40, 20), tr("SUT_2"));
    painter->drawText(QRectF(20, 0, 40, 20), tr("AA_1"));
    painter->drawText(QRectF(150, 0, 40, 20), tr("AA_2"));
    painter->drawText(QRectF(60, 50, 30, 20), tr("LPA"));
    painter->drawText(QRectF(50, 100, 40, 20), tr("LT"));

}
//! [6]

//! [7]
void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}
//! [7]

void GraphWidget::shuffle()
{
    foreach (QGraphicsItem *item, scene()->items()) {
        if (qgraphicsitem_cast<Node *>(item))
            item->setPos(-150 + QRandomGenerator::global()->bounded(300), -150 + QRandomGenerator::global()->bounded(300));
    }
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
