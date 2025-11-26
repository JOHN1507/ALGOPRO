#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdbool.h>

#define MAX_TEXT_SIZE 10000
#define MAX_WORDS 2000
#define MAX_WORD_LEN 50
#define THRESHOLD 50.0

typedef struct{
	char filename[100];
	char raw_content[MAX_TEXT_SIZE];
	char tokens[MAX_WORD][MAX_WORD_LEN];
	int token_count;
} File data;

//input validation
bool readfile(const char *path,char *buffer){
	FILE *file = fopen(path, "r");
	if(file == NULL){
		return false;
	}
	// Membaca isi file ke buffer
	char ch;
	int i = 0;
	while((ch = fgets(file)) != EOF && i < MAX_TEXT_SIZE - 1 ){
	buffer[i++] = ch;
	}
	buffer[i] = '\0';
	
	fclose(file);
	return true;
}

//Normalize : lowercase & remove punctuation
void normalizeAndSplit(FileData *data){
	char temp[MAX_WORD_LEN];
	int temp_idx = 0;
	data ->token_count = 0;
	
	for(int i =0;data -> raw_content[i] != '\0'; i++){
		char c = data ->raw_content[i];
		
		if(isalnum(c)){
			if(temp_idx < MAX_WORD_LEN - 1){
				temp[temp_idx++] = tolower(c);
			}
		} else{
			if(temp_idx > 0){
				temp[temp_idx] = '\0';
				if(data ->token_count < MAX_WORDS){
					strcpy(data->tokens[data->token_count],temp);
					data->token_count++;
				}
				temp_idx = 0;
			}
		}
	}
}

