/**
 * @file Testing.h
 * Test functions.

 * This file contains the definitions of some test functions,
 * using <a href="http://cunit.sourceforge.net/index.html" target="_blank">CUnit</a>.
 * @version 0.1
 * @date 29/may/2018
 * @authors Andrea Esposito, Graziano Montanaro
 * @copyright No copyright informations provided.
 */

#ifndef MODULES_TESTING_TESTING_H_
#define MODULES_TESTING_TESTING_H_

#include "CUnit/Basic.h"

#include "../Song/Song.h"
#include "../User/User.h"
#include "../Utilities/Utilities.h"

/*
 * Funzione di inizializzazione di default per le suite di CUnit.
 */
int init_suite_dafault();

/*
 * Funzione di distruzione di default per le suite di CUnit.
 */
int clean_suite_dafault();

//Test of Utilities
void test_isNameValid();
void test_isMailValid();
void test_isDateValid();
void test_getMusicGenreName();
void test_getMusicGenreIndex();
void test_isLeapYear();

extern int testingMain();


#endif /* MODULES_TESTING_TESTING_H_ */
