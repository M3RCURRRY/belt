#include "belt_utils.h"

/*
 Для каждого режима (их всего представлено три впроекте :
 sswap - режим простой замены блоков
 cblock - режим объединения блоков
 gamma - режим гаммирования с обратной связью)
 объявлены функции, описание которых находится в belt_sswap.c
 belt_cblock.c и belt_gamma.c соответственно.

 Функции с суффиксом init инициализируют алгоритм шифрования в
 заданном режиме (задают начальное состояние, инициализируют
 структуру этого состояния).

 Функции с суффиксом EncBlock/DecBlock - шифруют/дешифруют один блок.
 Используются в функциях Encrypt/Decrypt, которые работают с цельным
 сообщением.

 Функции с суффиксами Encrypt/Decrypt имеют тип int с целью возможности
 возврата кода ошибки, которая могла произошла в ходе работы алгоритма,
 либо при некорректном поданом сообщении на входе. В случае корректного
 окончания работы функции возвращается 0.
*/

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
