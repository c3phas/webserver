/*A web server that handles multiple clients at the same time  *
 * The ip address has been set to loopback address             *
 * This server was tested in a kali linux system using gcc     *
 * to compile on terminal (gcc -o serv server.c) 
 * The headers for the server must be stored in the same dir   *
 * Test it with a web broswer by going to 127.0.0.1:8080 address* 
*/


#include "headers.h"
#include <sys/wait.h>
#include <sys/sendfile.h>

/*function to calclate sieof image*/
unsigned long fsize(char * file){
	FILE *fp = fopen(file,"r");
	fseek(fp,0,SEEK_END);
	unsigned long len = (unsigned long)ftell(fp);
	fclose(fp);
	return len;	

}

char web[] = 
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"

"<!DOCTYPE html>\r\n"
"<html><head><title>Simple web server</title>\r\n"
"<style>body { background-color: red}</style></head>\r\n"
"<body><center><h1>IF YOU CAN SEE THIS THEN YOUR SERVER IS WORKING WELL </h1></center> <p>SERVER IS UP AND RUNNING</p></body>\r\n";


int main (int argc, char **argv)
{
	int sockfd,newSock;
	pid_t child;
	unsigned int childProcCount = 0;/*count the number of child proceess*/
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int yes = 1;
	socklen_t len;
	char buf[2048];
	int fdimage;
	/*create the socket*/
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if (sockfd == -1)
	{
		perror("socket");
		exit(1);
	}else{
/*set the socket options allow the port to be reused*/
if(	setsockopt(sockfd,	SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
		perror("setsockopt");
}

	/*fill the address*/
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr =inet_addr("127.0.0.1");
	memset(&(server_addr.sin_zero),'\0',8);
	/*bind the address to the port*/
	int ret_bind = bind(sockfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr));
	if (ret_bind == -1)
	{
		perror("bind");
		exit(1);
	}else{
	/*listen for any incoming connections*/
	int ret_listen = listen(sockfd,BACKLOG);
	if (ret_listen == -1)
	{
		perror("listen");
		exit(1);
	}else{
	printf("The server is running \n");
	printf("[+] listening for connections\n");
	/*time to accept connections from incoming clients(loop)*/
	for ( ; ;)/*loop forever*/
	{
		 len = sizeof(struct sockaddr_in);
	 newSock= accept(sockfd,(struct sockaddr *)&client_addr,&len);
		if (newSock == -1)
		
		{
		
			perror("accept");
			continue;
		}else{
		printf("The server got connection from %s at port %d\n",inet_ntoa(client_addr.sin_addr),client_addr.sin_port);
		/*if accept succeeds fork a new process to handle the connection*/
		child = fork();
		//fork returns the child process id in parent, 0 in child and -1 on error
		if (child == -1)
		{
			perror("fork");
			exit(1);
		}
	   else	if (child == 0)//this is the child process
		{
			close(sockfd);//the child closes the listening port
//			char buf[] = "This is a message from the server";
			/*images are not yet supported*/
			memset(buf,0,2048);
			read(newSock,buf,2047);
			if (!strncmp(buf,"GET /image",10)){
				fdimage = open("ceph.txt",O_RDONLY);
				sendfile(newSock,fdimage,NULL,fsize("serv.jpg"));
				
				close(fdimage);
				
			}else{
			
			//send(newSock,"hello",5,0);
			write(newSock,web,sizeof(web));

			close(newSock);
			exit(0);
		}
		}
	
	close(newSock);
//	close(sockfd);
	childProcCount++;
	}/*get rid of the zombie process*/
		while(childProcCount)
		{
			child = waitpid((pid_t)-1,NULL,WNOHANG);
			if (child < 0)
			{
				perror("waitpid");
				exit(1);
			}else if(child == 0)
			{
				break;
			}else{
					childProcCount--;
			}
		
		
		}
	}

}
}
}
	return 0;
}

