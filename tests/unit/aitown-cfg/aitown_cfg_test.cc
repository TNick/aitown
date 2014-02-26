/* ========================================================================= */
/* ------------------------------------------------------------------------- */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "gtest/gtest.h"
#include <aitown/aitown-cfg.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define xstr(s) str(s)
#define str(s) #s


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

void test_one_value_directly_in_section(aitown_cfg_t * cfg, const char * content)
{
    unsigned err_line = 0xFFFF;
    const char * err_string = NULL;
    func_error_t ret = aitown_cfg_parse_buffer (
                cfg, content, &err_line, &err_string);
    EXPECT_TRUE (ret == FUNC_OK);
    EXPECT_TRUE (err_line == 0xFFFF);
    EXPECT_TRUE (err_string == NULL);

    aitown_cfg_leaf_t * leaf = aitown_cfg_get (cfg, "key_1");
    EXPECT_FALSE (leaf == NULL);
    EXPECT_TRUE (strcmp(leaf->node.name, "key_1") == 0);
    EXPECT_TRUE (strcmp(leaf->value, "123") == 0);

    leaf = aitown_cfg_get_leaf (&cfg->root, "key_1");
    EXPECT_FALSE (leaf == NULL);
    EXPECT_TRUE (strcmp(leaf->node.name, "key_1") == 0);
    EXPECT_TRUE (strcmp(leaf->value, "123") == 0);

    aitown_cfg_sect_t * sect = aitown_cfg_get_sect (&cfg->root, "key_1");
    EXPECT_TRUE (sect == NULL);

    leaf = aitown_cfg_get (cfg, "  key_1   ");
    EXPECT_FALSE (leaf == NULL);
    EXPECT_TRUE (strcmp(leaf->node.name, "key_1") == 0);
    EXPECT_TRUE (strcmp(leaf->value, "123") == 0);

    leaf = aitown_cfg_get (cfg, "  /  key_1   ");
    EXPECT_FALSE (leaf == NULL);
    EXPECT_TRUE (strcmp(leaf->node.name, "key_1") == 0);
    EXPECT_TRUE (strcmp(leaf->value, "123") == 0);
}

/* ------------------------------------------------------------------------- */
TEST(cfg,values_directly_in_section) {

    aitown_cfg_t * cfg;
    aitown_cfg_init (&cfg, NULL);

    const char test_file_1[] = {
        "key_1=123"
    };
    test_one_value_directly_in_section (cfg, test_file_1);

    const char test_file_3[] = {
        "   \n"
        "\n"
        " \n"
        "  \n"
        "   key_1 = 123   \n"
        "   \n"
        "  \n"
        " \n"
    };
    test_one_value_directly_in_section (cfg, test_file_3);

    const char test_file_4[] = {
        "  key_1 = \t\t 123 \t\t\t  "
    };
    test_one_value_directly_in_section (cfg, test_file_4);

    const char test_file_5[] = {
        "  key_1 = \'123\'  "
    };
    test_one_value_directly_in_section (cfg, test_file_5);

    const char test_file_6[] = {
        "  key_1 =   \"123\"  "
    };
    test_one_value_directly_in_section (cfg, test_file_6);

    const char test_file_7[] = {
        "; some comment\n"
        "key_1 =   \"123\"  "
    };
    test_one_value_directly_in_section (cfg, test_file_7);

    const char test_file_8[] = {
        "; some comment\n"
        "key_1 =   \"123\"  \n"
        "; some comment\n"
    };
    test_one_value_directly_in_section (cfg, test_file_8);

    const char test_file_9[] = {
        "\n"
        " \n"
        "  \n"
        "; some comment\n"
        "   \n"
        "\n"
        " \n"
        "  \n"
        "   key_1 = 123   \n"
        "   \n"
        "  \n"
        " \n"
        "; some comment\n"
        "   \n"
        "  \n"
        " \n"
    };
    test_one_value_directly_in_section (cfg, test_file_9);



    aitown_cfg_decref (cfg, cfg);
}
/* ========================================================================= */

void test_one_value_directly_in_section_spaced(aitown_cfg_t * cfg, const char * content)
{
    unsigned err_line = 0xFFFF;
    const char * err_string = NULL;
    func_error_t ret = aitown_cfg_parse_buffer (
                cfg, content, &err_line, &err_string);
    EXPECT_TRUE (ret == FUNC_OK);
    EXPECT_TRUE (err_line == 0xFFFF);
    EXPECT_TRUE (err_string == NULL);

    aitown_cfg_leaf_t * leaf = aitown_cfg_get (cfg, "k  e  y  _  1");
    EXPECT_FALSE (leaf == NULL);
    EXPECT_TRUE (strcmp(leaf->node.name, "k  e  y  _  1") == 0);
    EXPECT_TRUE (strcmp(leaf->value, "123") == 0);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(cfg,one_value_directly_in_section_spaced) {

    aitown_cfg_t * cfg;
    aitown_cfg_init (&cfg, NULL);

    const char test_file_1[] = {
        "  k  e  y  _  1 =    123   "
    };
    test_one_value_directly_in_section_spaced (cfg, test_file_1);
    const char test_file_2[] = {
        "  k  e  y  _  1 = \\\n"
        "   123   "
    };
    test_one_value_directly_in_section_spaced (cfg, test_file_2);

    const char test_file_3[] = {
        "; some comment\n"
        "  k  e  y  _  1 =    123   \n"
        "; some comment\n"
    };
    test_one_value_directly_in_section_spaced (cfg, test_file_3);
    const char test_file_4[] = {
        "; some comment\n"
        "  k  e  y  _  1 = \\\n"
        "   123   \n"
        "; some comment\n"
    };
    test_one_value_directly_in_section_spaced (cfg, test_file_4);


    aitown_cfg_decref (cfg, cfg);
}
/* ========================================================================= */

void test_multiple_values_directly_in_section(aitown_cfg_t * cfg, const char * content)
{
    unsigned err_line = 0xFFFF;
    const char * err_string = NULL;
    func_error_t ret = aitown_cfg_parse_buffer (
                cfg, content, &err_line, &err_string);
    EXPECT_TRUE (ret == FUNC_OK);
    EXPECT_TRUE (err_line == 0xFFFF);
    EXPECT_TRUE (err_string == NULL);

    aitown_cfg_leaf_t * leaf1 = aitown_cfg_get (cfg, "key_1");
    EXPECT_FALSE (leaf1 == NULL);
    EXPECT_TRUE (strcmp(leaf1->node.name, "key_1") == 0);
    EXPECT_TRUE (strcmp(leaf1->value, "123") == 0);

    aitown_cfg_leaf_t * leaf2 = aitown_cfg_get (cfg, "key_2");
    EXPECT_FALSE (leaf2 == NULL);
    EXPECT_TRUE (strcmp(leaf2->node.name, "key_2") == 0);
    EXPECT_TRUE (strcmp(leaf2->value, "abcd") == 0);

}
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
TEST(cfg,multiple_values_directly_in_section) {

    aitown_cfg_t * cfg;
    aitown_cfg_init (&cfg, NULL);

    const char test_file_1[] = {
        "key_1=123\n"
        "key_2 = 'abcd'\n"
    };
    test_multiple_values_directly_in_section (cfg, test_file_1);
    const char test_file_2[] = {
        "; some comment\n"
        "; some comment\n"
        "key_1 = 456789\n"
        "; some comment\n"
        "; some comment\n"
        "key_2 = 'abcd'\n"
        "; some comment\n"
        "; some comment\n"
        "key_1=123\n"
        "; some comment\n"
        "; some comment\n"
    };
    test_multiple_values_directly_in_section (cfg, test_file_2);

    aitown_cfg_decref (cfg, cfg);
}
/* ========================================================================= */


void test_directly_in_and_section(aitown_cfg_t * cfg, const char * content)
{
    unsigned err_line = 0xFFFF;
    const char * err_string = NULL;
    func_error_t ret = aitown_cfg_parse_buffer (
                cfg, content, &err_line, &err_string);
    EXPECT_TRUE (ret == FUNC_OK);
    EXPECT_TRUE (err_line == 0xFFFF);
    EXPECT_TRUE (err_string == NULL);

    aitown_cfg_leaf_t * leaf1 = aitown_cfg_get (cfg, "key_1");
    EXPECT_FALSE (leaf1 == NULL);
    EXPECT_TRUE (strcmp(leaf1->node.name, "key_1") == 0);
    EXPECT_TRUE (strcmp(leaf1->value, "123") == 0);

    aitown_cfg_leaf_t * leaf2 = aitown_cfg_get (cfg, "key_2");
    EXPECT_FALSE (leaf2 == NULL);
    EXPECT_TRUE (strcmp(leaf2->node.name, "key_2") == 0);
    EXPECT_TRUE (strcmp(leaf2->value, "abcd") == 0);

    aitown_cfg_sect_t * s1 = aitown_cfg_get_sect( &cfg->root, "section1");
    EXPECT_FALSE (s1 == NULL);
    EXPECT_TRUE (strcmp(s1->node.name, "section1") == 0);

    aitown_cfg_leaf_t * s1leaf1 = aitown_cfg_get_leaf( s1, "key_1");
    EXPECT_FALSE (s1leaf1 == NULL);
    EXPECT_TRUE (strcmp(s1leaf1->node.name, "key_1") == 0);
    EXPECT_TRUE (strcmp(s1leaf1->value, "123") == 0);

}
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
TEST(cfg,directly_in_and_section) {

    aitown_cfg_t * cfg;
    aitown_cfg_init (&cfg, NULL);

    const char test_file_1[] = {
        "key_1=123\n"
        "key_2 = 'abcd'\n"
        "  [section1]  \n"
        "key_1=123\n"
    };
    test_directly_in_and_section (cfg, test_file_1);
    const char test_file_2[] = {
        "; some comment\n"
        "; some comment\n"
        "key_1 = 123\n"
        "; some comment\n"
        "; some comment\n"
        "key_2 = 'abcd'\n"
        "; some comment\n"
        "; some comment\n"
        "[section1]\n"
        "; some comment\n"
        "; some comment\n"
        "key_1=123\n"
        "; some comment\n"
        "; some comment\n"
    };
    test_directly_in_and_section (cfg, test_file_2);
    const char test_file_3[] = {
        "key_1=123\n"
        "key_2 = 'abcd'\n"
        "[    section1  ]  \n"
        "key_1=123\n"
    };
    test_directly_in_and_section (cfg, test_file_3);
    const char test_file_4[] = {
        "key_1=123\n"
        "   \n"
        "  \n"
        " \n"
        "key_2 = 'abcd'\n"
        "[    section1  ]  \n"
        "key_1=123qwerqewrqewr\n"
        "   \n"
        "  \n"
        " \n"
        "; some comment\n"
        "; some comment\n"
        "   \n"
        "  \n"
        " \n"
        "[    section1  ]  \n"
        "key_1=qwerqwerweqrqwer\n"
        "[    section1  ]  \n"
        "key_1=werwerwerw\n"
        "; some comment\n"
        "; some comment\n"
        "   \n"
        "  \n"
        " \n"
        "[    section1  ]  \n"
        "; some comment\n"
        "; some comment\n"
        "   \n"
        "  \n"
        " \n"
        "key_1 = werwerwerew  \n"
        "; some comment\n"
        "; some comment\n"
        "[    section1  ]  \n"
        "key_1=1ewrwe wewe ewwe we\n"
        "  [    section1  ]  \n"
        "   \n"
        "  \n"
        " \n"
        "key_1=123465\n"
        "   [    section1  ]  \n"
        "key_1=123\n"
    };
    test_directly_in_and_section (cfg, test_file_4);



    aitown_cfg_decref (cfg, cfg);
}
/* ========================================================================= */


void test_multiple_l1_section(aitown_cfg_t * cfg, const char * content)
{
    unsigned err_line = 0xFFFF;
    const char * err_string = NULL;
    func_error_t ret = aitown_cfg_parse_buffer (
                cfg, content, &err_line, &err_string);
    EXPECT_TRUE (ret == FUNC_OK);
    EXPECT_TRUE (err_line == 0xFFFF);
    EXPECT_TRUE (err_string == NULL);

    aitown_cfg_leaf_t * leaf1 = aitown_cfg_get (cfg, "key_1");
    EXPECT_FALSE (leaf1 == NULL);
    EXPECT_TRUE (strcmp(leaf1->node.name, "key_1") == 0);
    EXPECT_TRUE (strcmp(leaf1->value, "123") == 0);

    aitown_cfg_leaf_t * leaf2 = aitown_cfg_get (cfg, "key_2");
    EXPECT_FALSE (leaf2 == NULL);
    EXPECT_TRUE (strcmp(leaf2->node.name, "key_2") == 0);
    EXPECT_TRUE (strcmp(leaf2->value, "abcd") == 0);

#define test_multiple_l1_section_check(nr) \
    aitown_cfg_sect_t * s ## nr = aitown_cfg_get_sect( &cfg->root, "section" xstr(nr)); \
    EXPECT_FALSE (s ## nr == NULL); \
    EXPECT_TRUE (strcmp(s ## nr->node.name, "section" xstr(nr) ) == 0); \
    \
    aitown_cfg_leaf_t * s ## nr ## leaf1 = aitown_cfg_get_leaf( s ## nr, "key_1"); \
    EXPECT_FALSE (s ## nr ## leaf1 == NULL); \
    EXPECT_TRUE (strcmp(s ## nr ## leaf1->node.name, "key_1") == 0); \
    EXPECT_TRUE (strcmp(s ## nr ## leaf1->value, xstr(nr)) == 0);


    test_multiple_l1_section_check(1);
    test_multiple_l1_section_check(2);
    test_multiple_l1_section_check(3);


}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(cfg,multiple_l1_section) {

    aitown_cfg_t * cfg;
    aitown_cfg_init (&cfg, NULL);

    const char test_file_1[] = {
        "key_1=123\n"
        "key_2 = 'abcd'\n"
        "  [section1]  \n"
        "  key_1=1\n"
        "  [section2]  \n"
        "  key_1=2\n"
        "  [section3]  \n"
        "  key_1=3\n"
    };
    test_multiple_l1_section (cfg, test_file_1);

    const char test_file_2[] = {
        "key_1=123\n"
        "key_2 = 'abcd'\n"
        "  [section1]  \n"
        "  [section1]  \n"
        "  key_1=1\n"
        "  [section1]  \n"
        "  [section2]  \n"
        "  key_1=2\n"
        "  [section1]  \n"
        "  [section3]  \n"
        "  key_1=3\n"
    };
    test_multiple_l1_section (cfg, test_file_2);

    aitown_cfg_decref (cfg, cfg);
}
/* ========================================================================= */



void test_nested_sections(aitown_cfg_t * cfg, const char * content)
{
    unsigned err_line = 0xFFFF;
    const char * err_string = NULL;
    func_error_t ret = aitown_cfg_parse_buffer (
                cfg, content, &err_line, &err_string);
    EXPECT_TRUE (ret == FUNC_OK);
    EXPECT_TRUE (err_line == 0xFFFF);
    EXPECT_TRUE (err_string == NULL);

    aitown_cfg_leaf_t * leaf1 = aitown_cfg_get (cfg, "key_1");
    EXPECT_FALSE (leaf1 == NULL);
    EXPECT_TRUE (strcmp(leaf1->node.name, "key_1") == 0);
    EXPECT_TRUE (strcmp(leaf1->value, "123") == 0);

    aitown_cfg_leaf_t * leaf2 = aitown_cfg_get (cfg, "key_2");
    EXPECT_FALSE (leaf2 == NULL);
    EXPECT_TRUE (strcmp(leaf2->node.name, "key_2") == 0);
    EXPECT_TRUE (strcmp(leaf2->value, "abcd") == 0);

#define test_nested_sections_check(nr) \
    EXPECT_FALSE (s ## nr == NULL); \
    EXPECT_TRUE (strcmp(s ## nr->node.name, "section" xstr(nr) ) == 0); \
    \
    EXPECT_FALSE (s ## nr ## leaf1 == NULL); \
    EXPECT_TRUE (strcmp(s ## nr ## leaf1->node.name, "key_1") == 0); \
    EXPECT_TRUE (strcmp(s ## nr ## leaf1->value, xstr(nr)) == 0);


    aitown_cfg_sect_t * s1 = aitown_cfg_get_sect( &cfg->root, "section1");
    aitown_cfg_leaf_t * s1leaf1 = aitown_cfg_get_leaf( s1, "key_1");
    test_nested_sections_check(1);

    aitown_cfg_sect_t * s2 = aitown_cfg_get_sect( s1, "section2");
    aitown_cfg_leaf_t * s2leaf1 = aitown_cfg_get_leaf( s2, "key_1");
    test_nested_sections_check(2);

    aitown_cfg_sect_t * s3 = aitown_cfg_get_sect( s2, "section3");
    aitown_cfg_leaf_t * s3leaf1 = aitown_cfg_get_leaf( s3, "key_1");
    test_nested_sections_check(3);

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(cfg,nested_sections) {

    aitown_cfg_t * cfg;
    aitown_cfg_init (&cfg, NULL);

    const char test_file_1[] = {
        "key_1=123\n"
        "key_2 = 'abcd'\n"
        "  [section1]  \n"
        "  key_1=1\n"
        "  [section1/section2]  \n"
        "  key_1=2\n"
        "  [section1/section2/section3]  \n"
        "  key_1=3\n"
    };
    test_nested_sections (cfg, test_file_1);

    const char test_file_2[] = {
        "key_1=123\n"
        "key_2 = 'abcd'\n"
        "  [section1]  \n"
        "  [section1]  \n"
        "  key_1=1\n"
        "  [section1]  \n"
        "  [section1/section2]  \n"
        "  key_1=2\n"
        "  [section1]  \n"
        "  [section1/section2/section3]  \n"
        "  key_1=3\n"
    };
    test_nested_sections (cfg, test_file_2);

    aitown_cfg_decref (cfg, cfg);
}
/* ========================================================================= */




/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
