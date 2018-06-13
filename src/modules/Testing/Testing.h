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
 * Funzione di inizializzazione della suite sum.
 * Restituisce 0 in caso di successo.
 */
int init_suite_dafault();

/*
 * Funzione di distruzione della suite sum.
 * Restituisce 0 in caso di successo.
 */
int clean_suite_dafault();

void test_isNameValid();
void test_isMailValid();

extern int testingMain();


#endif /* MODULES_TESTING_TESTING_H_ */
