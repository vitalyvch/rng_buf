/** The implementation of rng_buf library
 *
 * @author Vitalii Chernookyi
 *                           
 * @copyright BSD
 */

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "rng_buf.h"


static const char rng_buf_default_name[] = "rng_buf";

uint64_t min_size2size(size_t min_size)
{
	const long pagesize = sysconf(_SC_PAGESIZE);

	uint64_t pages_qty = min_size / pagesize;

	if(min_size % pagesize)
		pages_qty ++;

	if(!pages_qty)
		pages_qty ++;

	return pages_qty * ((pagesize > 0) ? (unsigned long) pagesize : 4096UL);
}

rng_buf_t *rng_buf_create(const char *name, size_t min_size, _Bool use_file_in_tmp)
{
	int shm_fd = -1;
	int s_seg_counter = 0;
	int res, err_no;

	char seg_name[4096] = "";
	const uint64_t size = min_size2size(min_size);

	const char *tmp_dir = getenv("TMPDIR");

	if (NULL == tmp_dir)
		tmp_dir = P_tmpdir;

	if (NULL == name)
		name = rng_buf_default_name;

	if ('/' == name[0])
		name ++;

	if ('\0' == name[0])
		name = rng_buf_default_name;


	// open a new named shared memory segment
	while (1) {
		if (!use_file_in_tmp) {
			// This is the POSIX recommended "portable format".
			// Of course the "portable format" doesn't work on some systems...
			snprintf(seg_name, sizeof(seg_name), "/%s-%d-%d", name, getpid(), s_seg_counter);
			shm_fd = shm_open(seg_name, O_RDWR | O_CREAT | O_EXCL, 0600);
		} else {
			// Where the "portable format" doesn't work, we try building
			// a full filesystem pathname pointing into a suitable temporary directory.
			snprintf(seg_name, sizeof(seg_name), "%s/%s-%d-%d",
			               tmp_dir, name, getpid(), s_seg_counter);
			shm_fd = open(seg_name, O_RDWR | O_CREAT | O_EXCL, 0600);
		}

		if (shm_fd != -1)
			break;
	   
		// Something went wront, let't try basic WAs
		if (errno == EACCES && (!use_file_in_tmp)) {
			use_file_in_tmp = true;
			continue; // try again using a file in TMP dir.
		}

		s_seg_counter++;

		if (errno == EEXIST) // Named segment already exists (shouldn't happen).  Try again
			continue;

		goto return_null;
	}

	// We've got a new shared memory segment fd open.
	// Now set it's length to 2x what we really want and mmap it in.
	if (ftruncate(shm_fd, (off_t)2 * size) == -1)
		goto return_null;

	unsigned char * first_copy =
		mmap(0, 2 * size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, (off_t)0);

	if (first_copy == MAP_FAILED)
		goto return_null;

	// map the first half into the now available hole where the
	// second half used to be.
	unsigned char * second_copy = mmap((char*)first_copy + size,
	                         size,
	                         PROT_READ | PROT_WRITE,
	                         MAP_SHARED | MAP_FIXED,
	                         shm_fd,
	                         (off_t)0);

	if (second_copy == MAP_FAILED)
		goto return_null;


	close(shm_fd); // fd no longer needed.  The mapping is retained.
	shm_fd = -1;

	if (!use_file_in_tmp)
		res = shm_unlink(seg_name);
	else
		res = unlink(seg_name);
	(void) res;

	rng_buf_t *rb = malloc (sizeof(rb[0]) + strlen(seg_name));
	if (NULL == rb) {
		err_no = errno;
		munmap(first_copy, 2 * size);
		errno = err_no;
		goto return_null;
	}

	// Now remember the important stuff
	rb->base = first_copy;
	rb->size = size;
	rb->write_offset_bytes = 0;
	rb->read_offset_bytes = 0;
	strcpy(rb->name, seg_name);
	return rb;

return_null:
	err_no = errno;

	close(shm_fd); // cleanup
	if (!use_file_in_tmp)
		shm_unlink(seg_name);
	else
		unlink(seg_name);

	errno = err_no;
	return NULL;
}

void rng_buf_destroy(rng_buf_t* *rb)
{
	int res;

	res = munmap(rb[0]->base, 2 * rb[0]->size);

	(void) res;

	free(rb[0]);
	rb[0] = NULL;
}

unsigned char* rng_buf_write_address (struct rng_buf *rb)
{
	return rb->base + rb->write_offset_bytes;
}

void rng_buf_write_advance (struct rng_buf *rb, uint64_t size)
{
	rb->write_offset_bytes += size;
}

unsigned char* rng_buf_read_address (struct rng_buf *rb)
{
	return rb->base + rb->read_offset_bytes;
}

void rng_buf_read_advance (struct rng_buf *rb, uint64_t size)
{
	rb->read_offset_bytes += size;

	if (rb->read_offset_bytes >= rb->size)
	{
		rb->read_offset_bytes -= rb->size;
		rb->write_offset_bytes -= rb->size;
	}
}

uint64_t rng_buf_len (struct rng_buf *rb)
{
	return rb->write_offset_bytes - rb->read_offset_bytes;
}

uint64_t rng_buf_free_bytes (struct rng_buf *rb)
{
	return rb->size - rng_buf_len (rb);
}
