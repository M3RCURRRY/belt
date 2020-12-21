#include "belt_def.h"

typedef struct { ak_uint32 key[8]; } sswap_key;

void sswap_init(void* state, const uchar key[], size_t len)
{
	sswap_key* st = (sswap_key*)state;
	assert(state != 0);

	assert(st->key == 0 || len != 0);
	assert(len == 16 || len == 24 || len == 32);
	assert(key == 0 || len != 0);

	if (len == 16) {
		st->key[4] = st->key[0];
		st->key[5] = st->key[1];
		st->key[6] = st->key[2];
		st->key[7] = st->key[3];
	}
	else if (len == 24) {
		st->key[6] = st->key[0] ^ st->key[1] ^ st->key[2];
		st->key[7] = st->key[3] ^ st->key[4] ^ st->key[5];
	}
}

void sswap_EncBlock(void* buf, size_t count, void* state) {
	sswap_key* st = (sswap_key*)state;
	assert(count >= 16);
	assert((const uchar*)buf + count <= (const uchar*)state);

	while (count >= 16) {

		ak_uint32* cr_t = (ak_uint32*)buf;

		assert(buf != 0 && st->key != 0);
		assert((const uchar*)buf + 16 <= (const uchar*)st->key);

		cr_t[0] = cr_t[0] << 24 | (cr_t[0] & 0xFF00) << 8 | (cr_t[0] >> 8 & 0xFF00) | cr_t[0] >> 24;
		cr_t[1] = cr_t[1] << 24 | (cr_t[1] & 0xFF00) << 8 | (cr_t[1] >> 8 & 0xFF00) | cr_t[1] >> 24;
		cr_t[2] = cr_t[2] << 24 | (cr_t[2] & 0xFF00) << 8 | (cr_t[2] >> 8 & 0xFF00) | cr_t[2] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;

		E((cr_t + 0), (cr_t + 1), (cr_t + 2), (cr_t + 3), st->key);

		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;

		buf = (uchar*)buf + 16;
		count -= 16;
	}

	if (count) {
		assert((uchar*)buf - 16 != 0 || count == 0);
		assert(buf != 0 || count == 0);

		assert((const uchar*)buf - 16 + count <= (const uchar*)buf);
		assert((const uchar*)buf - 16 >= (const uchar*)buf + count);

		for (; count >= BLOCKBYTEW; count -= BLOCKBYTEW) {
			SWAPPER((*(ak_uint32*)((uchar*)buf - 16)), *(ak_uint32*)buf);
			INCRW((uchar*)buf - 16);
			INCRW(buf);
		}

		while (count--) {
			SWAPPER((*(uchar*)((uchar*)buf - 16)), * (uchar*)buf);
			INCRUCH((uchar*)buf - 16);
			INCRUCH(buf);
		}

		ak_uint32* cr_t = (ak_uint32*)((uchar*)buf - 16);

		assert(buf != 0 && st->key != 0);
		assert((const uchar*)buf + 16 <= (const uchar*)st->key);
		assert((const uchar*)buf >= (const uchar*)st->key + 32);

		cr_t[0] = cr_t[0] << 24 | (cr_t[0] & 0xFF00) << 8 | (cr_t[0] >> 8 & 0xFF00) | cr_t[0] >> 24;
		cr_t[1] = cr_t[1] << 24 | (cr_t[1] & 0xFF00) << 8 | (cr_t[1] >> 8 & 0xFF00) | cr_t[1] >> 24;
		cr_t[2] = cr_t[2] << 24 | (cr_t[2] & 0xFF00) << 8 | (cr_t[2] >> 8 & 0xFF00) | cr_t[2] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;

		E((cr_t + 0), (cr_t + 1), (cr_t + 2), (cr_t + 3), st->key);

		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;

		buf = (uchar*)buf + 16;
		count -= 16;
	}
}

void sswap_DecBlock(void* buf, size_t count, void* state)
{
	sswap_key* st = (sswap_key*)state;
	assert(count >= 16);
	assert((size_t)sizeof(sswap_key) == 0 || count == 0);
	assert((const uchar*)buf + count <= (const uchar*)state);
	assert((const uchar*)buf >= (const uchar*)state + (size_t)sizeof(sswap_key));

	while (count >= 16)
	{
		ak_uint32* cr_t = (ak_uint32*)buf;

		assert(buf != 0 && st->key != 0);
		assert((const uchar*)buf + 16 <= (const uchar*)st->key);
		assert((const uchar*)buf >= (const uchar*)st->key + 32);

		cr_t[0] = cr_t[0] << 24 | (cr_t[0] & 0xFF00) << 8 | (cr_t[0] >> 8 & 0xFF00) | cr_t[0] >> 24;
		cr_t[1] = cr_t[1] << 24 | (cr_t[1] & 0xFF00) << 8 | (cr_t[1] >> 8 & 0xFF00) | cr_t[1] >> 24;
		cr_t[2] = cr_t[2] << 24 | (cr_t[2] & 0xFF00) << 8 | (cr_t[2] >> 8 & 0xFF00) | cr_t[2] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;

		D((cr_t + 0), (cr_t + 1), (cr_t + 2), (cr_t + 3), st->key);

		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		buf = (uchar*)buf + 16;
		count -= 16;
	}

	if (count)
	{
		assert((uchar*)buf - 16 != 0 || count == 0);
		assert(buf != 0 || count == 0);

		assert((const uchar*)buf - 16 + count <= (const uchar*)buf);
		assert((const uchar*)buf - 16 >= (const uchar*)buf + count);

		for (; count >= BLOCKBYTEW; count -= BLOCKBYTEW) {
			SWAPPER((*(ak_uint32*)((uchar*)buf - 16)), *(ak_uint32*)buf);
			INCRW((uchar*)buf - 16);
			INCRW(buf);
		}

		while (count--) {
			SWAPPER((*(uchar*)((uchar*)buf - 16)), *(uchar*)buf);
			INCRUCH((uchar*)buf - 16);
			INCRUCH(buf);
		}

		ak_uint32* cr_t = (ak_uint32*)buf - 16;

		assert(buf != 0 && st->key != 0);
		assert((const uchar*)buf + 16 <= (const uchar*)st->key);
		assert((const uchar*)buf >= (const uchar*)st->key + 32);

		cr_t[0] = cr_t[0] << 24 | (cr_t[0] & 0xFF00) << 8 | (cr_t[0] >> 8 & 0xFF00) | cr_t[0] >> 24;
		cr_t[1] = cr_t[1] << 24 | (cr_t[1] & 0xFF00) << 8 | (cr_t[1] >> 8 & 0xFF00) | cr_t[1] >> 24;
		cr_t[2] = cr_t[2] << 24 | (cr_t[2] & 0xFF00) << 8 | (cr_t[2] >> 8 & 0xFF00) | cr_t[2] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;

		D((cr_t + 0), (cr_t + 1), (cr_t + 2), (cr_t + 3), st->key);

		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
	}
}

int sswap_Encrypt(void* dest, const void* src, size_t count, const uchar key[], size_t len) {
	void* state;

	assert(src != 0 || count == 0);
	assert(key != 0 || len == 0);
	assert(dest != 0 || count == 0);
	if (count < 16 || len != 16 && len != 24 && len != 32)
		//return ak_error_message(ak_error_undefined_value);
		return (-5);

	size_t size = (((sizeof(sswap_key)) + sizeof(size_t) + 1024 - 1) / 1024) * 1024;
	size_t* ptr = (size_t*)cross_malloc(size);
	if (ptr == 0) return 0;
	*ptr = sizeof(sswap_key);
	memset(((void*)((size_t*)ptr + 1)), 0, sizeof(sswap_key));
	state = ((void*)((size_t*)ptr + 1));
	if (state == 0)
		//return ak_error_message(ak_error_out_of_memory);
		return (-1);

	sswap_init(state, key, len); 
	memmove(dest, src, count);
	sswap_EncBlock(dest, count, state);


	if (state) {
		static uchar wipe = 0;
		size_t w = wipe;
		size_t count = (((((*((size_t*)state - 1))) + sizeof(size_t) + 1024 - 1) / 1024) * 1024);
		volatile uchar* wipe_p = (uchar*)(((size_t*)state - 1));

		while (count--) {
			*(wipe_p++) = (uchar)w, w += 17 + ((size_t)wipe_p & 15);
		}

		wipe_p = memchr((((size_t*)state - 1)), (uchar)w, count);
		if (wipe_p) {
			wipe += (63 + (size_t)wipe_p);
		}
		wipe = (uchar)w;
		FREE((((size_t*)state - 1)));
	}

	//return ak_error_message(ak_error_ok);
	return 0;
}

int sswap_Decrypt(void* dest, const void* src, size_t count, const uchar key[], size_t len)
{
	void* state;

	assert(src != 0 || count == 0);
	assert(key != 0 || len == 0);
	assert(dest != 0 || count == 0);
	if (count < 16 || len != 16 && len != 24 && len != 32)
		//return ak_error_message(ak_error_undefined_value);
		return (-5);

	size_t size = (((sizeof(sswap_key)) + sizeof(size_t) + 1024 - 1) / 1024) * 1024;
	size_t* ptr = (size_t*)cross_malloc(size);
	if (ptr == 0) return 0;
	*ptr = sizeof(sswap_key);
	memset(((void*)((size_t*)ptr + 1)), 0, sizeof(sswap_key));
	state = ((void*)((size_t*)ptr + 1));
	if (state == 0)
		//return ak_error_message(ak_error_out_of_memory);
		return (-1);


	sswap_init(state, key, len);
	memmove(dest, src, count);
	sswap_DecBlock(dest, count, state);

	assert(state == 0);
	if (state) {
		static uchar wipe = 0;
		size_t w = wipe;
		size_t count = (((((*((size_t*)state - 1))) + sizeof(size_t) + 1024 - 1) / 1024) * 1024);
		volatile uchar* wipe_p = (uchar*)(((size_t*)state - 1));

		while (count--) {
			*(wipe_p++) = (uchar)w, w += 17 + ((size_t)wipe_p & 15);
		}

		wipe_p = memchr((((size_t*)state - 1)), (uchar)w, count);
		if (wipe_p) {
			wipe += (63 + (size_t)wipe_p);
		}
		wipe = (uchar)w;
		FREE((((size_t*)state - 1)));
	}
	//return ak_error_message(ak_error_ok);
	return 0;
}