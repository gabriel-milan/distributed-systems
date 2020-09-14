/*
 * Sistemas Distribuídos - COS470
 *            2020/PLE
 * 
 * Trabalho Prático 1
 * Item 1 - Sinais
 * Parte 1 - Envio de Sinal
 * 
 * Grupo:
 * -> Gabriel Gazola Milan / DRE 116034377
 * -> João Pedro Wieland / DRE 116077278
 */
#include <common.h>
#include <iostream>
#include <signal.h>

int main(){
    int pid; 
    int signal; 


    std::cout << "Process ID:";
    std::cin >>  pid;

    if (kill(pid,0)!=0){
        std::cout <<"PID not Found\n";
        return PID_NOT_FOUND; 
    }

    std::cout << "Signal:";
    std::cin >> signal;


    try{
        if (kill(pid,signal) != 0){
            throw errno; 
        } 
        std::cout << "signal " << signal << " sent to " << pid << "\n";
        return OK;
    }
    catch(int e){
        std::cout << "error " << e << "\n" ;
        return SIGNAL_ERROR;  
    }
        
    

}
