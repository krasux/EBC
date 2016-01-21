#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>
#include <QDate>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    ~Scene();


private:
    QGraphicsEllipseItem *itemA;

    QPixmap *backgroud;

    QList<QStringList> markingsList;
    QHash<QString, QPointF> positionHash;


    QList<QBrush> getBrushes();
    QPen getPen();
    void test();
    void setBackground();

    void setUpPositionTable();
    QPointF positionOf(QString elementName, int translation);
    QSizeF getSizeOfMarking();

signals:


public slots:
    void drawMarkings(const QList<QStringList> &marking);
};

#endif // SCENE_H
