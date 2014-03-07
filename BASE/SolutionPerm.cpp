/*TSP_CCO
 * SolutionPerm.cpp
 *
 *  Created on: 12/07/2012
 *      Author: mauricio
 */

#include "SolutionPerm.h"
#include "problem.h"
#include "toolbox.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include "rand31pmc.h"

using namespace std;

SolutionPerm::SolutionPerm() {
	// TODO Auto-generated constructor stub
	Problem *ptr;
	ptr = Problem::instance();

//	toolbox *tb = toolbox::instance();

	dimension = ptr->getDimension();
//	cout << "dimension = " << dimension << endl;
	values = generatePermutation(dimension);
	setFitness( ptr->CalcularFitness( this) );
//	usleep(700000);
//	cout << "fitness = " << fitness << endl;
}

SolutionPerm::SolutionPerm(vector<int> _values){
	Problem *ptr;
	ptr = Problem::instance();
/*	cout << "creando nueva solucion" << endl;
	cout << "dimension de la nueva solucion = " << _values.size() << endl;*/
	if (ptr->getDimension() == _values.size()){
		dimension = ptr->getDimension();
		values = _values;
	}else{
		cout << "Error en la dimension del arreglo, en Solution::Solution(vector<int>)"<<endl;
		exit(0);
	}
//	cout << "paso la verificacion de dimensión" << endl;
	setFitness(ptr->CalcularFitness(this));
//	cout << "fitness = " << fitness << endl;
}


SolutionPerm::~SolutionPerm() {
	// TODO Auto-generated destructor stub
}

double SolutionPerm::Delta(double d, double X_i){
	return 1.0;
}

void* SolutionPerm::getValues(){
	return &values;
}

double SolutionPerm::Distance(Solution* other_sol){

	vector<int> values_other_sol = *(vector<int>*)other_sol->getValues();
//	cout << "realizando medicion de distancia" << endl;
	int n = values.size();
	int perm_inversa[n];
	int prod[n];
	int prod_chk[n][2];
	int c = 0;
//	cout << n;
	for (int i = 0; i < n; i++) {
		perm_inversa[values[i] - 1] = i + 1;
	}
	for (int i = 0; i < n; i++) {
		prod[i] = values_other_sol[perm_inversa[i] - 1];
	}
	for (int i = 0; i < n; i++) {
		prod_chk[i][0] = 0;
		prod_chk[i][1] = prod[i];
	}
	int j = 1;
	for (int i = 1; i < n + 1; i++) {
		if (prod_chk[i - 1][0] == 0) {
			c++;
			j = i;
			do {
				prod_chk[j - 1][0] = 1;
				j = prod_chk[j - 1][1];
			} while (j != i);
		}
	}
	int dist = n - c;
	return (double)dist;
}

double SolutionPerm::Distance(vector<int> perm, vector<int> perm_prima){

	int n = values.size();
	int perm_inversa[n];
	int prod[n];
	int prod_chk[n][2];
	int c = 0;
	//	cout << n;
	for (int i = 0; i < n; i++) {
		perm_inversa[values[i] - 1] = i + 1;
	}
	for (int i = 0; i < n; i++) {
		prod[i] = perm_prima[perm_inversa[i] - 1];
	}
	for (int i = 0; i < n; i++) {
		prod_chk[i][0] = 0;
		prod_chk[i][1] = prod[i];
	}
	int j = 1;
	for (int i = 1; i < n + 1; i++) {
		if (prod_chk[i - 1][0] == 0) {
			c++;
			j = i;
			do {
				prod_chk[j - 1][0] = 1;
				j = prod_chk[j - 1][1];
			} while (j != i);
		}
	}
	int dist = n - c;
	return (double) dist;
}

Solution* SolutionPerm::ExtendSolution( double delta ){

	toolbox *tb;
    tb = toolbox::instance();

	vector<int> vals = values;

	int moves = (int) tb->triangular((double)(delta*(double)(dimension - 1)/3), 1.0,delta*(double)(dimension - 1));
	if(moves == 0) moves = 1; //return new SolutionPerm(this->values);

	/*cout << "delta = " << delta << endl;
	cout << "dimension = " << dimension << endl;
	cout << (double)(delta*((double)dimension-1)/3) << endl;
	cout << "movimientos a extender: " << moves << endl;*/

	int swap1, swap2, e_swap1, e_swap2;
	e_swap1 = -2;
	e_swap2 = -2;
	vector <int> p_swap1;
	vector <int> p_swap2;
	p_swap1.push_back(-1);
	p_swap2.push_back(-1);
	int dist = 0;
	bool flag = true;
	do {
		swap1 = tb->azarInt(dimension);
		do {
			swap2 = tb->azarInt(dimension);
		} while (swap1 == swap2);

		e_swap1 = vals[swap1];
		e_swap2 = vals[swap2];
		vals[swap1] = e_swap2;
		vals[swap2] = e_swap1;

		dist = Distance(values, vals);
//		cout << "moves = "<< moves << " distancia = " << dist << " largo permutacion = " << vals.size() << " delta = "<< delta << " " << (double)(delta*((double)dimension-1)/3) << endl;
		if (dist == moves)	flag = false;
	} while (flag);
/*	cout << endl;
	cout << "retornando nueva solucion" << endl;
	for(int i=0; i < (int)vals.size(); i++)
		cout << vals[i] << " " ;
	cout << endl;*/
	return new SolutionPerm(vals);
}

/// @param out_distill Nombre del archivo que se agregarán los datos más relevantes del objeto de tipo Solution, tales como: las coordenadas de éste y su fitness
void SolutionPerm::show_solution( ofstream& out_distill ){
	out_distill << fitness << " | " << 100000/(fitness) << " | ";
	for ( int j = 0; j < (int)dimension ; j++ ){
		out_distill << values[ j ] <<" ";
	}


}

vector<int> SolutionPerm::generatePermutation(int size){

	// pointer object to it:
//	ptrdiff_t (*p_myrandom) (ptrdiff_t) = myrandom;
//	rand31dc *r = new rand31dc();
	toolbox *tb = toolbox::instance();
	double time = (double)tb->clockstop();
	srand ( unsigned (time) );
	vector<int> myvector;
	vector<int>::iterator it;

	// set some values:
	for (int i=1; i<=size; ++i) myvector.push_back(i);// 1 2 3 4 5 6 7 8 9

	// using built-in random generator:
	random_shuffle ( myvector.begin(), myvector.end() );

	// using myrandom:
//	random_shuffle ( myvector.begin(), myvector.end(), p_myrandom);

	// print out content:
/*	cout << "Generated Permutation:";
	for (it=myvector.begin(); it!=myvector.end(); ++it)
	cout << " " << *it;

	cout << endl;
*/

	return myvector;
}


