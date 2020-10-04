/*
 * Sistemas Distribuídos - COS470
 *            2020/PLE
 * 
 * Trabalho Prático 1
 * Item 3 - Pipes
 * 
 * Grupo:
 * -> Gabriel Gazola Milan / DRE 116034377
 * -> João Wieland / DRE 116077278
 */

// Return codes
#define OK 0
#define WRONG_ARG_NUMBER 1
#define INVALID_NUMBER 2
#define PIPE_ERROR 3
#define FORK_ERROR 4
#define WRITE_PIPE_ERROR 5
#define READ_PIPE_ERROR 6

// Macros
#define MESSAGE_MAX_SIZE 30