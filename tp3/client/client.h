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

// Macros
#define COORDINATOR_IP "127.0.0.1"
#define COORDINATOR_PORT 30742
#define MESSAGE_MAX_SIZE 10

// Message types
#define MESSAGE_REQUEST '1'
#define MESSAGE_GRANT '2'
#define MESSAGE_RELEASE '3'

// Error codes
#define OK 0
#define WRONG_ARG_NUMBER 1
#define SOCKET_OPEN_ERROR 2
#define SOCKET_CONNECT_ERROR 3
#define SOCKET_RECV_ERROR 4
#define SOCKET_SEND_ERROR 5
#define UNKNOWN_MESSAGE_ERROR 6
#define FORK_FAIL_ERROR 7