#include "my_shell.h"
#include <ctype.h>
#include <string.h>

/* Parse input into argv-style array, honoring single and double quotes and backslash escapes.
   Returns malloc'd NULL-terminated array; tokens and array must be freed with free_tokens(). */
char** parse_input(char* input)
{
    if (!input) return NULL;

    size_t capacity = 16;
    size_t count = 0;
    char** tokens = malloc(capacity * sizeof(char*));
    if (!tokens) { perror("malloc"); return NULL; }

    char* p = input;
    while (*p) {
        /* skip whitespace */
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p) break;

        /* ensure capacity */
        if (count + 1 >= capacity) {
            capacity *= 2;
            char** tmp = realloc(tokens, capacity * sizeof(char*));
            if (!tmp) { perror("realloc"); break; }
            tokens = tmp;
        }

        char quote = 0;
        if (*p == '\'' || *p == '"') {
            quote = *p;
            p++;
        }

        /* build token in a dynamic buffer */
        size_t buf_cap = 128;
        size_t buf_len = 0;
        char* buf = malloc(buf_cap);
        if (!buf) { perror("malloc"); break; }

        while (*p) {
            if (quote) {
                if (*p == quote) { p++; break; }        /* closing quote */
                if (*p == '\\' && quote == '"' && p[1]) { /* allow backslash escapes inside double quotes */
                    p++;
                    buf[buf_len++] = *p++;
                } else {
                    buf[buf_len++] = *p++;
                }
            } else {
                if (isspace((unsigned char)*p)) break;
                if (*p == '\'' || *p == '"') {
                    /* start quoted segment inside unquoted token */
                    quote = *p++;
                    continue;
                }
                if (*p == '\\' && p[1]) {
                    p++;
                    buf[buf_len++] = *p++;
                } else {
                    buf[buf_len++] = *p++;
                }
            }
            if (buf_len + 1 >= buf_cap) {
                buf_cap *= 2;
                char* nb = realloc(buf, buf_cap);
                if (!nb) { perror("realloc"); free(buf); buf = NULL; break; }
                buf = nb;
            }
        }

        if (!buf) break;
        buf[buf_len] = '\0';
        tokens[count++] = buf;
    }

    tokens[count] = NULL;
    return tokens;
}

void free_tokens(char** tokens)
{
    if (!tokens) return;
    for (size_t i = 0; tokens[i]; ++i) free(tokens[i]);
    free(tokens);
}