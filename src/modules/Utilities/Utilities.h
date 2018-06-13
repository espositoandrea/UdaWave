/**
* @file Utilities.h
* Miscellaneous data structures and functions.
*
* This library contains some useful functions and data structures.
* It is used by other libraries.
* @version 0.1
* @date 08/may/2018
* @authors Andrea Esposito, Graziano Montanaro
* @copyright No copyright informations provided.
*/

#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdbool.h>

/**
 * The maximum length of a string (excluding the terminal character).
 */
#define MAX_STRING_LENGTH 35

/**
 * The maximum length of a code (excluding the terminal character).
 */
#define MAX_CODE_LENGTH 5

/**
 * A string.
 *
 * This definition is used to simplify the creation of strings.
 */
typedef char string[MAX_STRING_LENGTH + 1];

/**
 * Music genres.
 *
 * An enum data type that contains the various music genres.
 */
typedef enum Genre
{
	BLUES = 0,         		///< "Blues" music genre.
	CLASSIC,           		///< "Classic" music genre.
	HEAVY_METAL,       		///< "Heavy Metal" music genre.
	HIP_HOP,           		///< "Hip-Hop" music genre.
	JAZZ,              		///< "Jazz" music genre.
	POP,               		///< "Pop" music genre.
	PUNK,              		///< "Punk" music genre.
	ROCK,              		///< "Rock" music genre.
	INVALID_GENRE = 999		///< Used to represent an invalid music genre.
} Genre;

/**
 * A data type to represent someone sex.
 */
typedef enum Sex
{
	MALE = 'M',				///< Male sex.
	FEMALE = 'F',			///< Female sex.
	OTHER = 'O'				///< Non-Binary sex.
} Sex;

/**
 * A date.
 */
typedef struct Date
{
	int day;				///< The day of a date.
	int month;				///< The month of a date.
	int year;				///< The year of a date.
} Date;



/**
 * Insert a sex.
 * Prompts the user to insert a sex and checks if the given value is valid.
 * @return The inserted sex.
 */
Sex getSex();

/**
 * Insert a music genre.
 * Prompts the user to insert a music genre and checks if the given value is valid.
 * @return The inserted music genre.
 */
Genre getGenre();

/**
 * Get the name of the given genre.
 * Gets a genre and returns its literal name.
 * @param[in] genre The music genre of which the name is wanted.
 * @return The genre name if it is valid. An empty string otherwise.
 */
const char* getMusicGenreName(Genre genre);

/**
* Get the genre with the given name.
* Gets a genre name and returns its enum-type value.
* @param[in] genre The name from which the function has to get the genre.
* @return The function returns the genre of the song if the name is valid.
*/
const Genre getMusicGenreIndex(char* genre);


/**
* Check if file exists.
*
* @param[in] path The path of the file that has to be checked.
* @return The function returns 0 if the file doesn't exist.
* If it does, the function returns 1.
*/
int doesFileExists(const char* path);

/**
* Check if the given string could be a valid email.
*
* @param[in] mail String to check.
* @return It returns 0 if the string isn't an email.
* It returns 1 if the string could be an email.
*/
bool isMailValid(const char *mail);

/**
* Check if a string is a valid name.
*
* @param[in] name String that has to be checked.
* @return The function returns 1 if the name is valid, 0 otherwise.
*/
bool isNameValid(const char* name);

/**
* Get an integer between two values.
*
* @param[in] minVal The minimum value of the integer.
* @param[in] maxVal The maximum value of the integer.
* @return The function return the number of the choice.
*/
int getInt(int minVal, int maxVal);

/**
 * Get a boolean choice (Y/N).
 * This function prompts the user a question and it only accept Y or N
 * (both upper case or lower case) as input.
 * @param[in] messageToPrint The message to show the user (usually a question).
 * @return The inserted choice (in upper case).
 */
char getChoice(const char* messageToPrint);

/**
 * Check a string using a regular expression.
 *
 * @param[in] regExp The regular expression.
 * @param[in] stringToCheck The string to check.
 * @return true if there are matches, false if there aren't.
 */
bool checkWithRegEx(const char* regExp, const char* stringToCheck);

/**
* Check if a date is valid.
*
* @param[in] date Pointer to the date that has to be checked.
* @return The function returns 1 if the date is valid.
* If it isn't, the function will return 0.
*/
int isDateValid(Date *date);

/**
* Insert a date;
* Prompts the user to insert a date and checks if the given value is valid.
* @return The inserted date.
*/
Date getDate();

/**
 * Check if an year is leap.
 *
 * @param year The year that should be checked.
 * @return 1 if the year is leap, 0 if it isn't.
 */
int isLeapYear(int year);

/**
* Insert a name;
* Prompts the user to insert a name and checks if the given value is valid.
* @param[out] name String to where the name will be saved.
* @param[in] typeOfName A string that will be printed while asking for input (usually "name" or "surname").
*/
void getName(char *name, const char *typeOfName);

/**
* Insert an email.
* Prompts the user to insert an email and checks if the given value is valid.
* @param[out] mail String where the mail will be saved.
*/
void getEmail(char *mail);

#endif
