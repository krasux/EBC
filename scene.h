#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItem>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    ~Scene();

private:
    QGraphicsRectItem *machine1;
    QGraphicsRectItem *machine2;
    QGraphicsRectItem *machine3;
    QGraphicsRectItem *machine4;
    QGraphicsRectItem *machine5;
    QGraphicsRectItem *washingStation1;
    QGraphicsRectItem *washingStation2;
    QGraphicsRectItem *rawMaterialsWarehouse;
    QGraphicsRectItem *parking;
    QGraphicsRectItem *endPoingWarehouse;
    QGraphicsEllipseItem *itemA;

    QPixmap *backgroud;

    void drawScheme();
signals:

public slots:

};

#endif // SCENE_H
