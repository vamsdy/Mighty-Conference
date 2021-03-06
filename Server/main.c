#include "client_list.h"
#include "socket_utilities.h"
#include "handle_client.h"
#include "common_headers.h"


pthread_t tid;

/*
	Usually SIGPIPE occurs when the pipe is broken, that means the connection between server and client terminates
	This signal causes the process to terminate by default. To avoid this, we need to handle ourseleves.
*/
void sig_hndlr()
{
	// This is the handler for the signal SIGPIPE
	// This is fired when the client terminates abruptly and at the same time server thread writes to the client socket descriptor
	// Here we don't do anything, we will handle at the write call itself.
	printf("\nSIGPIPE fired \n");
}

void* input_commands_function(void* arg)
{
	clients_list_t* list = (clients_list_t*)arg;

	int a;
	printf("Press 1 to view the online clients\n\n");
	while(1)
	{
		scanf("%d",&a);
		if( a == 1 )
		{
			display_clients(list);
		}
	}
}

int main()
{	
	// Add the signal handler
	signal(SIGPIPE, sig_hndlr);
	/*
		Allocate a list and assign it to list global variable
	*/
	list = (clients_list_t*)malloc(sizeof(clients_list_t));
	/*
	    Initialize the list 
	*/
	init_list();
	/*
		Listen to the requests. if any request from the client - spawn a thread, add the client to the list
	*/
	int listenfd, connfd;

	socklen_t clilen;

	struct sockaddr_in cliaddr,servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	// Clear the server and client structures
	bzero(&servaddr, sizeof(servaddr));
	bzero(&cliaddr, sizeof(cliaddr));

	// Assign the IP and port numbers
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	// Bind the address
	Bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	Listen(listenfd,LISTENQ);

	// Before accespting clients, create a thread for handling user commands to query clients,etc 

	if( pthread_create(&tid, NULL, input_commands_function, (void*)list) != 0)
	{
		printf("Failed to spawn a thread for the client");
		return -1;
	}

	/*if( pthread_create(&tid, NULL, monitor_list_for_stale_clients, NULL) != 0)
	{
		printf("Failed to spawn a thread for the client");
		return -1;
	}*/

	for(;;)
	{
		// Accept the clients and spawn a new thread for each of them
		clilen = sizeof(cliaddr);
		// Upon receiving the client connection
		connfd = Accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
		handle_client_request(connfd, cliaddr);
	}

}
