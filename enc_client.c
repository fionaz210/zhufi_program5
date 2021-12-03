#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()

/**
* Client code
* 1. Create a socket and connect to the server specified in the command arugments.
* 2. Prompt the user for input and send that input as a message to the server.
* 3. Print the message received from the server and exit the program.
*/

// Error function used for reporting issues
void error(const char *msg) { 
  perror(msg); 
  exit(0); 
} 

// Set up the address struct
void setupAddressStruct(struct sockaddr_in* address, 
                        int portNumber, 
                        char* hostname){
 
  // Clear out the address struct
  memset((char*) address, '\0', sizeof(*address)); 

  // The address should be network capable
  address->sin_family = AF_INET;
  // Store the port number
  address->sin_port = htons(portNumber);

  // Get the DNS entry for this host name
  struct hostent* hostInfo = gethostbyname(hostname); 
  if (hostInfo == NULL) { 
    fprintf(stderr, "CLIENT: ERROR, no such host\n"); 
    exit(0); 
  }
  // Copy the first IP address from the DNS entry to sin_addr.s_addr
  memcpy((char*) &address->sin_addr.s_addr, 
        hostInfo->h_addr_list[0],
        hostInfo->h_length);
}

int main(int argc, char *argv[]) {
  int socketFD, portNumber, charsWritten, charsRead, charsWritten2;
  struct sockaddr_in serverAddress;
  char buffer[2000];
  // Check usage & args
  if (argc < 3) { 
    fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); 
    exit(0); 
  } 

  // Create a socket
  socketFD = socket(AF_INET, SOCK_STREAM, 0); 
  if (socketFD < 0){
    error("CLIENT: ERROR opening socket");
  }

   // Set up the server address struct
    setupAddressStruct(&serverAddress, atoi(argv[3]), "localhost");

  // setupAddressStruct(&serverAddress, atoi(argv[2]), argv[1]);

  // Connect to server
  if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
    error("CLIENT: ERROR connecting");
  }

  charsWritten = send(socketFD, "!", 2, 0); 
  if (charsWritten < 0){
    error("CLIENT: ERROR writing to socket");
  }
  if (charsWritten < 1){
    printf("CLIENT: WARNING: Not all data written to socket!\n");
  }
  // Get input message from user
  // printf("CLIENT: Enter text to send to the server, and then hit enter: ");
  FILE *fp  = fopen(argv[1], "r"); // read only 
  char firstFile[70000]; 
  fgets(firstFile, 70000, (FILE*)fp);
  fclose(fp);

  FILE *f  = fopen(argv[2], "r"); // read only 
  char secondFile[70000]; 
  fgets(secondFile, 70000, (FILE*)f);
  fclose(f);

  if (strlen(secondFile) < strlen(firstFile)){
      fprintf(stderr,"key file is too short!\n");
      exit(1);
  }
  for (int i = 0; i < strlen(firstFile); i++){
    if (65 > firstFile[i]) {
      if (firstFile[i] != 32)
      {
        if (firstFile[i] != 10)
        {
          fprintf(stderr,"found a bad character\n");
          exit(1);
        }
      }
    }
    else if (90 < firstFile[i]) {
        fprintf(stderr,"found a bad character\n");
        exit(1);
    }
  }
  for (int i = 0; i < strlen(secondFile); i++){
    if (65 > secondFile[i]) {
      if (secondFile[i] != 32)
      {
        if (secondFile[i] != 10)
        {
        fprintf(stderr,"found a bad character in key\n");
        exit(1);
        }

      }
    }
    else if (90 < secondFile[i]) {
      if (secondFile[i] != 32)
      {
        if (secondFile[i] != 10)
        {
        fprintf(stderr,"found a bad character in key");
        exit(1);
        }
      }
    }
  }



  char temp[2000];
  memset(temp, '\0', 2000);
  memmove(temp, secondFile, strlen(firstFile)+1);
  int k = strlen(firstFile)+1;

  temp[k] = 10;

  strcat(temp, firstFile);

  int i = 0;
  int index = 0;
  while (index < strlen(temp))
  {
    memset(buffer, '\0', 2000);
    for (i = 0; i < 1000; i++){
      buffer[i] = temp[index];
      if (index==strlen(temp)-1){
        buffer[i] = 64;
      }

      index++;
    }
    charsWritten = send(socketFD, buffer, 1000, 0); 
    if (charsWritten < 0){
      error("CLIENT: ERROR writing to socket");
    }
    if (charsWritten < 10){
      printf("CLIENT: WARNING: Not all data written to socket!\n");
    }
  }




  // Get return message from server
  // Clear out the buffer again for reuse
  memset(secondFile, '\0', sizeof(secondFile));
  // Read data from the socket, leaving \0 at end
  charsRead = recv(socketFD, secondFile, 1000, 0); 
  if (charsRead < 0){
    error("CLIENT: ERROR reading from socket");
  }
  if (argc > 4){
    FILE *file  = fopen(argv[5], "r"); // read only 
    printf("%s\n", secondFile);
  }
  else{
    printf("%s", secondFile);
  }
  // Close the socket
  close(socketFD); 
  return 0;
}