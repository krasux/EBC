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
//fragment od Marcina
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
//koniec fragmentu marcina
signals:
    void FireTrans(QString value);
    void emitMarkings(const QList<QStringList> &marking);

public:
    void UpdateState(QString value);
public slots:
    void TaskEnd(int TaskNumber);
};




#endif // CONTROLLER_H
