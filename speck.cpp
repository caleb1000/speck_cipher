#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include <vector>
#define ROR(x, r) ((x >> r) | (x << (64 - r)))
#define ROL(x, r) ((x << r) | (x >> (64 - r)))
#define R(x, y, k) (x = ROR(x, 8), x += y, x ^= k, y = ROL(y, 3), y ^= x)
#define UR(x, y, k) (y ^= x, y = ROR(y, 3), x ^=k, x -= y, x = ROL(x,8))
#define ROUNDS 32

//source code based on code shown in wiki
//https://en.wikipedia.org/wiki/Speck_(cipher)

struct encrypted_data
{
   uint64_t key[2], data[2];
};


encrypted_data encrypt(uint64_t const pt[2],
             uint64_t const K[2])
{
   uint64_t ct[2] = {0,0};
   uint64_t y = pt[0], x = pt[1], b = K[0], a = K[1];

   R(x, y, b);//rotate inital values
   for (int i = 0; i < ROUNDS - 1; i++) {
      R(a, b, i);//rotate key
      R(x, y, b);//rotate new values
   }

   encrypted_data result;
   result.key[0] = b;
   result.key[1] = a;
   result.data[0] = y;
   result.data[1] = x;
   return result;
}

std::vector<uint64_t> decrypt(uint64_t ct[2],
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
   std::vector<uint64_t> result;
   result.push_back(ct[0]);
   result.push_back(ct[1]);
   //printf("Original Key = 0x%" PRIx64 ,b);
   //printf("%" PRIx64"\n",a);
   return result;
}


int main (){

    uint64_t pt[2] = {0xBEEFBEEFBEEFBEEF,0xDEADBEEFDEADBEEF};
    uint64_t ct[2] = {0,0};
    uint64_t k[2] = {0x1234567812345678,0x1234567812345678};

    encrypted_data test = encrypt(pt,k);
    printf("Encrypted data = 0x%" PRIx64,test.data[0]);
    printf("%" PRIx64 "\n",test.data[1]);
    printf("Encrypted Key = 0x%" PRIx64,test.key[0]);
    printf("%" PRIx64 "\n\n",test.key[1]);

    std::vector<uint64_t> decrypted_data = decrypt(ct,test.data,test.key);
    printf("Decrypted data = 0x");
    for(int x = 0; x<decrypted_data.size(); x++){
        printf("%" PRIx64 "",decrypted_data[x]);
    }

return 0;
}
