/*
 * Sistemas Distribuídos - COS470
 *            2020/PLE
 * 
 * Trabalho Prático 2
 * Item 3 - Produtor-Consumidor com Semáforos
 * 
 * Grupo:
 * -> Gabriel Gazola Milan / DRE 116034377
 * -> João Pedro Wieland / DRE 116077278
 */

#include <iostream>
#include <common.h>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>

/*
 *  Função para gerar números aleatórios
 *  entre 1 e 10^7
 */
int generateN()
{
  return rand() % (int)10e7 + 1;
}

/*
 *  Função para verificar se um número é primo
 */
bool isPrime(int n)
{
  for (unsigned int i = 2; i < n / 2; i++)
    if ((n % i) == 0)
      return false;
  return true;
}

/*
 *  Implementação de semáforos retirada da C++ Pedia.
 *  Foram realizadas alterações para atender a necessidade do trabalho.
 *  Link: https://cpluspluspedia.com/en/tutorial/9785/semaphore
 */
class Semaphore
{
public:
  Semaphore(int count_ = 0)
      : count(count_)
  {
  }

  inline void notify()
  {
    std::unique_lock<std::mutex> lock(mtx);
    count++;
    cv.notify_one();
  }
  inline void wait()
  {
    std::unique_lock<std::mutex> lock(mtx);
    while (count == 0)
      cv.wait(lock);
    count--;
  }

private:
  std::mutex mtx;
  std::condition_variable cv;
  int count;
};

/*
 *  Método que imprime a forma de uso do script
 */
void printUsage()
{
  std::cout << "Usage: ./main < Np > < Nc > < N > < M >" << std::endl;
  std::cout << "Arguments:" << std::endl;
  std::cout << " - Np: número de produtores (ex: 3)" << std::endl;
  std::cout << " - Nc: número de consumidores (ex: 3)" << std::endl;
  std::cout << " - N: tamanho máximo da memória compartilhada (ex: 10)" << std::endl;
  std::cout << " - M (opcional): quantidade de números que devem ser gerados (padão: 100000)" << std::endl;
}

/*
 *  Função principal
 */
int main(int argc, char **argv)
{

  // Tratando argumentos
  if ((argc != 4) && (argc != 5))
  {
    std::cerr << "Número inválido de argumentos!" << std::endl;
    printUsage();
    return WRONG_ARG_NUMBER;
  }

  srand(time(NULL));

  int M;
  int Np = atoi(argv[1]);
  int Nc = atoi(argv[2]);
  int N = atoi(argv[3]);
  if (argc > 4)
    M = atoi(argv[4]);
  else
    M = (int)10e5;

  std::mutex M_mutex;

  std::vector<int> data;
  std::vector<std::thread> producers;
  std::vector<std::thread> consumers;

  Semaphore mutex(1);
  Semaphore empty(N);
  Semaphore full(0);

  for (unsigned int i = 0; i < Np; i++)
  {
    producers.push_back(std::thread([&]() {
      while (true)
      {
        // Checa se deve gerar mais dados
        int val_m = M;
        if (val_m <= 0)
          break;

        // Gera dados
        empty.wait();
        mutex.wait();
        M--;
        data.push_back(generateN());
        mutex.notify();
        full.notify();
      }
    }));
  }

  for (unsigned int i = 0; i < Nc; i++)
  {
    consumers.push_back(std::thread([&]() {
      while (true)
      {
        // Pega dados
        full.wait();
        mutex.wait();
        int singlePrime = data.back();
        data.pop_back();
        int dataSize = data.size();
        mutex.notify();
        empty.notify();

        // Consome dados
        std::cout << "Recebi o número " << singlePrime << ", ele é primo? " << isPrime(singlePrime) << std::endl;

        // Checa se deve consumir mais dados
        int val_m = M;
        if ((val_m <= 0) && (dataSize <= 0))
          break;
      }
    }));
  }

  for (unsigned int i = 0; i < Np; i++)
    producers[i].join();
  for (unsigned int i = 0; i < Nc; i++)
    consumers[i].join();
  return 0;
}