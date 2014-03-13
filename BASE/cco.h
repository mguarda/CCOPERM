#ifndef CCO_H
#define CCO_H

#include <string>
#include "problem.h"
#include "colony.h"

class CCO{

private:
	/// Puntero a la clase abstracta Problem
	Problem* problem;	
	/// Lista que contiene a todas las colonias del patio
	std::list<Colony*> yard;	
	/// Palabra identificador del experimento
	string IDExp;
	/// Numero inicial de colonias
	int COL;
	/// Numero max de plantas por colonia
	int MPY;
	/// Numero max de generaciones
	int MAXGEN;	
	/// Fitness maximo y minimo del patio
	double fitmax,fitmin;
	
	// ********************************************** METODOS PRIVADOS
	/// Retorna todas las plantas del patio, con sus coordenadas y fitness
	std::vector< std::vector<double> > AllPlantsYard();
	/// Determina el fitness minimo y maximo entre todas las plantas presentes en el patio
	void min_max_yard();
	// ********************************************** METODOS PRIVADOS FIN
	
public:
	/// Contructor de la clase CCO
	CCO();
	/// Destructor de la clase CCO
	~CCO();
	/// Permite ejecutar la clase CCO
	void run();	
	/// Metodo que permite eliminar plantas a la colonia
	void Purge( Colony* col, int const iter );
	/// Metodo que permite agregar plantas a la colonia
	void Extend( Colony* col, int const iter );
	/// Provoca eventualmente la división de la colonia
	void Split( Colony* col, int const iter );
	/*/// Metodo que permite descongestionar el patio
	void Stress( int const iter );
	/// Reasigna el numero de plantas para cada colonia
	void Reallocate( int const iter );
	*/
	/// Metodo que estimula la competicion de las colonias
	void Competition( int const iter );
	/// Metodo que determina del patio la(s) solucion(es) mas robusta(s) y de buena calidad (fitness alto)
	void Distill();
	
	//Colony* prueba( Colony* col, int const iter );
	
};
#endif // CCO_H


/**
 * \mainpage Documentación de C.C.O. - Clonal Colony Optimization
 * 
 * En la presente documentación, se documentarán los métodos y clases implementadas en la metaheurística C.C.O. (<B>C</B>lonal <B>C</B>olony <B>O</B>ptmization), que se encuentran codificados en el lenguaje de programación <B>C++</B>.
 *
 * El principal objetivo que se desea lograr al implementar la metaheurística adaptativa e inspirada en la naturaleza C.C.O., es la de hallar soluciones robustas en problemas de optimización, sin incurrir en costos computacionales extras al evaluar la robustez de las soluciones candidatas. Ésto es posible, al imitar el esquema colaborativo de la <B>colonias clonales</B> presente en la naturaleza.
 * 
 * La estructura básica de la metaheurística C.C.O., se encuentra constituida por un <B>algoritmo iterativo generacional</B>, el cual se describe a continuación:
 * 
 * 
 * <IMG SRC="../html2/image/algorithm2.png" ALT="Algoritmo generacional iterativo" ALIGN="center"/> 
 * 
 * 
 * Un complemento idóneo a la presente documentación, correspondería a un reporte técnico. La utilidad que nos presentaría un reporte técnico, sería la de permitirnos relacionar a el código implementado, con la lógica de la metaheurística C.C.O.
 * <UL>
 * <LI>Enlace disponible para el reporte técnico: <a href="../html2/HTLATEXDocumentacionCCO.html">Link-></a> </LI>
 * </UL>
 * 
 * Por último, esto es parte de un trabajo de tesis realizado para optar al título de Ingeniero Civil en Informática de la <a href="www.uach.cl">Universidad Austral de Chile</a>, por lo que las personas relacionadas con ésta, son las siguientes:
 * 
 * <b>Profesor patrocinante:</b>
 * <UL>
 * <LI>Dr. Ing. Jorge Patricio Maturana Ortíz (<a href="http://www.inf.uach.cl/maturana/">Personal Page-></a>) </LI>
 * </UL>
 * <b>Alumno tesista:</b>
 * <UL>
 * <LI>Mauricio Alexis Guarda Oñate</LI>
 * </UL>
*/

