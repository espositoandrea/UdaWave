/**
 * @file Testing.c
 * Test functions.

 * This file contains the implementation of the functions defined in Testing.h.
 * @version 0.1
 * @date 29/may/2018
 * @authors Andrea Esposito, Graziano Montanaro
 * @copyright No copyright informations provided.
 */
#include "Testing.h"

/*
 * Funzione di inizializzazione della suite sum.
 * Restituisce 0 in caso di successo.
 */
int init_suite_dafault(){
	return 0;
}

/*
 * Funzione di distruzione della suite sum.
 * Restituisce 0 in caso di successo.
 */
int clean_suite_dafault(){
	return 0;
}

void test_isNameValid()
{
	//Testing with valid names/surnames
	CU_ASSERT_TRUE(isNameValid("Andrea"));
	CU_ASSERT_TRUE(isNameValid("Esposito"));
	CU_ASSERT_TRUE(isNameValid("De Salvo"));
	//Testing with invalid names/surnames
	CU_ASSERT_FALSE(isNameValid("kk99kk"));
	CU_ASSERT_FALSE(isNameValid("Mon_tanaro"));
	CU_ASSERT_FALSE(isNameValid("xXDe! !Salvo@xX"));
}

void test_isMailValid()
{
	//Testing with valid email
	CU_ASSERT_TRUE(isMailValid("g.montanaro16@studenti.uniba.it"));
	CU_ASSERT_TRUE(isMailValid("esposito_andrea99@hotmail.com"));
	CU_ASSERT_TRUE(isMailValid("graziano.montanaro_98@gmail.com"));
	//Testing with invalid email
	CU_ASSERT_FALSE(isMailValid("QUESTA NON E' UNA MAIL"));
	CU_ASSERT_FALSE(isMailValid("g..montanaro@studenti.uniba.it"));
	CU_ASSERT_FALSE(isMailValid("##a.esposito39!@studenti.uniba.it"));
}

void test_isDateValid()
{
	//Testing with valid dates
	CU_ASSERT_TRUE(isDateValid(&((Date){02,12,2018})));
	CU_ASSERT_TRUE(isDateValid(&((Date){29,02,2016})));
	CU_ASSERT_TRUE(isDateValid(&((Date){01,01,1990})));
	//Testing with invalid dates
	CU_ASSERT_FALSE(isDateValid(&((Date){31,11,2018})));
	CU_ASSERT_FALSE(isDateValid(&((Date){29,02,2015})));
	CU_ASSERT_FALSE(isDateValid(&((Date){-5,01,1990})));
}

void test_getMusicGenreName()
{
	//Testing correct returns
	CU_ASSERT_STRING_EQUAL(getMusicGenreName(0), "BLUES");
	CU_ASSERT_STRING_EQUAL(getMusicGenreName(2), "HEAVY METAL");
	CU_ASSERT_STRING_EQUAL(getMusicGenreName(999), "");
	//Testing wrong returns
	CU_ASSERT_STRING_NOT_EQUAL(getMusicGenreName(1), "BLUES");
	CU_ASSERT_STRING_NOT_EQUAL(getMusicGenreName(3), "HEAVY METAL");
	CU_ASSERT_STRING_NOT_EQUAL(getMusicGenreName(4), "");
}

void test_getMusicGenreIndex()
{
	//Testing correct returns
	CU_ASSERT_EQUAL(getMusicGenreIndex("BLUES"), 0);
	CU_ASSERT_EQUAL(getMusicGenreIndex("HEAVY METAL"), 2);
	CU_ASSERT_EQUAL(getMusicGenreIndex("NOT A GENRE"), 999);
	//Testing wrong returns
	CU_ASSERT_NOT_EQUAL(getMusicGenreIndex("CLASSICA"), 0);
	CU_ASSERT_NOT_EQUAL(getMusicGenreIndex("HIP-HOP"), 2);
	CU_ASSERT_NOT_EQUAL(getMusicGenreIndex("POP"), 999);
}

void test_isLeapYear()
{
	//Testing leap years
	CU_ASSERT_TRUE(isLeapYear(2016));
	CU_ASSERT_TRUE(isLeapYear(1804));
	CU_ASSERT_TRUE(isLeapYear(2400));
	//Testing non-leap years
	CU_ASSERT_FALSE(isLeapYear(2017));
	CU_ASSERT_FALSE(isLeapYear(1805));
	CU_ASSERT_FALSE(isLeapYear(2401));
}

extern int testingMain()
{
	CU_pSuite pSuiteValidityChecks = NULL;
	CU_pSuite pSuiteMisc = NULL;

	/* Initialize CUnit registry */
	if (CU_initialize_registry() != CUE_SUCCESS)
	  return CU_get_error();

	/* Initialize the test suite for some validity checks */
	pSuiteValidityChecks = CU_add_suite("Validity Checks", init_suite_dafault, clean_suite_dafault);
	if (pSuiteValidityChecks == NULL)
	{
	  CU_cleanup_registry();
	  return CU_get_error();
	}
	
	/* Initialize the test suite for some miscellaneous functions */
	pSuiteMisc = CU_add_suite("Miscellaneous", init_suite_dafault, clean_suite_dafault);
	if (pSuiteMisc == NULL)
	{
	  CU_cleanup_registry();
	  return CU_get_error();
	}

	/* Add test to Validity Checks suite*/
	if (CU_add_test(pSuiteValidityChecks, "Test of isNameValid()", test_isNameValid) == NULL ||
			CU_add_test(pSuiteValidityChecks, "Test of isMailValid()", test_isMailValid) == NULL ||
			CU_add_test(pSuiteValidityChecks, "Test of isLeapYear()", test_isLeapYear) == NULL ||
			CU_add_test(pSuiteValidityChecks, "Test of isDateValid()", test_isDateValid) == NULL)
	{
	  CU_cleanup_registry();
	  return CU_get_error();
	}
	
	/* Add test to Miscellaneous suite*/
	if (CU_add_test(pSuiteMisc, "Test of getMusicGenreIndex()", test_getMusicGenreIndex) == NULL ||
			CU_add_test(pSuiteMisc, "Test of getMusicGenreName()", test_getMusicGenreName) == NULL)
	{
	  CU_cleanup_registry();
	  return CU_get_error();
	}

	/* esegue i casi di test */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
