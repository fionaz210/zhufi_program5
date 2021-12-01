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
  char buffer[256];
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
    memset(buffer, '\0', 256);
    // Read the client's message from the socket
    charsRead = recv(connectionSocket, buffer, 256, 0); 
    printf("this is buffer %s\n", buffer);
    if (charsRead < 0){
      error("ERROR reading from socket");
    }

    // FILE *fp  = fopen(argv[1], "r"); // read only 
    // char firstFile[256]; 
    // fgets(firstFile, 256, (FILE*)fp);
    // // printf("%s", strlen(line));
    // fclose(fp);
    int index;
    char text[256];
    for (int i = 0; i < strlen(buffer); i++){
      if (buffer[i] != 10){
      text[i] = buffer[i];
      }
      // printf("%c",text[i]);
      else if (buffer[i] == 10){
        index = i+1;
        printf("this is index %d \n", i+1);
        break;
      }
    }
    printf("text %s \n",text);
    char key[256];
    int start = 0;
    for (int i = index; i < strlen(buffer); i++){
      key[start] = buffer[i];
      start++;
    }
    printf("!!!! %s\n",key);
    
    FILE *fp  = fopen(text, "r"); // read only 
    char firstFile[70000]; 
    fgets(firstFile, 70000, (FILE*)fp);
    // printf("%s", strlen(line));
    fclose(fp);

    // char s[2] = "\n";
    // key = strtok(buffer, s);
    // printf("this is key %s \n",key);

    FILE *f  = fopen(key, "r"); // read only 
    char secondFile[70000]; 
    fgets(secondFile, 70000, (FILE*)f);
    // printf("%s", strlen(line));
    fclose(f);

    printf(" textfile %s\n", firstFile);
    printf("secibdfile %s\n", secondFile);

    char cipher[70000];
    char spaces[70000];
    char s = 0;
    char t = 0;
    char uncipher[70000];
    for(int i = 0; i < strlen(firstFile); i++){
      if (firstFile[i] == 32){
        spaces[s] = i;
        s++;
      }
      if (secondFile[i] == 32){
        uncipher[t] = i;
        t++;
      }
      else
      {
        cipher[i] = secondFile[i] + firstFile[i] - 65;
        if (cipher[i] > 90){
          cipher[i] -= 26;
        }
      }
    }
    printf("cipher %s\n", cipher);

    // charsRead = recv(connectionSocket, buffer, 256, 0); 
    // if (charsRead < 0){
    //   error("ERROR reading from socket");
    // }

    // FILE *f  = fopen(buffer, "r"); // read only 
    // printf("this is second buffer aka key %s", buffer);
    // char secondFile[70000]; 
    // fgets(secondFile, 70000, (FILE*)f);
    // // printf("%s", strlen(line));
    // fclose(f);


    // for(int i = 0; i < strlen(text); i++){
    //   text[i] = text[i] + key[i] - 65;
    //   if (text[i] > 90){
    //     text[i] -= 26;
    //   }
    // }


    printf("SERVER: I received this from the client: \n");


    // Send a Success message back to the client
    charsRead = send(connectionSocket, 
                    "hi", 39, 0); 
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
