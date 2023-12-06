package project4.simulations;

import project4.Committee;
import project4.StudentGenerator;

network exam
{
    parameters:
        int NProf = default(0);
        bool examinationMode = default(false);
        double throughtputTimer = default(3600);
        
        bool distr = default(false);		//false = uniform, true = lognormal
        double scale = default(0);
        double shape = default(0);
        
        int min = default(0);
        int max = default(0);
    submodules:
        committee: Committee {
            //NProf = 
            @display("p=276,86");
        }
        sg: StudentGenerator {
            @display("p=84,74");
        }
        
    connections:
        for i=0..NProf-1 { sg.outStudents[i] --> { delay = 0ms; } --> committee.inStudents[i]; }
        for i=0..NProf-1 { committee.outStudents[i] --> { delay = 0ms; } --> sg.inStudents[i];}

}
