/**
 * \class Problem
 * \brief Clase abstracta Problem
*/

using namespace std;
#include "problem.h"
//#include "solutionrn.h"
#include "Tsp.h"
#include <cmath>

Problem* Problem::ptrinstance = 0; // initialization

Problem::Problem(){

}

Problem::~Problem(){
	delete ptrinstance;
}



/**
 * @return Retorna puntero del tipo Problem, con problema particular para Tsp
*/
Problem* Problem::instance(){

	if(ptrinstance == 0){// if is called from the first time

		ptrinstance = new Tsp();
	}
	return ptrinstance;
}



