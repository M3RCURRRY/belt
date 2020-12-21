#include "belt_def.h"

typedef struct {
	ak_uint32 key[8];
	uchar block[16];
	size_t reserved;
} gamma_key; 

void gamma_init(void* state, const uchar key[], size_t len, const uchar oct1[16])
{
	gamma_key* st = (gamma_key*)state;
	assert((const uchar*)oct1 >= (const uchar*)state + (size_t)sizeof(gamma_key));

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
	COPYBLOCK(st->block, oct1);
	st->reserved = 0;
}

void gamma_EncBlock(void* buf, size_t count, void* state)
{
	gamma_key* st = (gamma_key*)state;
	assert((const uchar*)buf + count <= (const uchar*)state);

	if (st->reserved)
	{
		if (st->reserved >= count)
		{
			PTRXOR(st->block + 16 - st->reserved, buf, count);
			memcpy(buf, st->block + 16 - st->reserved, count);
			st->reserved -= count;
			return;
		}
		PTRXOR(st->block + 16 - st->reserved, buf, st->reserved);
		memcpy(buf, st->block + 16 - st->reserved, st->reserved);
		count -= st->reserved;
		buf = (uchar*)buf + st->reserved;
		st->reserved = 0;
	}

	while (count >= 16)
	{
		ak_uint32* cr_t = (ak_uint32*)(st->block);

		assert(buf != 0 && st->key != 0);
		assert((const uchar*)buf + 16 <= (const uchar*)st->key);
		//assert((const uchar*)buf >= (const uchar*)st->key + 32);

		cr_t[0] = cr_t[0] << 24 | (cr_t[0] & 0xFF00) << 8 | (cr_t[0] >> 8 & 0xFF00) | cr_t[0] >> 24;
		cr_t[1] = cr_t[1] << 24 | (cr_t[1] & 0xFF00) << 8 | (cr_t[1] >> 8 & 0xFF00) | cr_t[1] >> 24;
		cr_t[2] = cr_t[2] << 24 | (cr_t[2] & 0xFF00) << 8 | (cr_t[2] >> 8 & 0xFF00) | cr_t[2] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;

		E((cr_t + 0), (cr_t + 1), (cr_t + 2), (cr_t + 3), st->key);

		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;

		SELFXOR(st->block, buf);
		COPYBLOCK(buf, st->block);
		buf = (uchar*)buf + 16;
		count -= 16;
	}

	if (count)
	{
		ak_uint32* cr_t = (ak_uint32*)(st->block);

		assert(buf != 0 && st->key != 0);
		assert((const uchar*)buf + 16 <= (const uchar*)st->key);
		//assert((const uchar*)buf >= (const uchar*)st->key + 32);

		cr_t[0] = cr_t[0] << 24 | (cr_t[0] & 0xFF00) << 8 | (cr_t[0] >> 8 & 0xFF00) | cr_t[0] >> 24;
		cr_t[1] = cr_t[1] << 24 | (cr_t[1] & 0xFF00) << 8 | (cr_t[1] >> 8 & 0xFF00) | cr_t[1] >> 24;
		cr_t[2] = cr_t[2] << 24 | (cr_t[2] & 0xFF00) << 8 | (cr_t[2] >> 8 & 0xFF00) | cr_t[2] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;

		E((cr_t + 0), (cr_t + 1), (cr_t + 2), (cr_t + 3), st->key);

		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;
		cr_t[3] = cr_t[3] << 24 | (cr_t[3] & 0xFF00) << 8 | (cr_t[3] >> 8 & 0xFF00) | cr_t[3] >> 24;

		PTRXOR(st->block, buf, count);
		memcpy(buf, st->block, count);
		st->reserved = 16 - count;
	}
}

void gamma_DecBlock(void* buf, size_t count, void* state)
{
	gamma_key* st = (gamma_key*)state;

	assert((size_t)sizeof(gamma_key) == 0 || count == 0);
	assert((const uchar*)buf + count <= (const uchar*)state);
	assert((const uchar*)buf >= (const uchar*)state + (size_t)sizeof(gamma_key));

	if (st->reserved)
	{
		if (st->reserved >= count)
		{
			PTRXOR(buf, st->block + 16 - st->reserved, count);
			PTRXOR(st->block + 16 - st->reserved, buf, count);
			st->reserved -= count;
			return;
		}
		PTRXOR(buf, st->block + 16 - st->reserved, st->reserved);
		PTRXOR(st->block + 16 - st->reserved, buf, st->reserved);
		count -= st->reserved;
		buf = (uchar*)buf + st->reserved;
		st->reserved = 0;
	}

	while (count >= 16)
	{
		ak_uint32* cr_t = (ak_uint32*)(st->block);

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

		SELFXOR(buf, st->block);
		SELFXOR(st->block, buf);
		buf = (uchar*)buf + 16;
		count -= 16;
	}

	if (count)
	{
		ak_uint32* cr_t = (ak_uint32*)(st->block);

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

		PTRXOR(buf, st->block, count);
		PTRXOR(st->block, buf, count);
		st->reserved = 16 - count;
	}
}

int gamma_Encrypt(void* dest, const void* src, size_t count, const uchar key[], size_t len, const uchar oct[16])
{
	void* state;

	assert(src != 0 || count == 0);
	assert(key != 0 || len == 0);
	assert(dest != 0 || count == 0);
	assert(oct != 0);
	if (count < 16 || len != 16 && len != 24 && len != 32)
		//return ak_error_message(ak_error_undefined_value);
		return (-5);

	size_t size = (((sizeof(gamma_key)) + sizeof(size_t) + 1024 - 1) / 1024) * 1024;
	size_t* ptr = (size_t*)cross_malloc(size);
	if (ptr == 0) return 0;
	*ptr = sizeof(gamma_key);
	memset(((void*)((size_t*)ptr + 1)), 0, sizeof(gamma_key));
	state = ((void*)((size_t*)ptr + 1));
	if (state == 0)
		//return ak_error_message(ak_error_out_of_memory);
		return (-1);

	gamma_init(state, key, len, oct);
	memmove(dest, src, count);
	gamma_EncBlock(dest, count, state);
	
	//assert(state == 0);
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

int gamma_Decrypt(void* dest, const void* src, size_t count,	const uchar key[], size_t len, const uchar oct[16])
{
	void* state;

	assert(src != 0 || count == 0);
	assert(key != 0 || len == 0);
	assert(dest != 0 || count == 0);
	assert(oct != 0);

	if (count < 16 || len != 16 && len != 24 && len != 32)
		//return ak_error_message(ak_error_undefined_value);
		return (-5);

	size_t size = (((sizeof(gamma_key)) + sizeof(size_t) + 1024 - 1) / 1024) * 1024;
	size_t* ptr = (size_t*)cross_malloc(size);
	if (ptr == 0) return 0;
	*ptr = sizeof(gamma_key);
	memset(((void*)((size_t*)ptr + 1)), 0, sizeof(gamma_key));
	state = ((void*)((size_t*)ptr + 1));
	if (state == 0)
		//return ak_error_message(ak_error_out_of_memory);
		return (-1);

	gamma_init(state, key, len, oct);
	memmove(dest, src, count);
	gamma_DecBlock(dest, count, state);

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