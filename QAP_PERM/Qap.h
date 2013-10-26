/*
 * Qap.h
 *
 *  Created on: 11/03/2013
 *      Author: mauricio
 */

#ifndef QAP_H_
#define QAP_H_

#include "problem.h"
#include <vector>
#include <string>
using namespace std;

class Qap: public Problem {
private:
	/// Vector con los mínimos valores de las coordenadas
	std::vector<double> min_values;
	/// Vector con los máximos valores de las coordenadas
	std::vector<double> max_values;
	/// Dimension del problema
	unsigned int dimension;
	//matriz de distancias que involucra el problema
	std::vector<std::vector<int> > distance_matrix;
	//matriz de flujo que involucra el problema
	vector<vector<int> > flow_matrix;

public:
	Qap();
	virtual ~Qap();

	// REDEFINICION DE METODOS VIRTUALES
	/// Redefinicion del método para la clase Tsp
	Solution* NewSolution();
	/// Redefinicion del método para la clase Tsp
	double CalcularFitness(Solution *sol);
	/// Redefinicion del método para la clase Tsp
	double getMinValuesPos(unsigned int pos);
	/// Redefinicion del método para la clase Tsp
	double getMaxValuesPos(unsigned int pos);
	/// Redefinicion del método para la clase Tsp
	unsigned int getDimension();
	/// Redefinicion del método para la clase Tsp
	bool EsDinamico();
	/// Redefinicion del método para la clase Tsp
	void step(std::vector<std::vector<double> >, int);
	///// Redefinicion del método para la clase Tsp
	//void fin_experimento( string );
	void readQAPProblemFile(string filename);
};

#endif /* QAP_H_ */
