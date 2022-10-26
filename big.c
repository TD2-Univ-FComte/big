#include "big.h"

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void bign_create_empty(struct bign *self) {
  self->capacity = 10;
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
  self->data = calloc(self->capacity, sizeof(uint32_t));

  if(self->data != NULL){
    size_t size = strlen(str);

    while(size > 0){

      char *tab = calloc(9, sizeof(char));

      for (size_t i = 0; i < 8 && size > 0; i++)
      {
        tab[i] = str[size - 1];
        size--;
      }

      printf("%li", size);
      char *endPtr;
      self->data[self->size] = strtoul(tab, NULL, 16);

      self->size++;

    }
  }
  
  
}

int str_to_integer_ex(const char *str, int base) {
    size_t len = strlen(str);
    int power = 1;
    int n = 0;

    for(size_t i = 0; i < len; i++){
      if((str[len - i - 1] >= '0' && str[len - i - 1] <= '9') || (str[len - i - 1] >= 'A' && str[len - i - 1] <= 'Z') || (str[len - i - 1] >= 'a' && str[len - i - 1] <= 'z')){
        if(str[len - i - 1] >= '0' && str[len - i - 1] <= '9'){
          int c = (str[len - i - 1] - '0') < base ? (str[len - i - 1] - '0') * power : 0;
          n += c;
        }else if(str[len - i - 1] >= 'A' && str[i] <= 'Z'){
          int c = (str[len - i - 1] - 'A' + 10) < base ? (str[len - i - 1] - 'A' + 10) * power : 0;
          n += c;
        }else if(str[len - i - 1] >= 'a' && str[i] <= 'z'){
          int c = ((str[len - i - 1] - 32) - 'A' + 10) < base ? ((str[len - i - 1] - 32) - 'A' + 10) * power : 0;
          n += c;
        }
        power = power * base;
      }
    }

    return n;
 
}

void bign_copy_from_other(struct bign *self, const struct bign *other) {
  self->size = other->size;
  self->capacity = other->capacity;
}

void bign_move_from_other(struct bign *self, struct bign *other) {
}

void bign_destroy(struct bign *self) {
  //free(self);
}

void bign_print(const struct bign *self) {
}

int bign_cmp(const struct bign *lhs, const struct bign *rhs) {
  return 0;
}

int bign_cmp_zero(const struct bign *self) {
  return 0;
}

void bign_add(struct bign *self, const struct bign *lhs, const struct bign *rhs) {
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
