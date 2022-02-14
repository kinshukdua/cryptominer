#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<stdint.h>
#include "parser.h"

int main(int argc , char *argv[])
{
	int socket_desc;
	struct sockaddr_in server;
	char *message , server_reply[2000];	
	char data[4000] = "";
	int data_len = 0;
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
		
	server.sin_addr.s_addr = inet_addr("54.89.191.69");
	server.sin_family = AF_INET;
	server.sin_port = htons( 3333 );

	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		printf("connect error");
		return 1;
	}
	
	printf("Connected\n");
	
	//Send some data
	message = "{\"id\": 1, \"method\": \"mining.subscribe\", \"params\": []}\n";
	if( send(socket_desc , message , strlen(message) , 0) < 0)
	{
		printf("Send failed");
		return 1;
	}
	printf("Data Send\n");
	int len;
	do {
		len = recv(socket_desc, server_reply , 2000 , 0);
		
		printf("%d",len);
		if( len < 0)
		{
			printf("recv failed");
			return 0;
		}
		for (int i = 0;i<len;i++)
		{
			data[data_len+i] = server_reply[i]; 
		}
		data_len += len;
		printf("Reply received\n");
	}
	while (server_reply[len-1] != '\n');

	data[data_len] = '\0';
	parse_data(data);
	return 0;
}

/*

			else 
			{
				//printf("%s\n",root->valuestring);
				
				cJSON *method = cJSON_GetObjectItem(root, "method");
				if (method==NULL)
				{
					free(method);
					printf("\n**************\n");
					printf("No METHOD");
					printf("\n**************\n");
					cJSON *result = cJSON_GetObjectItem(root, "result");
					printf("%s",root->valuestring);
					
					if (result!=NULL)
					{
						cJSON *extranonce1 = cJSON_GetArrayItem(result, 2);
						printf("%s\n", extranonce1->valuestring);
					}
					else
					{
						printf("dhebjfbej");
					}

				}
				else
				{

					printf("%s\n", method->valuestring);
					if (strcmp(method->valuestring,"mining.notify")==0)
					{
						printf("Sending mining DATA\n");
					}
					//free(method);
				}
			}
			*/
			//free(root);

