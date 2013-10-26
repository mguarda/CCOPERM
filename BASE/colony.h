#ifndef _COLONY_H_
#define _COLONY_H_

#include <list>
#include <iostream>
#include <cstring>
#include "plant.h"

class Colony{
	
private:

	///Puntero del tipo Plant, que indica la planta raíz
	Plant *root;
	/// Nombre de la colonia
	string name;		
	/// Número de plantas que puede contener la colonia
	int MPC;
	
public:
	
	///Constructor de la clase
	Colony();
	
	///Destructor de la clase
	~Colony();
	
	///Establece a la planta raíz de la colonia
	void set_root(Plant * const);
	
	///Retorna el puntero a la planta raíz
	Plant *get_root() const;
	
	/// Establece el nombre de la actual colonia
	void setName(string nombre);
	
	/// Retorna el nombre asignado a la colonia
	string getName();
	
	/// Establecer el número de plantas que puede poseer la colonia (MPC)
	void setMPC(int);
	
	/// Retorna el número de plantas que puede poseer la colonia (MPC)
	double getMPC() const;
	
	///Establece el reemplazante de la raíz
	Plant *replace_root(Plant *const addr);
	
	/// Retorna el tamaño de la colonia
	int getSize() const;
	
	/// Retorna la planta de mayor fitness
	Plant * getBestPlant();
	
	/// Retorna la planta de menor fitness
	Plant* getWorstPlant();
	
	///// retorna las hojas de la colonia
	//std::list<Plant *> getLeafs();		
	
	/// Retorna las plantas de la colonia
	std::list<Plant *> getPlants();
	
	/// Actualizar el TFitness para la colonia
	void update_TFitness();
	
	/// Retorna el promedio de Fitness de la colonia
	double getMeanColony() const;
	
	/// Escoge al azar una Planta de la colonia
	Plant* roulette();
	
	/// Retorna el area de la colonia
	double getArea();
	
	/// Retorna la varianza de la colonia con respecto al Fitness de las plantas
	double getVarianza();
	
	/// Retorna la menor coordenada de la colonia
	double getMinValue( int dimension );
	
	/// Retorna la mayor coordenada de la colonia
	double getMaxValue( int dimension );
	
	/// Retorna el area de intersección con respecto a otra colonia
	double Superposicion( Colony* col2 );
	
	/// Retorna la planta mas antigua
	Plant* getOldPlant();
	
	///// retorna en minimo de la coord
	//double get_min_coord( int coord);
	
	///// retorna en maximo de la coord
	//double get_max_coord( int coord);
	
	/// Establece la colonia actual 
	void change_colony();
	
	/// Split para la colonia
	Plant* Split( int tam , double max_dist );
	
	/// Purge para la colonia
	Plant* Purge();
	
	/// Re-evaluacion de todos los fitness las plantas de la colonia
	void Re_Eval_Func();
	
	/// despliega el contenido de la colonia
	void show_colony();

	/// Metodo que permite desplegar grafica de la colonia
	void show_plot( int );
};

#endif //_COLONY_H_
