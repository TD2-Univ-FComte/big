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
  self->data[0] = val;
}

void bign_create_from_string(struct bign *self, const char *str) {
  //Correspond à un index et à la longueur de la chaine
  size_t len = 0;

  while(str[len] != '\0'){
    len++;
  }
  
  for (size_t i = len; i > 0; i--)
  {
    if(str[i] > '9'){
      self->data[len - i - 1] = str[i] + 10 - 'A';
    }else {
      self->data[len - i - 1] = str[i] - '0'; 
    }
  }
  
}

void bign_copy_from_other(struct bign *self, const struct bign *other) {
  self->size = other->size;
  self->capacity = other->capacity;
}

void bign_move_from_other(struct bign *self, struct bign *other) {
}

void bign_destroy(struct bign *self) {
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
