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
 *  Wrapper para escrever na tela
 */
void log(std::string message, std::mutex coutMutex)
{
  coutMutex.lock();
  std::cout << message << std::endl;
  coutMutex.unlock();
}

/*
 *  Função principal
 */
int main(int argc, char **argv)
{

  srand(time(NULL));

  int M = (int)10e5;
  int N = 5;
  int Np = 1;
  int Nc = 1;

  std::mutex M_mutex;
  std::mutex coutMutex;

  std::vector<int> data;
  std::vector<std::thread> producers;
  std::vector<std::thread> consumers;

  Semaphore mutex(1);
  Semaphore empty(N);
  Semaphore full(0);

  std::thread producer([&]() {
    while (true)
    {
      // Checa se deve gerar mais dados
      M_mutex.lock();
      int val_m = M;
      M_mutex.unlock();
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
  });

  std::thread consumer([&]() {
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
      coutMutex.lock();
      std::cout << "Recebi o número " << singlePrime << ", ele é primo? " << isPrime(singlePrime) << std::endl;
      coutMutex.unlock();

      // Checa se deve consumir mais dados
      M_mutex.lock();
      int val_m = M;
      M_mutex.unlock();
      if ((val_m <= 0) && (dataSize <= 0))
        break;
    }
  });

  producer.join();
  consumer.join();
  return 0;
}