//EJERCICIOS DE SISTEMAS OPERATIVOS EN LENGUAJE C 
//https://sites.google.com/site/sogrupo15/2-llamadas-al-sistema-para-el-control-de-procesos
//Libreriras 
#include "funciones.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char const *argv[]){
	
	printf("\n\t*********************************************************\n");
	printf("\t***************** LLAMADAS AL SISTEMA *********************\n");
	printf("\t***********************************************************\n");
	printf("\n\t1. Control de procesos.\n");
	//pruebaFork();
	pruebaFork2(); 
	printf("\n");
	pruebaGetPid();
	printf("\n");
	//pruebaExec();
	//printf("\n");
	printf("\n\t2. Control de señales.\n");
	//pruebaSenal1();
	pruebaSenal2();


	return 0;
}
