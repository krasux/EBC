#include "simulation.h"
#include <QDebug>
Simulation::Simulation(QThread *parent) : QThread(parent)
{

}

void Simulation::UpdateState(QString value)
{
   //qDebug()<<"Wartosc pola statefield przed zmiana: "<<StateField<<endl;
        StateField=value;
        //qDebug()<<"Wartosc tego co przyszlo: "<<value<<endl;
        //this->value();
}

void Simulation::value()
{
    qDebug() <<StateField<<endl;
}

void Simulation::run()
{
   while(true)
   {
       this->sleep(1);
       this->value();
   }
}
