/**
 * \class CCO
 * \brief CCO (Clonal Colony Optimization)
 * 
 * 
 * */


//#include "toolbox.cpp"
//#include "colony.h"
//#include "plant.h"
//#include <list>
using namespace std;
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <unistd.h>
#include "toolbox.h"
#include "cco.h"
#include "plant.h"
//#include "Tracker.h"
#include <iostream>
#include <locale>

CCO::CCO(){
	toolbox *tb;
    tb = toolbox::instance();
    
    COL = tb->pval_double("COL");
    MPY =  tb->pval_double("MPY");
	MAXGEN = tb->pval_double("MAXGEN");
	
	IDExp = tb->pval_string("IDExp");
	int k = 0;
	for(int i=0;i<COL;i++){
		//Solution stmp;
		Colony *ctmp=new Colony();
		ctmp->setMPC( int( MPY / COL ));
		ctmp->setName( "Colony_" + tb->int2string( i+1 ) );
		srand(unsigned(time(NULL)+k));
		Plant *cplant= new Plant(ctmp);
	//	usleep(790*1000);
		cplant->setAnno( 0 );
		k++;
		yard.push_back(ctmp);
	}
	/*
	cout << "Colonias creadas:" << endl;
	list<Colony*>::iterator it;	
	for (it = yard.begin(); it!=yard.end(); it++){		
		cout << "Nombre: " << (*it)->getName() << " ID " << (*it)->get_root()->getColony() << " " << endl;   	 
	}*/
	//cout<<"por_renov "<<por_renov<<endl;
}
 
CCO::~CCO(){}

/**
 * En este método se implementa toda la <B>lógica de la metaheurística</B>, que coordina las interacciones entre las clases Plant y Colony.
 * 
 * Los pasos simplificados son los siguientes:
 * \code
 * While{ejecutar las MAXGEN iteraciones}{
 *
 * 	ForEach{colonia en el patio}{
 * 		Purge(colonia)
 * 		Extend(colonia)
 * 		Split(colonia)
 *	}
 * 
 * 	Competition()
 * }
 * 
 * Distill()
 * \endcode
 */

void CCO::run(){
    
    toolbox* tb;
    tb = toolbox::instance();
    
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime(buffer,80,"%c",timeinfo);
	string date_experiment(buffer);
	tb->storeVariable("date",date_experiment);

	string ruta_resultado = "./Results/";
	string formato = ".csv";
	string nombre = "fitness";
//  ofstream salida_main ( ("./Results/"+ IDExp + "_numcolonias.dat").c_str() );
    ofstream salida_main ( ("./Results/"+tb->pval_string("problemType")+"/"+ IDExp + "_numcolonias"+"("+buffer+")"+".dat").c_str() );
    ofstream resultado((ruta_resultado+tb->pval_string("problemType")+"/"+IDExp+"_"+nombre+"("+buffer+")"+formato).c_str());
    if(salida_main){
    	cout << "se ha creado el archivo " << IDExp + "_numcolonias"+"("+buffer+")"+".dat" << endl;
    }else{
    	cout << "Error al crear el archivo " << IDExp + "_numcolonias"+"("+buffer+")"+".dat" << endl;
    	exit(0);
    }
//================================================================================
    if(resultado){
    	cout << "se ha creado el archivo " << ruta_resultado+IDExp+"_"+nombre+"("+buffer+")"+formato << endl;
    }else{
      	cout << "Error al crear el archivo " << ruta_resultado+IDExp+"_"+nombre+"("+buffer+")"+formato << endl;
       	exit(0);
    }
    locale mylocale("");
    resultado.imbue(mylocale);
    resultado << "Generacion | Fitness | Costo | Solucion" << endl;
//================================================================================
	if( tb->pval_int("op_distill") != 1 ){
//		 if( remove( ("./Results/"+ IDExp + "_numcolonias.dat").c_str() ) != 0 )
		if( remove( ("./Results/"+tb->pval_string("problemType")+"/"+ IDExp + "_numcolonias"+"("+buffer+")"+".dat").c_str() ) != 0 )
			perror( "Error deleting file" );
		else
			puts( "File successfully deleted" );
	}

	problem = Problem::instance();
	
	//Tracker Tr( *( DynamicYurt *) problem->pointer() );
	
	for(int j = 0;j<MAXGEN;j++){
		cout << "============================= Generación Nº" << j+1 << "=============================" << endl;
		//cout << "Generacion " << j << endl;
		list<Colony*>::iterator it = yard.begin();	
		//for (it = yard.begin(); it!=yard.end(); it++){		
		COL = yard.size(); // numero de colonias
		
		//if( problem->EsDinamico() )
		//	problem->step();
		
		// aplicar min_max_yard solo cuando delta es dinamico
		if( tb->pval_int("op_deltadin") == 1 ){
			min_max_yard();
			//cout<<"iter "<<j<<" fitmin "<<fitmin<<" fitmax "<<fitmax<<endl;
		}
				
		for (int l=0; l < COL; l++){	
			//cout<< "Operando sobre colonia "<<l<<" +++++++++++++++++++++++++++++++"<<endl;	
			//cout<<"Size "<<(*it)->getSize()<<endl;
			//cout<<"MPC "<<(*it)->getMPC()<<endl;
			
			// pregunto si se aplica el porcentaje de renovacion
			if( tb->pval_int("op_renov") == 1 ){
				while( (*it)->getSize() > (*it)->getMPC()*(1 - tb->pval_double("por_renov")) ){
					Purge( *it , j );
				}
			}
			else{
				if( (*it)->getSize() > 1 ) Purge( *it , j );
			}
			
							
			while( (*it)->getSize() < (*it)->getMPC() ){
				//cout<<"Extend run"<<endl;
				Extend( *it , j );
			}
			
			//Split()
			Split( *it , j);
						
			/*
			printf("Inicio Colonia\n");
			cout << "Size: " << yard.size()<< endl;
			cout << "Nombre Colonia: " << (*it)->getName()<< endl;
			(*it)->show_list();
			printf("Fin Colonia\n");
			*/
			it++;
		}
		/*
		cout<<endl<<"Y el resultado es::::::::::"<<endl;
		list<Colony*>::iterator tt;
		cout << endl << "\t\tNuevo yard" << endl;
		cout << "\t\tSize Nuevo yard __ " << yard.size() << endl;
		for (tt = yard.begin(); tt!=yard.end(); tt++){		
			cout << (*tt)->getName() << " root " << (*tt)->get_root()->getParent() << endl;
			(*tt)->show_list();
			cout << endl;
		}
		cout << "Fin anno " << j << endl; */
		
		COL = yard.size(); // numero de colonias
		// gen num_colonias
		if( tb->pval_int("op_distill") == 1 ){
			salida_main << j << " " << COL << endl;
		}
		
		Competition(j);
		
		if( problem->EsDinamico() )
			problem->step( AllPlantsYard() , j );
//================================================================================
		list<Colony*>::iterator it2 = yard.begin();
		list<Colony*>::iterator paux_col;
		double b_fitness = 0;
		for(it2 = yard.begin(); it2 != yard.end(); it2++){
			if((*it2)->getBestPlant()->getFitness() > b_fitness){
				b_fitness = (*it2)->getBestPlant()->getFitness();
				paux_col = it2;
			}
		}
		resultado << (j+1) << " | ";
		(*paux_col)->getBestPlant()->getSolution()->show_solution(resultado);
		resultado << endl;
//================================================================================
	}
	
	//if( problem->EsDinamico() )
	//	problem->fin_experimento( "./Results/" + IDExp );
	
	// Pregunto por operar o no Distill
	if( tb->pval_int("op_distill") == 1 ){
		Distill();
	}
	
	if( tb->pval_int("op_distill") == 1 ){
		salida_main.close();
	}
	resultado.close();
	cout<<" --- CCO done ---"<<endl;
}

/**
 * El criterio que determina la planta a eliminar, es para aquella que posea el peor fitness de la colonia.
 * 
 * Los pasos simplificados son los siguientes:
 * <UL>
 * <LI>Se escoge la planta que tenga el peor fitness de la colonia</LI> 
 * <LI>Se elimina la planta, modificando así a la colonia</LI>  
 * </UL>
 * 
 * \param colonia* Puntero a la colonia que se le eliminará una planta.
 * \param iter Número de la iteración actual
 */ 
void CCO::Purge( Colony *colonia, int const iter){
	cout << "---------------------------------Inicio Purge ..."<<endl;
	
	// obtengo el candidato a eliminar
	Plant *peor = colonia->Purge();
	
	// elimino el candidato
	if( peor != NULL ){
		//cout << " Planta a eliminar ";
		//peor->show_plant();
		peor->remove_plant();
	}
	toolbox *tb;
	tb = toolbox::instance();
	// aplicar min_max_yard solo cuando el delta es dinamico
	if( tb->pval_int("op_deltadin") == 1 ){
		//min_max_yard();
		Plant *aux = colonia->getWorstPlant();
		double cmp = aux->getFitness();
		if( cmp < fitmin )fitmin = cmp;
		
		aux = colonia->getBestPlant();
		cmp = aux->getFitness();
		if( cmp > fitmax )fitmax = cmp;
	}
	
	//else{ cout << "ERROR Metodo Purge al eliminar peor ..." << endl;}
	
//	cout << " --------------------------------------Purge end." << endl;
}

/**
 * 
 * El criterio que determina la planta a extender, se le añade un hijo en su vecindad, de tal modo que quede a una distancia prudente, es decir, ni tan lejos, ni tan cerca de la planta padre.
 * 
 * Los pasos simplificados son los siguientes:
 * <UL>
 * <LI>Se escoge una planta (que se llamará aquí "padre") con el método roullete, sobre la colonia</LI> 
 * <LI>Se crea una nueva solución, a partir de la planta padre</LI>  
 * <LI>La nueva solución se extiende</LI>
 * <LI>La nueva solución se le agrega a una nueva planta hija</LI>
 * <LI>La nueva planta hija se agrega a la planta padre</LI>
 * </UL>
 * 
 * \param colonia* Puntero a la colonia que se añadirá una planta.
 * \param iter Número de la iteración actual
 */
void CCO::Extend( Colony *colonia, int const iter){
//	cout << "-------------------------------------- Extend INICIO" << endl;
	//obtengo nodos hojas y ruleta
	Plant *G = colonia->roulette();
//	cout<<"próximo padre: "<<G<<endl;
//	cout<<"padre del próximo padre: "<<G->getParent()<<endl;
	toolbox* tb;
	tb = toolbox::instance();
//	cout << "Se ha selecciionado una planta por roulette ... " << endl;
	// calculo del nuevo d
	double daux = tb->pval_double("d");
	if( tb->pval_int("op_deltadin") == 1 ){
		if( (fitmax - fitmin) > 0 ){
			daux = ((tb->pval_double("dmin")-tb->pval_double("dmax"))/(fitmax-fitmin)) * ( G->getFitness() - fitmin) + tb->pval_double("dmax");
		}
		else{
			// decido entre el dmin o el dmax
			daux = (tb->azar() < .5) ? tb->pval_double("dmin") : tb->pval_double("dmax");
		}
		
		//cout<<"fitmin: "<<fitmin<<" fitmax: "<<fitmax<<" Fitness "<<G->getFitness()<<" Valor daux: "<<daux<<endl;
	}
//	cout << "Extendiendo ..." << endl;
	Solution* sol_hijo = G->getSolution()->ExtendSolution( daux );
//	cout << "Se ha extendido una solucion de la planta "<< G << " seleccionada previamente ..." << endl;
	G->add_child( sol_hijo , iter );
//	cout << "se ha agregado el hijo ..." << endl;
	// verifico si se aplica el delta dinamico
	if( tb->pval_int("op_deltadin") == 1 ){
		double cmp = sol_hijo->getFitness();
		if( cmp < fitmin )fitmin = cmp;
		else if( cmp > fitmax )fitmax = cmp;		
	}
		
//	cout << "------------------------------------- Extend end." << endl;
}

/**
 * 
 * El criterio que determina las plantas a cortar, es para una distancia lo suficientemente larga y que a lo menos exista en sus extremos 3 plantas por lado, se procede a cortarlas y/o liberarlas. 
 * 
 * Las plantas cortadas de la colonia inicial, son tratadas como una nueva colonia en el patio, con la primera planta ubicada en el extremo de la mayor distancia, como la planta raíz (root) de la nueva colonia.
 * 
 * Los pasos simplificados son los siguientes:
 * <UL>
 * <LI>Encontrar la distancia que satisface el criterio de corte entre las plantas de la colonia</LI> 
 * <LI>Se crea una nueva colonia</LI>  
 * <LI>Se establece la primera planta del extremo, como la raíz (root) de la nueva colonia</LI>
 * <LI>La nueva colonia se agrega al patio</LI>
 * </UL>
 * 
 * \param colonia* Puntero a la colonia que se le aplicará o no un corte.
 * \param iter Número de la iteración actual
 */
//list<Colony*> CCO::Split( Colony *colonia , int const iter){
void CCO::Split( Colony *colonia , int const iter){
//	cout << "-------------------------------------- Split INICIO" << endl;;
	
	if( colonia->getSize() >= 6 ){
		Plant *aux = NULL;
		aux = colonia->getWorstPlant();
		
		toolbox* tb;
		tb = toolbox::instance();
		
		// verifico si se aplica el delta dinamico
		double daux = tb->pval_double("d");
		if(tb->pval_int("op_deltadin") == 1 ){
			if( (fitmax - fitmin) > 0 ){
				daux = ((tb->pval_double("dmin")-tb->pval_double("dmax"))/(fitmax-fitmin)) * ( aux->getFitness() - fitmin) + tb->pval_double("dmax");
			}
			else{
				// decido entre el dmin o el dmax
				daux = (tb->azar() < .5) ? tb->pval_double("dmin") : tb->pval_double("dmax");
			}
			//cout<<"Valor daux Split: "<<daux<<endl;
		}
		
	//	double sum_aux = 0;
		
		problem = Problem::instance();
		
	/*	for (unsigned int i = 0; i < problem->getDimension(); i++){
			sum_aux += pow( daux * (problem->getMaxValuesPos(i) - problem->getMinValuesPos(i)), 2) ;
		}
		
		double max_dist = tb->pval_double("fsplit") * sqrt( sum_aux );*/
		double max_dist = tb->pval_double("fsplit")*(problem->getDimension()-1);
		//Plant* new_root = colonia->get_root()->split_branch( colonia->get_root()->getBSize(), max_dist );	
		Plant* new_root = colonia->Split( colonia->getSize() , max_dist );	

		if ( new_root != NULL ){ 	
			//cout<<" !!!!! se decide cortar!!!"<<endl;
			//cout << "\tSplit CCO" << endl;
			//(lista.begin())->show_list();
			//cout<< " Col sin rama"<< endl;
			//colonia->show_list();
			//cout<<"Se aplica split"<<endl;
			
			Colony *col = new Colony();// cout << "1"<< endl;
			toolbox *tb;tb = toolbox::instance();col->setName("new_" + colonia->getName() + "_iter_"+ tb->int2string( iter ) );
			col->set_root( new_root );// cout << "2"<< endl;
			col->change_colony();
			col->setMPC( new_root->getBSize() );
			
			//cout << "Col nueva" << endl;
			//col->show_list();
			
			yard.push_back( col );
			//yard.push_back( lista.front() );
			
			/*list<Colony*>::iterator v;
			v=yard.end();v--;
			for(int j=0;j<2;j++){
				Extend( *v , j+20 );
				cout<<" Extend "<< j <<endl;
			}*/
			
			
			//yard.back()->show_list();
			
			/*
			list<Colony*>::iterator tt;
			cout << endl << "\t\tNuevo yard" << endl;
			cout << "\t\tSize Nuevo yard __ " << yard.size() << endl;
			for (tt = yard.begin(); tt!=yard.end(); tt++){		
				cout << (*tt)->getName() << " root " << (*tt)->get_root()->getParent() << endl;
				(*tt)->show_list();
				cout << endl;
			}*/
			//run( iter + 1 );
		}
	}
	/*
	else{
		cout << " no aplica Split " << endl;
	}*/
	
//	cout << "------------------------------------------- Split end." << endl;
	
	//return yard;
}

/**
 * En este método, se desea poner a prueba la versatilidad de las colonias, ajustándolas a mayores cambios.
 * 
 * Los criterios simplificados en su funcionamiento, son los siguientes:
 * 
 * <UL>
 * <LI><B>1.- Reasignación:</B> Para un número total de plantas que se puedan generar en todo el patio (MPY), se debe distribuir proporcionalmente para todas sus colonias(COL)</LI>
 * 
 * <LI><B>2.- Superposición y frente de pareto:</B> En la superposición de una colonia, para aquellas que sobrepasan el 50% de su área y además si su fitness proporcional es menor, está se debe eliminar. Para aquellas que se dominan mutuamente, osea más del 50% entre ellas, se escoge al azar cuál de éstas se elimina. Para el caso del frente de pareto, se debe eliminar aquella que sea dominada por las demás colonias, con el criterio del fitness promedio de cada colonia</LI>
 * </UL>
 * 
 * \param iter Número de la iteración actual
 */

void CCO::Competition( int iter ){
//	cout << "-----------------Competition:inicio -----------------" << endl;
	// REALLOCATE
	list<Colony*>::iterator it;
	for (it = yard.begin(); it!=yard.end(); it++){
		(*it)->setMPC( int( MPY / COL ) );
		//(*it)->show_plot(iter);
	}
	
	///// \todo REVISAR los metodos ademas de los SWITCH
	//cout << "------------------------------------------- Stress INICIO." << endl;
	
	toolbox *tb;
	tb = toolbox::instance();
	// OPCION DOS
	
	//cout << "opcion 2" << endl;
	list<Colony*>::iterator it1,it2;
	list<Plant*>::iterator plant;
	long double Acol1,Vcol1,Promcol1,Acol2,Vcol2,Promcol2;
	Colony* peor=NULL;
	int aux=0,domant=0,reponer=0;
	
	// pregunto si aplico o no criterio de colonia dominada
	if( tb->pval_int("op_coldom") == 1 ){

		// determino colonia mas pareto dominada
		////*
		for (it1 = yard.begin(); it1!=yard.end(); it1++){// recorro el patio
			Acol1 = (*it1)->getArea();
			Vcol1 = (*it1)->getVarianza();
			Promcol1 = (*it1)->getMeanColony();
//			cout << "Acol1/Vcol1 = " << (Acol1/Vcol1) << " colonia " << (*it1)->getName() << endl;
			aux=0;
			//cout<<"-------> Col "<<(*it1)->getName()<<" size "<<(*it1)->getSize()<<" aux "<< aux<<" domant "<<domant<<endl;
			for (it2 = yard.begin(); it2 != yard.end(); it2++){// con respecto a los demas del patio
				if( it1 != it2 ){
					Acol2 = (*it2)->getArea();
					Vcol2 = (*it2)->getVarianza();
					Promcol2 = (*it2)->getMeanColony();
					
					// realizo las comparaciones
					if( (Promcol1 <= Promcol2) && ((Acol1/Vcol1) <= (Acol2/Vcol2)) ){
						//peor = *it1;
						//cout<<Promcol1<<" "<<Promcol2<<" "<<Acol1/Vcol1<<" "<<Acol2/Vcol2<<endl;
						aux++;
					}
				}
			}//end_for it2
			//if( peor == NULL ){
			//	peor = *it1;
				//cout << "NULL iter " <<iter<<endl;
			//}
			//if( peor != NULL ){
				if(aux>domant){
					domant=aux;
					peor=*it1;
					//cout << "not NULL COL "<<peor->getName()<<" aux " <<aux<<" domant "<<domant<<endl;
				}
			//}
		}//end_for it1
		// verificar si es apta a eliminar la colonia
		if( peor != NULL ){
			Plant* old = peor->getOldPlant();// la planta mas antigua de la colonia
			//cout << "Not NULL" << endl;
			if( ((iter -old->getAnno())+1) > ( tb->pval_double("elderFactor") * peor->getSize()) ){
				// ELIMINAR COLONIA DEL PATIO
				//cout << "eliminar op DOS col " << peor->getName() << " iter " << iter<<endl;
				it1 = yard.begin();
				while ( *it1 != peor ) it1++;
				yard.erase( it1 );
				reponer++;
				COL=yard.size(); //nuevo tamanno del patio
			}
		}//*/
	}
	
	if( tb->pval_int("op_overlap") == 1 ){
		// OPCION UNO
//		cout << "op_overlap = 1" << endl;
		//cout << "opcion 1 COL " <<COL<<" size Yard " << yard.size()<<endl;
		////*
		it1=yard.begin();
		it2=yard.begin();	
		//list<Colony*> eliminar;
		vector<Colony*> eliminar;
		double Asuper=0, Asuper2=0;
		// recorro el patio
		for(it1=yard.begin();it1!=yard.end();it1++){
			for(it2=yard.begin();it2!=yard.end();it2++){
				if( *it1 != *it2 ){
					// determinar area superpuesta
					Asuper = (*it1)->Superposicion( *it2 );
					if( Asuper > 0 ){
//						cout << "Existe superposicion de areas entre colonias" <<endl;
	//					usleep(1000000);
						//cout << "super area " << Asuper << endl;
						Asuper2 = (*it2)->Superposicion( *it1 );
						// pregunto por eliminacion
						if( Asuper > .5 && Asuper2 > .5 ){// las 2 superpuestas sobre el 50%
							if( tb->azar() > 0.5 && ((*it2)->getMeanColony() > (*it1)->getMeanColony()) ){//! \todo revisar caso de moneda
								eliminar.push_back( *it1 );//cout <<" add it1 " <<*it1 << endl;
							}
							else{
								if ( (*it1)->getMeanColony() > (*it2)->getMeanColony() ){
									eliminar.push_back( *it2 );//cout <<" add it2 " <<*it2 << endl;
								}														
							}
						}
						else if( Asuper > .5 && ((*it2)->getMeanColony() > (*it1)->getMeanColony()) ){//col1 superpuesta >50%
							eliminar.push_back( *it1 );//cout <<" add it1 " <<*it1 << endl;
						}
						else if( Asuper2 > .5 && ((*it1)->getMeanColony() > (*it2)->getMeanColony()) ){// col2 superpuesta >50%
							eliminar.push_back( *it2 );//cout <<" add it2 " <<*it2 << endl;
						}
					}/*else{
						cout << "Asuper1 < 0" << endl;
					}*/
				}//end_if
			}//end_for j
		}//end_for i
		
		if(!eliminar.empty()){		
//			cout << "Lista a eliminar colonias está vacia" << endl;
			//for(int i=0;i<int(eliminar.size());i++){
			//	cout << eliminar[i] << " ";
			//}cout << " eliminar" << endl;	
			//eliminar.unique();eliminar.unique();//quitar las colonias duplicadas
			//for(it1=eliminar.begin();it1!=eliminar.end();it1++){
			//	cout << *it1 << " ";
			//}cout << " eliminar unique" << endl;
			//int aux_del = eliminar.size();
			////*
			//vector<Colony *> vectorTmp(eliminar.begin(),eliminar.end());
			vector<Colony *> vectorTmp;//,vec(eliminar.begin(),eliminar.end());
			bool estado=true;
			if( eliminar.size() > 1 ){
				vectorTmp.push_back( eliminar[0] );
				for(int i=1;i<int(eliminar.size());i++){//recorro vector original
					estado=true;
					for(int j=0;j<int(vectorTmp.size());j++){//recorro nuevo vector
						if( eliminar[i] == vectorTmp[j] )estado=false;
					}
					if(estado)vectorTmp.push_back( eliminar[i] );
				}
			}
			else vectorTmp.push_back( eliminar[0] );
			
			//int aux_del = vectorTmp.size();reponer += aux_del;
			reponer += vectorTmp.size();
			
			//cout << "Size vectorTmp: " << vectorTmp.size() << endl;
			//if(vectorTmp.size() >0)cout << "Num de plantas a eliminar " << vectorTmp.size() << " iter "<< iter << endl;
			//for(it1=yard.begin();it1!=yard.end();it1++){
			//	cout << *it1 << " ";
			//}cout << " Yard" << endl;
			//for(int i=0; i< int(vectorTmp.size()) ;i++){
			//	cout <<  vectorTmp[i] << " ";
			//}cout << " vectorTmp"<<endl;
			
			for(int i=0; i< int(vectorTmp.size()) ;i++){
				it1=yard.begin();
				while( vectorTmp[i] != *it1 ) it1++;
				//cout << "Eliminando ...Nombre "<< (*it1)->getName() <<endl;
				yard.erase( it1 );
				COL=yard.size();//nuevo tamanno del patio
			}
			eliminar.clear();
			vectorTmp.clear();
			//*/
		}
		//*/	
	}
	

	COL=yard.size()+reponer;
	//si elimino colonias, sustituirlas por nuevas semillas al azar en el patio
	for(int i=0;i<reponer;i++){
		Colony *nueva=new Colony();
		nueva->setMPC( int( MPY / COL ));
		nueva->setName( "Colony_" + tb->int2string( iter+MAXGEN+i ) );
		
		Plant *plantn= new Plant(nueva);
		plantn->setAnno( iter );
		//cout << " new col " <<  nueva->getName() <<" size "<<nueva->getSize() <<endl;
		//Plant *cplant= new Plant();
		//cplant->setColony(ctmp);
		yard.push_back(nueva);
	}	
	COL=yard.size();
	
//	cout << "------------------------------------------- Stress END" << endl;
}

/**
 * Este método determina el o los individuos que posean soluciones robustas y de buena calidad. Ofreciéndole al usuario resultados concretos a su problema.
 * 
 * La o las soluciones que se determinen, son aquellas que pertenecen al <B>frente de Pareto</B>.
 * 
 * Los criterios que se analizan en el frente de Pareto, son 2, y a continuación se mencionan:
 * 
 * <UL>
 * <LI><B>1.-</B>	Por cada colonia, se escoge la planta que esté más al centro de ésta. La candidata es aquella planta que tenga la menor suma de distancias a otras plantas de la misma colonia</LI>
 * 
 * <LI><B>2.-</B>	La robustez, se determina a partir de la división entre el área de la colonia y la varianza de los fitness de todas las plantas de la colonia</LI>
 * </UL>
 * 
 * En resumen, la identificación de las soluciones se realiza al finalizar el proceso de la búsqueda y son aquellas que maximizan los criterios anteriormente mencionados, robustez y muy buena calidad.
 */

void CCO::Distill(){
	///// \todo REVISAR
//	cout << "------------------------------------------- DISTILL INICIO." << endl;
	toolbox* tb = toolbox::instance();
	list<Plant*> plantas;
	vector<Plant*> final;
	double suma = 0;
	double var = 0;
	double area = 0;
	double menor = 9999999;
	Plant* ptr_peor;
	
	list<Colony*>::iterator it;
	list<Plant*>::iterator plant;
	list<Plant*>::iterator iter1, iter2;
	vector<Plant*>::iterator itv1, itv2;
	
	vector< vector<double> > val_decs; // almaceno los variables decision
	vector<double> v_aux;// | A/var | menor_distancia | dominado |
	
	//cout<<"Tam Patio "<<yard.size()<<endl;
	
	int z = 0;
	for (it = yard.begin(); it!=yard.end(); it++){ // recorro todo el patio
		z++;
		//(*it)->show_colony(); // muestro la colonia
//		cout << "para la colonia " << z << " ";
		//cout<<"Colonia "<<*it<<endl;
		// todas las plantas de la colonia
		plantas = (*it)->getPlants(); 
		// calculo el area de la colonia
		area = (*it)->getArea(); // area
//		cout << "el area es "<< area << " ";
		// calculo la varianza de la colonia
		var = (*it)->getVarianza();
//		cout << "la varianza es "<< var << " ";
		//cout<<"area: "<<area<<" varianza: "<<var<<endl;
		
		if( var > 0 ){ // problema de division por 0
			//cout<<"paso por la varianza"<<endl;
			// ROBUSTEZ
			v_aux.push_back( area / var ); // primer op decision
//			cout << "y la razon entre el area y la varianza es "<< area/var << endl;
			//cout<<"1v_aux "<< area/var <<endl;
			suma = 0;
			menor = 99999999;
			// calculo de la distancia entre las plantas de la colonia
			for (iter1 = plantas.begin(); iter1!=plantas.end(); iter1++){ // c/u de los elementos
				//cout<<"-------------------------------------------------------------------------------"<<endl;
				suma = 0;	
				//cout<<"iter1: "<< *iter1 <<endl;
				for (iter2 = plantas.begin(); iter2!=plantas.end(); iter2++){ // con respecto a los demas
					//cout<<"iter2: "<< *iter2 <<endl;
					if( iter1 != iter2 ){
//						suma += pow( (*iter1)->getSolution()->Distance( (*iter2)->getSolution() ) , 2);//-----CAMBIO A PERMUTACION ----
						suma += (*iter1)->getSolution()->Distance( (*iter2)->getSolution() );
						//cout<<"suma: "<<suma<<endl;
					}
				}// end_for	
				//cout<<" menor "<<menor<<" suma "<<suma<<endl;
				// escoger la planta con menor distancia
				if( suma < menor ){
					menor = suma;
					ptr_peor = *iter1;
					//cout<<"Colonia: "<<ptr_peor->getColony()<<endl;
				}
			}// end for
			
			// agrego la planta con menor distancia
			v_aux.push_back( menor ); // segunda op decision
			//cout<<"2v_aux "<<menor<<endl;
			v_aux.push_back( 0 ); // tercera op decision 
			final.push_back( ptr_peor ); // agrego el puntero al peor
			//cout<<"ptr_peor "<<ptr_peor<<endl;
			val_decs.push_back( v_aux ); // agrego los valores decision
			v_aux.clear();
			
		}//end_if
		plantas.clear();
	}//end_for
	
	/*cout<<"val_decs"<<endl;
	for (unsigned int i = 0; i < val_decs.size(); i++){
		cout<<(final[i])->getColony()<<" "<<(final[i])->getFitness()<<" "<<val_decs[i][0] <<" "<<val_decs[i][2]<< endl;
	}
	cout << endl;*/
	// determinar plantas dominantes
	for (unsigned int i = 0; i < val_decs.size() ; i++){
		for (unsigned int j = 0; j < val_decs.size() ; j++){
			if( i != j ){
				if( (val_decs[i][0] >= val_decs[j][0]) && ((final[i])->getFitness() >= (final[j])->getFitness()) ){
					val_decs[j][2] = 1; // dominada
				}
			}
		}
	}

	/*cout << "Valores de decision"<< endl;

	for (unsigned int i = 0; i < val_decs.size() ; i++){
			for (unsigned int j = 0; j < val_decs.size() ; j++){
				cout << val_decs[i][j]<< " " ;
			}
			cout << endl;
	}*/

	/*
	cout<<"val_decs Dom"<<endl;
	for (unsigned int i = 0; i < val_decs.size(); i++){
		cout<<(final[i])->getFitness()<<" "<<val_decs[i][0] <<" "<<val_decs[i][2]<< endl;
	}*/
	// archivo salida
	ofstream out_distill( ("./Results/"+tb->pval_string("problemType")+"/" + IDExp + "_paretofinal"+"("+tb->pval_string("date")+")"+".dat").c_str() );
	if(out_distill)
		cout << "Se ha creado el archivo " + IDExp + "_paretofinal"+"("+tb->pval_string("date")+")"+".dat" << endl;
	else
		cout << "Error: no se pudo crear el archivo "+ IDExp + "_paretofinal"+"("+tb->pval_string("date")+")"+".dat" << endl;
	//-------------------formato local--------------------------------
	locale mylocale("");
	out_distill.imbue(mylocale);
	//----------------------------------------------------------------
	for (unsigned int i = 0; i < val_decs.size(); i++){
		if( val_decs[i][2] != 1 ){
			// muestro los valores de la posicion
			//cout<<"pos final: "<<i<<endl;
			final[i]->getSolution()->show_solution( out_distill );
			out_distill<< endl;
			//out_distill<< val_decs[i][0] << endl;
		}
		
	}
		
	out_distill.close();
	
	cout << "------------------------------------------- DISTILL END" << endl;
	
}

void CCO::min_max_yard(){
//	cout << "-------------------------min_max_yard Inicio"<<endl;
	list<Colony*>::iterator it;
	Plant *aux=NULL;
	fitmin=100000;
	fitmax=-100000;
	
	problem = Problem::instance();
	
	for (it = yard.begin(); it!=yard.end(); it++){//recorro el patio
		
		if( problem->EsDinamico() )
			(*it)->Re_Eval_Func(); // reevaluar la colonia
			
		aux = (*it)->getWorstPlant();
		if( aux->getFitness() < fitmin )fitmin= aux->getFitness();
		aux = (*it)->getBestPlant();
		if( aux->getFitness() > fitmax )fitmax= aux->getFitness();
	}
//	cout << "-------------------------min_max_yard FIN"<<endl;
	
	//cout<<"Tam patio "<<yard.size()<<endl;
	//cout<<"fitmin "<<fitmin<<" fitmax "<<fitmax<<endl;
}

/**
 * \return Vector de vectores con todos las plantas del patio
 */

vector< vector<double> > CCO::AllPlantsYard(){
	//cout << "-------------------------ALLPLANTSYARD INICIO"<<endl;
	vector< vector<double> > arreglo;
	vector<double> aux;
	vector<int >aux_val;
	list<Plant*> plantas;
	
	// | x(0) | y(1) | fitness(2)
	list<Colony*>::iterator it;
	list<Plant*>::iterator it2;
	for (it = yard.begin(); it!=yard.end(); it++){// recorro el patio
		//(*it)->Re_Eval_Func();
		plantas = (*it)->getPlants();
		
		for (it2 = plantas.begin(); it2 !=plantas.end(); it2++){
			//cout<<"Planta: "<<(*it2)->getSolution().getX()<<" , "<<(*it2)->getSolution().getY()<<endl;
			aux_val = *( vector<int> *) (*it2)->getSolution()->getValues();
			for (unsigned int i = 0; i < (*it2)->getSolution()->getDimension() ; i++){
				aux.push_back( aux_val[i] );
			}
			aux.push_back( (*it2)->getFitness() );
			
			arreglo.push_back( aux );
			aux.clear();
		}
	}
	//cout << "-------------------------ALLPLANTSYARD FIN"<<endl;	
	return arreglo;
}

