/*
Resouce:https://www.geeksforgeeks.org/c-program-replace-word-text-another-given-word/
*/


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

char *replaceWord(const char *s, const char *oldW, 
                                 const char *newW) 
{ 
    char *result; 
    int i, cnt = 0; 
    int newWlen = strlen(newW); 
    int oldWlen = strlen(oldW); 
  
      for (i = 0; s[i] != '\0'; i++) 
    { 
        if (strstr(&s[i], oldW) == &s[i]) 
        { 
            cnt++; 
  
                        i += oldWlen - 1; 
        } 
    } 
  
     
    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1); 
  
    i = 0; 
    while (*s) 
    { 
         
        if (strstr(s, oldW) == s) 
        { 
            strcpy(&result[i], newW); 
            i += newWlen; 
            s += oldWlen; 
        } 
        else
            result[i++] = *s++; 
    } 
  
    result[i] = '\0'; 
    return result; 
} 
  
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
	int valread, number=0;
	char *temp, *copy, *result;
	char si[] = "%40";
	char change[] = "@";
	char *fin =NULL;
	FILE *newFile; 
	memset(buffer,0, MAXLINE);
	memset(buf,0,MAXLINE);

		
	
	valread = read( new_socket , buffer, MAXLINE);
	buffer[valread] = '\0';
	printf("This is a buffer %s\n",buffer);
	temp = strtok(buffer, " ");
		
	if (temp != NULL && strcmp(temp,HEAD) == 0)
	{
		copy = strtok(NULL," /");
		printf("Before open the file %s\n",copy);
		if(strlen(copy)>20)
		{
			result = strtok(copy,"?&+=");
			printf("This is result before while: %s\n",result);
			newFile = fopen(result,"w");
			if(newFile == NULL)
			{
				strncpy(buffer,"HTTP/1.1 404 NOT FOUND\r\n",100);
				strcat(buffer,"Connection: Loste\r\nContent-Type: text/html; charset=ISO-8859-1\r\n\r\n 404 NOT FOUND");				
			}
			else
			{
				fprintf(newFile,"<!DOCTYPE html>\n<html>\n<head>\n\t<title>\n\t\tOrder Detail\n\t</title>\n<style>\n");
				fprintf(newFile,"body\n{\n\tbackground-color: powderblue;\n}\nh1\n{\n\tcolor: blue;\n\tfont-family: verdana;\n\tfont-size: 150%%;\n}\np\n{\n\tcolor: black;\n");	
	                                                       fprintf(newFile,"\tfont-family: serif;\n\tfont-size: 110%%;\n}\n</style>\n</head>\n<body>\n<h1>Here is your order details.</h1>\n");
				while(1)
				{

					result = strtok(NULL,"&+=");
					printf("This is result: %s\n",result);
					if(strcmp(result,"submit")==0)
					{
						break;
					}
					fin = replaceWord(result,si,change);
					printf("This is the final: %s\n",fin);
					if (number==0)
					{
						number=1;
					}		
					else
					{	switch(number)
						{
							case 1:
								if(strncmp(fin,"Yes",3) == 0)
								{
									fprintf(newFile,"<p> Our customer server will contact you. </p>\n");
								}
								else
								{
									fprintf(newFile,"<p> Thank you for your ordering. </p>\n");
								}
								break;
							case 2:
								if(strncmp(fin,"1or2",4) == 0)
								{
									fprintf(newFile,"<p> Our restaurant will delivery for you two of tableware. </p>\n");
								}
								if(strncmp(fin,"2or4",4) == 0)
								{
									fprintf(newFile,"<p> Our restaurant will delivery for you four of tableware. </p>\n");
								}
								if(strncmp(fin,">4",2) == 0)
								{
									fprintf(newFile,"<p> Our restaurant will delivery for you maximum six  of tableware. </p>\n");
								}
								break;
							case 3:
								fprintf(newFile,"<p> Dear %s --",fin);
								break;
							case 4:
								fprintf(newFile, "%s</p>\n",fin);
								break;
							case 5:
								fprintf(newFile, "<p>Comfirm your email address: %s</p>\n",fin);
								break;
							case 6:
								fprintf(newFile, "<p>Your zip code is %s</p>\n",fin);
								break;
							case 7: 
								fprintf(newFile, "<p style=\"color: red; margin-left: 80px; font-size: 150%%;\">Here is your menu: </p>\n");
								fprintf(newFile, "<p>Your ordered %s as appetizer</p>\n",fin);
								break;
							case 8:
								fprintf(newFile, "<p>Your ordered %s as soup</p>\n",fin);
								break;
							case 9:
								fprintf(newFile, "<p>Your ordered %s as entree</p>\n",fin);
								break;
							case 10:
								fprintf(newFile, "<p>Our restaurant will extra provide %s for you.</p>\n",fin);
								break;
							case 11:
								fprintf(newFile, "<p>Thanks you for hearing us from %s.</p>\n",fin);
								break;							
						}			
						number++;
					}						
				}
				fprintf(newFile,"<p>Our restaurant will try our best to serve the precious customers!</p>\n\t</body>\n</html>\n");
			}
			fclose(newFile);
			file = fopen("order.html","r");
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
	}	
	else
	{
		strncpy(buffer,"HTTP/1.1 404 NOT FOUND\r\n",100);
		strcat(buffer,"Connection: Loste\r\nContent-Type:text/html; charset=ISO-8859-1\r\n\r\n 404 NOT FOUND");
		
	}
	send(new_socket, buffer, strlen(buffer) , 0 );
	printf("Sending request message to Client\n\n");				

	close(new_socket);

}
