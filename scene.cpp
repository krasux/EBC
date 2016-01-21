#include "scene.h"




Scene::Scene(QObject *parent) : QGraphicsScene(parent)
{

    setBackground();


    test();
    qDebug() << "Date:" << QDate::currentDate();
    markingsList << QStringList("5");
    qDebug() <<markingsList;

}


Scene::~Scene()
{
    delete machine1;
    delete machine2;
    delete machine3;
    delete machine4;
    delete machine5;
    delete washingStation1;
    delete washingStation2;
    delete rawMaterialsWarehouse;
    delete parking;
    delete endPoingWarehouse;
}

void Scene::drawScheme()
{
    QBrush redBrush(Qt::red);
    QBrush blueBrush(Qt::blue);
    QPen blackPen(Qt::black);
    blackPen.setWidth(3);
    QSizeF size(20, 20);
    QPointF point(60, 200);
    QRectF rect(point, size);
    itemA = addEllipse(rect, blackPen, redBrush);
    itemA->setOpacity(30);
    itemA->setFlag(QGraphicsItem::ItemIsMovable, true);
}

void Scene::test()
{
    //drawScheme();
    QList<QStringList> testMarkingList;
    drawMarkings(testMarkingList);
}

void Scene::drawMarkings(const QList<QStringList> &newMarkingsList)
{
    QBrush redBrush(Qt::red);
    QBrush blueBrush(Qt::blue);
    QPen blackPen(Qt::black);
    blackPen.setWidth(3);
    QSizeF size(20, 20);
    QPointF point(60, 200);
    QRectF rect(point, size);
    itemA = addEllipse(rect, blackPen, redBrush);
}

void Scene::setBackground()
{
    backgroud = new QPixmap(":/bg/backgroud.png");
    addPixmap(*backgroud);
}
