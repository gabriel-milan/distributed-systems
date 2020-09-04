/*
 * Sistemas Distribuídos - COS470
 *            2020/PLE
 * 
 * Trabalho Prático 1
 * Item 4 - Sockets
 * Arquivo em comum
 * 
 * Grupo:
 * -> Gabriel Gazola Milan / DRE 116034377
 * -> João Pedro Wieland / DRE 116077278
 */

// Error codes
#define OK 0
#define WRONG_ARG_NUMBER 1
#define SOCKET_OPEN_ERROR 2
#define SOCKET_BIND_ERROR 3
#define SOCKET_ACCEPT_ERROR 4
#define SOCKET_CONNECT_ERROR 5
#define SOCKET_RECV_ERROR 6
#define SOCKET_SEND_ERROR 7

// Macros
// #define DEBUG_MODE // Uncomment this line for DEBUG messages
#define SOCKET_PORT 15000
#define MESSAGE_MAX_SIZE 30