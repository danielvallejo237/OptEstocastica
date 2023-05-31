CC=g++
CFLAGS =-Wall -std=c++0x -g
OPTFLAGS=-O2
INCLUDES=-I$(PWD)

SOURCES=tinyxml2.cpp WindScenario.cpp WindFarmLayoutEvaluator.cpp KusiakLayoutEvaluator.cpp GridEvaluator.cpp
OBJECTS=$(SOURCES:.cpp=.o)

main:$(OBJECTS)
	$(CC) $(CFLAGS) -o main.o $(OBJECTS) main.cpp $(INCLUDES) $(OPTFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@ -w $(INCLUDES) $(OPTFLAGS)

clean:
	rm $(OBJECTS) main.o
