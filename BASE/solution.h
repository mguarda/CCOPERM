#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <fstream>
using namespace std;

class Solution{

protected:
	/// Fitness de la solucion
	double fitness;
	/// Dimension de la solucion
	unsigned int dimension;
	
public:
	/// Constructor por defecto de la clase
	Solution();
	/// Destructor de la clase
	~Solution();
	/// Establece el fitness de la solucion
	void setFitness( double _fitness );
	/// Retorna el fitness de la solucion
	double getFitness();
	/// Retorna la dimension de la Solucion
	unsigned int getDimension();
	
	/// Retorna el valores particulares de las soluciones
	virtual void* getValues() = 0;
	/// Retorna la distancia entre las soluciones
	virtual double Distance(Solution* other_sol) = 0;
	/// Para los valores de la solucion actual, se modifican (extienden) retornando otra nueva solucion 
	virtual Solution* ExtendSolution(double X_i) = 0;
	/// Registra las componentes de la solucion y su fitness en un archivo
	virtual void show_solution( ofstream& out_distill ) = 0;
	
};

#endif // SOLUTION_H
