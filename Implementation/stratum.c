#include "stratum.h"

struct Extranonce global_extranonce;
struct sockaddr_in server;
int socket_desc;
int curr_id = 1;




void connect_to_server()
{
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
		
	server.sin_addr.s_addr = inet_addr("54.89.191.69");
	server.sin_family = AF_INET;
	server.sin_port = htons( 3333 );

	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		printf("connect error");
	}
	
	printf("Connected\n");
}

void communicate(char * message, char * data)
{	
	char server_reply[2000];	
	int data_len = 0;
	//Create socket
	if( send(socket_desc , message , strlen_(message) , 0) < 0)
	{
		//printf("Send failed");
	}
	//printf("Data Send\n");
	int len;
	do {
		len = recv(socket_desc, server_reply , 2000 , 0);
		
		//printf("%d",len);
		if( len < 0)
		{
			//printf("recv failed");
		}
		for (int i = 0;i<len;i++)
		{
			data[data_len+i] = server_reply[i]; 
		}
		data_len += len;
		//printf("Reply received\n");
	}
	while (server_reply[len-2] != '}' && server_reply[len-3] != ']' && server_reply[len-1] != '\n');
	data[data_len] = '\0';

}

struct Job subscribe()
{
    struct Job job;	
	//Send some data
	char data[4000] = "";
	cJSON * reply = cJSON_CreateObject();
	cJSON_AddNumberToObject(reply, "id", curr_id);
	cJSON_AddStringToObject(reply, "method", "mining.subscribe");
	cJSON_AddItemToObject(reply, "params", cJSON_CreateArray());
	char * message = cJSON_PrintUnformatted(reply);
	strcat(message,"\n");
	curr_id++;
	communicate(message, data);

	job = parse_data(data);
    global_extranonce = job.extranonce;
	return job;
}

int authorize(const char * username, const char * password)
{
	char data[4000] = "";
	cJSON * reply = cJSON_CreateObject();
	cJSON_AddNumberToObject(reply, "id", curr_id);
	cJSON_AddStringToObject(reply, "method", "mining.authorize");
	cJSON * params = cJSON_CreateArray();
	cJSON_AddItemToArray(params, cJSON_CreateString(username));
	cJSON_AddItemToArray(params, cJSON_CreateString(password));
	cJSON_AddItemToObject(reply, "params", params);
	char * message = cJSON_PrintUnformatted(reply);
	strcat(message,"\n");
	curr_id++;
	communicate(message, data);
	struct Result result = parse_result(data);
	if (!result.success)
	{
		printf("Submission Error: %s\n",result.error);
	}
	else 
	{
		printf("Authorization successful\n");
	}
	return 1;
}

int submit(const char * username, char * job_id, struct darry extranonce2, uint32_t ntime, uint32_t nonce )
{ 
	char data[4000] = "";
	cJSON * reply = cJSON_CreateObject();
	cJSON_AddNumberToObject(reply, "id", curr_id);
	cJSON_AddStringToObject(reply, "method", "mining.submit");
	cJSON * params = cJSON_CreateArray();
	cJSON_AddItemToArray(params, cJSON_CreateString(username));
	cJSON_AddItemToArray(params, cJSON_CreateString(job_id));
	cJSON_AddItemToArray(params, cJSON_CreateString(bytes2hex(extranonce2.data, extranonce2.size)));
	cJSON_AddItemToArray(params, cJSON_CreateString(int2hexpad(ntime)));
	cJSON_AddItemToArray(params, cJSON_CreateString(int2hexpad(nonce)));
	cJSON_AddItemToObject(reply, "params", params);
	char * message = cJSON_PrintUnformatted(reply);
	strcat(message,"\n");
	curr_id++;
	communicate(message, data);
	struct Result result = parse_result(data);
	if (!result.success)
	{
		printf("Submission Error: %s\n",result.error);
	}
	else 
	{
		printf("Submitted\n");
	}
	return 1;

}

