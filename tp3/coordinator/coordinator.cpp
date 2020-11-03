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
#include <mutex>
#include <map>

/*
 * Iteração em chaves de hashmap extraído de
 * https://stackoverflow.com/a/16527081/9944075
 */
typedef std::map<int, int>::iterator counterMapIterable;
class key_iterator : public counterMapIterable
{
public:
  key_iterator() : counterMapIterable(){};
  key_iterator(counterMapIterable s) : counterMapIterable(s){};
  int *operator->() { return (int *const) & (counterMapIterable::operator->()->first); }
  int operator*() { return counterMapIterable::operator*().first; }
};

struct message
{
  int id;
  int fd;
  int type;
};

enum command
{
  command_print_queue,
  command_print_history,
  command_quit,
  command_invalid,
};

command resolveCommand(std::string inputCommand)
{
  if ((inputCommand == "print_queue") || (inputCommand == "1"))
    return command_print_queue;
  else if ((inputCommand == "print_history") || (inputCommand == "2"))
    return command_print_history;
  else if ((inputCommand == "quit") || (inputCommand == "3"))
    return command_quit;
  else
    return command_invalid;
}

message parseMessage(char buffer[MESSAGE_MAX_SIZE + 1], int fd)
{
  message ret;
  int messageType, i = 2;
  messageType = (int)buffer[0] - '0';
  char tmp[MESSAGE_MAX_SIZE - 1];
  while ((buffer[i] != SEPARATOR) && (buffer[i] != '\0'))
  {
    tmp[i - 2] = buffer[i];
    i++;
  }
  tmp[i - 2] = '\0';
  ret.fd = fd;
  ret.type = messageType;
  ret.id = atoi(tmp);
  return ret;
}

void getGrant(char msg[MESSAGE_MAX_SIZE + 1])
{
  msg[0] = '0' + MESSAGE_GRANT;
  msg[1] = '.';
  for (int i = 2; i <= MESSAGE_MAX_SIZE; i++)
    msg[i] = '0';
}

int main(int argc, char **argv)
{
  bool running = true;
  std::vector<std::thread> threads;
  std::deque<message> messageQueue;
  std::mutex messageQueueMutex;
  std::deque<message> queue;
  std::mutex queueMutex;
  std::map<int, int> counter;

  /*
   * Thread for socket connections
   * Código base para essa thread:
   * https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/
   */
  threads.push_back(std::thread([&]() {
    int opt = 1;
    int sock, addressSize, newConnectionFd, maxClients = MAX_CLIENTS, activity, maxFd;
    int clientSocket[MAX_CLIENTS];
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

    // Configurando timeout do select
    struct timeval tv;
    tv.tv_sec = 1;

    // Loop principal
    while (running)
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
      activity = select(maxFd + 1, &readfds, NULL, NULL, &tv);
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
        // printf("New connection , socket fd is %d , ip is : %s , port : %d \n ", newConnectionFd, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
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
          // Desconectou
          if ((valread = read(clientSocket[i], buffer, MESSAGE_MAX_SIZE + 1)) == 0)
          {
            // getpeername(clientSocket[i], (struct sockaddr *)&address, (socklen_t *)&addressSize);
            // printf("Host disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
            close(clientSocket[i]);
            clientSocket[i] = 0;
          }
          // Mandou mensagem
          else
          {
            message tmp = parseMessage(buffer, clientSocket[i]);
            if (counter.find(tmp.id) == counter.end())
              counter.insert(std::make_pair(tmp.id, 0));
            if (tmp.type != -35)
            {
              messageQueueMutex.lock();
              messageQueue.push_back(tmp);
              messageQueueMutex.unlock();
            }
          }
        }
    }
    return OK;
  }));

  /*
   * Thread for mutex
   */
  threads.push_back(std::thread([&]() {
    bool locked = false;
    bool work = false;
    bool queueWork = false;
    message msg;
    while (running)
    {
      // Pops a message from queue
      messageQueueMutex.lock();
      if (messageQueue.size() > 0)
      {
        msg = messageQueue.front();
        messageQueue.pop_front();
        work = true;
      }
      messageQueueMutex.unlock();

      // If a message has been popped, process it
      if (work)
      {
        // std::cout << "Message Type: " << msg[0] << std::endl;
        // std::cout << "Id: " << msg[1] << std::endl;
        switch (msg.type)
        {
        case MESSAGE_REQUEST:
          queueMutex.lock();
          // std::cout << "INFO: Adding REQUEST for process #" << msg.id << std::endl;
          queue.push_back(msg);
          queueMutex.unlock();
          break;
        case MESSAGE_RELEASE:
          if (!locked)
          {
            std::cerr << "Tried to released unlocked lock" << std::endl;
            return RELEASE_UNLOCKED_LOCK;
          }
          // std::cout << "INFO: RELEASE from process #" << msg.id << std::endl;
          locked = false;
          break;
        default:
          std::cerr << "Unknown message type " << msg.type << std::endl;
          return UNKNOWN_MESSAGE_TYPE;
        }
        work = false;
      }

      // Process queue
      if (!locked)
      {
        queueMutex.lock();
        if (queue.size() > 0)
        {
          msg = queue.front();
          queue.pop_front();
          queueWork = true;
        }
        queueMutex.unlock();
        if (queueWork)
        {
          // Send GRANT
          char grant[MESSAGE_MAX_SIZE + 1];
          getGrant(grant);
          send(msg.fd, grant, MESSAGE_MAX_SIZE + 1, 0);
          // std::cout << "INFO: GRANT to process #" << msg.id << std::endl;
          // Lock
          counter[msg.id]++;
          locked = true;
          //
          queueWork = false;
        }
      }
    }
    return OK;
  }));

  /*
   * Thread for interface
   */
  threads.push_back(std::thread([&]() {
    std::string strCommand;
    std::cout << "SD 2020/PLE TP3" << std::endl;
    while (running)
    {
      std::cout << "> ";
      getline(std::cin, strCommand);
      switch (resolveCommand(strCommand))
      {
      case command_print_queue:
        std::cout << "Fila de pedidos:" << std::endl;
        queueMutex.lock();
        if (queue.size() == 0)
          std::cout << "vazia." << std::endl;
        else
          for (int i = 0; i < queue.size(); i++)
            std::cout << "  - " << i + 1 << "º: " << queue[i].id << std::endl;
        queueMutex.unlock();
        break;

      case command_print_history:
        std::cout << "Histórico de pedidos atendidos:" << std::endl;
        for (key_iterator s = counter.begin(); s != counter.end(); ++s)
          std::cout << "  - Processo " << *s << ": " << counter[*s] << " vezes." << std::endl;
        break;

      case command_quit:
        std::cout << "Encerrando programa..." << std::endl;
        running = false;
        break;

      default:
        std::cout << "Comando \"" << strCommand << "\" inválido!" << std::endl;
        break;
      }
    }
    return OK;
  }));

  for (int i = 0; i < threads.size(); i++)
    threads[i].join();

  return OK;
}