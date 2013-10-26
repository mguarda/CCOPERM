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

using namespace std;
#include <iostream>
#include <sstream>
#include <string.h> // requerido para test local (strcpy)
#include <string>
#include <vector>
#include <fstream>          
#include <sys/time.h>
#include <math.h>
#include <iomanip>
#include "toolbox.h"
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <functional>
#include <ctime>
#include <cstdlib>

toolbox* toolbox::pinstance = 0; // initialization

toolbox* toolbox::instance(){
 if(pinstance == 0) // if is called from the first time
   pinstance = new toolbox;
 return pinstance;
}//;

toolbox::toolbox(){
  long unsigned int seedrnd = (long unsigned int)(time(NULL) * getpid()); // get PID and time to seed the random number generator
  cout<<"RandNumGen seed: "<<seedrnd << endl;
  rgo.seedi(seedrnd);
  //rgo.seedi(2088967102); // problemas deltadin 0
  //rgo.seedi(3964248065);// prob dim 4 DinYurt
  //rgo.seedi(3498004928);// prob dim 1 DinYurt division por 0 normalizando(Yurt)
  
  //rgo.seedi(100);
}//;
             

// -------------------------------------------- hold
void toolbox::hold() {
    //char s;
    cout<<" ENTER to continue... ";
    cin.ignore();
    cout<<endl;
}//;


// -------------------------------------------- chronometer
void toolbox::clockstart() {
    gettimeofday(&ti, NULL);
}//;

double toolbox::clockstop() {
    // obtains current time and returns the amount of microseconds elapsed
    // from http://stackoverflow.com/questions/588307/c-obtaining-milliseconds-time-on-linux-clock-doesnt-seem-to-work-properly
    gettimeofday(&tf, NULL);
    return 1000000*(tf.tv_sec - ti.tv_sec)+tf.tv_usec - ti.tv_usec;
}//;


// -------------------------------------------- random number generation

void toolbox::seed(long unsigned int semilla){
  cout<<"New RandNumGen seed: "<<semilla<<endl; 
  rgo.seedi(semilla);
}//;

inline double toolbox::azar(){   
  return(rgo.ranf());
}//;

int toolbox::azarInt(int n){
  int result;
  do{
    result=rgo.ranlui()/(2147483646/n);
  }while(result>=n);
  return result;
}//;

//! as seen in http://www.bearcave.com/misl/misl_tech/wavelets/hurst/random.html
double toolbox::normal(){
  double x1,x2,w,y;
  do{
    x1 = 2.0 * azar() - 1.0;
    x2 = 2.0 * azar() - 1.0;
    w = x1 * x1 + x2 * x2;
  }while( w >= 1.0);
  w = sqrt( (-2.0 * ( log(w)/log(exp(1)) )  ) / w );
  y = x1 * w;
  return y;
}//;

//! as seen in http://www.brighton-webs.co.uk/distributions/triangular.asp
double toolbox::triangular(double mode, double min, double max){
  double r,u=azar();
  if( u <= (mode-min)/(max-min))
    r = min+sqrt(u*(max-min)*(mode-min));
  else
    r = max-sqrt((1-u)*(max-min)*(max-mode));
  return r;
}//;

inline double toolbox::normal(double mu, double sigma){
  return normal()*sigma+mu;
}//;


// -------------------------------------------- math

inline double toolbox::mod(double a){
  if(a>=0)
    return a;
  return -a;
}//;

/*
inline int toolbox::mod(int a){
  if(a>=0)
    return a;
  return -a;
};
*/

// -------------------------------------------- algorithm parameters
/*
// Old method: reads format:  <name> <value> <commentary>
 
void toolbox::readparamfile(string filename){
  // read the first two words (name and value) and ignores the rest of the line
  char * charfile = new char[filename.length()+1];
  strcpy(charfile,filename.c_str());
  ifstream ifs(charfile,ifstream::in);
  string name="", value=""; // name and value of the parameter
  char rest[900]; // rest of the line (description of the parameter, or nothing at all)
  while(!ifs.eof()){
    ifs>>name;
    ifs>>value;
    ifs.getline(rest,900);
    if(name.size()>0 && value.size()>0)
      param[name] = value;
    name = "";
    value = "";
  };
 ifs.close();
};
*/

// New method: reads the former format and ignores lines starting with #

void toolbox::readparamfile(string filename){
  char * charfile = new char[filename.length()+1];
  strcpy(charfile,filename.c_str());
  ifstream ifs(charfile,ifstream::in);
  string name="", value=""; // name and value of the parameter
  while(!ifs.eof()){
    // read the first word. If the first character is #, ignore the line.
    ifs>>name;
    if(name[0]=='#' || name[0]==' '){
	//if(name[0]=='#' || name[0]==' ' || name.compare("min_values") || name.compare("max_values")){
      name=""; value="";
      ifs.ignore(99999,'\n');
    }      
    //else if( name.compare("max_values") == 0 ){ cout << " ----------------HOLASS " << endl; }
    else{ //store the pair (name,value)
      ifs>>value;
	  ifs.ignore(99999,'\n');
      if(name.size()>0 && value.size()>0){
        param[name] = value;
        cout<<"Parameter: "<<name<< " = "<<value<<endl; // show value loaded
      }//;
      name=""; value="";
    }//;
  }//;
  ifs.close();
  read_max_values(filename);
  read_min_values(filename);
}//;

void toolbox::read_min_values(string filename){
	string token; 
	double val=0;
	ifstream file( filename.c_str() ); 
	while(std::getline(file, token)) { 
		istringstream line(token); 
		while(line >> token) { 
			if( token.compare("min_values") == 0 ){
				while(line >> token) {
					val=atof( token.c_str() );
					min_values.push_back( val );
				}
			}
		} 
	} 
	cout << "min_values" <<" ";
	for (unsigned int i = 0; i < min_values.size() ; i++)
		cout<< min_values[i]<<" ";
	cout<<endl;
}

void toolbox::read_max_values(string filename){
	string token; 
	double val=0;
	ifstream file( filename.c_str() ); 
	while(std::getline(file, token)) { 
		istringstream line(token); 
		while(line >> token) { 
			if( token.compare("max_values") == 0 ){
				while(line >> token) {
					val=atof( token.c_str() );
					max_values.push_back( val );
				}
			}
		} 
	} 
	cout << "max_values" <<" ";
	for (unsigned int i = 0; i < max_values.size() ; i++)
		cout<< max_values[i]<<" ";
	cout<<endl;
}

vector<double> toolbox::get_min_values(){
	return min_values;
}

vector<double> toolbox::get_max_values(){
	return max_values;
}

void toolbox::storeVariable(string name, double val, int prec){
  param[name]= double2string(val,prec);
}//;

// fala's hand
void toolbox::storeVariable(string name, string val){
  param[name]= val;
}//;

void toolbox::storeVariable(string name, int val){
  param[name]= int2string(val);
}//;

double toolbox::pval_double(string pname){
  istringstream ss(param[pname]);
  double daux;
  ss >> daux;
  return daux; 
}//;

int toolbox::pval_int(string pname){
  istringstream ss(param[pname]);
  int iaux;
  ss >> iaux;
  return iaux; 
}//;

string toolbox::pval_string(string pname){
  return param[pname];
}//;

double toolbox::string2double(string s){
  istringstream ss(s);
  double daux;
  ss >> daux;
  return daux; 
}//;
   
int toolbox::string2int(string s){
  istringstream ss(s);
  int iaux;
  ss >> iaux;
  return iaux; 
}//;

string toolbox::double2string(double d, int prec){
  // basado en
  //  http://stackoverflow.com/questions/864250/converting-double-to-string-in-c
  //  http://www.cppreference.com/wiki/io/sstream/start
  stringstream out;
  out << fixed << setprecision(prec) << d;
  return out.str();
}//;


string toolbox::int2string(int i){
  stringstream out;
  out<< i;
  return out.str();
}//;

  /*
  //! transforms anything to a string (requires include<sstream> (inline))
  template <class T> string toString(const T& t){
    stringstream ss;
    ss << t;
    return ss.str();
};
  */

// -------------------------------------------- Sandhill

void toolbox::sandhill_create(string filename){
    char * charfile = new char[filename.length()+1];
    strcpy(charfile, filename.c_str());
    ifstream ifs(charfile, ifstream::in);
    //double aux;
    ifs>>sz;
    ifs>>npk;
    // resize vectors
    peak.resize(npk);
    posx.resize(npk);
    posy.resize(npk);
    reach1.resize(npk);
    hr1.resize(npk);
    reach2.resize(npk);
    for(int i=0;i<npk;i++){
      ifs>>posx[i];
      ifs>>posy[i];
      ifs>>peak[i];
      ifs>>reach1[i];
      ifs>>hr1[i];
      ifs>>reach2[i];
      ifs.ignore(99999,'\n');
    }
  }


/*
//! debe ser invocado DESPUES de readparamfile
void toolbox::sandhill_create(){
	
	//int numpeaks, double size, int shape, double reqpeak, string IDexp){

    // copy some value to local vars
    //tb = toolbox::instance();
    npk = pval_int("sh_numpeaks");
    sz = pval_double("sh_size");
    int shape = pval_int("sh_shape");
    
    //ExpID=IDexp;
    // resize vectors
    peak.resize(npk);
    posx.resize(npk);
    posy.resize(npk);
    reach.resize(npk);
    // generate peak heights
    for(int i=0;i<npk-1;i++){
      peak[i]=1;
      for(int j=0;j<shape;j++)
	peak[i] *= peak[i] * azar();
    };
    peak[npk-1] = pval_double("sh_reqpeak");
    //peak.sort(); // *************************** como?
    sort(peak.begin(),peak.end());
    // now generate peaks position and calculate reach
    for(int i=0;i<npk;i++){
      posx[i] = azar()*sz;
      posy[i] = azar()*sz;
      reach[i] = sz / (3+47*pow(peak[i],3));
    };
    // finally, export landscape data to an external file
    string filename = pval_string("ExpID")+".peaks";
    char * charfile = new char[filename.length()+1];
    strcpy(charfile,filename.c_str());  // ***********************
    ofstream pf(charfile, ios_base::out);
    pf<<sz<<endl; //write size
    pf<<npk<<endl; // write number of peaks
    for(int i=0;i<npk;i++)
      pf<<posx[i]<<" "<<posy[i]<<" "<<peak[i]<<" "<<reach[i]<<endl;
    pf.close();
  }//;;
*/

double toolbox::sandhill_fitness(double x, double y){
    double fit=0;
    for(int i=0;i<npk;i++){
      double dist = hypot(x-posx[i],y-posy[i]);
      if(dist < reach2[i]){
	double f_i;
	if(dist < reach1[i])
	  f_i = hr1[i] + (reach1[i]-dist)*(peak[i]-hr1[i])/reach1[i];
	else
	  f_i = hr1[i]*(reach2[i]-dist)/(reach2[i]-reach1[i]);
	if(f_i>fit)
	  fit=f_i;
      }
    }
    return fit;
  }

void toolbox::sandhill_show(){
    cout<<sz<<endl;
    cout<<npk<<endl;
    for(int i=0;i<npk;i++)
      cout<<posx[i]<<" "<<posy[i]<<" "<<peak[i]<<" "<<reach1[i]<<" "<<hr1[i]<<" "<<reach2[i]<<endl;
  }
	
string toolbox::filenameFromPath(string filepath){
	string file = "";
	int pivot1 = 0;
	int final = filepath.size() - 1;
	int index = final;
	while(filepath[index] != '.'){
		index --;
	}
	pivot1 = index;
	index--;
	while(filepath[index] != '/' && index != -1){
		index --;
	}
	index++;
	for(int i = index; i < pivot1; i++){
		file.push_back(filepath[i]);
	}
	return file;
}

/*
// ############# TEST ############
int main(){
 // toolbox is a singleton class, what it means that just one object is instantiated
 // this is how the toolbox object is used:
 toolbox *tb; // first, a pointer to a toolbox object is created
 tb = toolbox::instance(); // then we ask the class toolbox to return a reference to the (unique) toolbox object, and we asign it to our pointer
 
 // finally, the pointer is used to call methods ( tb->method(...) )

 tb->storeVariable("pi",3.141592,2);
 cout<<"pi con 2 dec: "<< tb->pval_double("pi")<<endl;
 tb->storeVariable("beast",666);
 cout<<"number of the beast: "<<tb->pval_int("beast")<<endl;
 //tb->seed(5);
 tb->clockstart(); 
 for(int i=0;i<5;i++)
   cout<<tb->azarInt(8)<<" \t "<<tb->azar()<<" \t "<<tb->normal()<<" \t "<<tb->normal(10,.5)<<" \t "<<endl;
 //tb->hold(); 
 cout<<tb->clockstop()<<endl;
 
 
 // *********************** COMO USAR SANDHILL **************
 
 // antes se deben leer los parámetros desde el archivo de configuración
 tb->readparamfile("setup.conf");
 
 // luego se debe setear el nombre del experimento en el toolbox
 tb->sandhill_setExpID("prueba");
 
 // después existen dos opciones:
 //   1. Se crea el sandhill al azar, a partir de los parámetros en el archivo de configuración: 
 tb->sandhill_create();
 
 //   2. Se recoge la configuración desde un archivo .peaks
 tb->sandhill_create("prueba1.peaks");
 
 // finalmente se invoca sandhill_fitness(double x, double y) para obtener el valor de fitness

 double siz=100; 
  for(int i=0;i<=siz;i++){
    for(int j=0;j<=siz;j++)
      cout<<i<<" "<<j<<" "<<tb->sandhill_fitness(i,j)<<endl; 
    cout<<endl;
};
  
  
  
  
};
//*/

