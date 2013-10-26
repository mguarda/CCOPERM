/**
 * \class Plant
 * \brief Define los objetos Plant
 * 
 */

using namespace std;

#include <cstdio>
#include <cmath>
#include "colony.h"
//#include "toolbox.h"
#include "problem.h"
#include <iostream>

Plant::Plant(){
}

/**
 * El constructor crea una planta con los siguientes valores:
 *  - Fitness = BFittnes : obtenidos de la clase Solution
 *  - BSize = 1 : BSize incrementara o decrementara dependiendo de la funcion report_change()
 *  - La variable offspring is inicializada vacia
 *  - parent se inicializa NULL
 *
 *  @param col Colonia a la cual pertenece la planta
 */
Plant::Plant(Colony* col){
    Problem *prob;
    prob = Problem::instance();

	Solution* tempSol = prob->NewSolution();
    
    setSolution(tempSol);//fala's hand
    setAnno(0);//fala's hand
    //setFitness(double(tempSol->getValue()));  //setFitness(0.0);
    setFitness( tempSol->getFitness() );
    setBFitness( getFitness() );
    setBSize(1);
    //list<Plant> offspring;
    setParent(NULL);
    setColony(col);
    if (col->get_root() == NULL) col->set_root(this);
}

// se utiliza en el add_child()
/**
 * El constructor crea una planta con los siguientes valores:
 *  - Fitness = BFittnes : obtenidos de la clase Solution
 *  - BSize = 1 : posteriormente BSize incrementará o decrementará dependiendo de la función report_change()
 *  - parent se inicializa NULL
 *
 *  @param col Colonia a la cual pertenece la planta
 * @param solution Solución que se asignará a la planta
 */
Plant::Plant(Colony * col, Solution* solution){
	
    setFitness( solution->getFitness() );//setFitness(double(solution->getValue()));
    setBFitness( this->getFitness() );//setBFitness(double(solution->getValue()));
    
    setBSize(1);
    //list<Plant> offspring;
    setParent(NULL);
    setColony(col);
    setAnno(0);
    setSolution(solution);
    if (col->get_root() == NULL) col->set_root(this);
}

Plant::~Plant(){}

double Plant::getBFitness() const{
    return bfitness;
}

unsigned int Plant::getBSize() const{
    return bsize;
}

void Plant::setBFitness(double fit){
    bfitness = fit;
}

void Plant::setBSize(unsigned int bsiz){
    bsize = bsiz;
}

/**
 * @param solu Solución que será asignada a la nueva planta "hija"
 * @param anno Iteración en la cual se ha creado la planta "hija"
 * 
 * @return Puntero del tipo Plant, perteneciente a la nueva planta "hija", agregada a la actual planta "padre"
 */
Plant* Plant::add_child(Solution* solu, int anno){
    Plant temporal(this->getColony(),solu);
    offspring.push_back(temporal);
    list<Plant>::iterator it;
    it = offspring.end();
    it--;
    //cout<<"Se esta creando una nueva planta en la direccion "<<&(*it)<<" y es hija de "<<this<<endl;
 
	(*it).setAnno(anno);
    (*it).setParent(this);
    (*it).report_change((*it).getBFitness(),(*it).getBSize());
    
    Plant * ppointer;
    ppointer = &(*it);
    return ppointer;
}

/**
 * @param fit Valor del fitness que se asignará a la actual planta
 */
void Plant::setFitness(double fit){
	fitness = fit;
}

/**
 * @return Retorna un valor double, con el valor del fitness de la planta
 */
double Plant::getFitness() const{
	return fitness;
}

/**
 * @param col Puntero a la colonia que pertenecerá la actual planta
 */
void Plant::setColony(Colony * const col){
    colony = col;
}

/**
 * @return Retorna un puntero del tipo Colony, a la colonia que pertenece la planta
 */
Colony * Plant::getColony() const{
    return colony;
}

/**
 *  @param par Puntero al "padre" de la actual planta "hija"
 */
void Plant::setParent(Plant * const par){
    parent = par;
}

/**
 * @return Retorna un puntero del tipo Plant, con el valor de planta "padre"
 */
Plant * Plant::getParent() const{
    return parent;
}

/**
 * @param sol Puntero a la solución que se le asignará a la actual planta
 */
void Plant::setSolution(Solution* nueva){
    solution = nueva;
}

/**
 * @return Retorna un puntero del tipo Solution, con la solución de la actual planta
 */
Solution* Plant::getSolution(){
    return solution;
}

/**
 * Este método recursivo, actualiza el BFitness y TFitness, también se ejecuta en el padre y éste a su vez en su propio "padre", hasta terminar en el "root" o raíz de la rama. Una vez en el root, se procede a actualizar el TFitness.
 *
 * Este método es invocado, cuando se añade o elimina una planta.
 * 
 * El funcionamiento es el siguiente:
 * - Se verifica que la actual planta no sea la raíz. De ser así, se procede a actualizar el tfitness en toda la colonia
 * - Para el resto de los casos, al padre se modifica con los valores relevantes (suma de fitness y el numero de planta), y al ser recursivo, a la vez se modifica el padre de éste, hasta llegar al root de la rama.
 * 
 * @param fit_sum Suma de los fitness de la(s) planta(s) a añadir o eliminar
 * @param plant_number Número de planta(s) a añadir o eliminar
 */
void Plant::report_change(double fit_sum, int plant_number){
	//cout<<"----- soy "<<this<<" y estoy reportando cambios"<<endl;
    if (this->getParent() != NULL){
		Plant * actual_parent = this->getParent();
		
		double fit_parent = actual_parent->getBFitness();
		unsigned int num_parent = actual_parent->getBSize();
		
		actual_parent->setBFitness(fit_parent + fit_sum);
		actual_parent->setBSize(num_parent + plant_number);
		
		actual_parent->report_change(fit_sum, plant_number);
    } 
    else{
		// root
		//update_TFitness( bfitness / bsize );
		colony->update_TFitness();
	}
}

/**
 * Existen 3 posibles casos:
 *  - Caso 1: La planta no posee descendencia y no es el nodo raíz:
 *	- El cambio es reportado
 *	- Se obtiene la dirección del padre
 *	- En la descendencia del padre se elimina a la actual planta
 * 
 *  - Caso 2: La planta tiene descendencia y no es la raíz
 *	- Se almacenan los datos a reportar
 *	- Se obtiene la dirección del padre
 *	- La descendencia de la planta, es agregada a la del padre
 *	- Se notifica el cambio en la colonia
 *	- La planta es eliminada de la descendencia del padre
 * 
 *  - Caso 3: La planta es la raíz
 *	- Opción 1: La raíz tiene descendencia:
 *	    - De las plantas "hijas", se escoge a la de mayor fitness
 *	    - Los datos de la mejor planta "hija", se actualizan ahora en el root
 *	    - La descendencia de la mejor planta, se traspasa a la raíz
 *		- Se actualizan los datos en la colonia
 * 		- Se elimina a la mejor de las plantas
 *	- Opción 2: La raíz no tiene descendencia:
 */
 // viejo
 void Plant::remove_plant(){
    //autopoyesis
    if (this->offspring.empty() && this->getParent() != NULL){ //cout<< "hoja";
		report_change(- this->getBFitness(), -this->getBSize());
		Plant * par = this->getParent();
		list<Plant>::iterator it = par->offspring.begin();
		while ((&(*it)) != &(*this)) it++;
		par->offspring.erase(it);
    }
    
    if (!(this->offspring.empty()) && this->getParent() != NULL){ //cout<< "interno";
		double fit = - this->getFitness();
		//int siz = - this->getSize();

		list<Plant>::iterator it;
		Plant * par = this->getParent();
		it = par->offspring.begin();
		while (&(*it) != &(*this)) it++;
		//this->report_change(fit,siz);
		this->report_change(fit, -1);

		/*// notificar cambio padre
		list<Plant>::iterator itr;
		for(itr=this->offspring.begin(); itr != this->offspring.end() ;itr++){
			(*itr).setParent(par);
		}*/
		
		par->offspring.splice(it,this->offspring);
		par->offspring.erase(it);
		
		list<Plant>::iterator itr;
		for(itr=par->offspring.begin(); itr != par->offspring.end() ;itr++){
			(*itr).setParent(par);
		}
    }

    if (this->getParent() == NULL){ //cout<< "root";
	
		list<Plant>::iterator it,best;
		best = this->offspring.begin();
		
		for (it=this->offspring.begin(); it != this->offspring.end(); it++){
			if ((*it).getBFitness() > (*best).getBFitness()) best = it;
		}

		(*best).setBFitness(this->getBFitness() - this->getFitness());
		(*best).setBSize(this->getBSize() - 1);
		
		this->setSolution(best->getSolution());//fala's hand
		this->setTFitness( best->getTFitness() );//fala's hand
		this->setAnno( best->getAnno());//fala's hand

		this->offspring.splice(best,(*best).offspring);

			this->getColony()->replace_root(&(*best));
			//this->getColony()->set_root(&(*best));

		this->offspring.erase(best);
		
		// notificar nuevo padre
		for(it=this->offspring.begin(); it != this->offspring.end() ;it++){
			(*it).setParent(this->getColony()->get_root());
		}
    }
}

/**
 * @return Retorna un entero, con el numero de conexiones de la actual planta
*/
//metodo que retorna el numero de conexiones de la planta
int Plant::getC() const{
	//if (this->getParent() == NULL)return this->offspring.size();	
	//else return this->offspring.size()+1;
    //return -1;
    return (this->getParent() == NULL) ? this->offspring.size() : this->offspring.size() +1 ; 
}

/**
 * @param _anno Número de la actual iteración del experimento
*/
void Plant::setAnno(unsigned int _anno){
	birth_day = _anno;
}

/**
 * @return Retorna un entero sin signo, con el número de la iteración en la cuál se creó la actual planta
*/
//metodo que retorna el anno de vida de la planta
unsigned int Plant::getAnno() const{
    return birth_day;
}

/**
 * @param _tfitness Valor con el nuevo TFitness a asignar
*/
// permite establecer el tfitness
void Plant::setTFitness(double _tfitness){
	tfitness = _tfitness;
}

/**
 * @return Retorna un double, con el TFitness de la actual planta
*/
// permite obtener el tfitness
double Plant::getTFitness(){
	return tfitness;
}

/**
 * @param aux Puntero temporal, con la planta de mayor fitness
 * 
 * @return Retorna un puntero del tipo Plant, con la planta del mayor fitness de la rama
*/
// permite obtener mejor planta de la rama
Plant *Plant::best_branch(Plant *aux){
	list<Plant>::iterator it;	
	for (it = offspring.begin(); it!=offspring.end(); it++){	
		if(it->getFitness() > aux->getFitness())aux=&(*it);	
		aux=it->best_branch(aux);
    }
    return aux;
}

/**
 * @param aux Puntero temporal, con la planta de menor fitness
 * 
 * @return Retorna un puntero del tipo Plant, con la planta del menor fitness de la rama
*/
// metodo que retorna el peor de todos para una determinada rama
Plant *Plant::worst_branch( Plant* aux ){
	list<Plant>::iterator it;	
	for (it = offspring.begin(); it!=offspring.end(); it++){		
		if(it->getFitness() < aux->getFitness())aux=&(*it);	
		aux=it->worst_branch(aux);		
    }
    return aux;
}

/**
 * Se determina que la planta con el menor TFitness de la rama, es aquella que se satisface del frente de pareto
 * 
 * @param peor Puntero temporal, con la planta de menor TFitness
 * 
 * @return Retorna un puntero del tipo Plant, con la planta del menor TFitness de la rama
*/
// frente pareto Purge
Plant* Plant::fpareto_purge( Plant* peor ){
	list<Plant>::iterator it;
    for (it = offspring.begin(); it!=offspring.end(); it++){	
		if( it->getTFitness() < peor->getTFitness() ) peor = &(*it);
		// TFitness igual
		else if ( it->getTFitness() == peor->getTFitness() ){
			// si tiene mas conexiones
			if( it->getC() > peor->getC() ) peor = &(*it);
		}
		// llamado de recursividad
		peor=it->fpareto_purge(peor);
    }
	return peor;
} 
// #### Purge fin

/**
 * @param plantas Lista temporal de punteros, con las plantas de la rama
 * 
 * @return Retorna una lista de punteros del tipo Plant, con las plantas de la rama
*/
// retorna todas las plantas de la rama en un arreglo
list<Plant*> Plant::plants_branch( list<Plant*> plantas ){
	list<Plant>::iterator it;
	for( it = offspring.begin() ; it != offspring.end() ; it++){
		plantas.push_back( &(*it) );
		//recursividad
		plantas = it->plants_branch(plantas);
	}
	return plantas;
}
/*
// retorna las hojas para la rama
list<Plant *> Plant::leafs_branch(list<Plant *> hojas){
	list<Plant>::iterator it;
    for (it = offspring.begin(); it!=offspring.end(); it++)
    {	
		// pregunto por: descendencia y existe padre.
		if (it->offspring.empty() && it->getParent() != NULL)hojas.push_back(&(*it));
		// recursividad
        hojas=it->leafs_branch(hojas);
    }
	return hojas;
}
*/

/**
 * @param fprom Valor del fitness promedio de la colonia
 * @param s Valor de la constante de TFitness a compartir
 * 
*/

// actualizar el TFitness para la rama
void Plant::update_TFitness( double const fprom , double const s){
	
	//tfitness = this->getFitness() - s * ( this->getFitness() - fprom );
	tfitness = this->getFitness() - s * ( this->getFitness() - fprom );
	
    list<Plant>::iterator it;
    for (it = offspring.begin(); it!=offspring.end(); it++){	
        it->update_TFitness(fprom , s);
    }	
}

// grafica para rama
void Plant::plot_branch( ofstream& file ){
	list<Plant>::iterator it;
	vector<double> vals1,vals2;
	for (it = offspring.begin(); it!=offspring.end(); it++)
    {
		//file << this->getSolution().getX() << " " << this->getSolution().getY() << " " << this->getFitness() << endl;
		//file << this->getSolution()->getValueCoord(0) << " " << this->getSolution()->getValueCoord(1) << " " << this->getFitness() << endl;
		vals1 = *(vector<double> *) this->getSolution()->getValues();
		for(unsigned int j=0;j< vals1.size() ;j++)
			file << vals1[j] << " ";
			
		file << this->getFitness() << endl;
		
        it->plot_branch(file);
        //file << it->getSolution().getX() << " " << it->getSolution().getY() << " " << it->getFitness() << endl;
        //file << it->getSolution()->getValueCoord(0) << " " << it->getSolution()->getValueCoord(1) << " " << it->getFitness() << endl;
        vals2 = *(vector<double> *) it->getSolution()->getValues();
		for(unsigned int j=0;j< vals2.size() ;j++)
			file << vals2[j] << " ";
			
		file << it->getFitness() << endl;
        
        
        //file << this->getSolution().getX() << " " << this->getSolution().getY() << " " << this->getFitness() << endl;
        
        //file << this->getSolution()->getValueCoord(0) << " " << this->getSolution()->getValueCoord(1) << " " << this->getFitness() << endl;
        vals1 = *(vector<double> *) this->getSolution()->getValues();
        for(unsigned int j=0;j< vals1.size() ;j++)
			file << vals1[j] << " ";
			
		file << this->getFitness() << endl;
    }
}

/**
 * Los criterios usados en éste método, son los siguientes:
 *  - Se obtienen todas las plantas de la rama en una lista enlazada
 *  - Se comprueba que en los extremos, a lo menos existan 3 plantas por lado
 *  - Se calcula la distancia entre las plantas y se compara con la mayor distancia
 *  - De ser mayor a la permitida, se debe cortar allí, de tal modo que se obtiene el puntero a esa planta
 *  - Se notifica a la rama que se cortará, a partir del puntero anteriormente mencionado
 *  - Se realiza el corte de la rama
 *  - Se retorna el puntero
 * 
 * @param total Número de planta en la rama
 * @param max_dist Valor de la distancia máxima permitida entre 2 plantas unidas
 * 
 * @return Retorna un puntero del tipo Plant, con la planta a ser root de una nueva colonia a agregar en el patio
*/
// otra opcion de split
Plant* Plant::split_branch( int total , double max_dist){
	Plant* ptrSplit = NULL;
    Plant *act=NULL;
    list<Plant>::iterator it;
	
	list<Plant*> plantas = this->getColony()->getPlants();
	
	list<Plant *>::iterator ter;
	for (ter = plantas.begin(); ter!=plantas.end(); ter++){
		if( act == NULL ){
			for (it = (*ter)->offspring.begin(); it != (*ter)->offspring.end(); it++){
				// a lo menos 3 platas por lado
				if( (total - it->getBSize() >= 3 ) && (it->getBSize() >= 3) ){
					// distancia mayor igual a max_dist
					if( (*ter)->getSolution()->Distance(it->getSolution()) >= max_dist ){
						act= &(*it);
						break;
					}
				}
			}	
		}
	}
	
    if(act != NULL){
		act=NULL;
		//Plant obj = (*it);
		Plant *obj = new Plant( *it );
		//Plant *obj = new Plant();
		//obj->copy( &(*it) );
		
		//obj->offspring.splice( obj->offspring.begin() , it->offspring ); // copio hijos
		//cout << "print it"<<endl;
		//it->show_list();
		
		
		obj->report_change( - obj->getBFitness(), - obj->getBSize()); // cambio
		//obj->getParent()->offspring.erase( it ); // elimino conexion ************
		(*ter)->offspring.erase( it );// linea sustituta
		
		obj->setParent( NULL );
		
		// cambio de padre
		obj->change_parent( obj );
		
		//cout << "\n\tSplit Plant" << endl;
		//obj->show_list(); // muestro la info
		
		ptrSplit = obj;
		return ptrSplit;
	}
    return ptrSplit;	
}

/**
 * Se asigna recursivamente el nuevo padre a la descendencia
 * 
 * @param ptr Puntero al nuevo "padre" de su descendencia
 * 
*/
// cambio de padre de la hoja
void Plant::change_parent(Plant * ptr){
	list<Plant>::iterator it;
	for(it = offspring.begin(); it != offspring.end() ;it++){
		it->setParent( ptr );
		// llamado de recursividad
		it->change_parent( &(*it) );
	}
}
		
/**
 * @param new_col Puntero a la colonia que pertenecerán los miembros de esa rama
 *
*/
// cambio de colonia para la rama
void Plant::change_colony_branch( Colony* new_col ){
	setColony( new_col );
	
	list<Plant>::iterator it;
    for (it = offspring.begin(); it!=offspring.end(); it++){	
        it->change_colony_branch( new_col );
    }
}

/**
 * @param coord Posición de la coordenada
 * @param val Valor de la mínima coordenada
 * 
 * @return Valor double, con la mínimo de los valores para esa coordenada
 * 
*/
// retorna en minimo de la coord
double Plant::get_min_branch_coord( int coord, double val ){
	list<Plant>::iterator it;
	vector<double> aux;
    for (it = offspring.begin(); it!=offspring.end(); it++){	
		aux = *(vector<double> *) it->getSolution()->getValues();
		if( aux[ coord ] < val ){
			val = aux[ coord ];
		}
		// llamado recursivo
		val = it->get_min_branch_coord( coord , val );
    }	
    return val;
}

/**
 * @param coord Posición de la coordenada
 * @param val Valor de la máxima coordenada
 * 
 * @return Valor double, con el máximo de los valores para esa coordenada
 * 
*/
// retorna en maximo de la coord
double Plant::get_max_branch_coord( int coord, double val ){
	list<Plant>::iterator it;
	vector<double> aux;
    for (it = offspring.begin(); it!=offspring.end(); it++){	
		aux = *(vector<double> *) it->getSolution()->getValues();
		if( aux[ coord ] > val ){
			val = aux[ coord ];
		}
		// llamado recursivo
		val = it->get_max_branch_coord( coord , val );
    }	
    return val;
}

/**
 * @param var Valor acumulado de la varianza
 * @param prom Valor promedio de los fitness de la rama
 * 
 * @return Valor de tipo double, con el valor de la varianza para la rama
 * 
*/
// retorna varianza de la rama
double Plant::get_varianza_branch( double var , double prom ){
	list<Plant>::iterator it;
    for (it = offspring.begin(); it!=offspring.end(); it++){	
        var += pow( it->getFitness() - prom , 2 );
		// llamado recursivo
		var = it->get_varianza_branch( var , prom );
    }	
    return var;
}

/**
 * @param old Puntero a la planta más antigua de la rama
 * 
 * @return Puntero del tipo Plant, con la planta con mayor antiguedad de la rama
 * 
*/
Plant* Plant::get_old_plant_branch(Plant *old){
	list<Plant>::iterator it;	
	for (it = offspring.begin(); it!=offspring.end(); it++){	
		if(it->getAnno() < old->getAnno()) old = &(*it);	
		// llamado de recursividad
		old = it->get_old_plant_branch(old);
    }
    return old;
}

void Plant::re_eval_func_branch(){
	Problem *ptr_prob;
	ptr_prob = Problem::instance();
	
	this->setFitness( ptr_prob->CalcularFitness(this->getSolution()) ); // re-evaluo la funcion
	list<Plant>::iterator it;	
	for (it = offspring.begin(); it!=offspring.end(); it++){
		it->re_eval_func_branch();// recursividad		
    }	
}

void Plant::show_plant(){
    cout << "F: "<< this->getFitness() ;
    cout << ", TF: "<< this->getTFitness() ;
    cout << ", BS: "<< this->getBSize() ;
    cout << ", BF: " << this->getBFitness();
    cout << ", Anno: "<< this->getAnno();
    cout << ", DPadre: " << this->getParent();
    cout << ", D: " << this;
    cout << ", C: "<< colony << endl;
}

/// @param tab Valor que nos permite desplazar la salida estandar por consola, en tab espacios.
void Plant::show_branch(int tab){
    for (int i = 0; i <= tab; i++) cout << " ";
    cout << "F: "<< this->getFitness() ;
    cout << ", TF: "<< this->getTFitness() ;
    cout << ", BS: "<< this->getBSize() ;
    cout << ", BF: " << this->getBFitness();
    cout << ", Anno: "<< this->getAnno();
    cout << ", DPadre: " << this->getParent();
    cout << ", D: " << this;
    cout << ", C: "<< colony << endl;
    list<Plant>::iterator it;
    for (it = offspring.begin(); it!=offspring.end(); it++)
    {	
        (*it).show_branch(tab+1);//it->show_list(tab+1);
    }
}
