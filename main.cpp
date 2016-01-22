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

    QObject::connect(w.ui->mBuffer1, SIGNAL(valueChanged(int)),
                     &Cont, SLOT(updatemBuffer1(int)));
    QObject::connect(w.ui->mBuffer2, SIGNAL(valueChanged(int)),
                     &Cont, SLOT(updatemBuffer2(int)));
    QObject::connect(w.ui->mBuffer3, SIGNAL(valueChanged(int)),
                     &Cont, SLOT(updatemBuffer3(int)));
    QObject::connect(w.ui->mBuffer4, SIGNAL(valueChanged(int)),
                     &Cont, SLOT(updatemBuffer4(int)));
    QObject::connect(w.ui->mBuffer5, SIGNAL(valueChanged(int)),
                     &Cont, SLOT(updatemBuffer5(int)));

    QObject::connect(w.ui->pBuffer, SIGNAL(valueChanged(int)),
                     &Cont, SLOT(udpateparkingBuffer(int)));
    QObject::connect(w.ui->epwBuffer, SIGNAL(valueChanged(int)),
                     &Cont, SLOT(updateepwBuffer(int)));
    QObject::connect(w.ui->rmwBuffer, SIGNAL(valueChanged(int)),
                     &Cont, SLOT(updatermwBuffer(int)));
    QObject::connect(w.ui->sBuffer1, SIGNAL(valueChanged(int)),
                    &Cont, SLOT(updatesBuffer1(int)));
    QObject::connect(w.ui->sBuffer2, SIGNAL(valueChanged(int)),
                     &Cont, SLOT(updatesBuffer12int));

    w.show();
    Sim.start();
    Cont.start();
    while(true)
    {
        a.processEvents();
    }


    return a.exec();
}
