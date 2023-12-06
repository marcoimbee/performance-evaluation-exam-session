//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __PROJECT4_STUDENTGENERATOR_H_
#define __PROJECT4_STUDENTGENERATOR_H_

#include <omnetpp.h>
#include <vector>
#include "Student_m.h"

using namespace omnetpp;
using namespace std;

class StudentGenerator : public cSimpleModule
{
    private:
        int NProf;
        double throughtputTimer;
        simsignal_t examinationTime;
        simsignal_t throughtputStudent;
        simsignal_t waitingTime;
        bool *profIdle;
        bool examinationMode;
        vector <vector<Student *>*> * waitingStudents;
        int getQueueLength(int);
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *) override;
        virtual void finish() override;

  public:

    //modular initialize
    void pipelineInitialize();
    void parallelInitialize();
    //modular handleMessage
    void pipelineHandleMessage(cMessage *);
    void parallelHandleMessage(cMessage *);

    int getNProf();
    void setNProf(int);
    bool getExaminationMode();
    void setExaminationMode(bool);
};

#endif
