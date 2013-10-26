/**
 * \class Colony
 * \brief Clase que gestiona a objetos del tipo Plant
 * 
 */

using namespace std;
#include <fstream>
#include <cstdio>
#include <list>
#include <math.h>
#include "colony.h"
#include "toolbox.h"
#include <unistd.h>

Colony::Colony(){
    //list<Plant> colony;
    setMPC( 0 );
    root = NULL;
}

Colony::~Colony(){
    //Destructor
}

/**
 * @param croot Puntero del tipo Plant, para establecer la planta raíz de la nueva colonia
 */
void Colony::set_root(Plant * const croot){
    root = croot;
    //root->setParent( NULL );// fala's hand
}

/**
 * @return Retorna puntero del tipo Plant, con la planta raíz de la colonia
 */
Plant *Colony::get_root() const{
    return root;
}

/**
 * @param _name String con el nombre a asignar a la colonia
 */
// return the name of the colony
void Colony::setName(string _name ){
	name = _name;
}

/**
 * @return Retorna un string, con el nombre de la colonia
 */
// return the name of the colony
string Colony::getName(){
	return name;
}

/**
 * @param _MPC Número que establece el número de plantas que puede almacenar la colonia
 */
// establecer MPC
void Colony::setMPC(int _MPC){
	MPC=_MPC;
}

/**
 * @return Retorna un double, con el número de plantas que puede contener la colonia
 */
// obtener MPC
double Colony::getMPC() const{
	return MPC;
}

/**
 * Se reescribe la raíz, con la informacion de la nueva planta
 * 
 * @param addr Puntero del tipo Plant, con la dirección a la nueva planta raíz
 * 
 * @return Retorna un puntero Plant, con la información de la raíz
 * 
 */
Plant *Colony::replace_root(Plant * const addr){
    root->setFitness(addr->getFitness());
    root->setBFitness(addr->getBFitness());
    root->setBSize(addr->getBSize());
    root->setTFitness( addr->getTFitness() ); // fala's hand
    root->setAnno( addr->getAnno() ); // fala's hand
    root->setSolution( addr->getSolution() ); // fala's hand
    return root;
}

/**
 * @return Retorna un int, con el tamaño de la colonia
 */
// obtener tamanno de la colonia
int Colony::getSize() const{
	return this->get_root()->getBSize();
}

/**
 * @return Retorna un puntero del tipo Plant, con la planta de mayor fitness de la colonia
 */
// retorna la planta con el mejor fitness de la Colonia
Plant * Colony::getBestPlant(){
	return this->get_root()->best_branch( this->get_root() );
}
/*
//metodo que retorna las hojas de una colonia
list<Plant *> Colony::getLeafs(){
	list<Plant *> hojas;
	if( getSize() == 1 ) hojas.push_back( root );
	else hojas = this->get_root()->leafs_branch(hojas);
	
	return hojas;
}
*/

/**
 * @return Retorna una lista de punteros del tipo Plant, con todas las plantas que pertenecen a la colonia
 */
// retorna las plantas de la colonia
list<Plant*> Colony::getPlants(){
	list<Plant*> plantas;
	plantas.push_back( this->get_root() );
	return this->get_root()->plants_branch( plantas );
}

// actualizar el TFitness para la colonia
void Colony::update_TFitness(){
	//root->update_TFitness( root->getBFitness() / root->getBSize() );
	toolbox *tb;
    tb = toolbox::instance();
    
    this->get_root()->update_TFitness( root->getBFitness() / root->getBSize() , tb->pval_double("s"));
}

/**
 * @return Retorna una valor del tipo double, con el promedio de los fitness de la colonia
 */
// obtener promedio de la colonia
double Colony::getMeanColony() const{
	return ( this->get_root()->getBFitness() / this->get_root()->getBSize() );
}

/**
 * Las opciones para escoger una planta al azar, son:
 * 
 * 	- Opción 1: Se selecciona según la proporcionalidad con su fitness, de tal modo que aquella que tenga un fitness alto, tiene una mayor probabilidad de ser escogida.
 * 	- Opción 2: Completamente al azar entre todas las plantas de la colonia
 * 
 * @return Retorna un puntero del tipo Plant, con la planta escogida
 */
Plant* Colony::roulette(){
	toolbox *tb;
    tb = toolbox::instance();
    
    list<Plant*> plants = getPlants();
	int op = tb->pval_int("op_ext");
	
	if( op != 1 ){ // roulette con mayor prob segun fitness
		double total=0.0;
		list<Plant*>::iterator it;		
		// sumo el numero de fitness de hojas
		for (it = plants.begin(); it != plants.end(); it++){		
			total += (*it)->getFitness();
		}
		// otengo numero al azar
		double rand = total * tb->azar();
		// pregunto por la planta = al azar
		double aux=0;
		for (it = plants.begin(); it != plants.end(); it++){		
			aux += (*it)->getFitness();
			if(aux >= rand) return *it;
		}
	}
	else{ // la misma probabilidad
		// obtengo numero al azar
		//cout<<tb->azarInt( 10 );
		//int rand = tb->azarInt( val.size() );
		int rand = int( tb->azar() * plants.size() );
		list<Plant*>::iterator tt = plants.begin();
		for (int k = 0; k < int(plants.size()) ;k++){		
			if( k == rand ) return *tt;
			tt++;
		}		
	}
	// si hay problemas retorno el primer elemento	  
	return *plants.begin();
}

/**
 * @return Retorna un valor del tipo double, con el área de la colonia que utiliza en el espacio de búsqueda
 */
 double Colony::getArea(){
//	cout << "calculando area de la colonia "<<name << "..." <<endl;
	/*typedef vector<int> Fila;
	typedef vector<Fila> Matriz;
	Fila myvector;
	Matriz permutaciones;*/

	toolbox *tb;
	tb = toolbox::instance();
//	int dim = tb->pval_int("dimension");

	list<Plant*> plants = getPlants();
	list<Plant*>::iterator it1,it2;
	vector<int>::iterator it;
	double area = 0;

	vector<int> dists_aux;
	vector<int> dists;

	int aux = 0, aux2 = 0;
	int sum = 0, plus = 0;
	double max = 0, max2 = 0;
	double avg = 0;

//	cout << "numero de plantas de la colonia "<<name << ": "<< plants.size()<<endl;
	for(it1 = plants.begin(); it1 != plants.end(); it1++){
		for(it2 = plants.begin(); it2 != plants.end(); it2++){
			aux = (*it1)->getSolution()->Distance((*it2)->getSolution());
//			cout << "distancia: " << aux << endl;
			plus = aux + plus;
			if (aux > max)
				max = aux;
			if (aux != 0){
				dists_aux.push_back(aux) ;
			}
		}
		aux2 = plus;

		if(sum > aux2 || sum == 0){
			sum = aux2;
			max2 = max;
			dists = dists_aux;
		}
//		cout << "valor de sum = " << sum << endl;
//		cout << "valor de max2 = " << max2 << endl;
		plus = 0;
		dists_aux.clear();

	}



	//--------------------Obtencion del promedio-------------------------
	avg = sum/(getMPC()-1);
//	cout << "El promedio de las distancias es: " << avg << endl;
	//--------------Obtencion de la desviacion estandar------------------
	double desvStd = 0;
	aux = 0;
	for(int j=0;j<(int)dists.size();j++){
		aux = pow(avg - dists[j],2);
		desvStd += aux ;
	}
	desvStd = desvStd/(getMPC()-1);
	desvStd = sqrt(desvStd);
	//--------------------

	double radius = avg + desvStd;

//	cout << "el radio es " <<radius << endl;
	area = radius*radius*tb->pval_double("PI");
//	cout << "el area es " << area << endl;


	return area;
}

/**
 * @return Retorna valor del tipo double, con la varianza de la colonia
 */
double Colony::getVarianza(){
	double var = 0, prom = getMeanColony();
	
	var = root->get_varianza_branch( pow( root->getFitness() - prom , 2 ) , prom );
	
	return sqrt( var ) / getSize();
}

/**
 * @param dimension Valor de la coordenada a analizar
 * 
 * @return Retorna valor del tipo double, con el mínimo valor para esa coordenada
 */
double Colony::getMinValue( int dimension ){
	vector<double> aux = * (vector<double> *) root->getSolution()->getValues() ;
	return root->get_min_branch_coord( dimension , aux[dimension] );
}

/**
 * @param dimension Valor de la coordenada a analizar
 * 
 * @return Retorna valor del tipo double, con el máximo valor para esa coordenada
 */
double Colony::getMaxValue( int dimension ){
	vector<double> aux = * (vector<double> *) root->getSolution()->getValues() ;
	return root->get_max_branch_coord( dimension , aux[dimension] );
}

/**
 * Los Pasos realizados son los siguientes:
 * 	- Se obtienen los puntos (permutaciones) centro de cada colonia, se calcula el area de cada colonia,
 * 		y la distancia entre los puntos centro de cada colonia.
 * 	- Se calcula el porcentaje de segmento de superposición (interseccion de areas) entre las colonias.
 *
 * @param col2 Puntero a la otra colonia, para determinar si existe superposición de áreas entre ellas
 * 
 * @return Retorna valor del tipo double, con el área de la superposición entre las dos colonias
 */	
// retorna el area de superposicion con otra colonia
double Colony::Superposicion( Colony* col2 ){
//	cout << "CALCULANDO SUPERPOSICION"<< endl;
//	usleep(1000000);
	toolbox *tb;
	tb = toolbox::instance();
//	int dim = tb->pval_int("dimension");
	double super = 1;
	
	list<Plant*> plants1 = getPlants();
	list<Plant*> plants2 = col2->getPlants();
	list<Plant*>::iterator it1, it2, plant1, plant2;

	double area1, area2,area = 0;

	vector<int> dists_aux1;
	vector<int> dists1;

	int aux1 = 0, aux2_1 = 0;
	int sum1 = 0, plus1 = 0;
	double max1 = 0, max2_1 = 0;
	double avg1 = 0;

//	cout << "numero de plantas de la colonia "<<name << ": "<< plants1.size()<<endl;
	for(it1 = plants1.begin(); it1 != plants1.end(); it1++){
		for(it2 = plants1.begin(); it2 != plants1.end(); it2++){
			aux1 = (*it1)->getSolution()->Distance((*it2)->getSolution());
//			cout << "distancia: " << aux << endl;
			plus1 = aux1 + plus1;
			if (aux1 > max1)
				max1 = aux1;
			if (aux1 != 0){
				dists_aux1.push_back(aux1) ;
			}
		}
		aux2_1 = plus1;

		if(sum1 > aux2_1 || sum1 == 0){
			sum1 = aux2_1;
			max2_1 = max1;
			dists1 = dists_aux1;
			plant1 = it1;
		}
//		cout << "valor de sum = " << sum1 << endl;
//		cout << "valor de max2 = " << max2_1 << endl;
		plus1 = 0;
		dists_aux1.clear();

	}
	//--------------------Obtencion del promedio-------------------------
	avg1 = sum1/(getMPC()-1);
//	cout << "El promedio de las distancias es: " << avg1 << endl;
	//--------------Obtencion de la desviacion estandar------------------
	double desvStd1 = 0;
	aux1 = 0;
	for(int j=0;j<(int)dists1.size();j++){
		aux1 = pow(avg1 - dists1[j],2);
		desvStd1 += aux1 ;
	}
	desvStd1 = desvStd1/(getMPC()-1);
	desvStd1 = sqrt(desvStd1);
	//--------------------

	double radius1 = avg1 + desvStd1;

//	cout << "el radio es " <<radius1 << endl;
	area1 = radius1*radius1*tb->pval_double("PI");


	if(area1 == 0){
	//		cout << "El area1 son 0" << endl;
			return 0;
		}
//	cout << "Area1 calculado: "<< area1 <<endl;


//-------------------- Area Conlonia 2 ----------------------------------

	vector<int> dists_aux2;
	vector<int> dists2;

	int aux2 = 0, aux2_2 = 0;
	int sum2 = 0, plus2 = 0;
	double max2 = 0, max2_2 = 0;
	double avg2 = 0;

//	cout << "numero de plantas de la colonia "<<name << ": "<< plants2.size()<<endl;
	for(it1 = plants2.begin(); it1 != plants2.end(); it1++){
		for(it2 = plants2.begin(); it2 != plants2.end(); it2++){
			aux2 = (*it1)->getSolution()->Distance((*it2)->getSolution());
//			cout << "distancia: " << aux << endl;
			plus2 = aux2 + plus2;
			if (aux2 > max2)
				max2 = aux2;
			if (aux2 != 0){
				dists_aux2.push_back(aux2) ;
			}
		}
		aux2_2 = plus2;

		if(sum2 > aux2_2 || sum2 == 0){
			sum2 = aux2_2;
			max2_2 = max2;
			dists2 = dists_aux2;
			plant2 = it1;
		}
//		cout << "valor de sum = " << sum2 << endl;
//		cout << "valor de max2 = " << max2_2 << endl;
		plus2 = 0;
		dists_aux2.clear();

	}
	//--------------------Obtencion del promedio-------------------------
	avg2 = sum2/(getMPC()-1);
//	cout << "El promedio de las distancias es: " << avg2 << endl;
	//--------------Obtencion de la desviacion estandar------------------
	double desvStd2 = 0;
	aux2 = 0;
	for(int j=0;j<(int)dists2.size();j++){
		aux2 = pow(avg2 - dists2[j],2);
		desvStd2 += aux2 ;
	}
	desvStd2 = desvStd2/(getMPC()-1);
	desvStd2 = sqrt(desvStd2);
	//--------------------

	double radius2 = avg2 + desvStd2;

//	cout << "el radio es " <<radius2 << endl;
	area2 = radius2*radius2*tb->pval_double("PI");
//	cout << "Area2 calculado: " << area2 <<endl;

	if (area2 == 0) {
//			cout << "El area2 es 0" << endl;
		return 0;
	}

	//-----------------Determinar Superposicion---------------------------------
	double dist_center = 0;
//	cout << "Posible superposicion de areas entre colonias" << endl;
	dist_center = (*plant1)->getSolution()->Distance((*plant2)->getSolution());
//	cout << "La distancia entre los centros de cada area es: " << dist_center << endl;
	double suma_r = radius1 + radius2;
	double R,r = 0;
	double d = dist_center;
	double sub1,sub2;
//	double diff = r1 - r2;
//	cout << "La suma de los radios es:" << suma_r<<endl;
	if((area1 - area2 < 0.001 || area2 - area1 < 0.001) && dist_center == 0){ return 1;}
	if(dist_center >= suma_r ){ return 0;}
	if(dist_center < radius1 && radius1 >= (2*radius2)){ return area2/area1;}
	if(dist_center < radius2 && radius2 >= (2*radius1)){ return 1;}
	if(dist_center < suma_r){ //ocurre el caso de circunferencias secantes, http://mathworld.wolfram.com/Circle-CircleIntersection.html
//		cout << "caso circunferencias secantes!!!" << endl;

		if(radius1 > radius2){
			R = radius1;
			r=radius2;
		}else{
			R=radius2;
			r=radius1;
		}
		sub1 = 0.5*sqrt((r+R-d)*(d+r-R)*(d-r+R)*(d+r+R));
		sub2 = r*r*acos((d*d+r*r-R*R)/(2*d*r)) + R*R*acos((d*d+R*R-r*r)/(2*d*R));
		area = sub2 - sub1;
	}
	super = area/area1;
/*	cout << "Area: " << area << " area1: "<< area1 << " area2: " << area2 << " Superposicion: " << super
		<< " nº plant col1: " << this->getSize() << " nº plant col2: " << col2->getSize()
		<< " R= " << R << " r= "<< r <<" dist_center= "<< dist_center <<" sub1= "<< sub1 <<" sub2= "<< sub2 <<  endl;*/
	//usleep(3000000);
	return super;
}

void Colony::change_colony(){
	this->get_root()->change_colony_branch( this );
}

/**
 * @return Retorna puntero del tipo Plant, con la planta más antigua de la colonia
 */
Plant* Colony::getOldPlant(){
	return root->get_old_plant_branch( root );
}

/**
 * @param tam Número de plantas en la colonia
 * @param max_dist Distancia máxima permitida entre una planta y otra
 * 
 * @return Retorna puntero del tipo Plant, con la planta que será la raíz de una nueva colonia
 */
Plant* Colony::Split( int tam , double max_dist ){
	return this->get_root()->split_branch( tam , max_dist );
}

void Colony::Re_Eval_Func(){
	this->get_root()->re_eval_func_branch();
}

/**
 * @return Retorna puntero del tipo Plant, con la planta de peor fitness de la colonia
 */
// retorna la planta con el peor fitness de la Colonia
Plant* Colony::getWorstPlant(){
	return this->get_root()->worst_branch( this->get_root() );
}

/**
 * @return Retorna puntero del tipo Plant, con la planta que se ha determinado con los criterios de Purge
 */
Plant* Colony::Purge(){
	return this->get_root()->fpareto_purge( this->get_root() );
}

/// despliega el contenido de la colonia
void Colony::show_colony(){
	this->get_root()->show_branch(0);
}

/// @param iter Valor de la iteración actual
void Colony::show_plot( int iter){
	toolbox *tb;
    tb = toolbox::instance();
	
	string nom("./Results/" + tb->pval_string("IDExp") + "_gen_" + tb->int2string( iter ) + "_" +name  + ".topo");
	
	ofstream salida ( nom.c_str() );
	
	this->get_root()->plot_branch(salida);
    
    salida.close();
}
