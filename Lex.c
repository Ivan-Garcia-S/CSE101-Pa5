/*********************************************************************************
 * * * Ivan Garcia-Sanchez, igarci33
 * * * 2020 Spring CSE101 PA5
 * * * Lex.c
 * * *********************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include"Dictionary.h"
#include<string.h>

#define MAX_LEN 160

int main(int argc, char * argv[]){
   int num = 0;
   FILE *in, *out;
   char line[MAX_LEN];
   char** key;
   Dictionary D = newDictionary(0);
   if(argc != 3){
      printf("Usage: %s <input file> <output file>\n", argv[0]);
      exit(1);
   }

   in = fopen(argv[1], "r");
   out = fopen(argv[2], "w");
   if(in == NULL){
      printf("Unable to open file %s for reading\n", argv[1]);
      exit(1);
   }
   if(out == NULL){
      printf("Unable to open file %s for writing\n", argv[2]);
      exit(1);
   }
   
   while(fgets(line,MAX_LEN,in) != NULL){
      num++;
   }
   rewind(in);
   key = calloc(num,sizeof(char*));
   for(int i = 0; i < num; i++){
      fgets(line,MAX_LEN,in);
      int x = strlen(line);
      key[i] = calloc(x+1,sizeof(char*));
      strcpy(key[i],line);
   }

   for(int i = 0; i < num; i++){   
      insert(D,key[i],i);
   }
   
   for(beginForward(D); currentVal(D)!=VAL_UNDEF; next(D)){
      KEY_TYPE x = currentKey(D);
      fprintf(out,KEY_FORMAT, x);
   }

   for(int i = 0; i < num; i++){
      free(key[i]);
   }
   free(key);
   
   fclose(in);
   fclose(out);
   freeDictionary(&D);

   return(0);
}
