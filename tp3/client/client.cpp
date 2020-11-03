/*
 * Sistemas Distribuídos - COS470
 *            2020/PLE
 * 
 * Trabalho Prático 3
 * Item 4 - Cliente
 * 
 * Grupo:
 * -> Gabriel Gazola Milan / DRE 116034377
 * -> João Pedro Wieland / DRE 116077278
 */

#include <client.h>
#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>

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

int connect()
{
  // Criando socket
  // SOCK_STREAM refere a TCP
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1)
  {
    std::cerr << "Erro ao abrir o socket." << std::endl;
    return SOCKET_OPEN_ERROR;
  }

  // Conectando ao consumidor
  sockaddr_in address;
  int addressSize = sizeof(sockaddr_in);
  address.sin_family = AF_INET; // IPv4
  address.sin_port = htons(COORDINATOR_PORT);
  inet_pton(AF_INET, (const char *)COORDINATOR_IP, &address.sin_addr);

  int connStatus = connect(sock, (const sockaddr *)&address, (socklen_t)addressSize);
  if (connStatus != 0)
  {
    std::cerr << "Erro ao conectar com o coordenador!" << std::endl;
    return SOCKET_CONNECT_ERROR;
  }
  std::cout << "Conectado com sucesso ao coordenador." << std::endl;
  return sock;
}

void disconnect(int socketFd)
{
  close(socketFd);
}

void baseMessage(int id, char msg[MESSAGE_MAX_SIZE + 1])
{
  char strId[MESSAGE_MAX_SIZE + 1];
  itoa(id, strId, 10);
  msg[1] = '.';
  int i = 0;
  while (strId[i] != '\0')
  {
    msg[i + 2] = strId[i];
    i++;
  }
  msg[i + 2] = '.';
  for (int j = i + 3; j < MESSAGE_MAX_SIZE; j++)
    msg[j] = '0';
  msg[MESSAGE_MAX_SIZE] = '\0';
}

void request(int socketFd, int id)
{
  char msg[MESSAGE_MAX_SIZE + 1];
  baseMessage(id, msg);
  msg[0] = MESSAGE_REQUEST;
  send(socketFd, &msg, sizeof(char[MESSAGE_MAX_SIZE + 1]), 0);
  ssize_t sockStatus = recv(socketFd, &msg, sizeof(char[MESSAGE_MAX_SIZE + 1]), 0);
  if (sockStatus == -1)
  {
    std::cerr << "Erro ao tentar ler dados enviados pelo consumidor!" << std::endl;
    exit(SOCKET_RECV_ERROR);
  }
  if (msg[0] == MESSAGE_GRANT)
    return;
  else
    exit(UNKNOWN_MESSAGE_ERROR);
}

void release(int socketFd, int id)
{
  char msg[MESSAGE_MAX_SIZE + 1];
  baseMessage(id, msg);
  msg[0] = MESSAGE_RELEASE;
  send(socketFd, &msg, sizeof(char[MESSAGE_MAX_SIZE + 1]), 0);
}

int main(int argc, char **argv)
{
  int myId = 123;
  int sock = connect();
  request(sock, myId);
  usleep(3000000);
  release(sock, myId);
  disconnect(sock);
  return OK;
}