#include "common_headers.h"
#include "socket_utilities.h"



int main()
{
	int sock_fd = socket(AF_INET,SOCK_STREAM,0);
	
	// Create the server socket
	struct sockaddr_in server_sock;
	// Initialize the socket structure to zero
	bzero(&server_sock,sizeof(struct sockaddr_in));
	printf("Enter the port number of server \n");
	int port;
	scanf("%d",&port);
	server_sock.sin_port = htons(port);
	server_sock.sin_family = AF_INET;
	inet_pton(AF_INET,"127.0.0.1",&server_sock.sin_addr);

	connect(sock_fd,(struct sockaddr*)&server_sock,sizeof(struct sockaddr_in));

	// Send the user name and password to the server for authentication

	char username[USERNAME_LENGTH];
	scanf("%s",username);
	printf("Now sending username\n");
	Write(sock_fd,username,USERNAME_LENGTH);

	char password[PASSWORD_LENGTH];
	scanf("%s",password);
	printf("Now sending password\n");
	Write(sock_fd,password,PASSWORD_LENGTH);

	// Now get the authentication status

	// RIGHT NOW EVERYTHING IS A NORMAL STRING< >>> WE HAVE TO CONVERT THEM TO JSON LATER


	char status[LOGIN_STATUS_LENGTH];
	Read(sock_fd,status,LOGIN_STATUS_LENGTH);

	while( strcmp(status,"DENY") == 0 )
	{
		// Denied access, try again
		scanf("%s",username);
		printf("Now sending username\n");
		Write(sock_fd,username,USERNAME_LENGTH);

		scanf("%s",password);
		printf("Now sending password\n");
		Write(sock_fd,password,PASSWORD_LENGTH);
	}


	// Client is succesfully authenticated by the server

	char cli_strlen_JSON[JSON_LEN_SIZE];

	// Now fetch the length of the upcoming list
	Read(sock_fd,cli_strlen_JSON, JSON_LEN_SIZE);

	
	printf("%si\n",cli_strlen_JSON);

	// With this length build a new string to receive online clients list
	int length = get_length(cli_strlen_JSON);

	if( length <= 0 )
	{
		// Error in extracting length
		printf("Error in fetching the length\n");
		exit(0);
	}

	printf("The length is %d\n",length);

	char online_clients[length+1]; // +1 for providing space for null character
	Read(sock_fd,online_clients, length+1); // Read the null character too
	printf("The online clients are : \n%sEND\n",online_clients);


	// This loop is for continous reading of messages from the server, if any
	// Otherwise, it will simply wait at this prompt untill a msg is seen from the server.
	while( read(sock_fd,cli_strlen_JSON,JSON_LEN_SIZE) > 0 )
	{
		length =  get_length(cli_strlen_JSON);
		printf("New string with length %d\n",length);

		char clients_str[length+1];
		Read(sock_fd,clients_str, length+1);

		printf("%sEND\n",clients_str);
	}

}



/*
	This function will extract the integer from the string ( length from the JSON value )
*/

int get_length(char* cli_strlen_JSON)
{
	// Extract the JSON part of the string
	json_error_t error; // For error, can be ignored
	json_t* root = json_loads(cli_strlen_JSON, 0, &error); // Make a JSON object of the received string

	if( root == NULL )
	{
		printf("Error is decoding JSON\n");
		exit(0);
	}

	// Get the length value
	json_t* len_value_JSON = json_object_get(root, "length"); // Get the value JSON object of key (length)

	char* len_text = json_string_value(len_value_JSON); // Convert the value JSON object to string to get the length in string format

	// Return the length value string
	return atoi(len_text); // return the length string in integer format

}