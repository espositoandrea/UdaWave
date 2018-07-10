/**
* @file Song.h
* Functions to manipulate songs.
*
* This library contains functions that work on songs and songs' arrays,
* Along with the definitions of songs-related data structures.
* Some of this functions are based on functions defined in Utilities.h.
* @version 0.1
* @date 08/may/2018
* @authors Andrea Esposito, Graziano Montanaro
* @copyright No copyright informations provided.
*/


#ifndef SONG_H
#define SONG_H

#include "../Utilities/Utilities.h"

/**
 * The maximum number of songs the software can manage.
 */
#define MAX_SONGS_NUMBER 50

/**
 * A song.
 *
 * This data structure contains all the informations about a song.
 */
typedef struct Song
{
	char code[MAX_CODE_LENGTH + 1];		///< The code of the song.
    string name;						///< The name of the song.
    string albumName;					///< The name of the album of the song.
    string artist;						///< The artist of the song.
    int trackNumber;					///< The track number of the song in the album.
     /**
      * A duration.
      *
      * This data structure contains the duration of a song.
      */
    struct Duration
    {
    	int min;						///< The minutes of the song's duration.
    	int sec;						///< The seconds of the song's duration.
    } duration;							///< The duration of the song.

    Genre genre;						///< The music genre of the song.
} Song;

/**
 * Search songs.
 * @param[in] song The list of songs.
 * @param[in] size The size of the list of songs.
 * @param[out] finalSize Where to store the number of found songs.
 * @param[in] researchType The type of search that will be used (by title, by code, etc.).
 * @param[in] researched The search parameter (it can be an empty string).
 * @return An array of pointer to songs.
 */
Song** searchSong(Song song[], int size, int *finalSize, unsigned short int researchType, const char* researched);

/**
 * Search a song by code.
 * @param[in] song The list of songs.
 * @param[in] size The size of the list of songs.
 * @param[out] finalSize Where to store the number of found songs.
 * @param[in] toSearch The search parameter.
 * @return An array of pointer to songs.
 */
Song** searchSongByCode(Song song[], int size, int *finalSize, const char* toSearch);

/**
 * Search songs by title.
 * @param[in] song The list of songs.
 * @param[in] size The size of the list of songs.
 * @param[out] finalSize Where to store the number of found songs.
 * @param[in] toSearch The search parameter.
 * @return An array of pointer to songs.
 */
Song** searchSongByTitle(Song song[], int size, int *finalSize, const char* toSearch);

/**
 * Search songs by artist.
 * @param[in] song The list of songs.
 * @param[in] size The size of the list of songs.
 * @param[out] finalSize Where to store the number of found songs.
 * @param[in] toSearch The search parameter.
 * @return An array of pointer to songs.
 */
Song** searchSongByArtist(Song song[], int size, int *finalSize, const char* toSearch);

/**
 * Search songs by album.
 * @param[in] song The list of songs.
 * @param[in] size The size of the list of songs.
 * @param[out] finalSize Where to store the number of found songs.
 * @param[in] toSearch The search parameter.
 * @return An array of pointer to songs.
 */
Song** searchSongByAlbum(Song song[], int size, int *finalSize, const char* toSearch);

/**
 * Search songs by genre.
 * @param[in] song The list of songs.
 * @param[in] size The size of the list of songs.
 * @param[out] finalSize Where to store the number of found songs.
 * @param[in] toSearch The search parameter.
 * @return An array of pointer to songs.
 */
Song** searchSongByGenre(Song song[], int size, int *finalSize, const char* toSearch);

/**
 * Print an array of songs.
 * @param[in] songs The array of songs.
 * @param[in] size The size of the array of songs.
 */
void printSongsArray(Song* songs[], int size);

/**
 * Print a song to screen.
 * @param[in] song The song that will be printed.
 */
void printSongToScreen(Song *song);

/**
 * Search for a song.
 * @param[in] songs The list of songs.
 * @param[in] songsNumber The size of the list of songs.
 */
void searchForSongs(Song songs[], int songsNumber);

/**
 * Print all songs in the catalog
 * @param[in] songs The list of songs.
 * @param[in] songsNumber The size of the list of songs.
 */
void printCatalog(Song songs[], int songsNumber);

/**
 * Print all songs of an album
 * @param[in] songs The list of songs.
 * @param[in] currentSongsNumber The size of the list of songs.
 */
void showSongsOfAlbum(Song songs[], int currentSongsNumber);

/**
 * Print all albums of an artist.
 * @param[in] songs The list of songs.
 * @param[in] currentSongsNumber The size of the list of songs.
 */
void printAlbumsOfArtist(Song songs[], int currentSongsNumber);

/**
 * Check if a string is formatted like a code.
 * @param[in] code The string to check.
 * @return true if it is formatted like a code, false otherwise.
 */
bool isCodeValid(const char* code);

/**
* Insert the code of a song.
* 
* @param[in] songList A list of songs.
* @param[in] size The number of songs in the list.
* @param[out] code The code that will be inserted.
*/
void insertCode(Song* songList, int size, char *code);

/**
* Load songs' datas from file.
*
* @param[in] path The path of the file.
* @param[in] array The array where datas should be saved.
* @param[in] maxSize The maximum size of the array.
* @return The function returns the number of read elements. It return -1 in case of any error.
*/
int loadSongsFromFile(const char* path, Song array[], int maxSize);

/**
* Insert a duration;
*
* @param[in] duration Pointer to where the duration will be saved.
*/
void insertDuration(struct Duration *duration);

/**
* Add a song to the songs' .csv file.
*
* @param[in] song Song to add to the file.
*/
void addToSongsFile(Song *song);

/**
 * Update users' file.
 *
 * @param[in] songs The array of users.
 * @param[in] size The size of the users' array.
 */
void modifySongsFile(Song songs[], int size);

/**
* Sort an array
* This function sorts an array of Songs.
*
* @param[in] array Array that has to be ordered (pointer to the first Song).
* @param[in] inf Inferior index.
* @param[in] sup Superior index.
* @param[in] sortBy Choose 0 if it has to sort by duration, 1 if it has to sort by track number.
* @return void.
*/
void quicksort(Song** array, int inf, int sup, int sortBy);

/**
* Sort a partition of an array
* This function is used to sort a partition of an array of Songs.
*
* @param[in] array Array that has to be ordered (pointer to the first integer).
* @param[in] inf Inferior index.
* @param[in] sup Superior index.
* @return pivot's new position (after the execution of the function).
*/
int partitionByDuration(Song** array, int inf, int sup);

/**
* Sort a partition of an array
* This function is used to sort a partition of an array of Songs.
*
* @param[in] array Array that has to be ordered (pointer to the first integer).
* @param[in] inf Inferior index.
* @param[in] sup Superior index.
* @return pivot's new position (after the execution of the function).
*/
int partitionByTrackNumber(Song** array, int inf, int sup);

/**
* Swap two song's pointers.
* This function swaps two given integers.
*
* @param[in] a First pointer to swap with second.
* @param[in] b Second pointer to swap with first.
*/
void swap(Song** a, Song** b);

#endif
