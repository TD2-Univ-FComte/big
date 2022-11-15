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
  self->capacity = 40;
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

      //Taille de la chaine tampon (utilisation d'une variable que l'on accrementera à chaque passage de la boucle qui suit, pour éviter de réutiliser strlen une nouvelle fois)
      size_t len_tab = 0;

      //On rempli une chaine tampon avec 8 charactères ou moins (selon ce qu'il reste dans la liste principale)
      for (size_t i = 0; i < 8; i++)
      {
        if(size <= 0){
          break;
        }
        tab[i] = str[size - 1];
        size--;
        len_tab++;
      }

      char *rev = calloc(9, sizeof(char));

      //On réinverse la chaine "tampon"
      for (size_t i = 0; i < len_tab; i++)
      {
        rev[i] = tab[len_tab - i - 1];
      }

      //Conversion de la chaine de caractère (tampon inversé) vers un entier
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

//Fonction réalisée par Anton Dolard dans la stringlib.
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
  self->data = calloc(other->size+1, sizeof(uint32_t));
  self->size = other->size;
  self->capacity = other->capacity;
  for (size_t i = 0; i < other->size; i++)
  {
    self->data[i] = other->data[i];
  }
  
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
    printf("Self->data[%li] : %x\n",k , self->data[k]);
  }
}
static void bign_normalize ( struct bign * self ){
  // fonction qui normalise un nombre, c'est a dire qui enleve les zero a la fin d'un bign si il y en a .
  size_t i = self->size;
  while((self->data[i-1] == 0 && self->size > 1)){
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
      if(lhs->data[i] > rhs->data[i]){
        return 1;
      }else if(lhs->data[i] < rhs->data[i]){
        return -1;
      }
    }
      
  }
  if(lhs->size > rhs->size){
    return 1;
  }else if(lhs->size < rhs->size){
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
   struct bign *selftemp = calloc(1,sizeof(struct bign));
   bign_create_empty(selftemp);

  uint64_t base = 0x100000000;
  if(lhs->size > rhs->size){
    selftemp->size = lhs->size;
    selftemp->data = calloc(lhs->size+1, sizeof(uint32_t));

    uint32_t retenu = 0;
    for (size_t i = 0; i < rhs->size; i++)
    {
      uint64_t c = (uint64_t)lhs->data[i] + rhs->data[i] + retenu;
      //printf("c = %x\n",c);
      selftemp->data[i] = c % base;
    
      retenu = c / base;
      //printf("%x + %x + %x = %x    retenu = %i\n",lhs->data[i] , rhs->data[i],retenu ,self->data[i] ,retenu);
    
    }
    
    if(retenu>0){
      selftemp->size+=1;
      selftemp->data[selftemp->size-1]=retenu;   
    }
    
    
  }else if(lhs->size < rhs->size){
    selftemp->size = rhs->size;
    
    selftemp->data = calloc(rhs->size+1, sizeof(uint32_t));


    uint32_t retenu = 0;
    for (size_t i = 0; i < lhs->size; i++)
    {
      
      uint64_t c = (uint64_t)lhs->data[i] + rhs->data[i] + retenu;
      selftemp->data[i] = c % base;
     
      retenu = c / base;
       //printf("%x + %x + %x = %x    retenu = %i\n",lhs->data[i] , rhs->data[i],retenu ,self->data[i] ,retenu);
    }
    if(retenu>0){
      selftemp->size+=1;
      selftemp->data[selftemp->size-1]=retenu;   
    }
    
  }else {
    selftemp->size = lhs->size;
   
    selftemp->data = calloc(lhs->size+1, sizeof(uint32_t));
  

    uint32_t retenu = 0;
    for (size_t i = 0; i < lhs->size; i++)
    {
      uint64_t c = (uint64_t)lhs->data[i] + rhs->data[i] + retenu;
      selftemp->data[i] = c % base;
 
      retenu = c / base;
     //printf("%x + %x + %x = %x    retenu = %i\n",lhs->data[i] , rhs->data[i],retenu ,self->data[i] ,retenu);
      
    }
    if(retenu>0){
      selftemp->size+=1;
      selftemp->data[selftemp->size-1]=retenu;   
    }
  }
 bign_copy_from_other(self,selftemp);
}

//sub a une erreur mémoire dans le random add sub mais je n'ai pas reussi a savoir pourquoi 
void bign_sub(struct bign *self, const struct bign *lhs, const struct bign *rhs) {
  struct bign *selftemp = calloc(1,sizeof(struct bign));
  bign_create_empty(selftemp);
  uint64_t base = 0x100000000;
  size_t max = lhs->size;
  //bign_print(lhs);
  if (bign_cmp(lhs,rhs)==0 || bign_cmp(lhs,rhs)>0){
    selftemp->data = calloc(max+1, sizeof(uint32_t));
  
    selftemp->size = max;
    uint32_t retenu = 0;
    uint32_t retenutemp = 0;
    for (size_t i = 0; i < max; i++)
    {
      uint64_t d = (uint64_t)lhs->data[i] - rhs->data[i] + retenu;

      selftemp->data[i] = d % base;
    
      retenu = d / base;
      //printf("%x - %x - %x = %x    retenu = %i\n",lhs->data[i] , rhs->data[i],retenu ,selftemp->data[i] ,retenu);
      //printf("retenutemp = %i\n",retenutemp);
      if(retenu!=0){
        retenutemp=retenu;
      }
    }
    
    selftemp->data[max-1]+=retenutemp;   
    bign_normalize(selftemp);
  //bign_print(selftemp);
    bign_copy_from_other(self,selftemp);
  }
}

void bign_mul(struct bign *self, const struct bign *lhs, const struct bign *rhs) {
  
  uint64_t base = 0x100000000;
  struct bign *selftemp = calloc(1,sizeof(struct bign));
  bign_create_empty(selftemp);
  selftemp->size=lhs->size+rhs->size+1;
  selftemp->capacity=selftemp->size*2;
  if(selftemp->data == NULL){
    selftemp->data = calloc(selftemp->size+1, sizeof(uint32_t));
  }

  for(size_t i = 0;i<lhs->size;i++){
    uint32_t retenu = 0;
    
    for(size_t j = 0;j<rhs->size;j++){
      uint64_t t =(uint64_t)rhs->data[j]*lhs->data[i]+retenu+ selftemp->data[i+j];
      selftemp->data[i+j]=t % base;
      //printf("%x * %x = %x    retenu = %i\n",lhs->data[j] , rhs->data[i],selftemp->data[i+j], retenu);
      retenu = t / base;
      }
     
     if(retenu>0){
      selftemp->data[i+rhs->size]=retenu;
    }
  }
  bign_normalize(selftemp);
  bign_copy_from_other(self,selftemp);
  //bign_print(selftemp);

}


void bign_mul_karatsuba(struct bign *self, const struct bign *lhs, const struct bign *rhs) {
}

void bign_mul_short(struct bign *self, const struct bign *lhs, uint32_t rhs) {
  uint32_t basetemp = 0;
  basetemp-=1;
  uint64_t base = (uint64_t)basetemp+1;
  struct bign *selftemp = calloc(1,sizeof(struct bign));
  bign_create_empty(selftemp);
  selftemp->size=lhs->size+1;
  selftemp->capacity=selftemp->size*2;
  if(selftemp->data == NULL){
    selftemp->data = calloc(selftemp->size+1, sizeof(uint32_t));
  }

  for(size_t i = 0;i<lhs->size;i++){
    uint32_t retenu = 0;
    
    uint64_t t =(uint64_t)rhs*lhs->data[i]+retenu+ selftemp->data[i];
    selftemp->data[i]=t % base;
    //printf("%x * %x = %x    retenu = %i\n",lhs->data[j] , rhs->data[i],selftemp->data[i+j], retenu);
    retenu = t / base;
      
     
    if(retenu>0){
      selftemp->data[i+1]=retenu;
    }
  }
  bign_normalize(selftemp);
  bign_copy_from_other(self,selftemp);
  //bign_print(selftemp);

}

// https://janmr.com/blog/2012/11/basic-multiple-precision-short-division/
void bign_div_short(struct bign *quo, uint32_t *rem, const struct bign *lhs, uint32_t rhs) {
  quo->data = calloc(quo->capacity, sizeof(uint32_t));
  uint32_t r = 0;
  uint64_t base = 0x100000000;
  for (size_t i = 0; i < lhs->size; i++)
  {
    uint64_t c = (uint64_t)r * base + lhs->data[i];
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
  uint64_t base = 0x100000000;
  uint32_t d =2;
  bign_mul_short(rhs,rhs,d);
  bign_mul_short(lhs,lhs,d);
  size_t k =lhs->size-rhs->size;
  for(size_t i = k;i>=0;--i){


  }*/
}


void bign_exp(struct bign *self, const struct bign *lhs, const struct bign *rhs) {
  /*uint32_t *rem;
  if(rhs->data == 1){
    return lhs;
  }else {
    if(rhs->data % 2 == 0){
      return pow3(bign_mul(lhs,lhs,lhs), bign_div_short(rhs,2));
    }else {
      return bign_mul(lhs,lhs,pow3(bign_mul(lhs,lhs,lhs), bign_div_short(rhs,rem,bign_sub(rhs,rhs,1),2),2));
    }
  }*/
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
   if(self->n.data != NULL){
    bigz_destroy(self);
  }
  self->n.data = calloc(other->n.size, sizeof(uint32_t));
  for (size_t i = 0; i < other->n.size; i++)
  {
    self->n.data[i] = other->n.data[i];
  }
  self->n.size = other->n.size;
  self->positive=other->positive;
}

void bigz_move_from_other(struct bigz *self, struct bigz *other) {
  if(self->n.data != NULL){
    bigz_destroy(self);
  }
   
  self->n.capacity = other->n.capacity;
  self->n.size = other->n.size;
  self->positive=other->positive;
  
  self->n.data = other->n.data;

  other->n.data = NULL;
  other->n.capacity = 0;
  other->n.size = 0;
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
  if(lhs->n.data[lhs->n.size-1] == 0 && rhs->n.data[rhs->n.size-1] == 0){
    return 0;
  }
  if(lhs->positive == true && rhs -> positive == false){  
    return 1;
  }else if(rhs->positive == true && lhs -> positive == false){
    return -1;
  }

  return bign_cmp(&lhs->n,&rhs->n);

}


int bigz_cmp_zero(const struct bigz *self) {
  return self->n.data[0] == 0 ? 0 : self->positive == true ? 1 : -1;
}


//bigz add marche théoriquement mais il ne passe pas les test car notre big z create from string ne marche pas pour les nombres décimaux
void bigz_add(struct bigz *self, const struct bigz *lhs, const struct bigz *rhs) {
  bigz_create_empty(self);
  if(lhs->positive == true && rhs->positive == true){
    bign_add(&self->n, &lhs->n,&rhs->n);
   self->positive = true;
  }else if(lhs->positive == false && rhs->positive == true){
    //x-y=-(y-x) 
    if(bign_cmp(&rhs->n,&lhs->n)>0){
        bign_sub(&self->n, &rhs->n,&lhs->n);
        self->positive = true;
      }else{
      bign_sub(&self->n, &lhs->n,&rhs->n);
      self->positive = false;
      }
  }else if(lhs->positive == true && rhs->positive == false){
    //x-y=-(y-x) 
    if(bign_cmp(&lhs->n,&rhs->n)>0){
        bign_sub(&self->n, &lhs->n,&rhs->n);
        self->positive = true;
      }else{
        bign_sub(&self->n, &rhs->n,&lhs->n);
        self->positive = false;
      }
  }else if(lhs->positive == false && rhs->positive == false){
    bign_add(&self->n, &lhs->n,&rhs->n);
    self->positive = false;
  }
}

//bigz sub marche théoriquement mais il ne passe pas les test car notre big z create from string ne marche pas pour les nombres décimaux
void bigz_sub(struct bigz *self, const struct bigz *lhs, const struct bigz *rhs) {
  bigz_create_empty(self);
   if(lhs->positive == true && rhs->positive == true){
    //x-y=-(y-x) 
      if(bign_cmp(&lhs->n,&rhs->n)>0){
        bign_sub(&self->n, &lhs->n,&rhs->n);
        self->positive = true;
      }else{
        bign_sub(&self->n, &rhs->n,&lhs->n);
        self->positive = false;
      }
  }else if(lhs->positive == false && rhs->positive == true){
    bign_add(&self->n, &rhs->n,&lhs->n);
    self->positive = false;
  }else if(lhs->positive == true && rhs->positive == false){
    bign_add(&self->n, &lhs->n,&rhs->n);
  }else if(lhs->positive == false && rhs->positive == false){
    //x-y=-(y-x) 
    if(bign_cmp(&rhs->n,&lhs->n)>0){
        bign_sub(&self->n, &rhs->n,&lhs->n);
        self->positive = true;
      }else{
        bign_sub(&self->n, &lhs->n,&rhs->n);
        self->positive = false;
      }
  }
}

//bigz mul marche théoriquement mais il ne passe pas les test car notre big z create from string ne marche pas pour les nombres décimaux
void bigz_mul(struct bigz *self, const struct bigz *lhs, const struct bigz *rhs) {
  bigz_create_empty(self);
   if(lhs->positive == true && rhs->positive == true){
    bign_mul(&self->n, &lhs->n,&rhs->n);
   self->positive = true;
  }else if(lhs->positive == false && rhs->positive == true){
    bign_mul(&self->n, &lhs->n,&rhs->n);
    self->positive = false;
  }else if(lhs->positive == true && rhs->positive == false){
    bign_mul(&self->n, &lhs->n,&rhs->n);
    self->positive = false;
  }else if(lhs->positive == false && rhs->positive == false){
    bign_mul(&self->n, &lhs->n,&rhs->n);
    self->positive = true;
  }
}

void bigz_div(struct bigz *quo, struct bigz *rem, const struct bigz *lhs, const struct bigz *rhs) {
}

void bigz_gcd(struct bigz *self, const struct bigz *lhs, const struct bigz *rhs) {
}