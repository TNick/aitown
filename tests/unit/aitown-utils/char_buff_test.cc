/* ========================================================================= */
/* ------------------------------------------------------------------------- */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "gtest/gtest.h"
#include <aitown/error_codes.h>
#include <aitown/dbg_assert.h>
#include <aitown/char_buff.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */


/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* Would like a semi-open interval [min, max) */
int random_in_range (unsigned int min, unsigned int max)
{
  int base_random = rand(); /* in [0, RAND_MAX] */
  if (RAND_MAX == base_random) 
	  return random_in_range(min, max);
  /* now guaranteed to be in [0, RAND_MAX) */
  int range       = max - min,
      remainder   = RAND_MAX % range,
      bucket      = RAND_MAX / range;
  /* There are range buckets, plus one smaller interval
     within remainder of RAND_MAX */
  if (base_random < RAND_MAX - remainder) {
    return min + base_random/bucket;
  } else {
    return random_in_range (min, max);
  }
}

/* ------------------------------------------------------------------------- */
TEST(char_buff,t1) {
	char_buff_t cbuff;
	int exit_code;
	int k;
	char loc_buff[64];
	size_t last_known_size;
	
	for ( k = 0; k < 10; k++ ) {
		char_buff_init(&cbuff, 124);
		EXPECT_TRUE (cbuff.data != NULL);
		EXPECT_EQ (strlen(cbuff.data), (size_t)0);
		EXPECT_TRUE (cbuff.allocated > 0);
		EXPECT_EQ (cbuff.used, (size_t)0);
		
		exit_code = char_buff_add (&cbuff,"");
		EXPECT_EQ(exit_code, FUNC_OK);
		EXPECT_TRUE (cbuff.data != NULL);
		EXPECT_EQ (strlen(cbuff.data), (size_t)0);
		EXPECT_TRUE (cbuff.allocated > 0);
		EXPECT_EQ (cbuff.used, (size_t)0);
		
		exit_code = char_buff_add (&cbuff,"test");
		EXPECT_EQ(exit_code, FUNC_OK);
		EXPECT_TRUE (cbuff.data != NULL);
		EXPECT_EQ (strlen (cbuff.data), (size_t)4);
		EXPECT_EQ (strcmp (cbuff.data,"test"), 0);
		EXPECT_EQ (cbuff.used, (size_t)4);
		
		exit_code = char_buff_add (&cbuff," is being build");
		EXPECT_EQ(exit_code, FUNC_OK);
		EXPECT_TRUE (cbuff.data != NULL);
		EXPECT_EQ (strcmp (cbuff.data,"test is being build"), 0);
		EXPECT_TRUE (cbuff.used == 19);
		
		int i; int j;
		last_known_size = cbuff.used;
		for ( i = 0 ; i < 1000; i++ ) {
			for ( j = 0 ; j < 63; j++ ) {
				loc_buff[j] = random_in_range('A', 'Z');
			}
			loc_buff[j] = 0;
			exit_code = char_buff_add (&cbuff, loc_buff);
			EXPECT_EQ(exit_code, FUNC_OK);
			EXPECT_EQ(cbuff.used, last_known_size+63);
			last_known_size += 63;
			i++;
			
			for ( j = 0 ; j < 63; j++ ) {
				loc_buff[j] = random_in_range('a', 'z');
			}
			loc_buff[j] = 0;
			exit_code = char_buff_add (&cbuff, loc_buff);
			EXPECT_EQ(exit_code, FUNC_OK);
			EXPECT_EQ(cbuff.used, last_known_size+63);
			last_known_size += 63;
			
		}
		
		char_buff_end(&cbuff);
		EXPECT_TRUE (cbuff.data == NULL);
		EXPECT_TRUE (cbuff.allocated == 0);
		EXPECT_TRUE (cbuff.used == 0);
	
	}
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(char_buff,t2) {
	char_buff_t cbuff;
	int exit_code;
	char loc_buff[64];
	size_t last_known_size;
	
	int i; int j;
	for ( i = 0 ; i < 1000; i++ ) {
		for ( j = 0 ; j < 63; j++ ) {
			loc_buff[j] = random_in_range('A', 'Z');
		}
		loc_buff[j] = 0;
		exit_code = char_buff_from_str (&cbuff, loc_buff);
		EXPECT_EQ(exit_code, FUNC_OK);
		EXPECT_EQ(cbuff.used, (size_t)63);
		last_known_size = cbuff.used;
		
		for ( j = 0 ; j < 63; j++ ) {
			loc_buff[j] = random_in_range('a', 'z');
		}
		loc_buff[j] = 0;
		exit_code = char_buff_add (&cbuff, loc_buff);
		EXPECT_EQ(exit_code, FUNC_OK);
		EXPECT_EQ(cbuff.used, (size_t)(last_known_size+63));
		
		char_buff_end(&cbuff);
		EXPECT_TRUE (cbuff.data == NULL);
		EXPECT_EQ (cbuff.allocated, (size_t)0);
		EXPECT_EQ (cbuff.used, (size_t)0);
		
	}
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
