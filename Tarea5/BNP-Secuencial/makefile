main: main.cpp crossover.o getDistance.o intensify.o print.o utils.o evaluate.o initialize_heuristic.o MA.o mutation.o Problem.o Individual.o
	g++ -O2 -o $@ $^

Individual.o: Individual.h Individual.cpp
	g++ -O2 -c -o $@ Individual.cpp 

crossover.o: crossover.cpp Individual.h 
	g++ -O2 -c -o $@ crossover.cpp

getDistance.o: getDistance.cpp Individual.h 
	g++ -O2 -c -o $@ getDistance.cpp

intensify.o: intensify.cpp Individual.h 
	g++ -O2 -c -o $@ intensify.cpp

print.o: print.cpp Individual.h 
	g++ -O2 -c -o $@ print.cpp

evaluate.o: evaluate.cpp Individual.h 
	g++ -O2 -c -o $@ evaluate.cpp

initialize_heuristic.o: initialize_heuristic.cpp Individual.h 
	g++ -O2 -c -o $@ initialize_heuristic.cpp

mutation.o: mutation.cpp Individual.h 
	g++ -O2 -c -o $@ mutation.cpp

Problem.o: Problem.h
	g++ -O2 -c -o $@ Problem.cpp

MA.o: MA.h Individual.h Problem.h MA.cpp
	g++ -O2 -c -o $@ MA.cpp

utils.o: utils.cpp utils.h
	g++ -O2 -c -o $@ utils.cpp

clean:
	rm -f main *.o
