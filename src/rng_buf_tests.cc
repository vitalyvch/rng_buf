/** A unit-test for rng_buf implementation.
 *
 * @author Vitalii Chernookyi
 *                           
 * @copyright BSD
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <cstdarg>

#include <gtest/gtest.h>

#include "rng_buf.h"


class RngBufFixture : public ::testing::Test {
 protected:
  // Per-test-suite set-up.
  // Called before the first test in this test suite.
  // Can be omitted if not needed.
  static void SetUpTestSuite() {
	  rng_buf = rng_buf_create("", 0, false);
  }

  // Per-test-suite tear-down.
  // Called after the last test in this test suite.
  // Can be omitted if not needed.
  static void TearDownTestSuite() {
	  rng_buf_destroy(&rng_buf);
  }

  // You can define per-test set-up logic as usual.
  //virtual void SetUp() { ... }

  // You can define per-test tear-down logic as usual.
  //virtual void TearDown() { ... }

  // Some expensive resource shared by all tests.
  static rng_buf_t *rng_buf;
};

rng_buf_t *RngBufFixture::rng_buf = NULL;

TEST_F(RngBufFixture, PositiveNos) {
	/////////////
	ASSERT_NE(nullptr, rng_buf);
	ASSERT_NE(nullptr, rng_buf->base);
	ASSERT_EQ(sysconf(_SC_PAGESIZE), (long)rng_buf->size);

	//////////////////////////////////////////////////
	/// ### ITER #1 ###
	unsigned char *w_ptr = rng_buf_write_address(rng_buf);
	memcpy(w_ptr, "1234", 4);
	rng_buf_write_advance(rng_buf, 4);

	ASSERT_EQ(rng_buf_len(rng_buf), 4U);
	ASSERT_EQ(rng_buf_free_bytes(rng_buf), rng_buf->size - 4);

	unsigned char *r_ptr = rng_buf_read_address(rng_buf);
	ASSERT_EQ(0, memcmp(r_ptr, "1234", 4));
	rng_buf_read_advance(rng_buf, 4);

	ASSERT_EQ(rng_buf_len(rng_buf), 0U);
	ASSERT_EQ(rng_buf_free_bytes(rng_buf), rng_buf->size);

	//////////////////////////////////////////////////
	/// ### ITER #2 ###
	unsigned char garbage[rng_buf->size-1];
	for (unsigned i=0; i<sizeof(garbage); i++)
		garbage[i] = random();

	w_ptr = rng_buf_write_address(rng_buf);
	memcpy(w_ptr, garbage, sizeof(garbage));
	rng_buf_write_advance(rng_buf, sizeof(garbage));

	ASSERT_EQ(rng_buf_len(rng_buf), sizeof(garbage));
	ASSERT_EQ(rng_buf_free_bytes(rng_buf), rng_buf->size - sizeof(garbage));

	r_ptr = rng_buf_read_address(rng_buf);
	ASSERT_EQ(0, memcmp(r_ptr, garbage, sizeof(garbage)));
	rng_buf_read_advance(rng_buf, sizeof(garbage));

	ASSERT_EQ(rng_buf_len(rng_buf), 0U);
	ASSERT_EQ(rng_buf_free_bytes(rng_buf), rng_buf->size);

	//////////////////////////////////////////////////
	/// ### ITER #3 ###
	unsigned char garb_2[rng_buf->size];
	for (unsigned i=0; i<sizeof(garb_2); i++)
		garb_2[i] = random();

	w_ptr = rng_buf_write_address(rng_buf);
	memcpy(w_ptr, garb_2, sizeof(garb_2));
	rng_buf_write_advance(rng_buf, sizeof(garb_2));

	ASSERT_EQ(rng_buf_len(rng_buf), sizeof(garb_2));
	ASSERT_EQ(rng_buf_free_bytes(rng_buf), rng_buf->size - sizeof(garb_2));

	r_ptr = rng_buf_read_address(rng_buf);
	ASSERT_EQ(0, memcmp(r_ptr, garb_2, sizeof(garb_2)));
	rng_buf_read_advance(rng_buf, sizeof(garb_2));

	ASSERT_EQ(rng_buf_len(rng_buf), 0U);
	ASSERT_EQ(rng_buf_free_bytes(rng_buf), rng_buf->size);
}
//############################################################################
