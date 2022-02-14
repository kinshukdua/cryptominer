#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include "parser.h"

// struct Extranonce global_extranonce;
void connect_to_server();
void communicate(char * message, char * data);
struct Job subscribe();
int authorize(const char * username, const char * password);
int submit(const char * username, char * job_id, struct darry extranonce2, uint32_t ntime, uint32_t nonce );