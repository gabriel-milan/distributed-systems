/*
 * Sistemas Distribuídos - COS470
 *            2020/PLE
 * 
 * Trabalho Prático 3
 * Item 5 - Medição e avaliação 
 * 
 * Grupo:
 * -> Gabriel Gazola Milan / DRE 116034377
 * -> João Pedro Wieland / DRE 116077278
 */

#include <testing.h>
#include <bits/stdc++.h> 
#include <iostream>

using namespace std;

string generateCommand(char * r, char * k, int id){
   string command = "../client/client ";
   std::string stringId = std::to_string(id);
   command.append(r);
   command= command+" ";
   command.append(k);
   command= command+" ";
   command= command+" "+stringId;
   return command;

}


int main(int argc, char **argv)
{
  int n = atoi(argv[1]);
  int id;


  for (int i = 0; i < n; i++) {
    id= 100+i;
    string comando = generateCommand(argv[1],argv[2],id);
    //system("../client/./client 3 1 123"); 
    system(comando.c_str());
  }


}
