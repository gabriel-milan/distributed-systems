/*
 * Sistemas Distribuídos - COS470
 *            2020/PLE
 * 
 * Trabalho Prático 2
 * Item 2 - Somador com Spinlocks
 * Arquivo em comum
 * 
 * Grupo:
 * -> Gabriel Gazola Milan / DRE 116034377
 * -> João Pedro Wieland / DRE 116077278
 */

//Este programa foi utilizado para facilitar a obtençao de dados para ser plotados no gráfico pedido no trabalho 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include<string>

#include<common.h>

using namespace std;


int main () {
    int parameters[20] = {1,2,4,8,16,32,64,128,256};
    char buffer[256];

    cout << "Tamanho vetor"<< TAMANHO_VETOR_SOMADOR <<"\n";

    	
	for (int i=0; i<9; i++) {
        string nomeComando = "./spinlock "+to_string(parameters[i]);
        const char *command = nomeComando.c_str();
        system(command);
        cout <<  "\n";
    }

    return OK; 






}
