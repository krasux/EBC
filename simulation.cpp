#include "simulation.h"
#include <QDebug>
#include <QTimer>
Simulation::Simulation(QThread *parent) : QThread(parent)
{

}
//SLOTS RESPONSES
void Simulation::UpdateState(QString value)
{
    StateField=value;
    QString temp;
    int occur=0;
    int shot_time=0;
    int place_number=0;
    int TaskNumber=0;
    for(int i=0; i<StateField.size();i++)
    {
        if(occur==0)
        {
            if(StateField[i]!='#')
            {
                temp.append(StateField[i]);
            }
            else
            {
                occur++;
                place_number=temp.toInt();
                temp.clear();
            }
        }
        else
        {
            if(StateField[i]!='#')
            {
                temp.append(StateField[i]);
            }
            else
            {
                TaskNumber=temp.toInt();
            }
        }
    }
   // QTimer::singleShot(200, this, SLOT(TimerTimeout(int));
            shot_time=place_times[place_number];
            /*QTimer * timer = new QTimer();
            QObject::connect(timer, &QTimer::timeout, [=]() {
                 //TimerTimeout(TaskNumber);
                emit TaskEnd(TaskNumber);
            });
            timer->start(shot_time);
*/

            QTimer * timer = new QTimer();


            connect(timer, SIGNAL(timeout()), this, SLOT(TimerTimeout()));
            timer->start(shot_time);
            timer->setProperty("A",TaskNumber);
            timer->setProperty("B",timer->timerId());

}
void Simulation::TimerTimeout()
{
    qDebug()<<"TIMER TIMEOUT"<<endl;
    int TaskNumber;
    int timer_id;
    timer_id=(QObject::sender()->property("B")).toInt();
    TaskNumber=(QObject::sender()->property("A")).toInt();
    qDebug()<<"timerId: "<<timer_id<<" taskNumber: "<<TaskNumber<<endl;
    QObject::sender()->killTimer(timer_id);
    emit TaskEnd(TaskNumber);
}

void Simulation::value()
{
    //qDebug() <<StateField<<endl;
}

void Simulation::run()
{
   while(true)
   {
       this->sleep(1);
       this->value();
   }
}

void Simulation::init()
{
    place_times = new int[30];
    for(int i=0; i<30;i++)
    {
        place_times[i]=5000;
    }
}
