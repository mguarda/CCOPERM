//============================================================================
// Name        : main.cpp
// Author      : Mauricio Guarda
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

using namespace std;
#include "cco.h"
#include "toolbox.h"
#include "problem.h"
#include <time.h>


int main(int argc , char **argv){

	if( argc > 1 ){
		toolbox *tb;
		tb = toolbox::instance();
		tb->readparamfile("./setup.conf");
		// almaceno en el toolbox el nombre del experimento
		string file = argv[1];

		tb->storeVariable("IDExp",tb->filenameFromPath(argv[1]));
		tb->storeVariable("filepath",argv[1]);
		tb->storeVariable("problemType","TSP");

		Problem *problem = Problem::instance();

		int sec = 0;


		CCO coo;
		tb->clockstart();
		coo.run();
		sec = tb->clockstop()/1000000;

		time_t seconds(sec);
		tm *p = localtime(&seconds);
		cout << "el tiempo de ejecucion ha sido de " << p->tm_min << " min " << p->tm_sec << "seg " << endl;
	}
 	else{
//		cout << "Invocación: ./EJECUTABLE_TSP_PERM <Experiment_ID>" << endl;
 		cout << "Se necesita referencia a archivo de problema TSP" << endl;
 		exit(0);
	}

    return 0;
}
