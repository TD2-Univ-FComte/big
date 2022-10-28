#include "big.h"

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void bign_create_empty(struct bign *self) {
  self->capacity = 20;
  self->size = 0;
  self->data = NULL;
}


void bign_create_from_value(struct bign *self, uint32_t val) {
  self->size = 1;
  self->data = calloc(1, sizeof(uint32_t));
  self->data[0] = val;
}

void bign_create_from_string(struct bign *self, const char *str) {
  
  bign_create_empty(self);
  size_t size = strlen(str);
  self->capacity = (size / 8) + (size % 8);
  self->data = calloc(self->capacity, sizeof(uint32_t));

  if(self->data != NULL){

    while(size > 0){

      char *tab = calloc(9, sizeof(char));

      for (size_t i = 0; i < 8 && size > 0; i++)
      {
        tab[i] = str[size - 1];
        size--;
      }

      char *rev = calloc(9, sizeof(char));

      size_t len_tab = strlen(tab);

      size_t j = len_tab- 1;

      for (size_t i = 0; i < len_tab; i++)
      {
        rev[i] = tab[j];
        j--;
      }

      self->data[self->size] = str_to_integer_ex(rev, 16);

      self->size++;

    }
  }

  if(self->data[self->size - 1] == 0 && self->size > 1){
    self->size--;
  }
  
}

int str_to_integer(const char *str) {
  int n = 0; 
  size_t i = 0;
  
  while (str && str[i] != '\0')
  {
    if(str[i] >= '0' && str[i] <= '9'){
      //Soustraction de 48 (correspondant au code ascii de 0), ex : 9 en ascii correspond à 57, 57 - 48 = 9
      n = n * 10 + (str[i] - '0');
    }
    i++;
  }

  return n;
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
  self->data = calloc(other->size, sizeof(uint32_t));
  for (size_t i = 0; i < other->size; i++)
  {
    self->data[i] = other->data[i]; 
  }
  bign_destroy(other);
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
}

int bign_cmp(const struct bign *lhs, const struct bign *rhs) {
  size_t max=rhs->size;

  if(lhs->size > rhs->size){
    printf("%s\n", "ici");
    return 1;
  }else if(lhs->size < rhs->size){
    printf("%s\n", "la");
    printf("%i\n", rhs->size);
    return -1;
  }else {
    if(lhs->size > rhs->size){
      max = lhs->size;
    }else if(lhs->size < rhs->size){
      max = rhs->size;
    }else {
      for(size_t i = 0; i <max ; i++){
        printf("Cmp lhs : (%i)\n", lhs->data[i]);
        printf("Cmp rhs : (%i)\n", rhs->data[i]);
        if(lhs->data[i] > rhs->data[i]){
          return 1;
        }else if(lhs->data[i] < rhs->data[i]){
          return -1;
        }
      }
      
    }
  }
  return  0;
}

int bign_cmp_zero(const struct bign *self) {
  return self->data[0] == 0 ? 0 : 1;
}

uint32_t pow3(uint32_t r, uint32_t n){
  uint32_t res = r;
  uint32_t i = 1;
  while (i < n)
  {
    res = res * r;
    i++;
  }

  return res;

}

void bign_add(struct bign *self, const struct bign *lhs, const struct bign *rhs) {

  uint32_t base = pow3(2, 31);

  if(lhs->size > rhs->size){

    if(self->data == NULL){
      self->data = calloc(lhs->size, sizeof(uint32_t));
    }

    uint32_t retenu = 0;
    for (size_t i = 0; i < rhs->size; i++)
    {
      printf("%i", lhs->data[i]);
      printf("-%i", rhs->data[i]);
      uint32_t c = lhs->data[i] + rhs->data[i] + retenu;

      self->data[i] = c % base;

      retenu = c / base;
    }
    self->size = rhs->size;
    self->data[lhs->size - 1] += retenu;

    printf("Size : %i\n", self->size);
    
  }else if(lhs->size < rhs->size){

    if(self->data == NULL){
      self->data = calloc(rhs->size, sizeof(uint32_t));
    }

    uint32_t retenu = 0;
    for (size_t i = 0; i < lhs->size; i++)
    {
      printf("%i", lhs->data[i]);
      printf("-%i", rhs->data[i]);
      uint32_t c = lhs->data[i] + rhs->data[i] + retenu;

      self->data[i] = c % base;

      retenu = c / base;
    }
    self->size = lhs->size;
    self->data[lhs->size - 1] += retenu;

  }else {

    if(self->data == NULL){
      self->data = calloc(lhs->size, sizeof(uint32_t));
    }

    uint32_t retenu = 0;
    for (size_t i = 0; i < lhs->size; i++)
    {
      printf("%i", lhs->data[i]);
      printf("-%i", rhs->data[i]);
      uint32_t c = lhs->data[i] + rhs->data[i] + retenu;

      self->data[i] = c % base;

      retenu = c / base;
    }

    self->size = lhs->size;
    self->data[lhs->size - 1] += retenu;
  }
}


void bign_sub(struct bign *self, const struct bign *lhs, const struct bign *rhs) {
}

void bign_mul(struct bign *self, const struct bign *lhs, const struct bign *rhs) {
}


void bign_mul_karatsuba(struct bign *self, const struct bign *lhs, const struct bign *rhs) {
}

void bign_mul_short(struct bign *self, const struct bign *lhs, uint32_t rhs) {
}

// https://janmr.com/blog/2012/11/basic-multiple-precision-short-division/
void bign_div_short(struct bign *quo, uint32_t *rem, const struct bign *lhs, uint32_t rhs) {
}


// https://janmr.com/blog/2014/04/basic-multiple-precision-long-division/
void bign_div(struct bign *quo, struct bign *rem, const struct bign *lhs, const struct bign *rhs) {
}


void bign_exp(struct bign *self, const struct bign *lhs, const struct bign *rhs) {
}

/*
 *
 * bigz
 *
 */

void bigz_create_empty(struct bigz *self) {
  //self->n = NULL;
  //self->positive = true;
}

void bigz_create_from_value(struct bigz *self, int32_t val) {
}

void bigz_create_from_string(struct bigz *self, const char *str, unsigned base) {
}

void bigz_copy_from_other(struct bigz *self, const struct bigz *other) {
}

void bigz_move_from_other(struct bigz *self, struct bigz *other) {
}

void bigz_destroy(struct bigz *self) {
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