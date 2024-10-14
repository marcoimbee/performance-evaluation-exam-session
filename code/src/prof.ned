package project4;

simple Prof
{
    parameters:
        @signal[idleTime](type = double); // Measures the sum of the time intervals which occur between the end of a student's interrogation and the next one's arrival time
        @statistic[idleTimeStat](source = idleTime; record = vector, sum); 
    gates:
    	input inStudent;
    	output outStudent;
}
