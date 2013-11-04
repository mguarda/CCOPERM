/*
 * Tsp.cpp
 *
 *  Created on: 12/07/2012
 *      Author: mauricio
 */

#include "Tsp.h"
#include "toolbox.h"
#include "SolutionPerm.h"
#include <cmath>
#include <string.h>
#include <vector>
#include <fstream>
using namespace std;

Tsp::Tsp() {
	// TODO Auto-generated constructor stub
	toolbox *tb = toolbox::instance();

	distance_matrix = readTSPProblemFile(tb->pval_string("filepath"));
	dimension = distance_matrix.size();
	tb->storeVariable("dimension",dimension);
	cout << "La dimension es " << dimension << endl;
}

Tsp::~Tsp() {
	// TODO Auto-generated destructor stub
}

/**
 * @param sol Puntero a la solución, que se desea determinar su fitness en la subclase Tsp
*/
double Tsp::CalcularFitness( Solution *sol ){

	vector <int> values_ = *(vector<int>*) sol ->getValues();
	int size = values_.size();
	double cost;
	cost = 0.0;
	double x1,x2,y1,y2;
	double dist = 0;
//	cout << "calculando costo..." << endl;
	for (int i = 0; i < (size - 1); i++) {
		/*cout << "values_[i]-1 = " << values_[i]-1 << endl;
		cout << "values_[i+1]-1 = " << values_[i+1]-1 << endl;
		cout << "distance_matrix[values_[i]-1][values_[i + 1]-1] = " << distance_matrix[values_[i]-1][values_[i + 1]-1] << endl;*/		
//		cost = cost + distance_matrix[values_[i]-1][values_[i + 1]-1];
		x1 = (double) distance_matrix[values_[i]-1][0];
		x2 = (double) distance_matrix[values_[i+1]-1][0];
		y1 = (double) distance_matrix[values_[i]-1][1];
		y2 = (double) distance_matrix[values_[i+1]-1][1];
		dist = sqrt(pow((y2-y1),2)+pow((x2-x1),2));

		cost = cost + dist;
	}
	/*cout << "values_[i]-1 = " << values_[size-1]-1 << endl;
	cout << "values_[i+1]-1 = " << values_[0]-1 << endl;
	cout << "distance_matrix[values_[i]-1][values_[i + 1]-1] = " << distance_matrix[values_[size-1]-1][values_[0]-1] << endl;*/
//	cost = cost + distance_matrix[values_[size-1]-1][values_[0]-1];
//	cout << "costo total es: " << (1/cost)*100 << endl;

	return (1/cost)*100000;
}

/**
 * @param pos Indica la posición de la coordenada
 *
 * @return Retorna valor del tipo double, con el valor de esa coordenada para los mínimos valores
*/
double Tsp::getMinValuesPos( unsigned int pos ){
	return min_values[ pos ];
}

/**
 * @param pos Posición de la coordenada
 *
 * @return Retorna valor del tipo double, con el valor de esa coordenada para los máximos valores
*/
double Tsp::getMaxValuesPos( unsigned int pos ){
	return max_values[ pos ];
}

/**
 * @return Retorna valor del tipo entero sin signo, con el valor de la dimensión del problema
*/
unsigned int Tsp::getDimension(){
	return dimension;
}

/**
 * @return Retorna valor del tipo booleano, con la respuesta de si es o no, dinámico el problema
*/
bool Tsp::EsDinamico(){
	return false;
}

/**
 * @param datos Arreglo de arreglo con valores de todas las plantas presentes en el patio
 * @param iter Numero de la iteracion actual
 *
*/
void Tsp::step( vector< vector<double> > datos , int iter){
}


/**
 * @return Retorna puntero del tipo Solution, con una nueva solución relacionada con la codificación en particular para el problema, específicamente SolutionRN
*/
Solution* Tsp::NewSolution(){
	return new SolutionPerm();
}

vector<vector<double> > Tsp::readTSPProblemFile(string filename){
//	toolbox *tb = toolbox::instance();

	typedef vector<double> Fila;
	typedef vector<Fila> Matriz;
	typedef vector<double> Fila_d;
	typedef vector<Fila_d> Matriz_d;
	Fila fila(2);
	char * charfile = new char[filename.length()+1];
	strcpy(charfile,filename.c_str());
	ifstream open (charfile,istream::in);
	if (open)
		cout << "El fichero TSP se ha abierto correctamente";
	else{
		cout << "No se ha podido abrir el fichero de problema TSP";
		exit(0);
	}
	cout << endl;
	Matriz matriz;
	string line = "";
	int i =1;
	int k = 1;
	int j = 0;
	int num = 0;
	int aux = 0;
	bool flag = 0;
	string node_coord_section = "NODE_COORD_SECTION";

	while(!open.eof()){
		open >> line;
		if (line[0] != ' ') {
			if (flag) {
				aux = atoi(line.c_str());
				if (k != 1 && line.compare("EOF") != 0){// && line.compare(" ") != 0) {
					num = aux;
					if(j==0 && k == 2){
						fila[0] = (double) num;
						j++;
						k++;
					}else if(k == 3){
						fila[1] = (double) num;
						matriz.push_back(fila);
						cout << "VECTOR AGREGADO a matriz "<< matriz[i-1][0] << " " << matriz[i-1][1]  << endl;
						j = 0;
						i++;
						k=1;
					}
				}else{
					k++;
				}
			}
			if (line.compare("NODE_COORD_SECTION") == 0) {
//				cout << "se encontro NODE COORD SECTION" << endl;
				flag = 1;
			}
/*			if (line.compare("EOF") == 0)
				cout << "Fin del archivo"<< endl;*/
		}else{
			open.ignore(99999,'\n');
		}
	}

	open.close();
/*
	cout << "Creando matriz de distancias" << endl;
//===========Creacion de la matriz de distancias===============
	Matriz_d distancias;
	Fila_d filas ;
	double x1, y1, x2, y2 = 0;
	double dist = 0;
	for (int i = 0; i < (int) matriz.size(); i++) {
		for (int j = 0; j < (int) matriz.size(); j++) {
			x1 = (double) matriz[i][0];
			x2 = (double) matriz[j][0];
			y1 = (double) matriz[i][1];
			y2 = (double) matriz[j][1];
			dist = sqrt(pow((y2-y1),2)+pow((x2-x1),2));
			filas.push_back(dist);
//			cout << "Dato agregado: " << dist << " i= " << i+1 << "| j= " << j+1 << endl;
		}
		cout << "Se ha agregado la fila n°" << i+1 << endl;
		distancias.push_back(filas);
		filas.clear();
	}
*/
	/*cout << "Matriz distancias: " << endl;
	for(int i=0; i < (int)distancias.size(); i++){
		for(int j=0; j < (int)distancias[i].size();j++){
			cout << distancias[i][j] << " ";
		}
		cout << endl;
	}*/

//	return distancias;
	return matriz;
}
