/**
 * @file Utilities.c
 * Implementation of the functions from Utilities.h.
 
 * This file contains the implementation of the function defined
 * in Utilities.h. It uses the standard C libraries.
 * @version 0.1
 * @date 08/may/2018
 * @authors Andrea Esposito, Graziano Montanaro
 * @copyright No copyright informations provided.
 */

#include "Utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

/**
 * The function gets a boolean choice (YES or NO).
 * It print a message (usually a question) on the screen
 * and it waits for the user input. When the user inserts
 * a response, this function converts it to upper case and
 * checks if it is Y (YES) or N (NO). Then, if the given value
 * is valid, it returns it to the caller.
 */
char getChoice(const char* messageToPrint)
{
	char choice;
	int isFirstTimeInput = 1;
	do
	{
		if(!isFirstTimeInput)
			printf("Risposta non valida. ");
		printf("%s (y/n) ", messageToPrint);
		scanf("%c", &choice);
		fflush(stdin);
		choice = toupper(choice);
		isFirstTimeInput = 0;
	} while(choice != 'Y' && choice != 'N');

	return choice;
}

/**
* This function gets an integer between two values.
* If the users inserts a value outside of the given range
* (or if he input a non-integer value, like a string),
* the function will ask the user to insert another value,
* showing an error message. If the input goes well, the
* function returns the given integer to the caller.
*
* @pre minVal and maxVal should be different.
* @post If minVal is equal to maxVal the function will accept all integers.
*/
int getInt(int minVal, int maxVal)
{
	string s;
	int scelta;
	do
	{
		scanf("%s",s);

		fflush(stdin);
		int isDigit = 1;
		for(char* i = s; *i != '\0'; i++)
			if(*i != '\0' && !isdigit(*i))
				isDigit = 0;

		if(isDigit)
			scelta=atoi(s);
		else
			scelta = minVal - 1;

		if((scelta<minVal)||(scelta>maxVal))
		{
			printf("ERRORE! Inserire un valore compreso tra %d e %d. \n", minVal, maxVal);
		}
	}
	while(minVal != maxVal && (scelta < minVal || scelta > maxVal));

	return scelta;
}

/**
* Check if a given string is a valid name (or surname). It means that
* the string can't have numbers or non-alphabetic characters in it.
* Spaces are allowed.
*
* @pre name should be a valid (not NULL) char pointer (string).
*/
bool isNameValid(const char* name)
{
	const char* nameRegex = "^([A-Za-z]*( )?)+$";

	return checkWithRegEx(nameRegex, name);
}

/**
 * Check if a string is can be recognized by a finite state automaton using a regular expression.
 *
 * @pre The regular expression should be valid.
 */
bool checkWithRegEx(const char* regExp, const char* stringToCheck)
{
	bool match = false;

	int rv;
	regex_t exp;
	rv = regcomp(&exp,
			regExp,
			REG_EXTENDED);
	if (rv != 0) {
		printf("regcomp failed with %d\n", rv);
	}

	regmatch_t matches[1]; //A list of the matches in the string
	//Compare the string to the expression
	//regexec() returns 0 on match, otherwise REG_NOMATCH
	if (regexec(&exp, stringToCheck, 1, matches, 0) == 0)
		match = true; //printf("\"%s\" matches characters %d - %d\n", originalMail, matches[0].rm_so, matches[0].rm_eo);
	else
		match = false; //printf("\"%s\" does not match\n", originalMail);

	return match;
}

/**
* This function checks if the string could be a valid email.
* @warning This function doesn't check if a mail exists
* or if it is of a valid hoster. It only checks if a given
* string is formatted like an e-mail.
*/
bool isMailValid(const char *mail)
{
	const char* mailRegex = "^(([a-z]|[A-Z]|[0-9])+(\.|_|!|#|\$|%|&|'|)?([a-z]|[A-Z]|[0-9])*)+([a-z]|[A-Z]|[0-9])@([a-z]|[A-Z]|[0-9])+\.([a-z]|[A-Z]|[0-9])+(\.([a-z]|[A-Z]|[0-9])+)?$";

	return checkWithRegEx(mailRegex, mail);
}

/**
* This function checks if a file exists,
* trying to open it. If it didn't worked,
* it means that the file or the directory
* couldn't be opened.
*/
int doesFileExists(const char* path)
{
	int fileExists = 0;
	FILE* file;

	//check if the file doesn't exist
	if((file = fopen(path, "r")) != NULL)
		fileExists = 1;

	fclose(file);

	return fileExists;
}



/**
* This function waits for the user input and
* checks if the given date is valid.
*/
Date getDate()
{
	int isFirstTimeInput = 1;	//Used for error messages
	int status;
	Date date;

	do
	{
		if(!isFirstTimeInput)
			printf("Inserire una data valida! Re");

		printf("Inserire la propria data di nascita (nel formato GG/MM/AAAA): ");
		status = scanf("%2d/%2d/%4d", &date.day, &date.month, &date.year);

		fflush(stdin);

		isFirstTimeInput = 0;
	} while(!isDateValid(&date) || status != 3);

	return date;
}

/**
* This function checks if the date is valid.
*
* @pre date Should be a valid date pointer (not NULL).
*/
int isDateValid(Date *date)
{
    int isValid = 1;

    if(date->month <= 12 && date->month > 0 && date->year > 0 && date->day > 0)
    {
        if(date->month == 2)
        {
            if(isLeapYear(date->year) && date->day > 29)
                isValid = 0;
            else if(!isLeapYear(date->year) && date->day > 28)
                isValid = 0;
        }
        else if((date->month == 1 || date->month == 3 || date->month == 5 || date->month == 7 || date->month == 8 || date->month == 10 || date->month == 12) && date->day > 31)
            isValid = 0;
        else if(date->day > 30)
            isValid = 0;
    }
    else
    {
        isValid = 0;
    }

	return isValid;
}

/**
 * This function checks if a given year is leap, using
 * the definition of "leap year". That is: an year is leap if it
 * can be divided by fir but not by 100 or if it can be divided by 400.
 *
 * @pre year Should be a valid year (positive integer).
 */
int isLeapYear(int year)
{
	int isLeap = 0;

	if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		isLeap = 1;

	return isLeap;
}


/**
* This function gets the name of a given genre.
* It basically returns a literal transcription for the enum-type value.
*
* @pre the genre should be valid.
* @post If the genre is invalid the function will return an empty string.
*/
const char* getMusicGenreName(Genre genre)
{
	static char *genres[] = { "BLUES", "CLASSICA", "HEAVY METAL", "HIP-HOP", "JAZZ", "POP", "PUNK", "ROCK" };

	if(genre == INVALID_GENRE)
		return "";


	return genres[genre];
}

/**
* This function gets the genre with a given name.
* It basically gets the enum-type value from its literal transcription.
*
* @pre the name should be a valid genre.
* @post If the name isn't a valid genre the function will return INVALID_GENRE.
*/
const Genre getMusicGenreIndex(char* genre)
{
	static char *genres[] = { "BLUES", "CLASSICA", "HEAVY METAL", "HIP-HOP", "JAZZ", "POP", "PUNK", "ROCK" };
	Genre songGenre = INVALID_GENRE;

	for(int i = 0; i < 8; i++)
		if(strcmp(genres[i], genre) == 0)
		{
			songGenre = i;
			break;
		}

	return songGenre;
}

/**
* This function inserts a music genre.
* It prompts the user to insert a music genre (its literal translation).
* It the given value is valid, the function will return its enum-type value.
* Otherwise it will ask the user to insert another value.
*/
Genre getGenre()
{
	int isFirstTimeInput = 1;
	string inputtedGenre;
	Genre genre;

	printf("Inserire il genere musicale a scelta tra:\n");
	for(int i = 0; i < 8; i++)
		printf("- %s\n", getMusicGenreName(i));

	do
	{
		if(!isFirstTimeInput)
			printf("Genere non valido. Inserire un genere valido: ");
		gets(inputtedGenre);

		strupr(inputtedGenre);

		isFirstTimeInput = 0;
	} while((genre = getMusicGenreIndex(inputtedGenre)) == INVALID_GENRE);

	return genre;
}

/**
* This function inserts a sex.
* It prompts a message and waits for the user input. Then it
* checks if the given value is a valid genre. If it isn't, the function
* will ask the user for another value.
*/
Sex getSex()
{
	int isFirstTimeInput = 1;	//Used for error messages
	char sex;

	do
	{
		if(!isFirstTimeInput)
			printf("Inserire una scelta valida! Re");

		printf("Inserire il proprio sesso:\n"
				"\tM- MASCHIO\n"
				"\tF- FEMMINA\n"
				"\tO- ALTRO\n"
				"Inserire la scelta: ");
		scanf("%c", &sex);

		sex = toupper(sex);

		isFirstTimeInput = 0;
	} while(sex != 'M' && sex != 'F' && sex != 'O');

	return sex;
}

/**
* This function inserts a name and it checks if there aren't any
* special characters or digit. If there are, it will ask for
* another value.
*
* @pre typeOfName should be like "name" of "surname".
*/
void getName(char *name, const char *typeOfName)
{
	int isFirstTimeInput = 1;	//Used for error messages
	do
	{
		if(!isFirstTimeInput)
			printf("Inserire un %s valido! Re", typeOfName);

		printf("Inserire il %s: ", typeOfName);
		gets(name);

		isFirstTimeInput = 0;
	} while(!isNameValid(name));
}

/**
* This function inserts an email and checks if it could be an email.
* If it can't, the function will ask for a new email.
*/
void getEmail(char *mail)
{
	int isFirstTimeInput = 1;	//Used for error messages

	do
	{
		if(!isFirstTimeInput)
			printf("Inserire una mail valida! Re");

		printf("Inserire la propria mail: ");
		scanf("%s", mail);

		isFirstTimeInput = 0;
	} while(!isMailValid(mail));
}
