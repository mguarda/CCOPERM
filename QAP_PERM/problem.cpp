/**
 * \class Problem
 * \brief Clase abstracta Problem
*/

using namespace std;
#include "problem.h"
//#include "solutionrn.h"
#include "Qap.h"
#include <cmath>

Problem* Problem::ptrinstance = 0; // initialization

Problem::Problem(){
}

Problem::~Problem(){
	delete ptrinstance;
}


/**
 * @return Retorna puntero del tipo Problem, con problema particular para Ackley
*/
Problem* Problem::instance(){
	//cout<<"instance()"<<endl;
	if(ptrinstance == 0){// if is called from the first time
		ptrinstance = new Qap();
	}
	
	return ptrinstance;
}
