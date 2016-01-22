#include "mainwindow.h"
#include <QApplication>
#include "controller.h"
#include "simulation.h"
#include <QDebug>
#include "ui_mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    Controller Cont;

    Simulation Sim;
    Sim.init();
    QObject::connect(&Cont, SIGNAL(FireTrans(QString)),
                      &Sim, SLOT(UpdateState(QString)));
    QObject::connect(&Sim, SIGNAL(TaskEnd(int)),
                      &Cont, SLOT(TaskEnd(int)));
    qRegisterMetaType< QList<QStringList> >( "QList<QString>" );

    QObject::connect(&Cont, SIGNAL(emitMarkings(QList<QStringList> )),
                      w.scene, SLOT(drawMarkings(QList<QStringList> )));

    QObject::connect(w.ui->btnUpdate, SIGNAL(clicked()),
                     &Cont, SLOT(update()));

    QObject::connect(w.ui->type1, SIGNAL(valueChanged(int)),
                     &Cont, SLOT(updateTask1(int)));
    QObject::connect(w.ui->type2, SIGNAL(valueChanged(int)),
                     &Cont, SLOT(updateTask2(int)));
    QObject::connect(w.ui->type3, SIGNAL(valueChanged(int)),
                     &Cont, SLOT(updateTask3(int)));
    QObject::connect(w.ui->type4, SIGNAL(valueChanged(int)),
                     &Cont, SLOT(updateTask4(int)));
    QObject::connect(w.ui->type5, SIGNAL(valueChanged(int)),
                     &Cont, SLOT(updateTask5(int)));



    w.show();
    Sim.start();
    Cont.start();
    while(true)
    {
        a.processEvents();
    }


    return a.exec();
}
