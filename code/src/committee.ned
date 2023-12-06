package project4;

module Committee
{
    parameters:
        int NProf;

    gates:
        input inStudents[NProf];
        output outStudents[NProf];

    submodules:
        prof[NProf]: Prof {
            @display("p=285,93;is=vl");
        }

    connections:
        for i=0..NProf-1 {
            inStudents[i] --> {  delay = 0ms;@display("ls=#00FF40,2,s"); } --> prof[i].inStudent;
        }
        for i=0..NProf-1 {
            prof[i].outStudent --> {  delay = 0ms; } --> outStudents[i];
        }
}
