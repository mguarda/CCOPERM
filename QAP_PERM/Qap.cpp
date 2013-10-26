/*
 * Qap.cpp
 *
 *  Created on: 11/03/2013
 *      Author: mauricio
 */

#include "Qap.h"
#include "toolbox.h"
#include "SolutionPerm.h"
#include <cmath>
#include <string.h>
#include <vector>
#include <fstream>
using namespace std;
Qap::Qap() {
	// TODO Auto-generated constructor stub
	toolbox *tb = toolbox::instance();

	readQAPProblemFile(tb->pval_string("filepath"));

	tb->storeVariable("dimension", dimension);
	cout << "La dimension es " << dimension << endl;
}

Qap::~Qap() {
	// TODO Auto-generated destructor stub
}
/**
 * @param sol Puntero a la solución, que se desea determinar su fitness en la subclase Tsp
 */
double Qap::CalcularFitness(Solution *sol) {
	//	toolbox *tb;
	//	tb = toolbox::instance();
	// el fitness en primera instancia se calcula por medio de la funcion 1/x, con x: costo total que tiene la solucion
	// una vez terminado el manejo de archivo de datos de entrada

	vector<int> values_ = *(vector<int>*) sol->getValues();
	int size = values_.size();

	double cost;
	cost = 0.0;
	for(int i=0 ; i<(int)dimension ; i++){
		for(int j=0; j < (int)dimension; j++){
			cost += flow_matrix[i][j]*distance_matrix[values_[i]-1][values_[j]-1];
		}
	}


	return (1 / cost)*100000;
}

/**
 * @param pos Indica la posición de la coordenada
 *
 * @return Retorna valor del tipo double, con el valor de esa coordenada para los mínimos valores
 */
double Qap::getMinValuesPos(unsigned int pos) {
	return min_values[pos];
}

/**
 * @param pos Posición de la coordenada
 *
 * @return Retorna valor del tipo double, con el valor de esa coordenada para los máximos valores
 */
double Qap::getMaxValuesPos(unsigned int pos) {
	return max_values[pos];
}

/**
 * @return Retorna valor del tipo entero sin signo, con el valor de la dimensión del problema
 */
unsigned int Qap::getDimension() {
	return dimension;
}

/**
 * @return Retorna valor del tipo booleano, con la respuesta de si es o no, dinámico el problema
 */
bool Qap::EsDinamico() {
	return false;
}

/**
 * @param datos Arreglo de arreglo con valores de todas las plantas presentes en el patio
 * @param iter Numero de la iteracion actual
 *
 */
void Qap::step(vector<vector<double> > datos, int iter) {
}

/**
 * @return Retorna puntero del tipo Solution, con una nueva solución relacionada con la codificación en particular para el problema, específicamente SolutionRN
 */
Solution * Qap::NewSolution() {
	return new SolutionPerm();
}

void Qap::readQAPProblemFile(string filename) {

	typedef vector<int> Fila;
	typedef vector<Fila> Matriz;
	char * charfile = new char[filename.length() + 1];
	strcpy(charfile, filename.c_str());
	string line = "";
	int dim = 0;
	///int k = 1;
	int j = 0;
	int i = 0;
	int m = 1;
	Matriz flujo;
	Matriz distancias;
	ifstream open(charfile, istream::in);
	if (open)
		cout << "El fichero se ha abierto correctamente";
	else{
		cout << "No se ha podido abrir el fichero problema";
		exit(0);
	}
	cout << endl;
	open >> line;
	dim = atoi(line.c_str());
	dimension = dim;
	Fila fila(dim);
	cout << " la dimension del experimento es = " << dim << endl;
	while (!open.eof()) {
		open >> line;

		if (line[0] != ' ') {

			if (i < dim && m == 1) {

				fila[i] = atoi(line.c_str());
				i++;
				if (i == dim) {

					i = 0;
					flujo.push_back(fila);
					j++;
					if (j == dim) {
						m = 2;
						i = 0;
						j = 0;
					}
				}
			} else {
				if (i < dim && m == 2) {

					fila[i] = atoi(line.c_str());
					i++;
					if (i == dim) {

						i = 0;
						distancias.push_back(fila);
						j++;
					}
				}
			}
		//	k++;
		} else {
			open.ignore(99999, '\n');
		}
	}
	distance_matrix = distancias;
	flow_matrix = flujo;
	//	cout << "el largo de la matriz de flujo es " << flujo.size() << " y el largo de la matriz distancias es " << distancias.size() << endl;

	/*cout << "la matriz de flujo es:" << endl;
	for (int h = 0; h < dim; h++) {
		for (int y = 0; y < dim; y++) {
			cout << flujo[h][y] << " ";
		}
		cout << endl;
	}

	cout << endl;

	cout << "la matriz de distancias es:" << endl;
	for (int v = 0; v < dim; v++) {
		for (int b = 0; b < dim; b++) {
			cout << distancias[v][b] << " ";
		}
		cout << endl;
	}*/

}

