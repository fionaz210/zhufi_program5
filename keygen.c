#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

int keygen(int length){
    // 10 \n, 32 space, 65 - 90 ascii
    char result[length+1]; 
    int r;
    time_t t;

    for (int i = 0; i < length+1; i++){
        if (i == length){
            result[i] = 10;
        }
        else{
            r = rand() % (91 + 1 - 65) + 65;
            // r = r + 65;
            if (r==91){
                result[i] = 32;
            }
            else{
                result[i] = r; 
            }
        }
    }

    for (int i = 0; i < length+1; i++){
        printf("%c", result[i]);
    }
}

int main(int argc, char *argv[]){
    srand (time(0));
    // fgets(input, sizeof(input), stdin);
    char *token;
    int num;
    char key;
    if (argc > 1){
        num = atoi(argv[1]);
        key = keygen(num);
    }
    // char s[10] = "keygen ";
    // if(strstr(input, "keygen")){
    //     token = strtok(input, s);
    //     num = atoi(token);
    //     keygen(num);
    // }

    if (argc > 3)
    {
        int targetFD = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (targetFD == -1) { 
            perror("target open()"); 
            exit(1); 
        }				
        // Redirect stdout to target file
        int result = dup2(targetFD, 1);
        if (result == -1) { 
            perror("target dup2()"); 
            exit(2); 
        }
    }
    return 0;
}