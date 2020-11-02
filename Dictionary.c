/*********************************************************************************
 * * * Ivan Garcia-Sanchez, igarci33
 * * * 2020 Spring CSE101 PA5
 * * * Dictionary.c
 * * *********************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include <stdbool.h>
#include "Dictionary.h"

	
typedef struct NodeObj{
   KEY_TYPE key;
   VAL_TYPE value;
   struct NodeObj* left;
   struct NodeObj* right;
   struct NodeObj* parent;
} NodeObj;

typedef NodeObj* Node;

Node newNode(KEY_TYPE k, VAL_TYPE v){
   Node N = malloc(sizeof(NodeObj));
   assert(N != NULL);
   N->key = k;
   N->value = v;
   N->parent = NULL;
   N->left = NULL;
   N->right = NULL;
   return N;	
}
	
typedef struct DictionaryObj{
   Node root;
   Node current;
   int unique;
   int size;
} DictionaryObj;
	
typedef DictionaryObj* Dictionary;

Dictionary newDictionary(int unique){
   Dictionary D = malloc(sizeof(DictionaryObj));
   D->root = NULL;
   D->current = NULL;
   D->unique = unique;
   D->size = 0;
   return D;
}

void freeDictionary(Dictionary* pD){
   if( pD!=NULL && *pD!=NULL ){
      if( size(*pD) == 0 ) makeEmpty(*pD);
      free(*pD);
      *pD = NULL;
   }
}	
	
void freeNode(Node* pN){
   if( pN!=NULL && *pN!=NULL ){
      free(*pN);
      *pN = NULL;
   }
}

int isEmpty(Dictionary D) {
   if(size(D) == 0){
      return 1;
   }
   return 0;
}
	

int size(Dictionary D) {
   return D->size;
}

int getUnique(Dictionary D){
   return D->unique;
}

Node TreeMinimum(Node x){
    if(x != NULL){
       while(x->left != NULL){
          x = x->left;
       }
    }
    return x;
}

Node TreeMaximum(Node x){
   if(x != NULL){
      while(x->right != NULL){
         x = x->right;
      }
   }
   return x;
}

Node TreePredecessor(Node x){
   if(x->left != NULL){
      x = TreeMaximum(x->left);
      return x; 
   }
   Node y = x->parent;
   while (y != NULL && KEY_CMP(x->key,y->key) < 0){
      x = y;
      y = y->parent;
   }
   return y;
}

Node TreeSuccessor(Node x){
   if(x->right != NULL){
      x = TreeMinimum(x->right);
      return x;
   }
   Node y = x->parent;
   while(y != NULL && x == y->right){
      x = y;
      y = y->parent;
   }
   return y;
}

Node TreeSearch(Node x, KEY_TYPE k){
   if(x == NULL || KEY_CMP(x->key,k) == 0){
      return x;
   }
   if(KEY_CMP(k, x->key) < 0){
      return TreeSearch(x->left,k);
   }
   return TreeSearch(x->right,k);
}

void Transplant(Dictionary D, Node u, Node v){
   if(u->parent == NULL){
      D->root = v;
      if(v != NULL){
         v->parent = NULL;
      }
   }
   else if(u == u->parent->left){
      u->parent->left = v;
   }
   else{
      u->parent->right = v;
   }
   if(v != NULL){
      v->parent = u->parent;
   }
}

void InOrderTreeWalk(FILE* out, Node x){
   if(x != NULL){
      InOrderTreeWalk(out,x->left);
      fprintf(out,"%s %d\n",x->key,x->value);
      InOrderTreeWalk(out,x->right);
   }
}


VAL_TYPE lookup(Dictionary D,KEY_TYPE k) {
   Node n = TreeSearch(D->root,k);
   if(n == NULL){
      return VAL_UNDEF;
   }
   return n->value;
}
	
void insert(Dictionary D, KEY_TYPE k, VAL_TYPE v){
   if((getUnique(D) && lookup(D,k) == VAL_UNDEF) || !getUnique(D)){
      Node y = NULL;
      Node x = D->root;
      Node z; 
         
      while (x != NULL){
         y = x;
         if(KEY_CMP(k,x->key) < 0){
            x = x->left;
         }
         else{
            x = x->right;
         }
      }
      if(y == NULL){
         z = newNode(k,v);
         z->parent = y;
         D->root = z;
      }
      else if(KEY_CMP(k,y->key) < 0){
         z = newNode(k,v);
         z->parent = y;
        y->left = z;
      }
      else{
         z = newNode(k,v);
         z->parent = y;
         y->right = z;
      }
      D->size += 1;
   }
   
   else{
      printf("Dictionary error: calling insert on existing key: %s\n",k);
      exit(1); 
   }
}
	
void delete(Dictionary D, KEY_TYPE k){
   if(lookup(D,k) != VAL_UNDEF){
      Node z = D->root;
      Node y = NULL;
      
      while(KEY_CMP(z->key,k) != 0){
         if(KEY_CMP(z->key,k) < 0){
            z = z->right;
         }	
         else if (KEY_CMP(z->key,k) > 0){
            z = z->left;
         }
         y = z->parent;
      }	
      if(D->current == z){
         D->current = NULL;
      }
      if(z->left == NULL){
         Transplant(D, z, z->right);
         freeNode(&z);
         z = NULL;
      }
      else if(z->right == NULL){
         Transplant(D, z, z->left);
         freeNode(&z); 
         z = NULL;
      }
      else{ 
         y = TreeMinimum(z->right);
         if(y->parent != z){
            Transplant(D,y,y->right);
            y->right = z->right;
            y->right->parent = y;
         }
         Transplant(D,z,y);
         y->left = z->left;
         y->left->parent = y;
         freeNode(&z);
         z = NULL;
      }  
      D->size -= 1;
   }

}
	
void makeEmpty(Dictionary D) {
   if(!isEmpty(D)){
      while(D->root != NULL){
         delete(D,D->root->key);
      }
      D->size = 0;
      D->current = NULL;
      D->root = NULL;
   }
}
	
VAL_TYPE beginForward(Dictionary D){
   D->current = TreeMinimum(D->root);
   return currentVal(D); 
}

VAL_TYPE beginReverse(Dictionary D){
   D->current = TreeMaximum(D->root);
   return currentVal(D);
}

KEY_TYPE currentKey(Dictionary D){
   if(D->current == NULL){
      return KEY_UNDEF;
   }
   return D->current->key;
}

VAL_TYPE currentVal(Dictionary D){
   if(D->current == NULL){
      return VAL_UNDEF;
   }
   return D->current->value;
}

VAL_TYPE next(Dictionary D){
  if(D->current == NULL){
     return currentVal(D);
  }  
  D->current = TreeSuccessor(D->current);
  return currentVal(D);
}

VAL_TYPE prev(Dictionary D){
  if(D->current == NULL){
     return currentVal(D);
  }
  D->current = TreePredecessor(D->current);
  return currentVal(D);
}

void printDictionary(FILE* out, Dictionary D){
   Node x = D->root;
   InOrderTreeWalk(out,x);
}

