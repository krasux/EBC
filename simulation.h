#ifndef SIMULATION_H
#define SIMULATION_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QThread>
class Simulation : public QThread
{

Q_OBJECT
public:
    explicit Simulation(QThread *parent = 0);
    void run();

private:
    QString StateField;
    int * place_times;
public:
    void value();
    void init();
signals:
    void TaskEnd(int TaskNumber);
public slots:
    void UpdateState(QString value);
public slots:
    void TimerTimeout();
};

#endif // SIMULATION_H
