/**
 * @file Song.c
 * Implementation of the functions from Song.h.
 
 * This file contains the implementation of the function defined
 * in Song.h. It uses the standard C libraries.
 * @version 0.1
 * @date 08/may/2018
 * @authors Andrea Esposito, Graziano Montanaro
 * @copyright No copyright informations provided.
 */

#include "Song.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

///This function updates the songs' file.
void modifySongsFile(Song songs[], int size)
{
	FILE* songsFile;

	if((songsFile = fopen("files/bin/songsList.csv", "w")) != NULL)
	{
		fprintf(songsFile, "codice,nome,album,artista,num_traccia,minuti,secondi,genere\n");

		for(int i = 0; i < size; i++)
		{
			fprintf(songsFile, "%s,%s,%s,%s,%d,%d,%d,%s\n",
				(songs + i)->code,
				(songs + i)->name,
				(songs + i)->albumName,
				(songs + i)->artist,
				(songs + i)->trackNumber,
				(songs + i)->duration.min,
				(songs + i)->duration.sec,
				getMusicGenreName((songs + i)->genre));
		}

		fclose(songsFile);
	}
	else //if we couldn't open or create the file, print an error message and close the program.
	{
		printf("[404] ERRORE! IMPOSSIBILE MODIFICARE IL FILE DEI BRANI (files/bin/songsList.csv)\n");
		system("pause");
		fflush(stdin);
		exit(404);
	}
}

/**
* This function adds a given song to the .csv user file.
* It uses a pointer to the song that it should save
* for optimization purposes.
*
*/
void addToSongsFile(Song *song)
{
	int creatingFile = 0;
	FILE* songsFile;

	creatingFile = !doesFileExists("files/bin/songsList.csv");

	if((songsFile = fopen("files/bin/songsList.csv", "a")) != NULL)
	{
		//if we are creating the file we must set the header of the .csv
		if(creatingFile)
			fprintf(songsFile, "codice,nome,album,artista,num_traccia,minuti,secondi,genere\n");

		fprintf(songsFile, "%s,%s,%s,%s,%d,%d,%d,%s\n",
				song->code,
				song->name,
				song->albumName,
				song->artist,
				song->trackNumber,
				song->duration.min,
				song->duration.sec,
				getMusicGenreName(song->genre));

		fclose(songsFile);
	}
	else //if we couldn't open or create the file, print an error message and close the program.
	{
		printf("[404] ERRORE! IMPOSSIBILE MODIFICARE IL FILE DEI BRANI (files/bin/songsList.csv)\n");
		system("pause");
		fflush(stdin);
		exit(404);
	}
}

/**
 * This function searches all songs of an artist and it prints them on screen.
 */
void printAlbumsOfArtist(Song songs[], int currentSongsNumber)
{
	int finalsize=0;
	Song** punt=searchSongByArtist(songs,currentSongsNumber,&finalsize,"");
	printf("Gli album di questo artista sono:\n");
	char* album[finalsize];
	int duplicated=0;
	for(int i=0;i<finalsize;i++)
		album[i]=punt[i]->albumName;

	for(int i=0;i<finalsize;i++)
	{
		for(int j=i+1;j<finalsize;j++)
		{
			if(!strcmp(album[j],punt[i]->albumName) )
			{
				duplicated=1;
				break;
			}
		}
		if(duplicated==0)
			printf("\t- %s \n",punt[i]->albumName);
		duplicated=0;
	}

	free(punt);
}

/**
 * Search for songs.
 * Based on the value of `researchType`, this function searches by different properties
 * of a song. To use this function properly, please refer to this table.
 *
 * | `researchType` | Search by... |
 * |:--------------:|:------------:|
 * |        1       |     code     |
 * |        2       |     title    |
 * |        3       |    artist    |
 * |        4       |     album    |
 * |        5       |     genre    |
 */
Song** searchSong(Song song[], int size, int *finalSize, unsigned short int researchType, const char* researched)
{
	Song** (*searchFunction)(Song[], int, int*, const char*);

	switch(researchType)
	{
	case 1:
		searchFunction = searchSongByCode;
		break;
	case 2:
		searchFunction = searchSongByTitle;
		break;
	case 3:
		searchFunction = searchSongByArtist;
		break;
	case 4:
		searchFunction = searchSongByAlbum;
		break;
	case 5:
		searchFunction = searchSongByGenre;
		break;
	default:
		return NULL;
		break;
	}

	return searchFunction(song, size, finalSize, researched);
}

/**
 * Search for the song with a given code.
 * If `toSearch` is an empty string, the function will ask the
 * user to insert a code that will be searched. Otherwise,
 * it will use the value of `toSearch` as key.
 */
Song** searchSongByCode(Song song[], int size, int *finalSize, const char* toSearch)
{
	string researched;
	Song** punt = NULL;
	if(!strcmp(toSearch, ""))
	{
		printf("Inserire il codice del brano ricercato: ");
		gets(researched);
	}
	else
		strcpy(researched, toSearch);

	strupr(researched);

	for(int i=0;i<size;i++)
		if(strcmp(song[i].code, researched) == 0)
		{
			punt = (Song**)calloc(1,sizeof(Song*));
			*punt = &song[i];
			break;
		}
	if(finalSize)
		*finalSize = 1;
	return punt;
}

/**
 * Search for songs with a given title.
 * If `toSearch` is an empty string, the function will ask the
 * user to insert a title that will be searched. Otherwise,
 * it will use the value of `toSearch` as key.
 */
Song** searchSongByTitle(Song song[], int size, int *finalSize, const char* toSearch)
{
	string researched;
	Song** punt = NULL;

	if(!strcmp(toSearch, ""))
	{
		printf("Inserire il titolo del brano ricercato: ");
		gets(researched);
	}
	else
		strcpy(researched, toSearch);

	strupr(researched);

	string name;

	short unsigned int positions[size];
	int sameName=0;
	for(int i=0;i<size;i++)
	{
		strcpy(name, song[i].name);
		strupr(name);
		if(strcmp(name, researched) == 0)
		{
			positions[sameName]=i;
			sameName++;
		}
	}
	if(sameName)
	{
		punt = (Song**)calloc(sameName,sizeof(Song*));
		for(int i=0;i<sameName;i++)
			*(punt + i) = &song[positions[i]];
	}
	if(finalSize)
		*finalSize = sameName;
	return punt;
}

/**
 * Search for songs of a given artist.
 * If `toSearch` is an empty string, the function will ask the
 * user to insert an artist that will be searched. Otherwise,
 * it will use the value of `toSearch` as key.
 */
Song** searchSongByArtist(Song song[], int size, int *finalSize, const char* toSearch)
{
	string researched;
	Song** punt = NULL;

	if(!strcmp(toSearch, ""))
	{
		printf("Inserire l'artista del brano ricercato: ");
		gets(researched);
	}
	else
		strcpy(researched, toSearch);

	strupr(researched);

	string artist;

	short unsigned int positions[size];
	int sameName=0;
	for(int i=0;i<size;i++)
	{
		strcpy(artist, song[i].artist);
		strupr(artist);
		if(strcmp(artist, researched) == 0)
		{
			positions[sameName]=i;
			sameName++;
		}
	}
	if(sameName)
	{
		punt = (Song**)calloc(sameName,sizeof(Song*));
		for(int i=0;i<sameName;i++)
			*(punt + i) = &song[positions[i]];
	}
	if(finalSize)
		*finalSize = sameName;
	return punt;
}

/**
 * Search for songs of a given album.
 * If `toSearch` is an empty string, the function will ask the
 * user to insert an album that will be searched. Otherwise,
 * it will use the value of `toSearch` as key.
 */
Song** searchSongByAlbum(Song song[], int size, int *finalSize, const char* toSearch)
{
	string researched;
	Song** punt = NULL;

	if(!strcmp(toSearch, ""))
	{
		printf("Inserire l'album del brano ricercato: ");
		gets(researched);
	}
	else
		strcpy(researched, toSearch);

	strupr(researched);

	string album;

	short unsigned int positions[size];
	int sameName=0;
	for(int i=0;i<size;i++)
	{
		strcpy(album, song[i].albumName);
		strupr(album);
		if(strcmp(album, researched) == 0)
		{
			positions[sameName]=i;
			sameName++;
		}
	}
	if(sameName)
	{
		punt = (Song**)calloc(sameName,sizeof(Song*));
		for(int i=0;i<sameName;i++)
			*(punt + i) = &song[positions[i]];
	}
	if(finalSize)
		*finalSize = sameName;
	return punt;
}

/**
 * Search for songs of a given genre.
 * If `toSearch` is an empty string, the function will ask the
 * user to insert a genre that will be searched. Otherwise,
 * it will use the value of `toSearch` as key.
 *
 * @pre toSearch must bean empty string or a valid genre.
 */
Song** searchSongByGenre(Song song[], int size, int *finalSize, const char* toSearch)
{
	Genre researched;
	Song** punt = NULL;

	if(!strcmp(toSearch, ""))
	{
		researched = getGenre();
	}
	else
	{
		string toSearchCopy;
		strcpy(toSearchCopy, toSearch);
		researched = getMusicGenreIndex(toSearchCopy);
	}

	short unsigned int positions[size];
	int sameName=0;
	for(int i=0;i<size;i++)
	{
		if(researched == song[i].genre)
		{
			positions[sameName]=i;
			sameName++;
		}
	}
	if(sameName)
	{
		punt = (Song**)calloc(sameName,sizeof(Song*));
		for(int i=0;i<sameName;i++)
			*(punt + i) = &song[positions[i]];
	}
	if(finalSize)
		*finalSize = sameName;
	return punt;
}

/**
 * Print an array of songs.
 * This function prints an array of songs using the function `printsongToScreen`.
 */
void printSongsArray(Song* songs[], int size)
{
	if(size > 0)
	{
		for(int i = 0; i < size; i++)
		{
			printf("- ");
			printSongToScreen(*(songs + i));
			if((i + 1) % 4 == 0)
			{
				system("pause");
				fflush(stdin);
			}
		}
	}
	else
		printf("\tNessun brano trovato\n");
}

/**
 * Print a song to screen.
 * This function prints all values of a song to screen.
 */
void printSongToScreen(Song *song)
{
	printf("CODICE: %s\n"
			"\tTITOLO: %s\n"
			"\tARTISTA: %s\n"
			"\tALBUM: %s\n"
			"\tNUMERO TRACCIA: %d\n"
			"\tDURATA: %d:%d\n"
			"\tGENERE: %s\n",
			song->code,
			song->name,
			song->artist,
			song->albumName,
			song->trackNumber,
			song->duration.min, song->duration.sec,
			getMusicGenreName(song->genre));
}

/**
* This function loads songs' data from a file.
* The informations are stored in an array (a void array,
* so that it can be used for various data structures).
*
* @pre the file should exist.
* @post If the file doesn't exist the function will close the program returning an error.
*/
int loadSongsFromFile(const char* path, Song array[], int maxSize)
{
	int currentSize;
	FILE* inputFile;

	if(doesFileExists(path))
	{
		printf("Caricamento brani da file...\n");
		currentSize = 0;

		inputFile = fopen(path, "r");

		char line[5 * MAX_STRING_LENGTH + MAX_SONGS_NUMBER * MAX_CODE_LENGTH + 21]; //exact maximum length
		fgets(line, sizeof(line), inputFile);

		while(fgets(line, sizeof(line), inputFile) && currentSize < maxSize)
		{
			strtok(line, "\n");

			char *token = strtok(line, ",");

			strcpy(array[currentSize].code, token);
			strcpy(array[currentSize].name, strtok(NULL, ","));
			strcpy(array[currentSize].albumName, strtok(NULL, ","));
			strcpy(array[currentSize].artist, strtok(NULL, ","));
			array[currentSize].trackNumber = atoi(strtok(NULL, ","));
			array[currentSize].duration.min = atoi(strtok(NULL, ","));
			array[currentSize].duration.sec = atoi(strtok(NULL, ","));
			array[currentSize].genre = getMusicGenreIndex(strtok(NULL, ","));

			currentSize++;
		}
		fclose(inputFile);
		system("cls");
	}
	else
	{
		printf("[404] ERRORE! NON E' POSSIBILE APRIRE IL FILE CONTENENTE I BRANI (%s)\n", path);
		system("pause");
		fflush(stdin);
		exit(404); //RESOURCE NOT FOUND
	}

	return currentSize;
}


/**
 * This function asks the user if he/she wants to search a song by title or by code, then it searches for a song.
 */
void searchForSongs(Song songs[], int songsNumber)
{
	printf("Si vuole ricercare per:\n"
			"\t 1- Codice\n"
			"\t 2- Titolo\n"
			"Inserisci una scelta: ");

	int researchType = getInt(1,2);

	int numberOfFoundSongs;
	Song** foundSongs = searchSong(songs, songsNumber, &numberOfFoundSongs, researchType, "");
	if(foundSongs == NULL)
	{
		printf("-----------------------------------------------------------------------------------------------------------------------\n"
				"Nessun riscontro con i criteri inseriti.\n"
				"-----------------------------------------------------------------------------------------------------------------------\n");
	}
	else
		printSongsArray(foundSongs, numberOfFoundSongs);

	free(foundSongs);
}

/**
 * This function prints the whole catalog to screen.
 */
void printCatalog(Song songs[], int songsNumber)
{
	Song** songsPointers = (Song**) calloc(songsNumber, sizeof(Song*));
	for(int i = 0; i < songsNumber; i++)
		songsPointers[i] = songs + i;


	printSongsArray(songsPointers, songsNumber);


}

/**
 * This function prints all songs of an album.
 */
void showSongsOfAlbum(Song songs[], int currentSongsNumber)
{
	int numberOfFoundSongs;
	Song** foundSongs = searchSong(songs, currentSongsNumber, &numberOfFoundSongs, 4, "");
	quicksort(foundSongs, 0, numberOfFoundSongs - 1, 1);
	printSongsArray(foundSongs, numberOfFoundSongs);
	free(foundSongs);
}

/**
* This function inserts a duration.
*/
void insertDuration(struct Duration *duration)
{
	char confirm;
	do
	{
		printf("Inserire la durata (nel formato minuti:secondi): ");
		int isFirstTimeInput = 1;
		do
		{
			if(!isFirstTimeInput)
				printf("Inserire una durata valida: ");
			scanf("%d:%d", &(duration->min), &(duration->sec));
			fflush(stdin);
			while(duration->sec >= 60)
			{
				duration->min++;
				duration->sec -= 60;
			}
			isFirstTimeInput = 0;
		} while(duration->min < 0 || duration->sec < 0);
		printf("Si e' immessa la durata [%d:%d]. ", duration->min, duration->sec);
		confirm = getChoice("Confermare?");
	} while(confirm == 'N');
}

/**
* This function sorts an array of integers using the quicksort algorithm
* (for more basic informations about this or other types of sorting algorithms,
* you can visit <a href="https://it.wikipedia.org/wiki/Quicksort" target="_blank">Wikipedia</a>).
*
* @pre array is the array that has to be ordered (pointer to the first integer).
* @pre inf has to be 0 at the first call.
* @pre sup has to be the size of array decremented of one (using pseudo-code: `array.size - 1`).
*/
void quicksort(Song** array, int inf, int sup, int sortBy)
{
	int (*partition)(Song**, int, int);
	switch(sortBy)
	{
	case 0:
		partition = partitionByDuration;
		break;
	case 1:
		partition = partitionByTrackNumber;
		break;
	default:
		break;
	}
	if((sup - inf) >= 1)
	{
		int pos = partition(array, inf, sup);
		if((pos - inf) < (sup - pos + 1))
		{
			quicksort(array, inf, pos - 1, sortBy);
			quicksort(array, pos + 1, sup, sortBy);
		}
		else
		{
			quicksort(array, pos + 1, sup, sortBy);
			quicksort(array, inf, pos - 1, sortBy);
		}
	}
}

/**
* This function is used by the <a href="https://it.wikipedia.org/wiki/Quicksort" target="_blank">quicksort</a>
* algorithm, that sorts an array of Songs based on their duration.
*
* @pre array is the array that has to be ordered (pointer to the first integer).
* @pre inf has to be 0 at the first call.
* @pre sup has to be the size of array decremented of one (using pseudo-code: `array.size - 1`).
*/
int partitionByDuration(Song** array, int inf, int sup)
{
	int med = (inf + sup) / 2;
	struct Duration x = array[med]->duration;

	swap(array + inf, array + med);

	int i = inf;
	int j = sup;
	while (i < j)
	{
		while(i <= sup && (array[i]->duration.min >= x.min || (array[i]->duration.min == x.min && array[i]->duration.sec >= x.sec)))
			i++;
		while(array[j]->duration.min < x.min || (array[j]->duration.min == x.min && array[j]->duration.sec < x.sec))
			j--;
		if(i < j)
			swap(array + i, array + j);
	}

	swap(array + inf, array + j);

	return j;
}

/**
* This function is used by the <a href="https://it.wikipedia.org/wiki/Quicksort" target="_blank">quicksort</a>
* algorithm, that sorts an array of Songs based on their track number in an album.
*
* @pre array is the array that has to be ordered (pointer to the first integer).
* @pre inf has to be 0 at the first call.
* @pre sup has to be the size of array decremented of one (using pseudo-code: `array.size - 1`).
*/
int partitionByTrackNumber(Song** array, int inf, int sup)
{
	int med = (inf + sup) / 2;
	int x = array[med]->trackNumber;

	swap(array + inf, array + med);

	int i = inf;
	int j = sup;
	while (i < j)
	{
		while(i <= sup && (array[i]->trackNumber <= x || (array[i]->trackNumber == x && array[i]->trackNumber <= x)))
			i++;
		while(array[j]->trackNumber > x || (array[j]->trackNumber == x && array[j]->trackNumber > x))
			j--;
		if(i < j)
			swap(array + i, array + j);
	}

	swap(array + inf, array + j);

	return j;
}

/**
* This function swaps two given song's pointers.
*/
void swap(Song** a, Song** b)
{
	Song* t;
	t = *a;
	*a = *b;
	*b = t;
}

/**
 * This function checks if a given string is formatted like
 * a song code. These codes are formatted like `AB123`.
 *
 * @pre code should be a valid string.
 */
bool isCodeValid(const char* code)
{
	const char* codeRegex = "^[A-Z]{2}[0-9]{3}$";

	return checkWithRegEx(codeRegex, code);
}

/**
* This function inserts a code. It waits for an input and if the
* given string is a valid code (if it is formatted
* like `AB123`), the function checks if the given code is
* already present in the songs list.
*/
void insertCode(Song* songList, int size, char *code)
{
	int isFirstTimeInput = 1;	//Used for error messages
	Song** songWithGivenCode = NULL;

	do
	{
		if(!isFirstTimeInput && songWithGivenCode == NULL)
			printf("Inserire un codice valido! Re");
		else if(!isFirstTimeInput)
			printf("Codice gia' in uso! Re");

		printf("Inserire un codice (2 lettere + 3 cifre): ");
		scanf("%5s", code);

		fflush(stdin);

		strupr(code);

		isFirstTimeInput = 0;
	} while((songWithGivenCode = searchSong(songList, size, NULL, 1, code)) != NULL ||
			!isCodeValid(code));
}
