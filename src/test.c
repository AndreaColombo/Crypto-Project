#include <stdio.h>
#include <unistd.h>
#include "rijndael-alg-fst.h"

static const u8 key[16] = {
//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09,0xcf, 0x4f, 0x3c
};

static const u8 pt[16] = {
//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
   'A','A','A','A','A','A','A','A',
   'A','A','A','A','A','A','A','A'
};

static const u8 ct_expected[16] = {
  0x6c,0x2a,0x3f,0xd9,0x3a,0xa7,0xb4,0x17,0xc5,0x43,0x8d,0x26,0xa4,0x61,0x95,0x19
};

u8 ct[16];

int main(int argc, char* argv[]){

   u32 rk[128];
   printf("START\n");
   int nr = rijndaelKeySetupEnc(rk, key, 128);
// rijndaelKeySetupDec(rk, key, 128);
   rijndaelEncrypt(rk, nr, pt, ct);
   int i;
   int eq = 1;
   for (i=0; i<16 && eq; i++){
      if(ct[i]!=ct_expected[i])
         eq=0;
   }
   eq ? printf("True\n") : printf("False\n");
   
   
   
   
  printf("CT:\t\t");
   for(i=0; i<16; i++){
      printf("%x", ct[i]);
   }
   printf("\n");
   
   printf("EX:\t\t");
   for(i=0; i<16; i++){
      printf("%x", ct_expected[i]);
   }
   printf("\n");
   char *name[3];
   name[0] = "echo";
   name[1] = "KODIO KAMADONNA";
   name[2] = NULL;
   execl("figlet KODIO KAMADONNA|/bin/sh");   
   return 0;
   
}
