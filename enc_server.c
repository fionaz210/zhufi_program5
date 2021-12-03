#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Error function used for reporting issues
void error(const char *msg) {
  perror(msg);
  exit(1);
} 

// Set up the address struct for the server socket
void setupAddressStruct(struct sockaddr_in* address, 
                        int portNumber){
 
  // Clear out the address struct
  memset((char*) address, '\0', sizeof(*address)); 

  // The address should be network capable
  address->sin_family = AF_INET;
  // Store the port number
  address->sin_port = htons(portNumber);
  // Allow a client at any address to connect to this server
  address->sin_addr.s_addr = INADDR_ANY;
}

int main(int argc, char *argv[]){
  int connectionSocket, charsRead;
  char buffer[1000];
  struct sockaddr_in serverAddress, clientAddress;
  socklen_t sizeOfClientInfo = sizeof(clientAddress);

  // Check usage & args
  if (argc < 2) { 
    fprintf(stderr,"USAGE: %s port\n", argv[0]); 
    exit(1);
  } 
  
  // Create the socket that will listen for connections
  int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (listenSocket < 0) {
    error("ERROR opening socket");
  }

  // Set up the address struct for the server socket
  setupAddressStruct(&serverAddress, atoi(argv[1]));

  // Associate the socket to the port
  if (bind(listenSocket, 
          (struct sockaddr *)&serverAddress, 
          sizeof(serverAddress)) < 0){
    error("ERROR on binding");
  }

  // Start listening for connetions. Allow up to 5 connections to queue up
  listen(listenSocket, 5); 
  
  // Accept a connection, blocking if one is not available until one connects
  while(1){
    // Accept the connection request which creates a connection socket
    connectionSocket = accept(listenSocket, 
                (struct sockaddr *)&clientAddress, 
                &sizeOfClientInfo); 
    if (connectionSocket < 0){
      error("ERROR on accept");
    }

    printf("SERVER: Connected to client running at host %d port %d\n", 
                          ntohs(clientAddress.sin_addr.s_addr),
                          ntohs(clientAddress.sin_port));

    // Get the message from the client and display it
    // Read the client's message from the socket
    char data[10];

    charsRead = recv(connectionSocket, data, 2, 0); 
    if (charsRead < 0){
      error("ERROR reading from socket");
    }
    int term = 0;
    int counter = 0;
    char copy[70000];
    while(term != 1)
    { 
      memset(buffer, '\0', 100);
      charsRead = recv(connectionSocket, buffer, 1000, 0); 
      if (charsRead < 0){
        error("ERROR reading from socket");
      }
      for (int i = 0; i < strlen(buffer); i++){
        if (buffer[i] != 64){
          copy[counter] = buffer[i];
          counter++;
        }
        else{
          term = 1;
          break;
        }
      }
    }

    int index;
    char key[70000];
    for (int i = 0; i < strlen(copy); i++){
      if (copy[i] != 10){
      key[i] = copy[i];
      }
      else if (copy[i] == 10){
        index = i+1;
        break;
      }
    }
    int i = 0;
    char text[70000];
    for (index; index < strlen(copy); index++){
      if (copy[index] != 64){
      text[i] = copy[index];
      i++;
      }
      else if (copy[index] == 64){
        break;
      }
    }

    int length = strlen(text);

    char cipher[70000];

    int x = 0;
    for(int i = 0; i < strlen(text); i++){
      if (text[i] == 32){
        cipher[x] = 32;
        x++;
      }      
      else if (key[i] == 32){
        cipher[x] = text[i];
        x++;
      }
      else{ 
      cipher[x] = key[i] + text[i] - 65;
      if (cipher[x] > 90){
        cipher[x] -= 26;
      }
      x++;
      }
    }



    // Send a Success message back to the client
    charsRead = send(connectionSocket, 
                    cipher, 1000, 0); 
    if (charsRead < 0){
      error("ERROR writing to socket");
    }
    // Close the connection socket for this client
    close(connectionSocket); 
  }
  // Close the listening socket
  close(listenSocket); 
  return 0;
}