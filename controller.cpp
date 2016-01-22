#include "controller.h"
#include <QDebug>
#include <QThread>
#include <QString>


Controller::Controller(QThread *parent) :
    QThread(parent), taskNumbersFromUI{1, 1, 1, 1, 1},
    taskTypesFromUI{0, 0, 1, 1, 2, 2},
    buffersFromUI(37)
{

    this->buffersFromUI << 0,3,0,2,0,3,0,3,0,2,5,0,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,2,0,1;
    qDebug() << buffersFromUI[R2F];

    updateInitialBuffers();

    init();
    qDebug() << buffersFromUI[R2F];
}

void Controller::UpdateState(QString value)
{
    //qDebug()<<"Updatestate controlera"<<value[0]<<endl;
    emit FireTrans(value);
}

void Controller::run()
{
    while(true)
    {
        this->sleep(1);
        this->process();
    }
}

int Controller::process()
{
    int NextTask=0;
    if(IfThereAreUnfinishedTasks(Finished))
    {
        NextTask=HighiestPriority(Priorities,Finished,RunningOnPlant,Incidence,Steps,TasksTypes,Types,Marking);
        if (NextTask != -1)
        {
            int TransitionToFire=Types[TasksTypes[NextTask]][Steps[NextTask]];
            if(TransitionToFire==T22)//If it's going to parking, then it's finished & we don't send it to plant - resolves immediately
            {
                FireTransition(Incidence, T22, Marking);
                Steps[NextTask]++;
                Finished[NextTask]=1;
                RunningOnPlant[NextTask]=0;
                //std::cout<<"T to fire: "<<TransitionToFire<<std::endl;
            }
            else
            {
                if(IfCanBeFired(Incidence, TransitionToFire, Marking))
                {
                    FireTransition(Incidence, TransitionToFire, Marking);
                    Steps[NextTask]++;
                    IncreasePrioritiesExcept(Priorities,Finished,RunningOnPlant,Incidence,Steps,TasksTypes,Types,Marking,NextTask);
                    RunningOnPlant[NextTask]=1;//TODO zlapany sygnal zmienia to pole w klasie
                    //wysylalmy tranz zadanie, otrzymujemy zadanie
                    QString message;
                    message.append(QString::number(ChangeToPlace(TransitionToFire)));
                    message.append("#");
                    message.append(QString::number(NextTask));
                    message.append("#");

                    emit FireTrans(message);
                }
            }


        }

        QList<QStringList> toEmit;
        toEmit = CreateVectorToVisualization(Finished,Steps,TasksTypes,Types);
        qDebug() << toEmit;
        emitMarkings(toEmit);
        return 1;
    }
    else
    {
        return 0;
    }

}

void Controller::init()
{
    MatrixXf _Incidence(37,23);
    _Incidence<<
                 0,0,0,0,0,1,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,-1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,1,0,-1,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,-1,0,1,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,1,0,0,-1,0,0,1,0,1,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,-1,0,0,1,0,0,-1,0,-1,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,1,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,-1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,-1,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,1,0,0,0,0,0,0,
            -1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
            0,0,1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            -1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,-1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,-1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,1,0,0,-1,-1,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,-1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,1,0,0,0,-1,-1,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,-1,0,0,0,1,1,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,-1,0,0,0,0,
            0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,1,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,-1,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,1,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,-1,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,1,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,-1,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,1,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,-1,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,1,0,
            0,0,0,1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,-1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,-1,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,1,0,0,
            0,1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,-1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0;

    this->Incidence=_Incidence;

    VectorXf type1(13);
    type1<<T00,T01,T02,T03,T04,T05,T08,T11,T13,T17,T20,T21,T22;

    VectorXf type2(13);
    type2<<T00,T01,T02,T03,T04,T05,T08,T12,T09,T18,T20,T21,T22;

    VectorXf type3(11);
    type3<<T00,T01,T02,T03,T04,T06,T09,T18,T20,T21,T22;

    VectorXf type4(13);
    type4<<T00,T01,T02,T03,T04,T05,T07,T10,T14,T18,T20,T21,T22;

    VectorXf type5(13);
    type5<<T00,T01,T02,T03,T04,T07,T10,T15,T16,T19,T20,T21,T22;

    std::vector<VectorXf> _Types(5);
    _Types[0]=type1;
    _Types[1]=type2;
    _Types[2]=type3;
    _Types[3]=type4;
    _Types[4]=type5;
    this->Types=_Types;


    VectorXf _Marking(37);
    _Marking<< 0,3,0,2,0,3,0,3,0,2,3,0,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,2,0,1;

    _Marking[PARKING]=18;


    this->Marking = _Marking;
    //updateBuffers();
    std::vector<int> _TasksTypes(5);
    _TasksTypes[0]=0;
    _TasksTypes[1]=1; // ilosc zadan
    _TasksTypes[2]=2;
    _TasksTypes[3]=3;
    _TasksTypes[4]=4;

    this->Marking = this->buffersFromUI;
    //this->TasksTypes = QList2Vector(taskTypesFromUI);
    updateTasksTypesList();
    std::vector<int> _Steps(TasksTypes.size(),0);		//next step
    std::vector<int> _Finished(TasksTypes.size(),0);	//0-running,1-finished
    std::vector<int> _Priorities(TasksTypes.size(),0);	//priorities from 0 (lowest)
    std::vector<int> _RunningOnPlant(TasksTypes.size(),0);
    this->Steps=_Steps;
    this->Finished=_Finished;
    this->Priorities=_Priorities;
    this->RunningOnPlant=_RunningOnPlant;

}

int IfCanBeFired(MatrixXf Incidence, int transition, VectorXf Marking)//ret 0 if cannot, ret 1 if can
{
    VectorXf AfterFiring=Incidence.col(transition)+Marking;
    if(AfterFiring.minCoeff()<0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int HighiestPriority(std::vector<int> Priorities,std::vector<int> Finished,std::vector<int> RunningOnPlant,MatrixXf Incidence,std::vector<int> Steps,std::vector<int> TasksTypes,std::vector<VectorXf> Types,VectorXf Marking)
{
    int HighestPrior=0;
    int Index=-1;
    for (int i=0;i<Priorities.size();i++)
    {
        if(Finished[i]==0 && RunningOnPlant[i]==0)
        {
            int TransitionToFire=Types[TasksTypes[i]][Steps[i]];
            if(IfCanBeFired(Incidence, TransitionToFire, Marking))
                if(Priorities[i] >= HighestPrior)
                {
                    HighestPrior = Priorities[i];
                    Index=i;
                }
        }
    }
    return Index;
}


int IfThereAreUnfinishedTasks(std::vector<int> Finished) //ret 1 if there are, 0 if there Arent
{
    for (int i=0;i<Finished.size();i++)
    {
        if(Finished[i]==0)
        {
            return 1;
        }
    }
    return 0;
}
int FireTransition(MatrixXf Incidence, int transition, VectorXf &Marking)
{
    Marking=Incidence.col(transition)+Marking;
    return 0;
}
int IncreasePrioritiesExcept(std::vector<int> &Priorities,std::vector<int> Finished,std::vector<int> RunningOnPlant,MatrixXf Incidence,std::vector<int> Steps,std::vector<int> TasksTypes,std::vector<VectorXf> Types,VectorXf Marking,int Except)
{


    for (int i=0;i<Priorities.size();i++)
    {
        if(Finished[i]==0 && RunningOnPlant[i]==0 && i!=Except)
        {
            int TransitionToFire=Types[TasksTypes[i]][Steps[i]];
            if(IfCanBeFired(Incidence, TransitionToFire, Marking))
            {
                Priorities[i]++;
            }
        }
    }
    return 0;
}

QList<QStringList> CreateVectorToVisualization(std::vector<int> Finished,std::vector<int> Steps,std::vector<int> TasksTypes,std::vector<VectorXf> Types)
{
    QList<QStringList> VisVecta;
    QStringList type1;
    QStringList type2;
    QStringList type3;
    QStringList type4;
    QStringList type5;

    QString VisVect;
    for (int i=0;i<TasksTypes.size();i++)
    {
        if(Finished[i]==1)
        {
            VisVect="END";
        }
        else
        {
            int TransitionToFire=Types[TasksTypes[i]][Steps[i]];
            switch (TransitionToFire)
            {
            case  T00 :
                VisVect="PARKING";break;
            case  T01 :
                VisVect="R10";break;
            case  T02 :
                VisVect="WR";break;
            case  T03 :
                VisVect="R1";break;
            case  T04 :
                VisVect="W1";break;
            case  T05 :
                VisVect="R2";break;
            case  T06 :
                VisVect="R2";break;
            case  T07 :
                VisVect="R2";break;
            case  T08 :
                VisVect="M1";break;
            case  T09 :
                VisVect="M3";break;
            case  T10 :
                VisVect="M4";break;
            case  T11 :
                VisVect="R3";break;
            case  T12 :
                VisVect="R3";break;
            case  T13 :
                VisVect="M2";break;
            case  T14 :
                VisVect="R4";break;
            case  T15 :
                VisVect="R4";break;
            case  T16 :
                VisVect="M5";break;
            case  T17 :
                VisVect="R7";break;
            case  T18 :
                VisVect="R5";break;
            case  T19 :
                VisVect="R6";break;
            case  T20 :
                VisVect="W2";break;
            case  T21 :
                VisVect="S";break;
            case  T22 :
                VisVect="R9";break;
            }
            switch(TasksTypes[i])
            {
            case 0:
                type1.append(VisVect);break;
            case 1:
                type2.append(VisVect);break;
            case 2:
                type3.append(VisVect);break;
            case 3:
                type4.append(VisVect);break;
            case 4:
                type5.append(VisVect);break;
            }

        }
    }
    VisVecta << type1 << type2 << type3 << type4 << type5;
    return VisVecta;
}


int ChangeToPlace(int TransitionToFire)
{
    int ret=-1;
    switch (TransitionToFire)
    {
    case  T00 :
        ret=R10;break;
    case  T01 :
        ret=WR;break;
    case  T02 :
        ret=R1;break;
    case  T03 :
        ret=W1;break;
    case  T04 :
        ret=R2;break;
    case  T05 :
        ret=M1;break;
    case  T06 :
        ret=M3;break;
    case  T07 :
        ret=M4;break;
    case  T08 :
        ret=R3;break;
    case  T09 :
        ret=R5;break;
    case  T10 :
        ret=R4;break;
    case  T11 :
        ret=M2;break;
    case  T12 :
        ret=M3;break;
    case  T13 :
        ret=R7;break;
    case  T14 :
        ret=M3;break;
    case  T15 :
        ret=M5;break;
    case  T16 :
        ret=R6;break;
    case  T17 :
        ret=W2;break;
    case  T18 :
        ret=W2;break;
    case  T19 :
        ret=W2;break;
    case  T20 :
        ret=S;break;
    case  T21 :
        ret=R9;break;
    case  T22 :
        ret=PARKING;break;
    }
    return ret;

}

void Controller::TaskEnd(int TaskNumber)
{
    RunningOnPlant[TaskNumber]=0;
}


std::vector<int> Controller::QList2Vector(QList<int> list)
{
    std::vector<int> newList(list.size());
    for(auto it = list.begin(); it != list.end(); ++it)
    {
        auto i = it - list.begin();
        newList[i] = *it;
    }
    return newList;
}

void Controller::restart()
{
    qDebug() << "Auch!";
    init();
}

void Controller::update()
{
    qDebug() << "Auch!";
    init();
}


void Controller::updateTask1(int value)
{
    taskNumbersFromUI[0] = value;
    qDebug() << "Auch!" << value;
}

void Controller::updateTask2(int value)
{
    taskNumbersFromUI[1] = value;
    qDebug() << "Auch!" << value;
}

void Controller::updateTask3(int value)
{
    taskNumbersFromUI[2] = value;
    qDebug() << "Auch!" << value;
}

void Controller::updateTask4(int value)
{
    taskNumbersFromUI[3] = value;
    qDebug() << "Auch!" << value;
}

void Controller::updateTask5(int value)
{
    taskNumbersFromUI[4] = value;
    qDebug() << "Auch!" << value;
}

void Controller::updateTasksTypesList()
{
    QList<int> tempList;
    for(int j=0; j < 5; ++j)
    {
        for (int i=0; i < taskNumbersFromUI[j]; ++i )
        {
            tempList.append(j);
        }
    }
    this->TasksTypes = QList2Vector(tempList);
    qDebug() << tempList;
}

void Controller::updateBuffers()
{

}


void Controller::updatemBuffer1(int value)
{
    qDebug() << "Auch!" << value;
    this->buffersFromUI[M1F] = value;
}

void Controller::updatemBuffer2(int value)
{
    qDebug() << "Auch!" << value;
    this->buffersFromUI[M2F] = value;
}

void Controller::updatemBuffer3(int value)
{
    qDebug() << "Auch!" << value;
    this->buffersFromUI[M3F] = value;
}

void Controller::updatemBuffer4(int value)
{
    qDebug() << "Auch!" << value;
    this->buffersFromUI[M4F] = value;
}

void Controller::updatemBuffer5(int value)
{
    qDebug() << "Auch!" << value;
    this->buffersFromUI[M5F] = value;
}

void Controller::updatesBuffer1(int value)
{
    qDebug() << "Auch!" << value;
    this->buffersFromUI[W1F] = value;
}

void Controller::updatesBuffer2(int value)
{
    qDebug() << "Auch!" << value;
    this->buffersFromUI[W2F] = value;
}



void Controller::udpateparkingBuffer(int value)
{
    qDebug() << "Auch!" << value;
    this->buffersFromUI[PARKING] = value;
}

void Controller::updateepwBuffer(int value)
{
    qDebug() << "Auch!" << value;
    this->buffersFromUI[S] = value;

}

void Controller::updatermwBuffer(int value)
{
    qDebug() << "Auch!" << value;
    this->buffersFromUI[WRF] = value;

}

void Controller::updateInitialBuffers()
{
    updatemBuffer1(3);
    updatemBuffer2(3);
    updatemBuffer3(3);
    updatemBuffer4(3);
    updatemBuffer5(3);

    updatesBuffer1(3);
    updatesBuffer2(3);

    udpateparkingBuffer(10);
    updateepwBuffer(3);
    updatermwBuffer(3);
}
