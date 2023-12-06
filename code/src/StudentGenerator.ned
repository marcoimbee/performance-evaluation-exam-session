package project4;

simple StudentGenerator
{
    
    parameters:
        int NProf = default(0);
        
        // examinationTime nel Parallel tende a NProf * E[t]
        @signal[examinationTime](type = double); // Misura la durata di un esame (nel pipeline --> si considerano anche i tempi di accodamento ossia il WaitingTime). 
        @statistic[examinationTimeStat](source = examinationTime; record = mean, vector, min, max);
        
        @signal[waitingTime](type = double); // Misura la somma dei tempi di accodamento (SENZA considerare la prima attesa).
        @statistic[waitingTimeStat](source = waitingTime; record = mean, vector, min, max);
        
        @signal[throughtputStudent](type = int); // Misura il numero di studenti esaminati ogni Ora
        @statistic[throughtputStudentStat](source = throughtputStudent; record = mean, vector, min, max);  
    gates:
        input inStudents[NProf];
        output outStudents[NProf];
}
