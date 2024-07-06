#define _XOPEN_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <crypt.h>

#define MAX_SIZE 1000
#define SUCCESS_COLOR "\033[1;32m"

void crack_the_hash(char *hash, char *salt, char *wordlist){
    char line[MAX_SIZE];
    char *encrypt_pass;
    FILE *rockyou;

    if(wordlist != NULL){
        rockyou = fopen(wordlist, "r");
    } else if(!wordlist){
        rockyou = fopen("/usr/share/wordlists/rockyou.txt", "r");
    } else {
        perror("\n[!] Wordlist not open\n");
        exit(1);
    }

    while(fscanf(rockyou, "%s", &line) != EOF){
        encrypt_pass = (char *) crypt(line, salt);
        
        if(strcmp(hash, encrypt_pass) == 0){
            printf("\n[*] Hash collision: %s / %s\n", encrypt_pass, hash);
            printf(SUCCESS_COLOR, "\n[*] Password found: %s\n\033[m", line);
            break;

        } else {

            printf("\n[-] Testing wordlist + salt.. %s", line);
        }
    }

    fclose(rockyou);
}

int main(int n_arg, char *args[]){
    char line[MAX_SIZE];

    if(n_arg > 4 || n_arg < 3){
        fprintf(stderr, "[!] Error: use ./hash_ripper < hash.txt > < salt > or + < wordlist >\n");
        return 1;
    } else if(args[1] != NULL){
        FILE *hash_txt = fopen("hash.txt", "r");
        if(hash_txt == NULL){
            fprintf(stderr, "\n[!] Hash file not found\n");
            return 1;
        }

        fgets(line, MAX_SIZE, hash_txt);
        line[strcspn(line, "\n")] = '\0';
        fclose(hash_txt);
    }

    char *salt = args[2];
    char *wordlist = args[3];

    crack_the_hash(line, salt, wordlist);
    return 0;
}
