#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdbool.h>

#define MAX_TEXT_SIZE 10000
#define MAX_WORDS 2000
#define MAX_WORD_LEN 50
#define MAX_NGRAMS 2000
#define THRESHOLD 50.0
#define NGRAM 3


typedef struct{
	char filename[100];
	char raw_content[MAX_TEXT_SIZE];
	char tokens[MAX_WORDS][MAX_WORD_LEN];
	int token_count;
}fileData;

//input validation
bool readfile(const char *path,char *buffer){
	FILE *file = fopen(path, "r");
	if(file == NULL){
		return false;
	}
	// Membaca isi file ke buffer
	char ch;
	int i = 0;
	while(fgets(buffer + i,MAX_TEXT_SIZE - i, file)){
	 	i = strlen(buffer);
	}
	buffer[i] = '\0';
	
	fclose(file);
	return true;
}

//Normalize : lowercase & remove punctuation
void normalizeAndSplit(fileData *data){
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
		if(temp_idx > 0 && data -> token_count <MAX_WORDS){
			temp[temp_idx] = '\0';
			strcpy(data->tokens[data->token_count],temp);
			data -> token_count++;
		}
	}
}
//JACCARD
double jaccard_similarity(fileData *A,fileData *B){
	int intersect =0;
	int union_count =A->token_count;
	
	for(int i=0;i< B->token_count;i++){
		bool found = false;
		for(int j=0;j< A->token_count;j++){
			if(strcmp(B->tokens[i],A->tokens[j]) == 0){
				found = true;
				break;
			}
		}
		if(found)
			intersect++;
		else
			union_count++;
	}
	return((double) intersect/union_count) * 100.0;
}
//N-GRAM
double ngram_similarity(fileData *A,fileData *B,int n){
	int match = 0;
	int total = A->token_count - n + 1;
	
	for(int i =0;i<total;i++){
		char gramA[200] = "";
		
		for(int w=0;w<n;w++){
			strcat(gramA,A->tokens[i + w]);
			strcat(gramA," ");
		}
	for(int j=0;j<= B->token_count - n; j++){
		char gramB[200] ="";
		for(int x=0;x<n;x++){
			strcat(gramB,B->tokens[j + x]);
			strcat(gramB," ");
		}
		if(strcmp(gramA,gramB) == 0){
			match++;
			break;
		}
	}
}
	return((double) match/total) *100.0;
}
//REPORT
void generate_report(double jaccard, double ngram, double similarity , int method){
	FILE *fp = fopen("report.txt","w");
	if(!fp){
		printf("Gagal membuat report\n");
		return;
		
	}
	fprintf(fp,"MINI TURNITIN REPORT");
	
	if(method == 1){
		fprintf(fp,"Method      : Jaccard Similarity\n");
		fprintf(fp,"Jaccard     : %.2f%%\n",similarity);
	}
	else if(method == 2){
		fprintf(fp,"Method      : N-Gram(3 Kata)\n");
		fprintf(fp,"N-Gram      : %.2f%%\n",similarity);
	}
	else if(method == 3){
		fprintf(fp,"Method     : Gabungan (Jaccard + N-Gram)\n");
        fprintf(fp,"Jaccard    : %.2f%%\n", jaccard);
        fprintf(fp,"N-Gram     : %.2f%%\n", ngram);
        fprintf(fp,"Combined   : %.2f%%\n", similarity);
	}
	
	fprintf(fp,"\n");

    if(similarity >= THRESHOLD){
        fprintf(fp,"Status     : High Similarity\n");
    }
    else{
        fprintf(fp,"Status     : Low Similarity\n");
    }
			
	fclose(fp);		
}

int main(){
	fileData A,B;
	int choice;
	double similarity;
	double jaccard =0.0;
	double ngram = 0.0;
	
	printf("Masukkan file pertama : ");
	scanf("%s", A.filename);
	
	printf("Masukkan file kedua : ");
	scanf("%s",B.filename);
	
	if(!readfile(A.filename, A.raw_content) || !readfile(B.filename, B.raw_content)) {
		printf("Error : File tidak ditemukan\n");
		return 1;
	}
	
	normalizeAndSplit(&A);
	normalizeAndSplit(&B);
	
	printf("\nPilih metode perbanding:\n");
	printf("1. Jaccard Similarity\n");
	printf("2. N-Gram(3 kata)\n");
	printf("3. Gabungan (Jaccard + N-Gram)\n");
	printf("Pilihan: ");
	scanf("%d", &choice);
	
	system("cls");
	
	if(choice == 1){
		similarity = jaccard_similarity(&A,&B);
	}
	else if(choice == 2){
		similarity = ngram_similarity(&A,&B,NGRAM);
	}
	else if(choice == 3){
		jaccard = jaccard_similarity(&A,&B);
		ngram = ngram_similarity(&A,&B,NGRAM);
		
		printf("Jaccard : %.2f%%\n", jaccard);
		printf("N-Gram : %.2f%%\n", ngram);
		
		similarity = (jaccard + ngram) / 2.0;
	}
	else{
		printf("Pilihan tidak valid\n");
		return 1;
	}
	
	
	printf("\nHASIL\n");
	printf("Similarity : %.2f%%\n",similarity);
	
	if(similarity >= THRESHOLD)
		printf("Status :High Similarity\n");
	else
		printf("Status :Low Similarity\n");
		
	generate_report(jaccard, ngram, similarity, choice);
	printf("Report disimpan di report.txt\n");
	
	return 0;	

}
