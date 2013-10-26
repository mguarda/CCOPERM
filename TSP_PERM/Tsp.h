/*
 * Tsp.h
 *
 *  Created on: 12/07/2012
 *      Author: mauricio
 */

#ifndef TSP_H_
#define TSP_H_

#include "problem.h"
#include <vector>
#include <string>

class Tsp: public Problem {

private:
	/// Vector con los mínimos valores de las coordenadas
	std::vector<double> min_values;
	/// Vector con los máximos valores de las coordenadas
	std::vector<double> max_values;
	/// Dimension del problema
	unsigned int dimension;
	//matriz de distancias que involucra el problema
	std::vector<std::vector<double> > distance_matrix;

public:
	Tsp();

	~Tsp();

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
	vector<vector<double> > readTSPProblemFile(string filename);
};

#endif /* TSP_H_ */
