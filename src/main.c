#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct{
    uint8_t* line;
    uint32_t len;
} lines_t;

typedef struct{
    uint8_t FILE_NAME[32];
    uint32_t FILE_SIZE;
    uint32_t FILE_LINE_LENGHT;
    lines_t* FILE_LINES;
} file_t;

typedef struct{
    uint8_t* TOKEN;
} token_t;

typedef struct{
    token_t* TOKENS;
    uint32_t TOKEN_LENGHT;
} lexer_t;

#pragma region io_ops
uint32_t get_file_size(const char* file_path);
uint32_t get_lines(const char* file_path);
lines_t* get_line_data(const char *file_path,uint32_t line_len);
#pragma endregion

#pragma region lexer_token
uint32_t get_token_lenght(file_t file);
lexer_t tokenize_file(file_t file);
#pragma endregion

void debug_lexer(lexer_t lexer);

uint32_t get_file_size(const char* file_path){
    FILE* fp = fopen(file_path,"r");
    fseek(fp,0,SEEK_END);
    uint32_t file_size = ftell(fp);
    fclose(fp);
    return file_size;
}

uint32_t get_lines(const char* file_path){
    FILE* fp = fopen(file_path,"r");
    unsigned int lines = 0;
    char buffer[1024];
    while (fgets(buffer,1024,fp))
    {
        lines++;
    }

    fclose(fp);
    return lines;
}

lines_t* get_line_data(const char *file_path,uint32_t line_len){
    lines_t *len = (lines_t*)malloc(sizeof(lines_t) * line_len);
    uint32_t i = 0;

    FILE* fp = fopen(file_path,"r");
    char buffer[1024];
    while(fgets(buffer,1024,fp)){
        uint32_t lenline = strlen(buffer);
        len[i].line = (uint8_t*)malloc(lenline);
        memcpy(&len[i].line[0], &buffer[0],lenline);
        len[i].line[lenline-1] = '\0';
        i++;
    }

    fclose(fp);
    return len;
}

file_t open_file(const char* file_path){
    uint32_t file_size = get_file_size(file_path);
    uint32_t lines = get_lines(file_path);
    lines_t* line_buffer = get_line_data(file_path,lines);
    file_t file = {.FILE_SIZE = file_size, .FILE_LINE_LENGHT = lines, .FILE_LINES = line_buffer};
    memcpy(&file.FILE_NAME[0],file_path,32);

    return file;
}

uint32_t get_token_lenght(file_t file){
    uint32_t b;
    b = 0;
    for(uint32_t i = 0; i < file.FILE_LINE_LENGHT; i++){
        uint8_t temp[1024] = {0};
        strcpy(&temp[0],&file.FILE_LINES[i].line[0]);
        char* token = strtok(file.FILE_LINES[i].line," ");
        while(token != NULL){
            ++b;
            token = strtok(NULL," ");
        }
        strcpy(&file.FILE_LINES[i].line[0],&temp[0]);
    }
    return b;
}

lexer_t tokenize_file(file_t file){
    uint32_t token_lenght = get_token_lenght(file);
    token_t* tokens = (token_t*)malloc(sizeof(token_t)*token_lenght);
    uint32_t b = 0;
    for(uint32_t i = 0; i < file.FILE_LINE_LENGHT; i++){
        char* token = strtok(file.FILE_LINES[i].line," ");
        while(token != NULL){
            tokens[b].TOKEN = (uint8_t*)malloc(strlen(token));
            strcpy(tokens[b].TOKEN,token);
            ++b;
            token = strtok(NULL," ");
        }
    }


    return (lexer_t){.TOKENS = tokens,.TOKEN_LENGHT = token_lenght};
}

void debug_lexer(lexer_t lexer){
    for(uint32_t i = 0; i < lexer.TOKEN_LENGHT; i++){
        printf("hye token: %s\n",lexer.TOKENS[i].TOKEN);
    };
}


int main(int argc, char* argv[]){
    file_t file = open_file(argv[1]);
    lexer_t lexer = tokenize_file(file);
    debug_lexer(lexer);
    return -1;
}