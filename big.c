//Binome : Gayot Arthur (TP2B) & Dolard Anton (TP2B)

#include "big.h"

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void bign_create_empty(struct bign *self) {
  self->capacity = 40;
  self->size = 0;
  self->data = NULL;
}

//Algorithme du cours pour étendre la taille d'un tableau dynamique (en augmentant sa capacité de x2)
void bign_array_add(struct bign *self, uint32_t value) {
  if (self->size == self->capacity) {
    self->capacity *= 2;
    uint32_t *data = calloc(self->capacity, sizeof(uint32_t));
    memcpy(data, self->data, self->size * sizeof(uint32_t));
    free(self->data);
    self->data = data;
  }
  self->data[self->size] = value;
  self->size++;
}

void bign_create_from_value(struct bign *self, uint32_t val) {
  self->size = 1;
  self->data = calloc(1, sizeof(uint32_t));
  self->data[0] = val;
}

void bign_create_from_string(struct bign *self, const char *str) {
  
  bign_create_empty(self);
  size_t size = strlen(str);
  self->data = calloc(self->capacity, sizeof(uint32_t));

  if(self->data != NULL){

    while(size > 0){

      char *tab = calloc(9, sizeof(char));
      size_t len_tab = 0;

      //On rempli une chaine tampon avec 8 charactères ou moins (selon ce qu'il reste dans la liste principale)
      for (size_t i = 0; i < 8 && size > 0; i++)
      {
        tab[i] = str[size - 1];
        size--;
        len_tab++;
      }

      char *rev = calloc(9, sizeof(char));

      size_t j = len_tab- 1;

      //On réinverse la chaine "tampon"
      for (size_t i = 0; i < len_tab; i++)
      {
        rev[i] = tab[j];
        j--;
      }

      //Conversion de la chaine de caractère vers un entier
      bign_array_add(self, str_to_integer_ex(rev, 16));

      free(tab);
      free(rev);

    }
  }

  //Normalisation (suppression du 0 en chiffre significatif)
  if(self->data[self->size - 1] == 0 && self->size > 1){
    self->size--;
  }
  
}

int str_to_integer_ex(char *str, int base) {
    size_t resultat=0;
    bool endfor=false;
    for(size_t i = 0;i< strlen(str) && endfor==false ;i++){
        if(str[i]>='A' && str[i]<='A'+(base-11)){
            resultat= resultat * base+str[i]-'A'+10;
        }else if(str[i]>='0' && str[i]<='0'+10 && str[i]<='0'+(base-1)){
            resultat=resultat * base+str[i]-'0';
        }else if(str[i]>='a' && str[i]<='a'+(base-11)){
            resultat=resultat * base+str[i]-'a'+10;
        }else{
            endfor=true;
        }

    }
    
    return resultat;
 
} 

void bign_copy_from_other(struct bign *self, const struct bign *other) {
  if(self->data != NULL){
    bign_destroy(self);
  }
  self->data = calloc(other->size, sizeof(uint32_t));
  for (size_t i = 0; i < other->size; i++)
  {
    self->data[i] = other->data[i];
  }
  self->size = other->size;
}

void bign_move_from_other(struct bign *self, struct bign *other) {
  if(self->data != NULL){
    bign_destroy(self);
  }
   
  self->capacity = other->capacity;
  self->size = other->size;
  
  self->data = other->data;

  other->data = NULL;
  other->capacity = 0;
  other->size = 0;
}

void bign_destroy(struct bign *self) {
  if(self->data != NULL){
    free(self->data);
    self->data = NULL;
  }
}

void bign_print(const struct bign *self) {
  for(size_t k=0;k<self->size;k++){
    printf("Self->data[%i] : %x\n",k , self->data[k]);
  }
}
static void bign_normalize ( struct bign * self ){
  size_t i = self->size;
  while(self->data[i]==0){
    self->size=self->size-1;
    i--;
  }
}

int bign_cmp(const struct bign *lhs, const struct bign *rhs) {
  size_t max=rhs->size;
  if(lhs->size > rhs->size){
    max = lhs->size;
  }else if(lhs->size < rhs->size){
    max = rhs->size;
  }else {
    for(size_t i = 0; i <max ; i++){
       // printf("Cmp lhs : (%i)\n", lhs->data[i]);
        //printf("Cmp rhs : (%i)\n", rhs->data[i]);
      if(lhs->data[i] > rhs->data[i]){
        return 1;
      }else if(lhs->data[i] < rhs->data[i]){
        return -1;
      }
    }
      
  }
  if(lhs->size > rhs->size){
   // printf("%s\n", "ici");
    return 1;
  }else if(lhs->size < rhs->size){
    //printf("%s\n", "la");
    //printf("%i\n", rhs->size);
    return -1;
  }
  return  0;
}

int bign_cmp_zero(const struct bign *self) {
  return self->data[0] == 0 ? 0 : 1;
}

//Fonction permettant de calculer la puissance n d'un nombre donné (exponentiation rapide)
uint32_t pow3(uint32_t r, uint32_t n){
  if(n == 1){
    return r;
  }else {
    if(n % 2 == 0){
      return pow3(r * r, n/2);
    }else {
      return r * pow3(r * r, (n-1)/2);
    }
  }
}

void bign_add(struct bign *self, const struct bign *lhs, const struct bign *rhs) {

   uint32_t base = pow3(2, 31);

  if(lhs->size > rhs->size){
    self->size = lhs->size;
    if(self->data == NULL){
      self->data = calloc(lhs->size, sizeof(uint32_t));
    }

    uint32_t retenu = 0;
    for (size_t i = 0; i < rhs->size; i++)
    {
      uint32_t c = lhs->data[i] + rhs->data[i] + retenu;
      printf("c = %x\n",c);
      self->data[i] = c % base;
    printf("%x + %x + %x = %x    retenu = %i\n",lhs->data[i] , rhs->data[i],retenu ,self->data[i] ,retenu);
      retenu = c / base;
      
      if(retenu>0){
      self->size+=1;
      self->data[i+1]+=retenu;
      }
    }
    

    bign_print(self);
    
  }else if(lhs->size < rhs->size){
    self->size = rhs->size;
    if(self->data == NULL){
      self->data = calloc(rhs->size, sizeof(uint32_t));
    }

    uint32_t retenu = 0;
    for (size_t i = 0; i < lhs->size; i++)
    {
      uint32_t c = lhs->data[i] + rhs->data[i] + retenu;
      printf("c = %x\n",c);
      self->data[i] = c % base;
      printf("%x + %x + %x = %x    retenu = %i\n",lhs->data[i] , rhs->data[i],retenu ,self->data[i] ,retenu);
      retenu = c / base;
      
      if(retenu>0){
      self->size+=1;
      self->data[i+1]+=retenu;
      }
    }
    
    bign_print(self);
  }else {
    self->size = lhs->size;
    if(self->data == NULL){
      self->data = calloc(lhs->size, sizeof(uint32_t));
    }

    uint32_t retenu = 0;
    for (size_t i = 0; i < lhs->size; i++)
    {
      uint32_t c = lhs->data[i] + rhs->data[i] + retenu;
      printf("c = %x\n",c);
      self->data[i] = c % base;
 printf("%x + %x + %x = %x    retenu = %i\n",lhs->data[i] , rhs->data[i],retenu ,self->data[i] ,retenu);
      retenu = c / base;
     
      if(retenu>0){
      self->size+=1;
      self->data[i+1]+=retenu;
      }
    }

    
    bign_print(self);
  }
}


void bign_sub(struct bign *self, const struct bign *lhs, const struct bign *rhs) {
  uint32_t base = pow3(2, 31);
  size_t max = rhs->size;
  if (bign_cmp(lhs,rhs)==0 || bign_cmp(lhs,rhs)>0){
    if(lhs->size > rhs->size){
      max = rhs->size;
    }else if(lhs->size < rhs->size){
      max = lhs->size;
    }
    if(self->data == NULL){
      self->data = calloc(max, sizeof(uint32_t));
    }

    uint32_t retenu = 0;
    for (size_t i = 0; i < max; i++)
    {
      uint32_t d = lhs->data[i] - rhs->data[i] - retenu;

      self->data[i] = d % base;

      retenu = d / base;
    }
    self->size = max;
    self->data[lhs->size - 1] -= retenu;
    
  }

}

void bign_mul(struct bign *self, const struct bign *lhs, const struct bign *rhs) {
  self->size=lhs->size+rhs->size-1;
  self->data = calloc(self->size, sizeof(uint32_t));

  uint32_t base = pow3(2,31);

  for(size_t i = 0;i<lhs->size;i++){
    uint32_t retenu = 0;
    
    for(size_t j = 0;j<rhs->size;j++){
      uint32_t t = rhs->data[j]*lhs->data[i]+retenu+self->data[i+j];
      self->data[i+j]=t % base;
      retenu = t / base;
      printf("%x * %x = %x    retenu = %i\n",lhs->data[j] , rhs->data[i],self->data[i+j], retenu); 
    }
    if(retenu>0){
      self->size+=1;
      self->data[i+lhs->size]=retenu;
    }
  }
  bign_print(self);
}


void bign_mul_karatsuba(struct bign *self, const struct bign *lhs, const struct bign *rhs) {
}

void bign_mul_short(struct bign *self, const struct bign *lhs, uint32_t rhs) {
}

// https://janmr.com/blog/2012/11/basic-multiple-precision-short-division/
void bign_div_short(struct bign *quo, uint32_t *rem, const struct bign *lhs, uint32_t rhs) {
  quo->data = calloc(quo->capacity, sizeof(uint32_t));
  uint32_t r = 0;
  uint32_t base = pow3(2, 31);
  for (size_t i = 0; i < lhs->size; i++)
  {
    uint32_t c = r * base + lhs->data[i];
    quo->data[i] = c % rhs;
    quo->size += 1;
    r = c / rhs;
  }

  *rem = r;
}


// https://janmr.com/blog/2014/04/basic-multiple-precision-long-division/
void bign_div(struct bign *quo, struct bign *rem, const struct bign *lhs, const struct bign *rhs) {
  /*quo->data = calloc(quo->capacity, sizeof(uint32_t));
  rem->data = calloc(rem->capacity, sizeof(uint32_t));
  if(bign_cmp(lhs, rhs) == -1){
    for (size_t i = 0; i < lhs->size; i++)
    {
      rem->data[i] = lhs->data[i];
    }
    rem->size = lhs->size;

    quo->data[0] = 0;
    quo->size = 1;
  }else if(rhs->size == 1){
    uint32_t r = 0;
    bign_div_short(quo, r, lhs, rhs);
  }else {

  }*/
}


void bign_exp(struct bign *self, const struct bign *lhs, const struct bign *rhs) {
}

/*
 *
 * bigz
 *
 */

void bigz_create_empty(struct bigz *self) {
  bign_create_empty(&self->n);
  self->positive = true;
}

void bigz_create_from_value(struct bigz *self, int32_t val) {
  self->n.data = calloc(1, sizeof(int32_t));
  self->n.size = 1;
  if(val > 0){
    self->n.data[0] = val;
    self->positive = true;
  }else {
    self->n.data[0] = (-1) * val;
    self->positive = false;
  }
}

void bigz_create_from_string(struct bigz *self, const char *str, unsigned base) {
  bigz_create_empty(self);
  self->n.data = calloc(self->n.capacity, sizeof(uint32_t));

  self->positive = str[0] == '-' ? false : true;
  size_t size = strlen(str);
  
  if(self->n.data != NULL){
    size_t limit = self->positive ? 0 : 1;
    while(size > limit){

      size_t base_c = base == 2 ? 32 : base == 10 ? 10 : 8;

      char *tab = calloc(base_c + 1, sizeof(char));
      size_t len_tab = 0;

      //On rempli une chaine tampon avec 8 charactères ou moins (selon ce qu'il reste dans la liste principale)
      for (size_t i = 0; i < base_c && size > limit; i++)
      {
        tab[i] = str[size - 1];
        size--;
        len_tab++;
      }

      char *rev = calloc(base_c + 1, sizeof(char));

      size_t j = len_tab- 1;

      //On inverse la chaine "tampon"
      for (size_t i = 0; i < len_tab; i++)
      {
        rev[i] = tab[j];
        j--;
      }

      //Conversion de la chaine de caractère vers un entier
      bign_array_add(&self->n, str_to_integer_ex(rev, base));

      free(tab);
      free(rev);

    }
  }

  //Normalisation (suppression du 0 en chiffre significatif)
  /*if(self->n.data[self->n.size - 1] == 0 && self->n.size > 1){
    self->n.size--;
  }*/
}

void bigz_copy_from_other(struct bigz *self, const struct bigz *other) {
}

void bigz_move_from_other(struct bigz *self, struct bigz *other) {
}

void bigz_destroy(struct bigz *self) {
  if(self->n.data != NULL){
    free(self->n.data);
    self->n.data = NULL;
  }
}

void bigz_print(const struct bigz *self) {
}

int bigz_cmp(const struct bigz *lhs, const struct bigz *rhs) {

  return 0;
}

int bigz_cmp_zero(const struct bigz *self) {
  return 0;
}

void bigz_add(struct bigz *self, const struct bigz *lhs, const struct bigz *rhs) {
}

void bigz_sub(struct bigz *self, const struct bigz *lhs, const struct bigz *rhs) {
}

void bigz_mul(struct bigz *self, const struct bigz *lhs, const struct bigz *rhs) {
}

void bigz_div(struct bigz *quo, struct bigz *rem, const struct bigz *lhs, const struct bigz *rhs) {
}

void bigz_gcd(struct bigz *self, const struct bigz *lhs, const struct bigz *rhs) {
}