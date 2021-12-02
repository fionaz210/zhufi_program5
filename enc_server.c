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
    char data[70000];
    int term = 0;
    int counter = 0;
    char copy[70000];
    while(term != 1)
    { 
      memset(buffer, '\0', 1000);
      charsRead = recv(connectionSocket, buffer, 10, 0); 
      printf("this is buffer %s\n", buffer);
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
    printf("this is copy %s\n", copy);

    // char stuff[70000];
    // int stop = 0;
    // int count = 0;
    // char key[70000];
    // char temp[70000];
    // while(stop != 1)
    // { 
    //   memset(temp, '\0', 1000);
    //   charsRead = recv(connectionSocket, temp, 10, 0); 
    //   printf("this is key %s\n", temp);
    //   if (charsRead < 0){
    //     error("ERROR reading from socket");
    //   }
    //   for (int i = 0; i < strlen(temp); i++){
    //     if (temp[i] != 64){
    //       key[count] = temp[i];
    //       count++;
    //     }
    //     else{
    //       stop = 1;
    //       break;
    //     }
    //   }
    //   // if (counter = 0){
    //   //   strcpy(copy, buffer);
    //   // }
    //   // else{
    //   //   strcat(copy, buffer);
    //   // }
    //   // for (int i = 0; i < strlen(buffer); i++){
    //   //   if (buffer[i] == 64){
    //   //     term = 1;
    //   //   }
    //   // }
    // }
    // printf("this is key %s\n", key);



    // FILE *fp  = fopen(argv[1], "r"); // read only 
    // char firstFile[256]; 
    // fgets(firstFile, 256, (FILE*)fp);
    // // printf("%s", strlen(line));
    // fclose(fp);
    // int index;
    // char text[256];
    // for (int i = 0; i < strlen(buffer); i++){
    //   if (buffer[i] != 10){
    //   text[i] = buffer[i];
    //   }
    //   // printf("%c",text[i]);
    //   else if (buffer[i] == 10){
    //     index = i+1;
    //     printf("this is index %d \n", i+1);
    //     break;
    //   }
    // }
    // printf("text %s \n",text);
    // char key[256];
    // int start = 0;
    // for (int i = index; i < strlen(buffer); i++){
    //   key[start] = buffer[i];
    //   start++;
    // }
    
    // FILE *fp  = fopen(text, "r"); // read only 
    // char firstFile[1000]; 
    // fgets(firstFile, 1000, (FILE*)fp);
    // // printf("%s", strlen(line));
    // fclose(fp);

    // // char s[2] = "\n";
    // // key = strtok(buffer, s);
    // // printf("this is key %s \n",key);

    // FILE *f  = fopen(key, "r"); // read only 
    // char secondFile[1000]; 
    // fgets(secondFile, 1000, (FILE*)f);
    // // printf("%s", strlen(line));
    // fclose(f);

    // printf(" textfile %s\n", firstFile);
    // printf("secibdfile %s\n", secondFile);

    // char cipher[1000];
    // char uncipher[1000];
    // char spaces[1000];
    // int t = 0;
    // int s = 0;
    // int x = 0;
    // for(int i = 0; i < strlen(firstFile); i++){
    //   if (firstFile[i] == 32){
    //     spaces[t] = i;
    //     t++;
    //   }      
    //   if (secondFile[i] == 32){
    //     uncipher[s] = i;
    //     s++;
    //   }
    //   else{
    //     cipher[x] = secondFile[i] + firstFile[i] - 65;
    //     if (cipher[x] > 90){
    //       cipher[x] -= 26;
    //     }
    //     x++;
    //   }
    // }
    // printf("cipher %s\n", cipher);

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
                    "hi", 1000, 0); 
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
