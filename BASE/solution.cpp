/**
 * \class Solution
 * \brief Clase Abstracta Solution
 *  
 * En esta clase, se define la estructura general de las distintas soluciones a ser codificadas.
 * 
 * Al ser una clase general, los métodos más particulares y específicos que se requieran para cada una de las distintas codificaciones, se definen como <B>virtuales</B>. En todas las subclases de Solution, obligatoriamente se deben redefinir esos métodos. Los cuales son:
 * 
 * <UL>
 * <LI>getValues</LI> 
 * <LI>Distance</LI>  
 * <LI>ExtendSolution</LI>  
 * <LI>show_solution</LI>  
 * </UL>
 * 
 * De los cuales se comenta más abajo.
*/

using namespace std;
#include "solution.h"
#include "problem.h"
#include "toolbox.h"
#include <cmath>

Solution::Solution(){}

Solution::~Solution(){}

/**
 * \param _fitness valor a establecer como fitness de la solucion
 */
void Solution::setFitness( double _fitness ){
	fitness = _fitness;
}

/**
 * \return Retorna el fitness de la solucion
 */
double Solution::getFitness(){
	return fitness;
}

/**
 * \return Retorna la dimension de la solucion
 */
unsigned int Solution::getDimension(){
	return dimension;
}
