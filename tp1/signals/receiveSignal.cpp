#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/types.h>
#include <common.h>

using namespace std;

int defineWaitType(){
    int typedNumber=2;
    while (typedNumber!=0 && typedNumber!=1){
        std::cout << "Digite 0 para busy wait ou 1 para blocking wait:";
        std::cin >> typedNumber;
    }
    return typedNumber; 
}

void signalHandler( int signalNumber ) {


   switch(signalNumber) {
    case 1:
        cout << "Recebi o SigHup aqui! valeu! \n";
        exit(signalNumber);  
        break; 
    case 2:
        cout << "Recebi o SigInt aqui! valeu! \n";
        exit(signalNumber);  
        break; 
    case 3:
        cout << "Recebi o SigQuit aqui! valeu! \n";
        exit(signalNumber);  
        break; 
    case 4:
        cout << "Recebi o SigIll aqui! valeu! \n";
        exit(signalNumber);  
        break; 
    
}
}

int main () {
   int waitType; 
   pid_t pid = getpid();

   signal(SIGHUP, signalHandler);  
   signal(SIGINT, signalHandler);  
   signal(SIGQUIT, signalHandler);  
   signal(SIGILL, signalHandler);  

   cout << "O PID desse programa é " <<pid << "\n "; 
   waitType = defineWaitType();

   if (waitType ==1)
        //Blocking Wait
        while(true) {
            cout << "Não recebi um sinal ainda. A mimir...\n";
            sleep(2);
        }
   else
        //Busy waiting
        while(true){
            cout << "manda logo! Vou ficar tentando sem parar até receber\n";
        }
   return OK;
}
