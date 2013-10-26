/*
 * SolutionPerm.h
 *
 *  Created on: 12/07/2012
 *      Author: mauricio
 */

#ifndef SOLUTIONPERM_H_
#define SOLUTIONPERM_H_

#include <vector>
#include "solution.h"
using namespace std;

class SolutionPerm: public Solution {
private:
	vector<int> values;
	int dimension;

public:
	SolutionPerm();
	SolutionPerm(vector<int> _values);
	~SolutionPerm();

	/// Definicion de la distancia a desplazar a la planta hija
	double Delta(double d, double X_i);

	/// Redefinicion metodo virtual: retorna valores propios de la solucion
	void* getValues();

	/// Redefinicion metodo virtual: retorna la distancia euclidiana entre las soluciones
	double Distance(Solution* other_sol);

	double Distance(vector<int> perm, vector<int> perm_prima);

	/// Redefinicion metodo virtual: retorna solucion extendida
	Solution* ExtendSolution(double X_i);

	/// Redefinicion metodo virtual: desplegar las componentes de la solucion
	void show_solution(ofstream& out_distill);

	/*! \return permutation generated as a integer vector
	@param size number of elements
	 */
	vector<int> generatePermutation(int size);


};

#endif /* SOLUTIONPERM_H_ */
