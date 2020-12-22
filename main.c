#include <stdio.h>

#include "belt_def.h"

void generateSBox(uchar ins[256]) {
	size_t x;
	size_t i;
	ins[10] = 0;
	ins[11] = 0x8E;
	for (x = 12; x < 10 + 256; ++x)
	{
		WORD wt = ins[(x - 1) % 256];
		for (i = 0; i < 116; ++i)
			wt = WT(wt);
		ins[x % 256] = (uchar)wt;
	}
}

int main() {

	uchar buf[128], buf2[128], state[1024];

	generateSBox(state);

	//BELT : Шифрование в режиме простой замены
	printf("BELT : Simple swap encryption mode\n\n");
	memcpy(buf, getH(), 48);
	for (int i = 0; i < 48; ++i) {
		printf("%02x", ((uchar*)buf)[i]);
	}
	printf("\n\n");
	sswap_init(state, getH() + 128, 32);
	sswap_EncBlock(buf, 32, state);
	sswap_EncBlock(buf + 32, 48 - 32, state);
	sswap_Encrypt(buf2, getH(), 128, getH() + 128, 32);
	for (int i = 0; i < 48; ++i) {
		printf("%02x", ((uchar*)buf)[i]);
	}
	printf("\n\n\n------------------------------------\n\n\n");



	//BELT : Шифрование в режиме соединения блоков
	printf("BELT : Blocks connection encryption mode\n\n");
	memcpy(buf, getH(), 48);
	for (int i = 0; i < 48; ++i) {
		printf("%02x", ((uchar*)buf)[i]);
	}
	printf("\n\n");
	cblock_init(&state, getH() + 128, 32, getH() + 192);
	cblock_EncBlock(buf, 32, state);
	cblock_EncBlock(buf + 32, 48 - 32, state);
	cblock_Encrypt(buf2, getH(), 48, getH() + 128, 32, getH() + 192);
	for (int i = 0; i < 48; ++i) {
		printf("%02x", ((uchar*)buf)[i]);
	}
	printf("\n\n\n------------------------------------\n\n\n");



	//BELT : Шифрование в режиме гаммирования с обратной связью
	printf("BELT : Gamma-reversed-connection encryption mode\n\n");
	memcpy(buf, getH(), 48);
	for (int i = 0; i < 48; ++i) {
		printf("%02x", ((uchar*)buf)[i]);
	}
	printf("\n\n");
	gamma_init(state, getH() + 128, 32, getH() + 192);
	gamma_EncBlock(buf, 32, state);
	gamma_EncBlock(buf + 16, 3, state);
	gamma_EncBlock(buf + 16 + 3, 48 - 16 - 3, state);
	gamma_Encrypt(buf2, getH(), 48, getH() + 128, 32, getH() + 192);
	for (int i = 0; i < 48; ++i) {
		printf("%02x", ((uchar*)buf)[i]);
	}
	printf("\n\n\n------------------------------------\n\n\n");

	return 0;
}
