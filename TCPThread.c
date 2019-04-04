#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>
#include <pthread.h>

#define PORT 60002
#define MAXLINE 10000
#define MAXPENDING 10
#define HEAD "GET"

char buffer[MAXLINE];
char buf[MAXLINE];
int number = 0;

FILE *file;
int count =0;
pthread_t threadID;
struct ThreadArgs
{
	int clientSock;
};

void *ThreadMain(void *args);
void HandleTCPClient(int new_socket);


int main(int argc, char const *argv[])
{
	int server_fd, new_socket;
	struct sockaddr_in servaddr;
	int addrlen = sizeof(servaddr);
	/*pthread_t threadID;*/
	struct ThreadArgs *threadArgs;

	memset(buffer, 0, MAXLINE);

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	servaddr.sin_port = htons( PORT );

	if (bind(server_fd, (struct sockaddr *)&servaddr, sizeof(servaddr))<0)
	{
		perror("bind failed!");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, MAXPENDING) < 0)
	{
		perror("listen failed!");
		exit(EXIT_FAILURE);
	}
	puts("Waiting for connecting . . .");

	for(;;)
	{

		if ((new_socket = accept(server_fd, (struct sockaddr *)&servaddr,(socklen_t*)&addrlen)) < 0)
	        {
                	perror("accept failed!");
                	exit(EXIT_FAILURE);
        	}
		/*count++;*/
		if ((threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs))) == NULL)
		{
			perror("Malloc() failed");
		}
		threadArgs ->clientSock = new_socket;

		if (pthread_create(&threadID, NULL, ThreadMain, (void *) threadArgs) != 0)
		{
			perror("pthread_create() failed");
		}

	}

	return 0;
}

void *ThreadMain(void *threadArgs)
{
	int clientSock;
	/*printf("With thread %ld\n",(long int) threadID);*/

	pthread_detach(pthread_self());

	clientSock = ((struct ThreadArgs*) threadArgs) -> clientSock;

	free(threadArgs);

	/*puts("Before the Handle function!");*/
	HandleTCPClient(clientSock);

	return (NULL);
}

void HandleTCPClient(int new_socket)
{
	int valread;
	char *temp, *copy,  *result;
	FILE *newFile; 
	memset(buffer,0, MAXLINE);
	memset(buf,0,MAXLINE);

		
	if(number == 0)
	{

		valread = read( new_socket , buffer, MAXLINE);
		buffer[valread] = '\0';
		printf("This is a buffer %s\n",buffer);
		temp = strtok(buffer, " ");
		
		if (temp != NULL && strcmp(temp,HEAD) == 0)
		{
			copy = strtok(NULL," /?");
			/*printf("Before open the file %s\n",copy);*/
			file = fopen(copy,"r");
			if (file == NULL )
			{
				strncpy(buffer,"HTTP/1.1 404 NOT FOUND\r\n",100);
				strcat(buffer,"Connection: Loste\r\nContent-Type: text/html; charset=ISO-8859-1\r\n\r\n 404 NOT FOUND");
			}
			else
			{
		 		strncpy(buffer,"HTTP/1.1 200 OK\r\n",100);
		 		strcat(buffer,"Server: Apache/2.2.22(Ubuntu)\r\nConnection: Keep-Alive\r\nContent-Type: text/html; charset=ISO-8859-1\r\n\r\n");
		 		while (fread(buf, 1, sizeof(buf), file) > 0)
		 		{
                 			strcat(buffer,buf);
               			}
			}
		}
		else
		{
			strncpy(buffer,"HTTP/1.1 404 NOT FOUND\r\n",100);
			strcat(buffer,"Connection: Loste\r\nContent-Type:text/html; charset=ISO-8859-1\r\n\r\n 404 NOT FOUND");
			
		}
		send(new_socket, buffer, strlen(buffer) , 0 );
		printf("Sending request message to Client\n\n");				
		
	}

	else 
	{
		number =0;
		valread = read( new_socket , buffer, MAXLINE);
		buffer[valread] = '\0';
		printf("This is a buffer %s\n",buffer);
		temp = strtok(buffer, " ");
		 newFile = fopen("order.html","w");		
		while(1)
		{
			temp = strtok(NULL, " ");
			printf("This is temp: %s\n",temp);
			if(strcmp(temp,"image/webp,image/apng,image/*,*/*;q=0.8\r\nReferer:") == 0)
			{
				break;
			}

		}
		if (temp != NULL && strcmp(temp,"image/webp,image/apng,image/*,*/*;q=0.8\r\nReferer:") == 0)
		{

			copy = strtok(NULL," ");
			/*result = strtok(copy,"%3D=");
			printf("This is result before while: %s\n",result);
			while(1)
			{
				result = strtok(NULL,"%3D=");
				printf("This is result: %s\n",result);
				break;
			}
			*/
			
			result = strtok(copy,"/?");
			printf("This is result before while: %s\n",result);
			while(1)
			{
				result = strtok(NULL,"/?");
				printf("This is result: %s\n",result);
				if(strcmp(result,"project.cgi")==0)
				{
					break;
				}
				
			}

			
			if (file == NULL )
			{
				strncpy(buffer,"HTTP/1.1 404 NOT FOUND\r\n",100);
				strcat(buffer,"Connection: Loste\r\nContent-Type: text/html; charset=ISO-8859-1\r\n\r\n 404 NOT FOUND");
			}
			else
			{
		 		strncpy(buffer,"HTTP/1.1 200 OK\r\n",100);
		 		strcat(buffer,"Server: Apache/2.2.22(Ubuntu)\r\nConnection: Keep-Alive\r\nContent-Type: text/html; charset=ISO-8859-1\r\n\r\n");
		 		while (fread(buf, 1, sizeof(buf), file) > 0)
		 		{
                 					strcat(buffer,buf);
               				}
			}
		}
		else
		{
			strncpy(buffer,"HTTP/1.1 404 NOT FOUND\r\n",100);
			strcat(buffer,"Connection: Loste\r\nContent-Type:text/html; charset=ISO-8859-1\r\n\r\n 404 NOT FOUND");
			/*break;*/
		}
		/*send(new_socket, buffer, strlen(buffer) , 0 );*/
		printf("Sending request message to Client\n\n");				
		memset(buffer,0, MAXLINE);
		memset(buf,0,MAXLINE);

	}
	/*send(new_socket,buffer,strlen(buffer),0);*/
	close(new_socket);

}
