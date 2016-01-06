#include "scene.h"



Scene::Scene(QObject *parent) : QGraphicsScene(parent)
{

    backgroud = new QPixmap(":/bg/backgroud.png");
    addPixmap(*backgroud);
    //setBackgroundBrush(Qt::blue);
    drawScheme();
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
    blackPen.setWidth(6);
    itemA = addEllipse(60, 200, 20, 20, blackPen, redBrush);
    itemA->setOpacity(30);
    itemA->setFlag(QGraphicsItem::ItemIsMovable, true);

}
