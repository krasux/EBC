#include "scene.h"



void Scene::setUpPositionTable()
{
    positionHash.insert("WR", QPointF(20, 205));
    positionHash.insert("W1", QPointF(182, 205));
    positionHash.insert("W2", QPointF(650, 205));
    positionHash.insert("PARKING", QPointF(365, 465));
    positionHash.insert("S", QPointF(540, 465));

    positionHash.insert("M1", QPointF(365, 75));
    positionHash.insert("M2", QPointF(540, 75));
    positionHash.insert("M3", QPointF(450, 200));
    positionHash.insert("M4", QPointF(365, 320));
    positionHash.insert("M5", QPointF(540, 320));

    positionHash.insert("R1", QPointF(132, 180));
    positionHash.insert("R2", QPointF(315, 180));
    positionHash.insert("R3", QPointF(476, 54));
    positionHash.insert("R4", QPointF(476, 296));
    positionHash.insert("R5", QPointF(580, 180));
    positionHash.insert("R6", QPointF(650, 296));
    positionHash.insert("R7", QPointF(650, 54));
    positionHash.insert("R8", QPointF(650, 445));
    positionHash.insert("R9", QPointF(476, 445));
    positionHash.insert("R10", QPointF(200, 445));
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
    list1 << "M1" << "S" << "PARKING" << "R4"
         << "R5" << "R6" << "R7" << "R8" << "R9" << "R10";
    list2 << "W1";
    list3 << "W2";
    list4 << "M1" << "M2" << "M3";

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

QList<QPen> Scene::getPens()
{
    QList<QPen> pens;
    QPen blackPen(Qt::black);
    blackPen.setWidth(3);

    QPen magentaPen(Qt::magenta);
    magentaPen.setWidth(3);

    QPen cyanPen(Qt::cyan);
    cyanPen.setWidth(3);

    QPen grayPen(Qt::gray);
    grayPen.setWidth(3);

    QPen lightGrayPen(Qt::lightGray);
    lightGrayPen.setWidth(3);

    pens << blackPen << magentaPen << cyanPen << grayPen << lightGrayPen;
    return pens;
}

QSizeF Scene::getSizeOfMarking()
{
    QSizeF size(20, 20);

    return size;
}

void Scene::drawMarkings(const QList<QStringList> &marking)
{
    clear();
    setBackground();
    QList<QBrush> brushes = getBrushes();
    QList<QPen> pens = getPens();
    QSizeF size = getSizeOfMarking();

    QStringList printed;
    for(auto it = marking.begin(); it != marking.end(); ++it)
    {
        auto i = it - marking.begin();
        int j = 0;
        foreach(QString elem, *it)
        {
            int l = printed.count(elem);

            printed << elem;
            QPointF position = positionOf(elem, l);
            QRectF rect(position, size);
            addEllipse(rect, pens[j % pens.size()], brushes[i]);
            ++j;
        }
    }

}

void Scene::setBackground()
{
    backgroud = new QPixmap(":/bg/backgroud.png");
    addPixmap(*backgroud);
}
