#include "my_shell.h"

char** parse_input(char* input)
{
    size_t buffer_size = MAX_INPUT;
    char** tokens = malloc(buffer_size * sizeof(char*));
    char* token = NULL;
    size_t position = 0;
    size_t token_length = 0;

    if (!tokens) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; input[i]; i++) {
        token = &input[i];

        while (input[i] && input[i] != ' ' && input[i] != '\n') {
            token_length++;
            i++; 
        }

        tokens[position] = malloc((token_length + 1) * sizeof(char));
        if (!tokens[position]) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        for (size_t j = 0; j < token_length; j++)
        {
            tokens[position][j] = token[j];
        }
        tokens[position][token_length] = '\0'; //Null terminate token
        position++;
        token_length = 0;
    }

    tokens[position] = NULL; //Terminate the array with NULL
    return tokens;
}