/*
 * Sistemas Distribuídos - COS470
 *            2020/PLE
 * 
 * Trabalho Prático 3
 * Item 3 - Coordenador
 * 
 * Grupo:
 * -> Gabriel Gazola Milan / DRE 116034377
 * -> João Pedro Wieland / DRE 116077278
 */
#include <coordinator.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <deque>

int main(int argc, char **argv)
{
  std::vector<std::thread> threads;

  threads.push_back(std::thread([&]() {
    /*
     * Thread socket connections
     * Código base para essa thread:
     * https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/
     */
    int opt = 1;
    int sock, addressSize, newConnectionFd, clientSocket[MAX_CLIENTS], maxClients = MAX_CLIENTS, activity, maxFd;
    struct sockaddr_in address;
    char buffer[MESSAGE_MAX_SIZE + 1];
    fd_set readfds;

    // Zera descritores para clientes
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
      clientSocket[i] = 0;
    }

    // Abrindo socket
    // SOCK_STREAM se refere a TCP
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
      std::cerr << "Falha ao abrir o socket." << std::endl;
      return SOCKET_OPEN_ERROR;
    }

    // Configura socket para aceitar múltiplas conexões
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
    {
      std::cerr << "Falha ao configurar socket" << std::endl;
      return SOCKET_CONFIG_ERROR;
    }

    // Abrindo porta para conexões
    addressSize = sizeof(sockaddr);
    address.sin_family = AF_INET;          // IPv4
    address.sin_addr.s_addr = INADDR_ANY;  // Todas as interfaces de rede
    address.sin_port = htons(SOCKET_PORT); // Porta
    int bindStatus = 0;
    bindStatus = bind(sock, (struct sockaddr *)&address, (socklen_t)addressSize);
    if (bindStatus != 0)
    {
      std::cerr << "Erro ao realizar bind na porta " << SOCKET_PORT << std::endl;
      return SOCKET_BIND_ERROR;
    }

    // Especifica máximo de conexões na fila
    if (listen(sock, MAX_QUEUED_CONNECTIONS) < 0)
    {
      std::cerr << "Erro ao especificar máximo de conexões na fila" << std::endl;
      return SOCKET_LISTEN_ERROR;
    }

    // Loop principal
    while (true)
    {

      // Limpa o set de sockets
      FD_ZERO(&readfds);

      // Adiciona o master à lista de sockets
      FD_SET(sock, &readfds);
      maxFd = sock;

      // Adiciona os sockets filho
      for (int i = 0; i < MAX_CLIENTS; i++)
      {
        if (clientSocket[i] > 0)
          FD_SET(clientSocket[i], &readfds);
        if (clientSocket[i] > maxFd)
          maxFd = clientSocket[i];
      }

      // Aguarda indefinidamente por atividade em algum socket
      activity = select(maxFd + 1, &readfds, NULL, NULL, NULL);
      if ((activity < 0) && (errno != EINTR))
        std::cout << "Erro no comando select" << std::endl;

      // Se houve atividade no socket master, é nova conexão
      if (FD_ISSET(sock, &readfds))
      {
        if ((newConnectionFd = accept(sock, (struct sockaddr *)&address, (socklen_t *)&addressSize)) < 0)
        {
          std::cerr << "Erro ao aceitar nova conexão" << std::endl;
          return SOCKET_ACCEPT_ERROR;
        }
        printf("New connection , socket fd is %d , ip is : %s , port : %d \n ", newConnectionFd, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
        for (int i = 0; i < MAX_CLIENTS; i++)
          if (clientSocket[i] == 0)
          {
            clientSocket[i] = newConnectionFd;
            break;
          }
      }

      // Caso contrário, é I/O em outro socket
      for (int i = 0; i < MAX_CLIENTS; i++)
        if (FD_ISSET(clientSocket[i], &readfds))
        {
          int valread;
          // Alguém desconectou
          if ((valread = read(clientSocket[i], buffer, MESSAGE_MAX_SIZE)) == 0)
          {
            getpeername(clientSocket[i], (struct sockaddr *)&address, (socklen_t *)&addressSize);
            printf("Host disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
            close(clientSocket[i]);
            clientSocket[i] = 0;
          }
          // Mandou mensagem
          else
          {
            buffer[valread] = '\0';
            std::cout << buffer << std::endl;
          }
        }
    }
  }));

  threads.push_back(std::thread([&]() {
    /*
     * Thread mutex
     */
  }));

  threads.push_back(std::thread([&]() {
    /*
     * Thread interface
     */
  }));

  for (int i = 0; i < threads.size(); i++)
    threads[i].join();

  return OK;
}