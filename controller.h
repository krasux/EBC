#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <eigen3/Eigen/Dense>
#include <QString>
#include <QThread>

#include "trans.hpp"

using namespace Eigen;
int IfCanBeFired(MatrixXf Incidence, int transition, VectorXf Marking);
int HighiestPriority(std::vector<int> Priorities,std::vector<int> Finished,std::vector<int> RunningOnPlant,MatrixXf Incidence,std::vector<int> Steps,std::vector<int> TasksTypes,std::vector<VectorXf> Types,VectorXf Marking);
int IfThereAreUnfinishedTasks(std::vector<int> Finished);
int FireTransition(MatrixXf Incidence, int transition, VectorXf &Marking);
int IncreasePrioritiesExcept(std::vector<int> &Priorities,std::vector<int> Finished,std::vector<int> RunningOnPlant,MatrixXf Incidence,std::vector<int> Steps,std::vector<int> TasksTypes,std::vector<VectorXf> Types,VectorXf Marking,int Except);
QList<QStringList> CreateVectorToVisualization(std::vector<int> Finished,std::vector<int> Steps,std::vector<int> TasksTypes,std::vector<VectorXf> Types);
void PutVisVectOnConsole(std::vector<std::string> In);
int ChangeToPlace(int TransitionToFire);




class Controller : public QThread
{
    Q_OBJECT
public:
    explicit Controller(QThread *parent = 0);

    MatrixXf Incidence;
    std::vector<VectorXf> Types;
    VectorXf Marking;
    std::vector<int> TasksTypes;

    std::vector<int> Steps;		//next step
    std::vector<int> Finished;	//0-running,1-finished
    std::vector<int> Priorities;	//priorities from 0 (lowest)
    std::vector<int> RunningOnPlant;


    void init();
    void run();
    int process();
    int EmitState();
private:
    QList<int> taskTypesFromUI;
    QList<int> taskNumbersFromUI;
    VectorXf buffersFromUI;
    std::vector<int> QList2Vector(QList<int> list);
    void updateTasksTypesList();
    void updateBuffers();

    void updateInitialBuffers();

signals:
    void FireTrans(QString value);
    void emitMarkings(const QList<QStringList> &marking);

public:
    void UpdateState(QString value);
public slots:
    void TaskEnd(int TaskNumber);
    void restart();
    void update();
    void updateTask1(int value);
    void updateTask2(int value);
    void updateTask3(int value);
    void updateTask4(int value);
    void updateTask5(int value);

    void updatemBuffer1(int value);
    void updatemBuffer2(int value);
    void updatemBuffer3(int value);
    void updatemBuffer4(int value);
    void updatemBuffer5(int value);

    void updatesBuffer1(int value);
    void updatesBuffer2(int value);

    void udpateparkingBuffer(int value);
    void updateepwBuffer(int value);
    void updatermwBuffer(int value);

};




#endif // CONTROLLER_H
