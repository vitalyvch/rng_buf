/** The API for rng_buf library
 *
 * This code was published on 25 November 2007 by Lucius Annaeus Seneca under
 * GFDL License and some time later it was re-licensed under CC BY-SA 3.0
 * conditions.
 *
 * Since 21 March 2008 the contact with Lucius Annaeus Seneca was lost, so
 * there is no ways to ask about re-licensing under other conditions.
 *
 * @author Lucius Annaeus Seneca
 * @author Vitalii Chernookyi
 *               
 * @copyright GFDL            
 * @copyright Creative Commons Attribution-ShareAlike 3.0 Unported License (CC BY-SA 3.0)
 */
#ifndef RNG_BUF_H
#define RNG_BUF_H


#ifndef __cplusplus
#include <stdbool.h>
#else
#include <cstdbool>

extern "C" {
#endif


struct rng_buf {
	unsigned char *base;
	uint64_t size;
	uint64_t write_offset_bytes;
	uint64_t read_offset_bytes;

	char name[1];
};

typedef struct rng_buf rng_buf_t;

rng_buf_t *rng_buf_create(const char *name, size_t min_size, _Bool use_file_in_tmp);
void rng_buf_destroy(rng_buf_t* *rb);

unsigned char* rng_buf_write_address (struct rng_buf *rb);

void rng_buf_write_advance (struct rng_buf *rb, uint64_t size);

unsigned char* rng_buf_read_address (struct rng_buf *rb);

void rng_buf_read_advance (struct rng_buf *rb, uint64_t size);

uint64_t rng_buf_len (struct rng_buf *rb);

uint64_t rng_buf_free_bytes (struct rng_buf *rb);

#ifdef __cplusplus
}
#endif

#endif /* RNG_BUF_H */
