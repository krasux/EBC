    #include "controller.h"
    #include <QDebug>
    #include <QThread>
    #include <QString>


    Controller::Controller(QThread *parent) : QThread(parent)
    {

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
         /*
        QString wiadomosc[3] = {"to jest wiadomosc","to jest druga","to jest rzecia"};
        QString mess;
        int cc=0;
        while(true)
        {
            this->sleep(3);
            mess=wiadomosc[cc];
            emit FireTrans(mess);
            cc++;
            if(cc>2)
                    cc=0;
        }
        */
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
                             //RunningOnPlant[NextTask]=1;//TODO zlapany sygnal zmienia to pole w klasie
                                         //wysylalmy tranz zadanie, otrzymujemy zadanie
                             QString message;
                             message.append(QString::number(TransitionToFire));
                             message.append("#");
                             message.append(QString::number(NextTask));
                             message.append("#");
                             emit FireTrans(message);
                         }
                     }


                 }
                 //TODO Sleep???
                 //std::vector<std::string> vect=CreateVectorToVisualization(Finished,Steps,TasksTypes,Types);
                 //PutVisVectOnConsole(vect);
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

     int Controller::EmitState()
     {
         //std::vector<std::string> vect=CreateVectorToVisualization(Finished,Steps,TasksTypes,Types);
         //PutVisVectOnConsole(vect);

         //setMarkings();
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
         _Marking[M1F]; // buffers

         this->Marking=_Marking;

         std::vector<int> _TasksTypes(2);
         _TasksTypes[0]=1;
         _TasksTypes[1]=2; // ilosc zadan

         this->TasksTypes=_TasksTypes;

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

     void PutVisVectOnConsole(std::vector<std::string> In)
     {
         QString str;



         for(int i=0;i<In.size();i++)
         {
             str=QString::fromStdString(In[i]);
             qDebug()<<str<<"\t";
         }
         qDebug()<<endl;
     }

