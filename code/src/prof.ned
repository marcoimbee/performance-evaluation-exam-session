package project4;

simple Prof
{
    parameters:
        @signal[idleTime](type = double); // Misura la somma degli intervalli di tempo fra la fine dell'interrogazione di uno studente, e l'arrivo del successivo.
        @statistic[idleTimeStat](source = idleTime; record = vector, sum); 
    gates:
    	input inStudent;
    	output outStudent;
}
