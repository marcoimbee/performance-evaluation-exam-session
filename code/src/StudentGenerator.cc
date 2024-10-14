#include "StudentGenerator.h"
#include "Student_m.h"

Define_Module(StudentGenerator);

int totStudents = 0;

void StudentGenerator::finish() {
    if(getExaminationMode()) {
        for(int i = 0; i < getNProf(); ++i) {
            int size = (*waitingStudents)[i]->size();
            for(int j = 0; j < size; ++j) {
                Student *S = (*waitingStudents)[i]->front();
                delete(S);
                (*waitingStudents)[i]->erase((*waitingStudents)[i]->begin());
            }
        }
    }
}


void StudentGenerator::initialize() {
    cModule *network = getParentModule();
    setExaminationMode(network->par("examinationMode").boolValue());
    setNProf(par("NProf").intValue());
    throughtputTimer = network->par("throughtputTimer").doubleValue();

    examinationTime = registerSignal("examinationTime");
    waitingTime = registerSignal("waitingTime");
    throughtputStudent = registerSignal("throughtputStudent");

    scheduleAt(simTime() + throughtputTimer, new cMessage("throughtputTimer"));

    if(getExaminationMode())
        pipelineInitialize();
    else
        parallelInitialize();
}

void StudentGenerator::parallelInitialize() {
    for(int i = 0; i < getNProf(); i++) {
        Student *S = new Student(getNProf());
        send(S, "outStudents", i);
    }
}

void StudentGenerator::pipelineInitialize() {
    profIdle = new bool[getNProf()];
    waitingStudents = new vector<vector<Student *> *>(getNProf());

    for(int i = 0; i < getNProf(); i++)
        (*waitingStudents)[i] = new vector<Student*>(); // Creating an empty queue for each teacher

    for(int i = 0; i < getNProf(); ++i)
        profIdle[i] = true;

    Student *S = new Student(1);
    profIdle[0] = false;
    send(S, "outStudents", 0);
}

void StudentGenerator::handleMessage(cMessage *msg){
    if(msg->isSelfMessage()) {
        emit(throughtputStudent, totStudents);
        totStudents = 0;
        scheduleAt(simTime() + throughtputTimer, new cMessage("throughtputTimer"));
        delete(msg);
        return;
    }
    
    if(getExaminationMode())
        pipelineHandleMessage(msg);
    else
        parallelHandleMessage(msg);
}

void StudentGenerator::pipelineHandleMessage(cMessage *msg) {
    int idProf = msg->getArrivalGate()->getIndex();
    Student *stud = check_and_cast<Student *>(msg);
    profIdle[idProf] = true;

    if(getNProf() == 1){
        emit(examinationTime, stud->getExaminationTime());

        totStudents++;

        stud->setExaminationTime(0);
        send(stud, "outStudents", 0);
        return;
    }

    if(idProf < (getNProf()-1)) {  // If idProf == Nprof - 1  ==>  unable to access to idProf + 1
        if(getQueueLength(idProf) > 0) {
            Student *S = (*waitingStudents)[idProf]->front();
            (*waitingStudents)[idProf]->erase((*waitingStudents)[idProf]->begin());

            double t = S->getWaitingTime() + simTime().dbl() - S->getStartingWaitingTime();
            S->setWaitingTime(t);
            profIdle[idProf] = false;
            send(S, "outStudents", idProf);
        }

        if(profIdle[idProf+1]) {
            profIdle[idProf+1] = false;
            send(stud, "outStudents", idProf + 1);
        } else {
           stud->setStartingWaitingTime(simTime().dbl());     // Memorizing enqueueing time
           (*waitingStudents)[idProf+1]->push_back(stud);
        }

        if (idProf == 0){
            Student *S = new Student(1);
            profIdle[idProf] = false;
            send(S, "outStudents", 0);
        }
    } else {
        emit(waitingTime, stud->getWaitingTime());
        emit(examinationTime, stud->getWaitingTime() + stud->getExaminationTime());
        totStudents++;
        delete(stud);

        if(getQueueLength(idProf) != 0) {
            Student *S = (*waitingStudents)[idProf]->front();
            (*waitingStudents)[idProf]->erase((*waitingStudents)[idProf]->begin());
            S->setQueueLengthProf((*waitingStudents)[idProf]->size());

            double t = S->getWaitingTime() + simTime().dbl() - S->getStartingWaitingTime();
            S->setWaitingTime(t);
            profIdle[idProf] = false;
            send(S, "outStudents", idProf);
        }
    }
}

int StudentGenerator::getQueueLength(int idProf) {
    return ((*waitingStudents)[idProf]->size());
}

void StudentGenerator::parallelHandleMessage(cMessage *msg) {
    int idProf = msg->getArrivalGate()->getIndex();

    Student *s = check_and_cast<Student *>(msg);

    emit(examinationTime, s->getExaminationTime());

    s->setNumQuestions(getNProf());         // Student "Reset"
    s->setExaminationTime(0);

    totStudents++;

    send(s, "outStudents", idProf);

}

bool StudentGenerator::getExaminationMode(){ return this->examinationMode; }
void StudentGenerator::setExaminationMode(bool examinationMode) { this->examinationMode = examinationMode; }

int StudentGenerator::getNProf(){ return this->NProf; }
void StudentGenerator::setNProf(int NProf) { this->NProf = NProf; }
