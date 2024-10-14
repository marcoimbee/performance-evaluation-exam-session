#include "prof.h"
#include "Student_m.h"

Define_Module(Prof);

void Prof::initialize() {
    cModule *commissione = getParentModule();
    cModule *network = commissione->getParentModule();

    examinationMode = network->par("examinationMode").boolValue();
    distr = network->par("distr");

    idleTime = registerSignal("idleTime");
    answerTime = registerSignal("answerTime");


    if(distr == false) {                 //uniform
        min = network->par("min");
        max = network->par("max");
    } else {                              //lognormal
        scale = network->par("scale");
        shape = network->par("shape");
    }

    setSumIdleTimes(simTime().dbl());
}



void Prof::handleMessage(cMessage *msg) {
    double t;
    if(distr == false) 
        t = uniform(min, max);
    else 
        t = lognormal(scale, shape);

    if(examinationMode) {         //true: Pipeline, false: Parallel
        if(msg->isSelfMessage()) {
            setStartingTime(simTime().dbl());     // IdleTime starting time
            send(msg, "outStudent");
        } else {
            double idleTimeDiff = simTime().dbl() - getStartingTime();      // starting time from THIS moment
            setSumIdleTimes(getSumIdleTimes() + idleTimeDiff);

            Student *s = check_and_cast<Student *>(msg);
            s->setExaminationTime(s->getExaminationTime() + t);

            emit(idleTime, getSumIdleTimes());

            scheduleAt(simTime() + t, msg);
        }
    } else {
        Student *s = check_and_cast<Student *>(msg);
        int remainingQuestions = s->getNumQuestions();

        if(remainingQuestions != 0){   //there are still remainingQuestions questions to be asked
            remainingQuestions--;
            s->setNumQuestions(remainingQuestions);         //setting the parameter

            s->setExaminationTime(s->getExaminationTime() + t);
            scheduleAt(simTime() + t, s);
        } else {
            send(s, "outStudent");     //send back the student to the generator
        }
    }
}

void Prof::setStartingTime(double startingTime){ this->startingTime = startingTime; }

double Prof::getStartingTime(){ return this->startingTime; }

void Prof::setSumIdleTimes(double sumIdleTimes){ this->sumIdleTimes = sumIdleTimes; }

double Prof::getSumIdleTimes(){ return this->sumIdleTimes; }
