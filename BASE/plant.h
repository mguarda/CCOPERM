#ifndef _PLANT_H_
#define _PLANT_H_

#include <fstream>
#include <list>
#include "solution.h"
#include "colony.h"

class Colony;

class Plant{
	
private:
	/// Fitness de la planta
	double fitness;
	
	/// Fitness de la rama.
	/// Suma de todos los fitness de la rama
	double bfitness;
	
	/// Transport fitness
	double tfitness;
	
	/// Puntero a la colonia de la planta
	Colony * colony;
	
	/// Puntero a la clase solucion que posee la planta
	Solution* solution;
	
	/// Cantidad de planta en la rama
	unsigned int bsize;
	
	/// Descendencia de la planta
	/// Lista de plantas que tienen a un padre en común
	std::list<Plant> offspring;
	
	/// Puntero a la planta padre
	Plant * parent;
	
	/// Año de creación
	unsigned int birth_day;

	// ************************************** METODOS PRIVADOS
	/// Reporta cambios al padre, de manera recursiva hacia sus padres, hasta el root.
	/// Desde la hoja a la raiz
	void report_change(double delta_fit , int delta_size );

	/// Cambio de padre para una rama
	/// Desde la raiz a la hoja
	void change_parent( Plant* );
	// ************************************** METODOS PRIVADOS FIN

public:
	/// Constructor por defecto de la clase Plant
	Plant();
	
	/// Constructor de la clase Plant
	Plant(Colony * col);
	
	/// Constructor de la clase Plant
	Plant(Colony * col, Solution* );
	
	/// Destructor de la clase Plant
	~Plant();
	
	/// Establece el fitness de la clase
	void setFitness(double fit);
	
	/// Retorna el actual fitness de la clase
	double getFitness() const;
	
	/// Establece el fitness de la rama
	void setBFitness(double);	
	
	/// Retorna el fitness de la rama
	double getBFitness() const;
	
	/// Permite establecer el tfitness
	void setTFitness(double);
	
	/// Permite obtener el tfitness
	double getTFitness();
	
	/// Establece el tamaño de la rama
	void setBSize(unsigned int);	
	
	/// Retorna el tamaño de la rama
	unsigned int getBSize() const;
			
	/// Añade un nuevo hijo a la planta
	Plant * add_child(Solution* solu, int anno);

	/// Establece el padre de la planta
	void setParent(Plant * const);

	/// Retorna el actual padre de la planta
	Plant * getParent() const;

	/// Permite eliminar la planta
	void  remove_plant();
	
	/// Establece la colonia a la que pertenece la planta
	void setColony(Colony * const);
	
	/// Retorna la actual colonia a la que pertenece la planta
	Colony* getColony() const;

	/// Establece la solucion de la planta
	void setSolution(Solution* solu);
	
	/// Retorna la actual solucion que tiene la planta
	Solution* getSolution();
	
	/// Establece el año de creacion planta
	void setAnno(unsigned int);
	
	/// Retorna el año de la planta
	unsigned int getAnno() const;
	
	/// Retorna el numero de conexiones que posee
	int getC() const;
	
	/// Retorna el puntero a la planta con mayor fitness de la rama
	Plant * best_branch( Plant* best ); 
	
	/// Retorna el puntero a la planta con menor fitness de la rama
	Plant * worst_branch( Plant* worst ); 
	
	/// Retorna un puntero a la planta segun el frente pareto Purge
	Plant* fpareto_purge( Plant* peor ); 
	
	/// grafica para una rama
	void show_branch(int);
	
	/// grafica para una planta
	void show_plant();
		
	// /// retorna las hojas para la rama
	//std::list<Plant *> leafs_branch( std::list<Plant *> );
	
	/// Retorna todas las plantas de la rama en un arreglo de punteros del tipo planta
	std::list<Plant *> plants_branch( std::list<Plant *> );
	
	// /// grafica para rama
	void plot_branch( ofstream& );
	
	/// Split para la rama
	Plant* split_branch( int , double );
	
	/// Cambio de colonia a la rama
	void change_colony_branch( Colony* new_col );
	
	/// Retorna la menor de las coordenadas existente en la rama
	double get_min_branch_coord( int coord, double val );
	
	/// Retorna la mayor de las coordenadas existente en la rama
	double get_max_branch_coord( int coord, double val );
	
	/// Retorna la varianza de la rama
	double get_varianza_branch( double var , double prom );
	
	/// Retorna la planta mas antigua de la rama
	Plant* get_old_plant_branch( Plant* old );

	/// Actualizar el TFitness, desde la raiz a la hoja
	void update_TFitness( double const , double const );
	
	/// Re-evalua el fitness de todas las plantas de la rama
	void re_eval_func_branch();
	
};

#endif //_PLANT_H_
