package project4;

simple StudentGenerator
{
    
    parameters:
        int NProf = default(0);
        
        // examinationTime in the Parallel case approaches NProf * E[t]
        @signal[examinationTime](type = double); // Measures an exam's duration (pipeline case --> also enqueueing times are considered, i.e. WaitingTime). 
        @statistic[examinationTimeStat](source = examinationTime; record = mean, vector, min, max);
        
        @signal[waitingTime](type = double); // Measures the sum of enqueueing times (WITHOUT considering the first wait).
        @statistic[waitingTimeStat](source = waitingTime; record = mean, vector, min, max);
        
        @signal[throughtputStudent](type = int); // Measures the number of students that get examined each hour
        @statistic[throughtputStudentStat](source = throughtputStudent; record = mean, vector, min, max);  
    gates:
        input inStudents[NProf];
        output outStudents[NProf];
}
