#include "mainwindow.h"
#include <QApplication>
#include "controller.h"
#include "simulation.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Controller Cont;
    Cont.init();
    Simulation Sim;
    QObject::connect(&Cont, SIGNAL(FireTrans(QString)),
                      &Sim, SLOT(UpdateState(QString)));

    qRegisterMetaType< QList<QStringList> >( "QList<QString>" );

    QObject::connect(&Cont, SIGNAL(emitMarkings(QList<QStringList> )),
                      w.scene, SLOT(drawMarkings(QList<QStringList> )));
    Sim.start();
    Cont.start();
    while(true)
    {
        a.processEvents();
    }
    return a.exec();
}
