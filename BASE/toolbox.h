/***************************************************************************
 *   Copyright (C) 2009 by Jorge Maturana                                  *
 *   jorge.maturana@gmail.com                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _TOOLBOX_
#define _TOOLBOX_

#include "rand31pmc.h"      
#include <map>
#include <vector>
#include <sys/time.h>

#include <string>
#include "math.h"
#include <fstream>   
#include <iostream>

#include <algorithm> // para sort
using namespace std;
//! singleton class (only one object is instantiated) that provides general-use methods
class toolbox{
 private:
  //! pointer to singleton object 
  static toolbox* pinstance;
  
  //! STL map with parameter values obtained from parameter file
  std::map<string,string> param;
  
  //! initial and final time for chronometer
  struct timeval ti,tf;
  
  //! random generator object 
  rand31dc rgo;
  
  // begin ------------- sandhill variables
  vector<double> peak; //!< height of the peak
  vector<double> posx; //!< x-position of the peak
  vector<double> posy; //!< y-position of the peak
  vector<double> reach1; //!< reach of the peak
  vector<double> hr1; //!< heigh of reach 1
  vector<double> reach2; //!< reach of the peak
  int npk; //!< number of peaks
  double sz; //!< size of domain [1:sz]x[1:sz]
  //string ExpID; //!< experiment ID
  // end  ------------- sandhill variables
  
  // begin -------------------- CCO variables
  vector<double> min_values;
  vector<double> max_values;
  // end ---------------------- CCO variables
    
 protected:
  //! constructor
  toolbox();
  
  toolbox(const toolbox&);
  
  toolbox &operator = (const toolbox&);
  
  
 public:
  //! return the pointer to the singleton instance
  static toolbox* instance();
   
  // ----------------------------------- CCO methods
  void read_min_values(string filename);
  void read_max_values(string filename);
  
  vector<double> get_min_values();
  vector<double> get_max_values();
   
  // ----------------------------------- sandhill methods
  
  void sandhill_create(string filename); //!< create from file
  //void sandhill_create(); //!< create from raw (random process)
  double sandhill_fitness(double x, double y);
  void sandhill_show();
  //void sandhill_setExpID(string id);
  
  // ----------------------------------- hold methods
  //! stops the execution waiting for an ENTER
  void hold();
  
  // ----------------------------------- chronometer methods
  //! start chronometer (counts microseconds until clockstop() is called)
  void clockstart();
  
  //! stop chronometer \return elapsed time in microseconds since clockstart() was called
  double clockstop();
  
  // ----------------------------------- random number generation methods
  /*! seed pseudo-random number generation
  @param semilla seed for pseudo-random generator
  */
  void seed(long unsigned int semilla);
  
  //! \return a random number in [0,1)
  double azar();
  
  //! \returns a random integer between [0,n-1)
  int azarInt(int n);
  
  //! \returns a number from distribution Normal(0,1)
  double normal();
  
  //! \returns a number from distribution Normal(mu,sigma). Uses: X: N(0,1), N(mu,sigma): Z=(X-mu)/sigma
  double normal(double mu, double sigma); 
  
  //! \returns a numbref from distribution triangular(mode,min,max)
  double triangular(double mode, double min, double max);
  
  // ----------------------------------- math methods
  //! \return the module of a: |a|
  double mod(double a);
  //int mod(int a);  //!< return the module of a: |a|
  
  // ----------------------------------- cast methods
  /*! \returns double from string (warning: doesn't check that the input can be 'translated')
  @param s input string
  */
  double string2double(string s);
  
  /*! \returns int from string (warning: doesn't check that the input can be 'translated')
  @param s input string
  */
  int string2int(string s);
  
  /*! \returns string from double (warning: doesn't check that the input can be 'translated')
  @param d input double
  @param prec number of decimal places
  */
  string double2string(double d, int prec);
  
  /*! \returns string from int **** not yet implemented
  @param i intput int
  */
  string int2string(int i);
  
  // ----------------------------------- algorithm parameters methods
  /*! read the parameter file and store values in the map param
  @param filename name of the file with parameters
  */
  void readparamfile(string filename);
  
  /*! store or rewrite the value of a double (floating point) variable (~ works like global variables)
  @param name name or parameter
  @param val value
  @param prec precision
  */
  void storeVariable(string name, double val, int prec);

  /*! store or rewrite the value of an string variable
  @param name name or parameter
  @param val value
  */
  void storeVariable(string name, string val);
  
  /*! store or rewrite the value of an integer variable (~ works like global variables)
  @param name name or parameter
  @param val value
  */
  void storeVariable(string name, int val);
  
  /*! \return value of a parameter as a double
  @param pname name of the parameter
  */
  double pval_double(string pname);
  
  /*! \return value of a parameter as an integer
  @param pname name of the parameter
  */
  int pval_int(string pname);
  
  /*! \return value of a parameter as a string
  @param pname name of the parameter
  */
  string pval_string(string pname);

  /*! \return name of the file without the extension (works only in Unix systems)
  @param path name of the path
   */
  string filenameFromPath(string path);
};

//#include "toolbox.cpp"
#endif
