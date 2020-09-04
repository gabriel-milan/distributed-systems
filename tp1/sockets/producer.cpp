/*
 * Sistemas Distribuídos - COS470
 *            2020/PLE
 * 
 * Trabalho Prático 1
 * Item 4 - Sockets
 * Parte 2 - Produtors
 * 
 * Grupo:
 * -> Gabriel Gazola Milan / DRE 116034377
 * -> João Pedro Wieland / DRE 116077278
 */

#include <common.h>
#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

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
  std::cout << "Usage: ./producer <n_numbers> <ip>" << std::endl;
  std::cout << "Arguments:" << std::endl;
  std::cout << " - n_numbers: número de números a serem gerados (1 <= n_numbers) (ex. 1000)" << std::endl;
  std::cout << " - ip (opcional): IP da máquina onde está o consumidor (caso não informado, o padrão é 127.0.0.1)" << std::endl;
  std::cout << "* Disclaimer: os IPs digitados não serão verificados quanto a sua validez" << std::endl;
}

// Função principal
int main(int argc, char **argv)
{
  // Tratando argumentos
  if ((argc != 2) && (argc != 3))
  {
    std::cerr << "Número inválido de argumentos!" << std::endl;
    printUsage();
    return WRONG_ARG_NUMBER;
  }

  char *ip;
  int n_numbers = atoi(argv[1]);
  if (argc > 2)
    ip = argv[2];
  else
    ip = (char *)"127.0.0.1";

  // Criando socket
  // SOCK_STREAM refere a TCP
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1)
  {
    std::cerr << "Erro ao abrir o socket." << std::endl;
    return SOCKET_OPEN_ERROR;
  }

  // Conectando ao consumidor
  sockaddr_in address;
  int addressSize = sizeof(sockaddr_in);
  address.sin_family = AF_INET; // IPv4
  address.sin_port = htons(SOCKET_PORT);
  inet_pton(AF_INET, (const char *)ip, &address.sin_addr);

  int connStatus = connect(sock, (const sockaddr *)&address, (socklen_t)addressSize);
  if (connStatus != 0)
  {
    std::cerr << "Erro ao conectar com o consumidor!" << std::endl;
    return SOCKET_CONNECT_ERROR;
  }
  std::cout << "Conectado com sucesso ao consumidor." << std::endl;

  // Loop principal
  ssize_t sockStatus;
  char msg[MESSAGE_MAX_SIZE];
  bool result;
  int tmp = 0;
  for (unsigned int i = 0; i < n_numbers; i++)
  {
    // Gera número e o transforma em texto
    tmp = generateN(tmp);
    itoa(tmp, msg, 10);

    // Envia ao consumidor
    sockStatus = send(sock, &msg, sizeof(char[MESSAGE_MAX_SIZE]), 0);
    if (sockStatus == -1)
    {
      std::cerr << "Erro ao enviar mensagem ao consumidor!" << std::endl;
      return SOCKET_SEND_ERROR;
    }

    // Adquire dados do consumidor
    sockStatus = recv(sock, &result, sizeof(bool), 0);
    if (sockStatus == -1)
    {
      std::cerr << "Erro ao tentar ler dados enviados pelo consumidor!" << std::endl;
      return SOCKET_RECV_ERROR;
    }

    // Imprime os resultados
    std::cout << "Número #" << i + 1 << ": " << tmp << ". É primo? " << result << std::endl;
  }

  // Envia condição de parada
  tmp = 0;
  itoa(tmp, msg, 10);
  send(sock, &msg, sizeof(char[MESSAGE_MAX_SIZE]), 0);

  // Finaliza socket
  close(sock);
  std::cout << "Conexão com o consumidor encerrada." << std::endl;

  return OK;
}