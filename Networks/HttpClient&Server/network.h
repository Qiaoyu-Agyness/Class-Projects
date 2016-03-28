/* header file for both the server and the client */
/* author: Qiaoyu Liao
	qliao@wpi.edu
	*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>
#include <fcntl.h>

#define HOST_LENGTH 256
#define BUF_SIZE 1024
#define BACKLOG 10
#define DEF_PORT 80








