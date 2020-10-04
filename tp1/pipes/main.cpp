/*
 * Sistemas Distribuídos - COS470
 *            2020/PLE
 * 
 * Trabalho Prático 1
 * Item 3 - Pipes
 * 
 * Grupo:
 * -> Gabriel Gazola Milan / DRE 116034377
 * -> João Pedro Wieland / DRE 116077278
 */

#include <iostream>
#include <main.h>
#include <stdlib.h>
#include <unistd.h>

// Função para verificar se um número é primo
bool isPrime(int n)
{
  for (unsigned int i = 2; i < n / 2; i++)
    if ((n % i) == 0)
      return false;
  return true;
}

// Função para gerar números para o produtor
int generateN(int prevN = 0)
{
  if (prevN == 0)
    return 1;
  return prevN + rand() % 100 + 1;
}

// Método que imprime a forma de uso do script
void printUsage()
{
  std::cout << "Usage: ./pipe <n_numbers>" << std::endl;
  std::cout << "Arguments:" << std::endl;
  std::cout << " - n_numbers: número de números a serem gerados (1 <= n_numbers) (ex. 1000)" << std::endl;
}

/* A utility function to reverse a string  */
// Retirado de: https://www.geeksforgeeks.org/implement-itoa/#:~:text=For%20example%3A%2D%20if%20base,with%20a%20minus%20sign%20(%2D).
void reverse(char str[], int length)
{
  int start = 0;
  int end = length - 1;
  while (start < end)
  {
    std::swap(*(str + start), *(str + end));
    start++;
    end--;
  }
}

// Implementation of itoa()
// Retirado de: https://www.geeksforgeeks.org/implement-itoa/#:~:text=For%20example%3A%2D%20if%20base,with%20a%20minus%20sign%20(%2D).
char *itoa(int num, char *str, int base)
{
  int i = 0;
  bool isNegative = false;

  /* Handle 0 explicitely, otherwise empty string is printed for 0 */
  if (num == 0)
  {
    str[i++] = '0';
    str[i] = '\0';
    return str;
  }

  // In standard itoa(), negative numbers are handled only with
  // base 10. Otherwise numbers are considered unsigned.
  if (num < 0 && base == 10)
  {
    isNegative = true;
    num = -num;
  }

  // Process individual digits
  while (num != 0)
  {
    int rem = num % base;
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    num = num / base;
  }

  // If number is negative, append '-'
  if (isNegative)
    str[i++] = '-';

  str[i] = '\0'; // Append string terminator

  // Reverse the string
  reverse(str, i);

  return str;
}

// Função principal
int main(int argc, char **argv)
{
  // Checando número de parâmetros
  if (argc != 2)
  {
    std::cerr << "Número de argumentos incorreto!" << std::endl;
    printUsage();
    return WRONG_ARG_NUMBER;
  }

  // Inicializando a seed aleatória
  srand(time(NULL));

  // Tratando argumentos
  int n_numbers = atoi(argv[1]);
  if (n_numbers <= 0)
  {
    std::cerr << "Número fornecido inválido!" << std::endl;
    printUsage();
    return INVALID_NUMBER;
  }

  // Inicializando o pipe
  // pipe[0] é a ponta do consumidor
  // pipe[1] é a ponta do produtor
  int pipeEnds[2];
  if (pipe(pipeEnds) == -1)
  {
    std::cerr << "Falha ao iniciar o pipe!" << std::endl;
    return PIPE_ERROR;
  }

  // Criando processo filho
  int pid = fork();
  if (pid == -1)
  {
    std::cerr << "Falha ao realizar o fork!" << std::endl;
    return FORK_ERROR;
  }

  // *****************************
  // Processo filho é o produtor
  // *****************************
  if (pid == 0)
  {
    // Fecha-se a ponta do consumidor
    close(pipeEnds[0]);

    // Produzindo números e os escrevendo
    char msg[MESSAGE_MAX_SIZE];
    int tmp = 0;
    for (unsigned int i = 0; i < n_numbers; i++)
    {
      tmp = generateN(tmp);
      itoa(tmp, msg, 10);
      if (write(pipeEnds[1], &msg, sizeof(char[MESSAGE_MAX_SIZE])) == -1)
      {
        std::cerr << "Erro ao escrever no pipe!" << std::endl;
        return WRITE_PIPE_ERROR;
      }
    }
    tmp = 0;
    itoa(tmp, msg, 10);
    if (write(pipeEnds[1], &msg, sizeof(char[MESSAGE_MAX_SIZE])) == -1)
    {
      std::cerr << "Erro ao escrever no pipe!" << std::endl;
      return WRITE_PIPE_ERROR;
    }
  }
  // *****************************
  // Processo pai é o consumidor
  // *****************************
  else
  {
    // Fecha-se a ponta do produtor
    close(pipeEnds[1]);

    // Adicionando um contador (facilita debugging e visualização)
    unsigned int counter = 0;

    // Adquirindo os números e realizando verificação
    while (true)
    {
      int val;
      char msg[MESSAGE_MAX_SIZE];
      counter++;
      if (read(pipeEnds[0], &msg, sizeof(char[MESSAGE_MAX_SIZE])) == -1)
      {
        std::cerr << "Erro ao ler do pipe!" << std::endl;
        return READ_PIPE_ERROR;
      }
      val = atoi(msg);
      if (val == 0)
        break;
      std::cout << "Número #" << counter << ": " << val << ". É primo? " << isPrime(val) << std::endl;
    }
  }

  return OK;
}