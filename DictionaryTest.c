/*********************************************************************************
 * * * * Ivan Garcia-Sanchez, igarci33
 * * * * 2020 Spring CSE101 PA5
 * * * * DictionaryTest.c
 * * * *********************************************************************************/

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"Dictionary.h"

int main(int argc, char* argv[]){
   
   int i, n=7;
   Dictionary A = newDictionary(0);
   Dictionary B = newDictionary(1);
   char* word[] = { "a", "b", "c", "d", "e", "f", "g" };
   VAL_TYPE x;
   VAL_TYPE y;

   for(i=0; i<n; i++){
      insert(A, word[i], i);
   }

   for(i=n-1; i>=0; i--){
      insert(B, word[i], i);
   }

   printf("forward A:\n");
   for(x=beginForward(A); currentVal(A)!=VAL_UNDEF; x=next(A)){
      printf("key: "KEY_FORMAT" value: "VAL_FORMAT"\n", currentKey(A), x);
   }
   printf("\n\n");

   printf("reverse B:\n");
   for(y=beginReverse(B); currentVal(B)!=VAL_UNDEF; y=prev(B)){
      printf("key: "KEY_FORMAT" value: "VAL_FORMAT"\n", currentKey(B), y);
   }
   printf("\n\n");


   printf("Dictionary A (%sunique keys):\n", (getUnique(A)?"":"non-"));
   printDictionary(stdout, A);
   printf("\n");

   printf("Dictionary B (%sunique keys):\n", (getUnique(B)?"":"non-"));
   printDictionary(stdout, B);
   printf("\n");
   
   makeEmpty(A);
   printf("After makeEmpty(A): \n");
   printDictionary(stdout,A);

   for(i=0; i<n; i++){
      insert(A, word[i], i);
   }
   printf("After inserts: \n");
   printDictionary(stdout,A);
   for(i=0; i<n; i++){
      delete(A, word[i]);
   }
   printf("After deletes: \n");
   printDictionary(stdout,A);


   freeDictionary(&A);
   freeDictionary(&B);

   return(0);
}
