#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#define ROR(x, r) ((x >> r) | (x << (64 - r)))
#define ROL(x, r) ((x << r) | (x >> (64 - r)))
#define R(x, y, k) (x = ROR(x, 8), x += y, x ^= k, y = ROL(y, 3), y ^= x)
#define UR(x, y, k) (y ^= x, y = ROR(y, 3), x ^=k, x -= y, x = ROL(x,8))
#define ROUNDS 32

void encrypt(uint64_t ct[2],
             uint64_t const pt[2],
             uint64_t const K[2])
{
   uint64_t y = pt[0], x = pt[1], b = K[0], a = K[1];

   R(x, y, b);//rotate inital values
   for (int i = 0; i < ROUNDS - 1; i++) {
      R(a, b, i);//rotate key
      R(x, y, b);//rotate new values
   }

   ct[0] = y;
   ct[1] = x;
   printf("Encrypted ct[0]=0x%" PRIx64 "\n",ct[0]);
   printf("Encrypted ct[1]=0x%" PRIx64 "\n",ct[1]);
   printf("Final Key k[0]=0x%" PRIx64 "\n",b);
   printf("Final Key k[1]=0x%" PRIx64 "\n\n",a);
}

void decrypt(uint64_t ct[2],
             uint64_t const pt[2],
             uint64_t const K[2])
{
   uint64_t y = pt[0], x = pt[1], b = K[0], a = K[1];

   for (int i = ROUNDS-2; i >= 0; i--) {
      UR(x, y, b);
      UR(a, b, i);
   }
   UR(x, y, b);

   ct[0] = y;
   ct[1] = x;
   printf("Decrypted ct[0]=0x%" PRIx64 "\n",ct[0]);
   printf("Decrypted ct[1]=0x%" PRIx64 "\n",ct[1]);
   printf("Original Key k[0]=0x%" PRIx64 "\n",b);
   printf("Original Key k[1]=0x%" PRIx64 "\n\n",a);

}


int main (){

    uint64_t pt[2] = {0xBEEFBEEFBEEFBEEF,0xDEADBEEFDEADBEEF};
    uint64_t ct[2] = {0,0};
    uint64_t k[2] = {0x1234567812345678,0x1234567812345678};

    encrypt(ct,pt,k);

    pt[0]=0x729b46cdef16b988;//return value of speck
    pt[1]=0x35d93a10c51a854c;//return value of speck
    k[0]=0x35de98aa88def348;//return key of speck
    k[1]=0x18f8c9e684d9f9b;//return key of speck

    decrypt(ct,pt,k);


return 0;
}
