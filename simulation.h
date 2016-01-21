#ifndef SIMULATION_H
#define SIMULATION_H

#include <QObject>
#include <QDebug>
#include <QString>
#include<QThread>
class Simulation : public QThread
{

Q_OBJECT
public:
    explicit Simulation(QThread *parent = 0);
    void run();

private:
    QString StateField;
public:
    void value();
public slots:
    void UpdateState(QString value);
};

#endif // SIMULATION_H
