#ifndef COMMON_HEADERS_H
#define COMMON_HEADERS_H
// ------------------START----------------------- //

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

#define USERNAME_LENGTH 20
#define PASSWORD_LENGTH 20
#define LOGIN_STATUS_LENGTH 5
#define MAX_LENGTH 1024
#define LEN_STR_LENGTH 6 // It has 5 characters for length , 1 for \0

// Protocol lengths (JSON STRINGS)
#define JSON_LEN_SIZE 26

// Jansson

#include <jansson.h>

// Multithreading

#include <pthread.h>

// Networking

#include <netinet/in.h>
#include <sys/socket.h>
#define SERV_PORT 4444
#define LISTENQ 20
#define INET_ADDRSTRLEN 16

// ------------------END-------------------------//

#endif /* COMMON_HEADERS_H */
