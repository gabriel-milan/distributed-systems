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

// Macros
#define SOCKET_PORT 30742
#define MESSAGE_MAX_SIZE 10
#define MAX_CLIENTS 150
#define MAX_QUEUED_CONNECTIONS 3
#define SEPARATOR '.'

// Message types
#define MESSAGE_REQUEST 1
#define MESSAGE_GRANT 2
#define MESSAGE_RELEASE 3

// Error codes
#define OK 0
#define WRONG_ARG_NUMBER 1
#define SOCKET_OPEN_ERROR 2
#define SOCKET_BIND_ERROR 3
#define SOCKET_ACCEPT_ERROR 4
#define SOCKET_RECV_ERROR 5
#define SOCKET_SEND_ERROR 6
#define SOCKET_CONFIG_ERROR 7
#define SOCKET_LISTEN_ERROR 8
#define UNKNOWN_MESSAGE_TYPE 9
#define RELEASE_UNLOCKED_LOCK 10