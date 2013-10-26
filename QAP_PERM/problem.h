#ifndef PROBLEM_H
#define PROBLEM_H

#include "solution.h"
#include <vector>
#include <string>

class Problem{

private:
	//! pointer to singleton object
	static Problem* ptrinstance;
	
public:
	Problem();
	~Problem();
	//! return the pointer to the singleton instance
	static Problem* instance();
	
	// METODOS VIRTUALES
	/// Método virtual que retorna un puntero del tipo Solution, relacionado con la codificación del problema
	virtual Solution* NewSolution() = 0;
	/// Método virtual que permite establecer el cálculo del fitness de las soluciones, para cada problema en particular
	virtual double CalcularFitness( Solution* sol ) = 0;
	/// Método virtual que permite retornar el valor para una determinada coordenada, entre los mínimos valores posibles para cada problema en particular
	virtual double getMinValuesPos( unsigned int pos ) = 0;
	/// Método virtual que permite retornar el valor para una determinada coordenada, entre los maximos valores posibles para cada problema en particular
	virtual double getMaxValuesPos( unsigned int pos ) = 0;	
	/// Método virtual que permite retornar la dimension en cada problema en particular
	virtual unsigned int getDimension() = 0;
	/// Método virtual que permite saber si es dinámico o no un problema en particular
	virtual bool EsDinamico() = 0;
	/// Método virtual que permite realizar cambio en el espacio de búsqueda en un problema en particular que sea dinámico
	virtual void step( std::vector< std::vector<double> > , int ) = 0;
	///// Método virtual
	//virtual void fin_experimento( string ) = 0;
	
	// ----------------------------------- DynamicYurt metodos
	//void DynamicYurt_create();
	//virtual void DynamicYurt_step() = 0;
	//double DynamicYurt_fitness( std::vector<double> valores );
	//void DynamicYurt_genPlot( double ss , int it , string filename );
  
};

#endif // PROBLEM_H
