/*
 ============================================================================
 Name        : UdaWave
 Author      : Andrea Esposito (677021) & Graziano Montanaro (677909)
 Version     : 0.0.1
 A.A.        : 2017/2018
 Description : Caso di studio - Traccia 3 (Musica on Demand)
 	 	 	   	   	   Laboratorio di Informatica
 ============================================================================
 */
 
/**
 * @file UdaWave.c
 * Main file of the project.
 *
 * This file contains the main function of the project, along
 * with some functions that use types and functions defined in
 * external libraries. It also contains some functions to customize
 * the O.S. console.
 * @version 0.1
 * @date A.A. 2017/2018
 * @authors Andrea Esposito, Graziano Montanaro
 * @copyright No copyright informations provided.
 */


/*!
 * @mainpage UdaWave
 *
 * \section authors Authors
 * |   Name   |  Surname  |
 * |:--------:|:---------:|
 * |  Andrea  |  Esposito |
 * | Graziano | Montanaro |
 *
 * We are two **I.C.T. students** from *Taranto*. This software and documentation has been created
 * as part of an exam.
 *
 * \section project_info The Project
 * We have chosen to create a software for *on demand music*. It has some of
 * the basic functions of other apps (like *Spotify*), except for the music itself (due to
 * copyright reasons).
 * \subsection links Important Links
 * Here are some important links regarding this project.
 * - <a href="downloadable/Documentazione.pdf" download>Project Documentation (**ITA**)</a>
 * - <a href="https://github.com/mineand99/UdaWave">Source code (**GitHub**)</a>
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "modules/Song/Song.h"
#include "modules/User/User.h"
#include "modules/Testing/Testing.h"
#include "resources/resources.h"

/**
 * The name of the project.
 *
 * This constant holds the name of the project, which is used by
 * some functions to print it to the screen.
 */
const char* APP_NAME = "UdaWave";

/**
 * Add a song.
 *
 * @param[in] songs Array of songs.
 * @param[in] currentSize Pointer to the current dimension of the array.
 * @param[in] currentLoggedUser
 * @return The function returns the new dimension of the array if it succeed. If it fails, it returns -1.
 *
 */
int addSong(Song songs[], int *currentSize,  User * const *currentLoggedUser);


/**
* Shows on screen a choice menu.
*
* @return The function returns the number of the choice.
*/
int menu(User **currentLoggedUser);

/**
 * Set the console title.
 *
 * @param[in] newTitle The new title of the console.
 */
void setConsoleTitle(const char* newTitle);

/**
 * Entry point of the program.
 *
 * This is the entry point of the program. It creates the console window (and set its
 * properties) and it defines all the important variables that will be used throughout
 * the program.
 *
 * @return If everything went well, it returns EXIT_SUCCESS (0)
 */
int main(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);

	system("color 0a");
	setConsoleTitle(APP_NAME);

	Song songs[MAX_SONGS_NUMBER];
	int currentSongsNumber = loadSongsFromFile("files/bin/songsList.csv", songs, MAX_SONGS_NUMBER);

	User users[MAX_USERS_NUMBER];
	int currentUsersNumber = loadUsersFromFile("files/bin/usersList.csv", songs, currentSongsNumber, users, MAX_USERS_NUMBER);

	loadPlaylistsFromFile(users, currentUsersNumber, songs, currentSongsNumber);

	User *currentLoggedUser = NULL;

	int choice;

	do
	{
		choice = menu(&currentLoggedUser);

		switch(choice)
		{
		case 1: //Stampa catalogo
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"\tCATALOGO:\n"
					"-----------------------------------------------------------------------------------------------------------------------\n");
			printCatalog(songs, currentSongsNumber);
			printf("-----------------------------------------------------------------------------------------------------------------------\n");
			system("pause");
			fflush(stdin);
			break;
		case 2: //Stampa lista utenti
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"\tLISTA DEGLI UTENTI:\n"
					"-----------------------------------------------------------------------------------------------------------------------\n");
			system("cls");
			User** usersPointers = (User**) calloc(currentUsersNumber, sizeof(User*));
			for(int i = 0; i < currentUsersNumber; i++)
				usersPointers[i] = users + i;

			printUsersArray(usersPointers, currentUsersNumber);
			printf("-----------------------------------------------------------------------------------------------------------------------\n");

			system("pause");
			fflush(stdin);
			break;
		case 3:
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"\tLOGIN\n"
					"-----------------------------------------------------------------------------------------------------------------------\n");
			currentLoggedUser = login(users, currentUsersNumber);
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"Benvenuto, %s\n"
					"-----------------------------------------------------------------------------------------------------------------------\n",
					currentLoggedUser->name);
			system("pause");
			fflush(stdin);
			break;
		case 4: //Aggiunta utente
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"\tINSERIMENTO NUOVO UTENTE\n"
					"-----------------------------------------------------------------------------------------------------------------------\n");
			addUser(users, &currentUsersNumber, &currentLoggedUser);
			printf("-----------------------------------------------------------------------------------------------------------------------\n");
			system("pause");
			fflush(stdin);
			break;
		case 5:
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"\t\tLOGOUT\n"
					"-----------------------------------------------------------------------------------------------------------------------\n");
			logout(&currentLoggedUser);
			printf("-----------------------------------------------------------------------------------------------------------------------\n");
			system("pause");
			fflush(stdin);
			break;
		case 6: //Aggiunta canzone
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"\tINSERIMENTO BRANI\n"
					"-----------------------------------------------------------------------------------------------------------------------\n");
			addSong(songs, &currentSongsNumber, &currentLoggedUser);
			printf("-----------------------------------------------------------------------------------------------------------------------\n");
			system("pause");
			fflush(stdin);
			break;
		case 7: //Stampa dei brani di un particolare genere (ordinati per durata)
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"\tBRANI PREFERITI PER GENERE\n"
					"-----------------------------------------------------------------------------------------------------------------------\n");
			printSongsOfGenre(users, currentUsersNumber, &currentLoggedUser);
			printf("-----------------------------------------------------------------------------------------------------------------------\n");
			system("pause");
			fflush(stdin);
			break;
		case 8:
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"\tSALVATAGGIO PLAYLIST\n"
					"-----------------------------------------------------------------------------------------------------------------------\n");
			savePlaylistsToFile(users, currentUsersNumber, &currentLoggedUser);
			printf("-----------------------------------------------------------------------------------------------------------------------\n");
			system("pause");
			fflush(stdin);
			break;
		case 9: //Ricerca di un brano
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"\tRICERCA BRANO\n"
					"-----------------------------------------------------------------------------------------------------------------------\n");
			searchForSongs(songs, currentSongsNumber);
			printf("-----------------------------------------------------------------------------------------------------------------------\n");
			system("pause");
			fflush(stdin);
			break;
		case 10:
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"\tMODIFICA UTENTE\n"
					"-----------------------------------------------------------------------------------------------------------------------\n");
			modifyUser(users, currentUsersNumber, &currentLoggedUser);
			printf("-----------------------------------------------------------------------------------------------------------------------\n");
			system("pause");
			fflush(stdin);
			break;
		case 11:
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"\tMODIFICA PREFERITI\n"
					"-----------------------------------------------------------------------------------------------------------------------\n");
			modifyFavourites(&currentLoggedUser ,users, currentUsersNumber, songs, currentSongsNumber);
			printf("-----------------------------------------------------------------------------------------------------------------------\n");
			system("pause");
			fflush(stdin);
			break;
		case 12:
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"\tMOSTRA CANZONI IN UN ALBUM\n"
					"-----------------------------------------------------------------------------------------------------------------------\n");
			showSongsOfAlbum(songs, currentSongsNumber);
			printf("-----------------------------------------------------------------------------------------------------------------------\n");
			system("pause");
			fflush(stdin);

			break;
		case 13:
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"\tMOSTRA GLI ALBUM DI UN ARTISTA\n"
					"-----------------------------------------------------------------------------------------------------------------------\n");
			printAlbumsOfArtist(songs, currentSongsNumber);
			printf("-----------------------------------------------------------------------------------------------------------------------\n");
			system("pause");
			fflush(stdin);

			break;
		case 14:
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"\tMODIFICA PLAYLISTS\n"
					"-----------------------------------------------------------------------------------------------------------------------\n"
					"\t1) Aggiungi una tua playlist\n"
					"\t2) Modifica una tua playlist (rinomina e cambia la privacy)\n"
					"\t3) Rimuovi una tua playlist\n"
					"\t4) Aggiungi un brano a una tua playlist\n"
					"\t5) Rimuovi un brano da una tua playlist\n"
					"Inserisci una scelta: ");
			int x = getInt(1, 5);
			switch(x)
			{
			case 1:
				addPlaylist(currentLoggedUser, songs, currentSongsNumber);
				break;
			case 2:
				modifyPlaylist(currentLoggedUser, songs, currentSongsNumber);
				break;
			case 3:
				removePlaylist(currentLoggedUser, songs, currentSongsNumber);
				break;
			case 4:
				addSongToPlaylist(currentLoggedUser, songs, currentSongsNumber);
				break;
			case 5:
				removeSongFromPlaylist(currentLoggedUser, songs, currentSongsNumber);
				break;
			default:
				break;
			}
			break;
		case 15:
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"\tPLAYLIST PUBBLICHE\n"
					"-----------------------------------------------------------------------------------------------------------------------\n");
			printPublicPlaylistsToScreen(users, currentUsersNumber, currentLoggedUser);
			printf("-----------------------------------------------------------------------------------------------------------------------\n");
			system("pause");
			fflush(stdin);
			break;
		case 16: //Mostra le playlist
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"\tLISTA PLAYLIST\n"
					"-----------------------------------------------------------------------------------------------------------------------\n");
			printPlaylistsToScreen(currentLoggedUser);
			printf("-----------------------------------------------------------------------------------------------------------------------\n");
			system("pause");
			fflush(stdin);
			break;
		case 17:
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"\tTESTING\n"
					"-----------------------------------------------------------------------------------------------------------------------\n");
			int CU_ERROR = testingMain();
			if(CU_ERROR)
				printf("\n!!! CUnit closed with error code %d !!!\n", CU_ERROR);
			printf("\n");
			system("pause");
			fflush(stdin);
			break;
		default:
			for(int i = 0; i < currentUsersNumber; i++)
				writePlaylistToFile(users + i);
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"\tARRIVEDERCI!\n"
					"-----------------------------------------------------------------------------------------------------------------------\n");
			system("pause");
			break;
		}
	}
	while(choice!=0);

	return EXIT_SUCCESS;
}



/**
* This function adds a user and it saves its datas in a file.
*
* @pre currentSize should be less than the maximum number of users
* @post If currentSize is greater than the maximum number of users, the function 
* fails and it returns -1.
*/
int addSong(Song songs[], int *currentSize, User * const *currentLoggedUser)
{
	if(*currentLoggedUser != NULL)
	{
		if(*currentSize < MAX_SONGS_NUMBER && !strcmp((*currentLoggedUser)->nickname, "admin"))
		{
			char choice = '\0';
			do
			{
				if(choice != '\0')
					printf("-----------------------------------------------------------------------------------------------------------------------\n");
				//Inserting the song code
				insertCode(songs, *currentSize, songs[*currentSize].code);

				//Inserting the song name
				printf("Inserire il nome del brano: ");
				gets(songs[*currentSize].name);

				//Inserting the song artist
				printf("Inserire il nome dell'artista: ");
				gets(songs[*currentSize].artist);

				//Inserting the song album name
				printf("Inserire il nome dell'album: ");
				gets(songs[*currentSize].albumName);

				//Inserting the song track number
				printf("Inserire il numero di traccia: ");
				songs[*currentSize].trackNumber = getInt(0,999);

				//Inserting the song track number
				insertDuration(&(songs[*currentSize].duration));

				//Inserting music genre
				songs[*currentSize].genre = getGenre();

				addToSongsFile(&songs[*currentSize]);

				printf("-----------------------------------------------------------------------------------------------------------------------\n");

				choice = getChoice("Si vuole inserire un altro brano?");

			} while(*currentSize < MAX_SONGS_NUMBER && choice == 'Y');

			return ++(*currentSize);
		}
		else if(*currentLoggedUser != NULL || strcmp((*currentLoggedUser)->nickname, "admin"))
		{
			printf("Impossibile aggiungere brani: non si hanno i privilegi da amministratore.\n");
		}
		else
		{
			printf("Impossibile aggiungere altri brani: si e' raggiunta la quantita' limite.\n");
		}
	}
	else
	{
		printf("Impossibile aggiungere brani: si deve effettuare l'accesso come amministratore.\n");
	}
	return -1;
}


/**
* Shows on screen a choice menu. Asks the user to make a choice by
* inserting a value, checking the inseryed value.
*/
int menu(User **currentLoggedUser)
{
	int scelta=0;
	//Stampa del menu
	system ("cls");
	printf("-----------------------------------------------------------------------------------------------------------------------\n"
			"\tMENU PRINCIPALE\t| Utente: %s (%s %s)\n"
			"-----------------------------------------------------------------------------------------------------------------------\n"
			"Benvenuto in %s, scegli cosa vuoi fare <0-16>\n"
			"  UTILITA'\n"
			"\t 1) Stampa a video il catalogo\n"
			"\t 2) Stampa a video la lista degli utenti\n"
			"  LOGIN\n"
			"\t 3) Accedi (Tramite nome utente e email)\n"
			"\t 4) Registra Nuovo utente.\n"
			"\t 5) Esci dall'account\n"
			"  AMMINISTRAZIONE\n"
			"\t 6) Aggiungi un brano.\n"
			"\t 7) Mostra i brani preferiti di un utente di un particolare genere musicale (ordinati per durata decrescente).\n"
			"\t 8) Salva le playlist di un utente su un file\n"
			"  FUNZIONI\n"
			"\t 9) Cerca un brano nel catalogo.\n"
			"\t10) Modifica dati utente (ad eccezione del nickname).\n"
			"\t11) Modifica la lista dei brani preferiti di un utente.\n"
			"\t12) Cerca tutti i brani di un album nel catalogo ordinati per numero di traccia.\n"
			"\t13) Ricerca tutti gli album di un artista nel catalogo. \n"
			"\t14) Modifica le tue playlist\n"
			"\t15) Mostra le playlist pubbliche\n"
			"\t16) Mostra le tue playlist\n"
			"  TESTING\n"
			"\t17) Test (tramite CUnit)"
			"\n"
			"\t0) Chiudi il programma. \n"
			"-----------------------------------------------------------------------------------------------------------------------\n",
			(*currentLoggedUser)? (*currentLoggedUser)->nickname : "",
			(*currentLoggedUser)? (*currentLoggedUser)->name : "Utente",
			(*currentLoggedUser)? (*currentLoggedUser)->surname : "Non Loggato",
			APP_NAME);

	printf("Inserisci una scelta: ");
	scelta = getInt(0,17);
	system ("cls");
	return scelta;
}
/**
 * Set the console title using a given string.
 * @warning This function uses instructions that
 * only work on Windows O.S.
 */
void setConsoleTitle(const char* newTitle)
{
	char title[strlen(newTitle) + 7];
	strcpy(title, "title ");
	system(strcat(title, newTitle));
	free(title);
}
