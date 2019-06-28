#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

// ssize_t send(int socket, const void *buffer, size_t length, int flags);
// ssize_t recv(int socket, void *buffer, size_t length, int flags);

int main () {
	int sckt_id, server, rd;
	struct sockaddr_in serv_addr;
	char message[50];
	
	sckt_id = socket(AF_UNIX, SOCK_STREAM, 0);
   	if(sckt_id == -1) {
      perror("socket error");
      exit(1);
   	}
   	printf("> Socket A foi criado\n");
   	
   	serv_addr.sin_family = AF_INET;
  	serv_addr.sin_port = htons(5000);
  	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	server = connect(sckt_id, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	if (server == -1) {
      perror("connect error");
      exit(1);
    }
	
	return 0;
}
