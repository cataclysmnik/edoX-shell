#include "my_shell.h"
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

/* Find matches in a list of strings */
static char** find_string_matches(const char** list, const char* prefix, int* out_count) {
    if (!list || !prefix) {
        *out_count = 0;
        return NULL;
    }
    
    size_t prelen = strlen(prefix);
    char** matches = NULL;
    int count = 0;
    
    for (size_t i = 0; list[i]; ++i) {
        if (prelen == 0 || strncmp(list[i], prefix, prelen) == 0) {
            char** tmp = realloc(matches, (count + 1) * sizeof(char*));
            if (!tmp) {
                for (int j = 0; j < count; ++j) free(matches[j]);
                free(matches);
                *out_count = 0;
                return NULL;
            }
            matches = tmp;
            matches[count] = strdup(list[i]);
            if (!matches[count]) {
                for (int j = 0; j < count; ++j) free(matches[j]);
                free(matches);
                *out_count = 0;
                return NULL;
            }
            count++;
        }
    }
    
    *out_count = count;
    return matches;
}

/* Find command matches (builtins + PATH executables) */
char** find_command_matches(const char* prefix, char** env, int* out_count) {
    static const char* builtins[] = {
        "cd", "pwd", "echo", "env", "setenv", "unsetenv", 
        "which", "help", ".help", "run", "exit", "quit", 
        "ls", "history", NULL
    };
    
    int builtin_count = 0;
    char** builtin_matches = find_string_matches(builtins, prefix, &builtin_count);
    
    /* Get PATH matches */
    char* path_str = get_path(env);
    char** matches = builtin_matches;
    int count = builtin_count;
    
    if (path_str) {
        int ndirs = 0;
        char** dirs = split_paths(path_str, &ndirs);
        
        for (int d = 0; d < ndirs; ++d) {
            DIR* dp = opendir(dirs[d]);
            if (!dp) {
                free(dirs[d]);
                continue;
            }
            
            struct dirent* de;
            while ((de = readdir(dp)) != NULL) {
                if (de->d_name[0] == '.') continue;
                
                size_t prelen = strlen(prefix);
                if (prelen > 0 && strncmp(de->d_name, prefix, prelen) != 0) continue;
                
                /* Build full path to check if executable */
                size_t pathlen = strlen(dirs[d]) + strlen(de->d_name) + 2;
                char* full = malloc(pathlen);
                if (!full) continue;
                snprintf(full, pathlen, "%s/%s", dirs[d], de->d_name);
                
                if (access(full, X_OK) == 0) {
                    /* Check for duplicates */
                    int dup = 0;
                    for (int k = 0; k < count; ++k) {
                        if (strcmp(matches[k], de->d_name) == 0) {
                            dup = 1;
                            break;
                        }
                    }
                    
                    if (!dup) {
                        char** tmp = realloc(matches, (count + 1) * sizeof(char*));
                        if (!tmp) {
                            free(full);
                            continue;
                        }
                        matches = tmp;
                        matches[count] = strdup(de->d_name);
                        if (matches[count]) {
                            count++;
                        }
                    }
                }
                free(full);
            }
            closedir(dp);
            free(dirs[d]);
        }
        free(dirs);
        free(path_str);
    }
    
    *out_count = count;
    return matches;
}

/* Find file/directory matches in current directory or given path */
char** find_file_matches(const char* prefix, int* out_count) {
    char** matches = NULL;
    int count = 0;
    
    /* Extract directory and filename parts */
    const char* last_slash = strrchr(prefix, '/');
    char* dir_path = NULL;
    const char* file_prefix = prefix;
    
    if (last_slash) {
        size_t dir_len = last_slash - prefix + 1;
        dir_path = malloc(dir_len + 1);
        if (!dir_path) {
            *out_count = 0;
            return NULL;
        }
        memcpy(dir_path, prefix, dir_len);
        dir_path[dir_len] = '\0';
        file_prefix = last_slash + 1;
    } else {
        dir_path = strdup("./");
        if (!dir_path) {
            *out_count = 0;
            return NULL;
        }
    }
    
    DIR* dp = opendir(dir_path);
    if (!dp) {
        free(dir_path);
        *out_count = 0;
        return NULL;
    }
    
    size_t prefix_len = strlen(file_prefix);
    struct dirent* de;
    
    while ((de = readdir(dp)) != NULL) {
        /* Skip . and .. unless explicitly typed */
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
            if (prefix_len == 0 || 
                (strcmp(file_prefix, ".") != 0 && strcmp(file_prefix, "..") != 0)) {
                continue;
            }
        }
        
        /* Check if name matches prefix */
        if (prefix_len > 0 && strncmp(de->d_name, file_prefix, prefix_len) != 0) {
            continue;
        }
        
        /* Build full match string (directory + filename) */
        size_t match_len = strlen(dir_path) + strlen(de->d_name) + 2;
        char* match = malloc(match_len);
        if (!match) continue;
        
        if (strcmp(dir_path, "./") == 0) {
            snprintf(match, match_len, "%s", de->d_name);
        } else {
            snprintf(match, match_len, "%s%s", dir_path, de->d_name);
        }
        
        /* Add trailing slash for directories */
        struct stat st;
        char* check_path = malloc(strlen(dir_path) + strlen(de->d_name) + 1);
        if (check_path) {
            snprintf(check_path, strlen(dir_path) + strlen(de->d_name) + 1, 
                     "%s%s", dir_path, de->d_name);
            if (stat(check_path, &st) == 0 && S_ISDIR(st.st_mode)) {
                size_t len = strlen(match);
                if (len > 0 && match[len-1] != '/') {
                    char* new_match = malloc(len + 2);
                    if (new_match) {
                        snprintf(new_match, len + 2, "%s/", match);
                        free(match);
                        match = new_match;
                    }
                }
            }
            free(check_path);
        }
        
        /* Add to matches */
        char** tmp = realloc(matches, (count + 1) * sizeof(char*));
        if (!tmp) {
            free(match);
            continue;
        }
        matches = tmp;
        matches[count++] = match;
    }
    
    closedir(dp);
    free(dir_path);
    
    *out_count = count;
    return matches;
}

/* Handle tab completion */
void handle_tab_completion(char* input_buf, size_t* input_len, size_t* cursor, char** env) {
    /* Find the token boundaries at cursor position */
    size_t token_start = *cursor;
    while (token_start > 0 && input_buf[token_start - 1] != ' ' && 
           input_buf[token_start - 1] != '\t') {
        token_start--;
    }
    
    size_t token_end = *cursor;
    while (token_end < *input_len && input_buf[token_end] != ' ' && 
           input_buf[token_end] != '\t') {
        token_end++;
    }
    
    /* Extract the token to complete */
    size_t token_len = *cursor - token_start;
    char* token = malloc(token_len + 1);
    if (!token) return;
    memcpy(token, input_buf + token_start, token_len);
    token[token_len] = '\0';
    
    /* Determine if this is the first token (command) or an argument (file) */
    int is_first_token = 1;
    for (size_t i = 0; i < token_start; i++) {
        if (input_buf[i] != ' ' && input_buf[i] != '\t') {
            is_first_token = 0;
            break;
        }
    }
    
    /* Get matches */
    int match_count = 0;
    char** matches = NULL;
    
    if (is_first_token) {
        /* Complete command names */
        matches = find_command_matches(token, env, &match_count);
    } else {
        /* Complete file/directory names */
        matches = find_file_matches(token, &match_count);
    }
    
    if (match_count == 0) {
        /* No matches - do nothing */
        free(token);
        return;
    } else if (match_count == 1) {
        /* Single match - complete it */
        const char* match = matches[0];
        size_t match_len = strlen(match);
        
        /* Calculate how much to insert */
        size_t to_insert = match_len - token_len;
        
        if (*input_len + to_insert < MAX_INPUT) {
            /* Make room for the new text */
            memmove(input_buf + token_end + to_insert, 
                    input_buf + token_end, 
                    *input_len - token_end + 1);
            
            /* Copy the completion */
            memcpy(input_buf + token_start, match, match_len);
            
            *input_len += to_insert;
            *cursor += to_insert;
        }
    } else {
        /* Multiple matches - show them and find common prefix */
        printf("\n");
        for (int i = 0; i < match_count; ++i) {
            printf("%s  ", matches[i]);
        }
        printf("\n");
        
        /* Find common prefix among all matches */
        size_t common_len = strlen(matches[0]);
        for (int i = 1; i < match_count; ++i) {
            size_t j = 0;
            while (j < common_len && matches[i][j] == matches[0][j]) {
                j++;
            }
            common_len = j;
        }
        
        /* Complete up to common prefix if it's longer than current token */
        if (common_len > token_len) {
            size_t to_insert = common_len - token_len;
            if (*input_len + to_insert < MAX_INPUT) {
                memmove(input_buf + token_end + to_insert, 
                        input_buf + token_end, 
                        *input_len - token_end + 1);
                memcpy(input_buf + token_start, matches[0], common_len);
                *input_len += to_insert;
                *cursor += to_insert;
            }
        }
    }
    
    /* Clean up */
    for (int i = 0; i < match_count; ++i) {
        free(matches[i]);
    }
    free(matches);
    free(token);
}
