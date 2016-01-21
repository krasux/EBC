#include "scene.h"




void Scene::setUpPositionTable()
{
    positionHash.insert("WR", QPointF(35, 205));
    positionHash.insert("M1", QPointF(365, 75));
    positionHash.insert("M2", QPointF(540, 75));
    positionHash.insert("M3", QPointF(450, 200));
    positionHash.insert("M4", QPointF(365, 320));
    positionHash.insert("M5", QPointF(540, 320));
}

QPointF Scene::positionOf(QString elementName, int translation = 0)
{
    QPointF trans = QPointF(25, 0) * translation;
    QPointF position = positionHash[elementName];
    return  position + trans;
}

Scene::Scene(QObject *parent) : QGraphicsScene(parent)
{
    setBackground();
    setUpPositionTable();

    test();

    qDebug() << "Date:" << QDate::currentDate();




    qDebug() << positionHash;
    qDebug() << positionOf("M3", 1);
}


Scene::~Scene()
{

}


void Scene::test()
{
    QList<QStringList> testMarkingList;
    QStringList list1;
    QStringList list2;
    QStringList list3;
    QStringList list4;
    list1 << "M1" << "M2";
    list2 << "M3";
    list3 << "M4";
    list4 << "M5";

    testMarkingList << list1 << list2 << list3 << list4;
    qDebug() << testMarkingList;
    drawMarkings(testMarkingList);

}

QList<QBrush> Scene::getBrushes()
{
    QList<QBrush> brushes;
    QBrush redBrush(Qt::red);
    QBrush blueBrush(Qt::blue);
    QBrush greenBrush(Qt::green);
    QBrush yellowBrush(Qt::yellow);
    QBrush magentaBrush(Qt::darkMagenta);
    brushes << redBrush << blueBrush << greenBrush << yellowBrush << magentaBrush;

    return brushes;
}

QPen Scene::getPen()
{
    QPen blackPen(Qt::black);
    blackPen.setWidth(3);

    return blackPen;
}

QSizeF Scene::getSizeOfMarking()
{
    QSizeF size(20, 20);

    return size;
}

void Scene::drawMarkings(const QList<QStringList> &marking)
{

    QList<QBrush> brushes = getBrushes();
    QPen pen = getPen();
    QSizeF size = getSizeOfMarking();

    for(auto it = marking.begin(); it != marking.end(); ++it)
    {
        auto i = it - marking.begin();
        qDebug() << i;
        foreach(QString elem, *it)
        {
            QPointF position = positionOf(elem);
            QRectF rect(position, size);
            addEllipse(rect, pen, brushes[i]);
        }
    }


}

void Scene::setBackground()
{
    backgroud = new QPixmap(":/bg/backgroud.png");
    addPixmap(*backgroud);
}
