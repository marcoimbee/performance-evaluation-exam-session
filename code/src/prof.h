#ifndef __PROJECT4_PROF_H_
#define __PROJECT4_PROF_H_

#include <omnetpp.h>

using namespace omnetpp;

class Prof : public cSimpleModule
{
private:
    bool distr;
    double scale;
    double shape;
    int min;
    int max;
    bool examinationMode;
    simsignal_t idleTime;
    simsignal_t answerTime;
    double sumIdleTimes = 0;
    double startingTime = 0;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    void setSumIdleTimes(double);
    double getSumIdleTimes();

    void setStartingTime(double);
    double getStartingTime();
};

#endif
