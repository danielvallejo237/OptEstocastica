#include "Individual.h"

//Reservar memoria para la codificacion
Individual::Individual(){
}

TFitness Individual::getCost(){
	return -cost;
}
