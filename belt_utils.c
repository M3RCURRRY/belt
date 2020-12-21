#include "belt_utils.h"

#define CPYBLCK(d, src)\
	((ak_uint32*)(d))[0] = ((const ak_uint32*)(src))[0],\
	((ak_uint32*)(d))[1] = ((const ak_uint32*)(src))[1],\
	((ak_uint32*)(d))[2] = ((const ak_uint32*)(src))[2],\
	((ak_uint32*)(d))[3] = ((const ak_uint32*)(src))[3],\
	((ak_uint32*)(d))[4] = ((const ak_uint32*)(src))[4],\
	((ak_uint32*)(d))[5] = ((const ak_uint32*)(src))[5],\
	((ak_uint32*)(d))[6] = ((const ak_uint32*)(src))[6],\
	((ak_uint32*)(d))[7] = ((const ak_uint32*)(src))[7]\

void* cross_malloc(size_t size) {
#ifdef OS_WIN
	return HeapAlloc(GetProcessHeap(), 0, size);
#else
	return malloc(size);
#endif
}

void FREE(void* buf) {
#ifdef OS_WIN
	HeapFree(GetProcessHeap(), 0, buf);
#else
	free(buf);
#endif
}

void INCRW(void* buf) {
	buf = (WORD_TYPE*)buf + 1;
}

void INCRUCH(void* buf) {
	buf = (uchar*)buf + 1;
}

void PTRXOR(void* dest, const void* src, size_t count) {
	for (; count >= BLOCKBYTEW; count -= BLOCKBYTEW)
	{
		*(ak_uint32*)dest ^= *(const ak_uint32*)src;
		src = (const ak_uint32*)src + 1;
		dest = (ak_uint32*)dest + 1;
	}
	while (count--)
	{
		*(uchar*)dest ^= *(const uchar*)src;
		src = (const uchar*)src + 1;
		dest = (uchar*)dest + 1;
	}
}

int moveW(register ak_uint32 w)
{
	w ^= w >> 1;
	w ^= w >> 2;
	w ^= w >> 4;
	w ^= w >> 8;
	w ^= w >> 16;
	return (int)(w & ((ak_uint32)1));
}

WORD WT(WORD wt) {
	return (wt >> 1 | moveW(wt & 0x63) << 7);
}

const uchar* getH() {
	return H;
}