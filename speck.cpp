#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include <vector>
#include <string>
#define ROR(x, r) ((x >> r) | (x << (64 - r)))
#define ROL(x, r) ((x << r) | (x >> (64 - r)))
#define R(x, y, k) (x = ROR(x, 8), x += y, x ^= k, y = ROL(y, 3), y ^= x)
#define UR(x, y, k) (y ^= x, y = ROR(y, 3), x ^=k, x -= y, x = ROL(x,8))
#define ROUNDS 32

//source code based on code shown in wiki
//https://en.wikipedia.org/wiki/Speck_(cipher)

struct crypted_data
{
   uint64_t key[2], data[2];
};


crypted_data encrypt(uint64_t const pt[2],
             uint64_t const K[2])
{
   uint64_t ct[2] = {0,0};
   uint64_t y = pt[0], x = pt[1], b = K[0], a = K[1];

   R(x, y, b);//rotate inital values
   for (int i = 0; i < ROUNDS - 1; i++) {
      R(a, b, i);//rotate key
      R(x, y, b);//rotate new values
   }

   crypted_data result;
   result.key[0] = b;
   result.key[1] = a;
   result.data[0] = y;
   result.data[1] = x;
   return result;
}

crypted_data decrypt(uint64_t const pt[2],
             uint64_t const K[2])
{
   uint64_t y = pt[0], x = pt[1], b = K[0], a = K[1];

   for (int i = ROUNDS-2; i >= 0; i--) {
      UR(x, y, b);
      UR(a, b, i);
   }
   UR(x, y, b);

   crypted_data result;
   result.key[0] = b;
   result.key[1] = a;
   result.data[0] = y;
   result.data[1] = x;
   return result;
}

std::vector<uint64_t> format_c_str(char* str){

    std::string message = str;
    std::vector<uint64_t> raw_data(message.begin(), message.end());//convert message into uint64_t elements
    if(raw_data.size()%16 != 0){
         int size = 16-(raw_data.size()%16);
         for (int x = 0; x < size; x++){
             raw_data.push_back(0);//append zeros if data doesn't fit 128bit blocks
         }
    }
    int shift = 7;//mulitply by 8 to get the number of bit shifts left
    std::vector<uint64_t> formated_data;//a vector to hold data from stdin that is put in a format to be encrypted
    uint64_t temp = 0;
    //assume data is big endian
    printf("Original data ascii= %s\n",str);
    printf("Original data Hex= 0x");
    for(int x = 0; x<raw_data.size(); x++){
        temp += raw_data.at(x) << shift*8;
        if(shift == 0){
            shift = 7;
            formated_data.push_back(temp);
            printf("%" PRIx64,temp);
            temp = 0;
        }
        else{
            shift--;
        }
    }
    return formated_data;
}


int main (int argc, char* argv[]){

    if(argc != 2){
        printf("Wrong number of arguments, exiting code\n");
        return -1;
    }

    std::vector<uint64_t> formated_data = format_c_str(argv[1]);//format c string into 128bit blocks
    uint64_t k[2] = {0x1234567812345678,0x1234567812345678};//starting key block
    uint64_t pt[2];//input data block

    //Encrypt data
    std::vector<uint64_t> keys; //vector to hold rotating keys for each encrypted block
    std::vector<uint64_t> encrypt_data; //vector to hold encrypted data to match with key to decrypt
    for(int x = 0; x<formated_data.size()-1; x+=2){
          pt[0] = formated_data.at(x);
          pt[1] = formated_data.at(x+1);
          crypted_data temp = encrypt(pt,k);
          encrypt_data.push_back(temp.data[0]);
          encrypt_data.push_back(temp.data[1]);
          k[0] = temp.key[0];
          k[1] = temp.key[1];
          keys.push_back(k[0]);
          keys.push_back(k[1]);
    }
    printf("\nEncrypted data Hex= 0x");
    for(int x = 0; x<encrypt_data.size();x++){
        printf("%" PRIx64 ,encrypt_data.at(x));
    }


    //Decrypt data
    printf("\n");
    std::vector<uint64_t> decrypt_data;
    for(int x = 0; x<encrypt_data.size()-1; x+=2){
          k[0] = keys.at(x);
          k[1] = keys.at(x+1);
          pt[0] = encrypt_data.at(x);
          pt[1] = encrypt_data.at(x+1);
          crypted_data temp = decrypt(pt,k);
          decrypt_data.push_back(temp.data[0]);
          decrypt_data.push_back(temp.data[1]);
    }
    printf("Decrypted data Hex= 0x");
    for(int x = 0; x<decrypt_data.size();x++){
        printf("%" PRIx64 ,decrypt_data.at(x));
    }
    printf("\n");
    printf("Decrypted data Ascii= 0x");
    for(int x = 0; x<decrypt_data.size();x++){
        for(int shift = 56; shift >= 0; shift-=8){
            printf("%c",(char)(decrypt_data.at(x) >> shift));
        }
    }
    printf("\n");
    return 0;
}
