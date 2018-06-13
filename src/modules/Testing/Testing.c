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

extern int testingMain()
{
	CU_pSuite pSuiteValidityChecks = NULL;

	/* inizializza il registro dei casi di test di CUnit */
	if (CUE_SUCCESS != CU_initialize_registry())
	  return CU_get_error();

	/* inizializza la suite dei casi di test per la funzione sum */
	pSuiteValidityChecks = CU_add_suite("Validity Checks", init_suite_dafault, clean_suite_dafault);
	if (pSuiteValidityChecks == NULL) {
	  CU_cleanup_registry();
	  return CU_get_error();
	}

	/* aggiunge i test alle suite sum*/
	if (CU_add_test(pSuiteValidityChecks, "Test of isNameValid()", test_isNameValid) == NULL ||
			CU_add_test(pSuiteValidityChecks, "Test of isMailValid()", test_isMailValid) == NULL){
	  CU_cleanup_registry();
	  return CU_get_error();
	}

	/* esegue i casi di test */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
