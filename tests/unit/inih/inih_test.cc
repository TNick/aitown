/* ========================================================================= */
/* ------------------------------------------------------------------------- */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "gtest/gtest.h"
#include <aitown/ini.h>
#include <aitown/struct_ini.h>


/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#ifdef AITOWN_WIN32
#define REAL_TREE_DIR_1 "C:"
#define REAL_TREE_DIR_2 getenv("TEMP")
#else
#define REAL_TREE_DIR_1 getenv("HOME")
#define REAL_TREE_DIR_2 "/tmp"
#endif

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

/* ------------------------------------------------------------------------- */
TEST(inih,structure) {
    struct_ini_t *sini;
    char *p_file = (char*)malloc (1024);

    *p_file = 0;
    strcat(p_file, REAL_TREE_DIR_2);
    strcat(p_file, "/test.ini");

    FILE *f = fopen(p_file,"w");
    fprintf(f,"[General]\n");
    fprintf(f,"a = 1\n");
    fprintf(f,"b = 2\n");
    fprintf(f,"c = 3\n");
    fprintf(f,"d = 4\n");
    fprintf(f,"[S1]\n");
    fprintf(f,"S1a = 1\n");
    fprintf(f,"S1b = 2\n");
    fprintf(f,"S1c = 3\n");
    fprintf(f,"S1d = 4\n");
    fprintf(f,"[S2]\n");
    fprintf(f,"S2_int = -159\n");
    fprintf(f,"S2_dbl = -159.159\n");
    fprintf(f,"S2_int_wrong = -159abc\n");
    fprintf(f,"S2_dbl_wrong = -159.159abc\n");
    fprintf(f,"[S3]\n");
    fclose (f);

    int reti = struct_ini_get (&sini, p_file);
    EXPECT_EQ(reti, INIH_ERR_SUCCESS);

    struct_ini_value_t * val;

    struct_ini_sect_t * sect1 = struct_ini_find_section (sini, "General");
    EXPECT_TRUE(sect1 != NULL);
    EXPECT_TRUE(sect1->name != NULL);
    EXPECT_TRUE(strcasecmp(sect1->name,"General") == 0);

    val = struct_ini_find_value (sect1, "a");
    EXPECT_TRUE(val != NULL);
    EXPECT_TRUE(val->name != NULL);
    EXPECT_TRUE(strcasecmp(val->name,"a") == 0);
    EXPECT_TRUE(strcasecmp(val->value,"1") == 0);

    val = struct_ini_find_value (sect1, "b");
    EXPECT_TRUE(val != NULL);
    EXPECT_TRUE(val->name != NULL);
    EXPECT_TRUE(strcasecmp(val->name,"b") == 0);
    EXPECT_TRUE(strcasecmp(val->value,"2") == 0);

    val = struct_ini_find_value (sect1, "c");
    EXPECT_TRUE(val != NULL);
    EXPECT_TRUE(val->name != NULL);
    EXPECT_TRUE(strcasecmp(val->name,"c") == 0);
    EXPECT_TRUE(strcasecmp(val->value,"3") == 0);

    val = struct_ini_find_value (sect1, "d");
    EXPECT_TRUE(val != NULL);
    EXPECT_TRUE(val->name != NULL);
    EXPECT_TRUE(strcasecmp(val->name,"d") == 0);
    EXPECT_TRUE(strcasecmp(val->value,"4") == 0);

    struct_ini_sect_t * sect2 = struct_ini_find_section (sini, "S1");
    EXPECT_TRUE(sect2 != NULL);
    EXPECT_TRUE(sect2->name != NULL);
    EXPECT_TRUE(strcasecmp(sect2->name,"S1") == 0);

    struct_ini_sect_t * sect3 = struct_ini_find_section (sini, "S2");
    EXPECT_TRUE(sect3 != NULL);
    EXPECT_TRUE(sect3->name != NULL);
    EXPECT_TRUE(strcasecmp(sect3->name,"S2") == 0);

    struct_ini_sect_t * sect4 = struct_ini_find_section (sini, "S3");
    EXPECT_TRUE(sect4 != NULL);
    EXPECT_TRUE(sect4->name != NULL);
    EXPECT_TRUE(strcasecmp(sect4->name,"S3") == 0);

    struct_ini_sect_t * sect5 = struct_ini_find_section (sini, "S4");
    EXPECT_TRUE(sect5 != NULL);
    EXPECT_TRUE(sect5->name != NULL);
    EXPECT_TRUE(strcasecmp(sect5->name,"S4") == 0);


    EXPECT_TRUE(strcasecmp(struct_ini_get_value(sini,"S1","S1a"),"1") == 0);
    EXPECT_TRUE(struct_ini_get_value(sini,"abcde","") == NULL);
    EXPECT_TRUE(struct_ini_get_value(sini,"abcde","S1a") == NULL);
    EXPECT_TRUE(struct_ini_get_value(sini,"S1","abcde") == NULL);

    long int ret_v1;
    EXPECT_EQ(struct_ini_get_int(sini,"S2","S2_int", &ret_v1, 0 ), STRUCT_INI_OK);
    EXPECT_EQ(ret_v1, -159);
    EXPECT_EQ(struct_ini_get_int(sini,"S2","S2_int_fake", &ret_v1, 123 ), STRUCT_INI_NOT_FOUND);
    EXPECT_EQ(ret_v1, 123);
    EXPECT_EQ(struct_ini_get_int(sini,"S2","S2_int_wrong", &ret_v1, 123 ), STRUCT_INI_CANT_CONVERT);
    EXPECT_EQ(ret_v1, 123);

    double ret_v2;
    EXPECT_EQ(struct_ini_get_double (sini,"S2","S2_dbl", &ret_v2, 0 ), STRUCT_INI_OK);
    EXPECT_EQ(ret_v2, -159.159);
    EXPECT_EQ(struct_ini_get_double (sini,"S2","S2_dbl_fake", &ret_v2, 1.23 ), STRUCT_INI_NOT_FOUND);
    EXPECT_EQ(ret_v2, 1.23);
    EXPECT_EQ(struct_ini_get_double (sini,"S2","S2_dbl_wrong", &ret_v2, 1.23 ), STRUCT_INI_CANT_CONVERT);
    EXPECT_EQ(ret_v2, 1.23);



    struct_ini_end (sini);
}
/* ========================================================================= */



/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
