#include "trie.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){
    if (argc > 2){
        fprintf(stderr, "invalid parameters\n");
    }
    char* file = argc == 2 ? argv[1] : "dictionary.txt";
    FILE* fp = fopen(file,"r");
    trie_t* trie = create_trie_t();
    char* word = NULL;
    size_t size = 0;
    ssize_t len = 0;
    while ((len = getline(&word, &size, fp)) > 0){
        // if (word[len-1] == '\n'){
        //     word[--len] = '\0';
        // }
        add(trie,word);
    }

    puts("enter a word to use:");
    char buffer[50] = {0};
    while (fgets(buffer,20,stdin)){
        len = strlen(buffer);
        if (buffer[len-1] == '\n'){
            buffer[--len] = '\0';
        }
        print_word_possibilities(trie,buffer);
        puts("\nenter another:");
    }

    free(word);
    fclose(fp);
    trie_t_destroy(&trie);
}