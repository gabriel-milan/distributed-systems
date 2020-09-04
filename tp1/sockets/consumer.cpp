/*
 * Sistemas Distribuídos - COS470
 *            2020/PLE
 * 
 * Trabalho Prático 1
 * Item 4 - Sockets
 * Parte 1 - Consumidor
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

// Função para verificar se um número é primo
bool isPrime(int n)
{
  for (unsigned int i = 2; i < n / 2; i++)
    if ((n % i) == 0)
      return false;
  return true;
}

// Método que imprime a forma de uso do script
void printUsage()
{
  std::cout << "Usage: ./consumer" << std::endl;
  std::cout << "No arguments required." << std::endl;
}

// Função principal
int main(int argc, char **argv)
{
  // Criando socket
  // SOCK_STREAM refere a TCP
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1)
  {
    std::cerr << "Erro ao abrir o socket." << std::endl;
    return SOCKET_OPEN_ERROR;
  }

  // Abrindo porta para aguardar conexões
  sockaddr_in address;
  int addressSize = sizeof(sockaddr);
  address.sin_family = AF_INET;          // IPv4
  address.sin_addr.s_addr = INADDR_ANY;  // Todas as interfaces de rede
  address.sin_port = htons(SOCKET_PORT); // Porta
  int bindStatus = 0;
  bindStatus = bind(sock, (const sockaddr *)&address, (socklen_t)addressSize);

  if (bindStatus != 0)
  {
    std::cerr << "Erro ao realizar bind na porta " << SOCKET_PORT << std::endl;
    return SOCKET_BIND_ERROR;
  }

  // Aguarda dados do produtor
  std::cout << "Consumidor: aguardando produtor enviar dados..." << std::endl;
  listen(sock, 0);

  // Realiza a conexão com o produtor
  int connectionFileDescriptor = accept(sock, (struct sockaddr *)&address, (socklen_t *)&addressSize);
  if (connectionFileDescriptor == -1)
  {
    std::cerr << "Erro ao abrir conexão com o produtor!" << std::endl;
    return SOCKET_ACCEPT_ERROR;
  }
  std::cout << "Conexão com produtor realizada com sucesso." << std::endl;

  // Loop principal
  ssize_t sockStatus;
  char msg[MESSAGE_MAX_SIZE];
  int val;
  bool result;
  while (true)
  {
    // Adquire dados do produtor
    sockStatus = recv(connectionFileDescriptor, &msg, sizeof(char[MESSAGE_MAX_SIZE]), 0);
    if (sockStatus == -1)
    {
      std::cerr << "Erro ao tentar ler dados enviados pelo produtor" << std::endl;
      return SOCKET_RECV_ERROR;
    }
    val = atoi(msg);

    // Verifica condição de parada
    if (val == 0)
      break;

    // Verifica se o valor recebido é primo
    result = isPrime(val);
#ifdef DEBUG_MODE
    std::cout << "[DEBUG] val=" << val << ", isPrime=" << result << std::endl;
#endif

    // Envia o resultado de volta ao produtor
    sockStatus = send(connectionFileDescriptor, &result, sizeof(bool), 0);
    if (sockStatus == -1)
    {
      std::cerr << "Erro ao tentar enviar dados para o produtor" << std::endl;
      return SOCKET_SEND_ERROR;
    }
  }

  // Quando o número requerido for o zero, chegaremos aqui
  close(sock);
  std::cout << "Conexão com o produtor encerrada." << std::endl;

  return OK;
}