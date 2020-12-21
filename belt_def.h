#include "belt_utils.h"

void cblock_init(void*, const uchar[], size_t, const uchar[16]);
void cblock_EncBlock(void*, size_t, void*);
void cblock_DecBlock(void*, size_t, void*);
int cblock_Encrypt(void*, const void*, size_t, const uchar[], size_t, const uchar[16]);
int cblock_Decrypt(void*, const void*, size_t, const uchar[], size_t, const uchar[16]);

void sswap_init(void*, const uchar[], size_t);
void sswap_EncBlock(void*, size_t, void*);
void sswap_DecBlock(void*, size_t, void*);
int sswap_Encrypt(void*, const void*, size_t, const uchar[], size_t);
int sswap_Decrypt(void*, const void*, size_t, const uchar[], size_t);

void gamma_init(void*, const uchar[], size_t, const uchar[16]);
void gamma_EncBlock(void*, size_t, void*);
void gamma_DecBlock(void*, size_t, void*);
int gamma_Encrypt(void*, const void*, size_t, const uchar[], size_t, const uchar[16]);
int gamma_Decrypt(void*, const void*, size_t, const uchar[], size_t, const uchar[16]);

//cblock_Encrypt