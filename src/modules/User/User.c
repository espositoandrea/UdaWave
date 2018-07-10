/**
 * @file User.c
 * Implementation of the functions from User.h.
 
 * This file contains the implementation of the function defined
 * in User.h. It uses the standard C libraries, along with the custom
 * libraries Utilities.h and Song.h.
 * @version 0.1
 * @date 08/may/2018
 * @authors Andrea Esposito, Graziano Montanaro
 * @copyright No copyright informations provided.
 */

#include "User.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

///Print an array of users to screen using the function `printUserToScreen`.
void printUsersArray(User* users[], int size)
{
	for(int i = 0; i < size; i++)
	{
		printf("- ");
		printUserToScreen(*(users + i));
	}
}

///Print all data of a user to screen.
void printUserToScreen(User *user)
{
	printf("NICKNAME: %s\n"
			"\tNOME: %s\n"
			"\tCOGNOME: %s\n"
			"\tEMAIL: %s\n"
			"\tSESSO: %c\n"
			"\tDATA DI NASCITA: %d/%d/%d\n"
			"\tBRANI PREFERITI:\n",
			user->nickname,
			user->name,
			user->surname,
			user->email,
			user->sex,
			user->birthDate.day, user->birthDate.month, user->birthDate.year);

	for(int i = 0; i < user->numberOfFavouriteSongs; i++)
		printf("\t- %s (by %s)", user->favouriteSongs[i]->name, user->favouriteSongs[i]->artist);

	printf("\n");
}

/**
* This function gets the index of a user in an array.
* It searches using the nickname, so the function ask
* the user to insert one (and it checks if it's valid).
*/
int getUserIdByNickname(User users[], int size)
{
	string nickname;

	int isFirstTimeInput = 1;	//Used for error messages
	int nicknamePosition = -1;

	do
	{
		if(!isFirstTimeInput && nicknamePosition != -1)
			printf("Inserire un nickname valido! Re");
		else if(!isFirstTimeInput)
			printf("Nickname non in uso! Re");

		printf("Inserire un nickname (che abbia meno di 20 caratteri e che inizi con una lettera): ");
		scanf("%20s", nickname);

		fflush(stdin);

		isFirstTimeInput = 0;
	} while((nicknamePosition = searchInUsers(users, size, nickname)) == -1 || !isalpha(nickname[0]));

	return nicknamePosition;
}

/**
 * A function to modify the favorite list of a user.
 * It asks the user if he/she wants to add or remove a song to/from their favorite list.
 */
void modifyFavourites(User **userToModify, User users[], int usersNumber, Song songs[], int songsNumber)
{
	if((*userToModify))
	{
		printf("Si vuole:\n"
				"\t1) Aggiungere un  brano ai preferiti\n"
				"\t2) Rimuovere un brano dai preferiti\n"
				"Inserire la scelta: ");
		int c = getInt(1,2);

		switch(c)
		{
		case 1:
			addFavourite(userToModify, users, usersNumber, songs, songsNumber);
			break;
		case 2:
			removeFavourite(userToModify, users, usersNumber, songs, songsNumber);
			break;
		default: //Never executed
			break;
		}

		modifyUsersFile(users, usersNumber);
	}
	else
		printf("Impossibile modificare un utente: si deve effettuare l'accesso.\n");
}

///Called by `modifyFavourites`. Add a favorite to a user's favorite list.
void addFavourite(User **userToModify, User users[], int usersNumber, Song songs[], int songsNumber)
{
	char repeat;
	do
	{
		int numberOfFoundSongs;
		Song** foundSongs;
		do
		{
			printf("Si vuole ricercare per:\n"
					"\t 1- Codice\n"
					"\t 2- Titolo\n"
					"Inserisci una scelta: ");

			int researchType = getInt(1,2);

			foundSongs = searchSong(songs, songsNumber, &numberOfFoundSongs, researchType, "");
			if(foundSongs == NULL)
				printf("Nessun riscontro con i criteri inseriti.\n");
		} while(foundSongs == NULL);

		if(numberOfFoundSongs > 1)
		{
			printSongsArray(foundSongs, numberOfFoundSongs);
			printf("Inserire il codice del brano da aggiungere tra i vari \"%s\"\n", foundSongs[0]->name);
			foundSongs = searchSong(songs, songsNumber, &numberOfFoundSongs, 1, "");
		}

		Song **favouriteWithCode = NULL;

		for(int i=0;i<(*userToModify)->numberOfFavouriteSongs;i++)
				if(strcmp((*userToModify)->favouriteSongs[i]->code, (*foundSongs)->code) == 0)
				{
					*favouriteWithCode = (*userToModify)->favouriteSongs[i];
					break;
				}

		if(!favouriteWithCode)
		{
			(*userToModify)->favouriteSongs[(*userToModify)->numberOfFavouriteSongs] = (Song*) calloc(1, sizeof(Song));
			(*userToModify)->favouriteSongs[(*userToModify)->numberOfFavouriteSongs] = *foundSongs;
			(*userToModify)->numberOfFavouriteSongs++;

			printf("\nBrano aggiunto ai preferiti:\n");
			printSongToScreen(*foundSongs);
		}
		else
			printf("Il brano \"%s\" (%s) e' gia' nella lista dei preferiti.\n", (*foundSongs)->name, (*foundSongs)->code);

		free(foundSongs);
		free(favouriteWithCode);
		repeat = getChoice("Si vuole inserire un altro preferito?");
	} while(repeat == 'Y');
}

///Called by `modifyFavourites`. Remove a favorite from a user's favorite list.
void removeFavourite(User **userToModify, User users[], int usersNumber, Song songs[], int songsNumber)
{
	if((*userToModify)->numberOfFavouriteSongs)
	{
		printf("\nBRANI PREFERITI:\n");
		printSongsArray((*userToModify)->favouriteSongs, (*userToModify)->numberOfFavouriteSongs);
		printf("\n");
		char repeat;
		do
		{
			int numberOfFoundSongs;
			Song** foundSongs;
			Song favSongs[(*userToModify)->numberOfFavouriteSongs];
			do
			{
				printf("Si vuole ricercare per:\n"
						"\t 1- Codice\n"
						"\t 2- Titolo\n"
						"Inserisci una scelta: ");

				int researchType = getInt(1,2);

				for(int i = 0; i < (*userToModify)->numberOfFavouriteSongs; i++)
					favSongs[i] = *((*userToModify)->favouriteSongs[i]);

				foundSongs = searchSong(favSongs, (*userToModify)->numberOfFavouriteSongs, &numberOfFoundSongs, researchType, "");
				if(foundSongs == NULL)
					printf("Nessun riscontro con i criteri inseriti.\n");
			} while(foundSongs == NULL);

			if(numberOfFoundSongs > 1)
			{
				printSongsArray(foundSongs, numberOfFoundSongs);
				printf("Inserire il codice del brano da aggiungere tra i vari \"%s\"\n", foundSongs[0]->name);
				foundSongs = searchSong(favSongs, (*userToModify)->numberOfFavouriteSongs, &numberOfFoundSongs, 1, "");
			}

			int favouritePosition;

			for(int i=0;i<(*userToModify)->numberOfFavouriteSongs;i++)
					if(strcmp((*userToModify)->favouriteSongs[i]->code, (*foundSongs)->code) == 0)
					{
						favouritePosition = i;
						break;
					}

			for(int i=favouritePosition;i<(*userToModify)->numberOfFavouriteSongs - 1;i++)
				(*userToModify)->favouriteSongs[i] = (*userToModify)->favouriteSongs[i + 1];

			free((*userToModify)->favouriteSongs[(*userToModify)->numberOfFavouriteSongs - 1]);
			(*userToModify)->numberOfFavouriteSongs--;

			printf("\nBrano rimosso dai preferiti:\n");
			printSongToScreen(*foundSongs);

			free(foundSongs);
			repeat = getChoice("Si vuole rimuovere un altro preferito?");
		} while(repeat == 'Y');
	}
	else
		printf("Nessun preferito trovato.\nImpossibile rimuovere un preferito.\n");
}


/**
* This function adds a user and it saves its data in a file.
*
* @pre currentSize should be less than the maximum number of users
* @post If currentSize is greater than the maximum number of users, the function fails and it returns -1.
*/
int addUser(User users[], int *currentSize, User **currentLoggedUser)
{
	if(*currentSize < MAX_USERS_NUMBER && (*currentLoggedUser== NULL || !strcmp((*currentLoggedUser)->nickname, "admin")))
	{
		//Inserting the user's nickname
		getNickname(users, *currentSize, users[*currentSize].nickname);

		//Inserting the user's name
		getName(users[*currentSize].name, "nome dell'utente");

		//Inserting the user's surname
		getName(users[*currentSize].surname, "cognome dell'utente");

		//Inserting the user's sex
		users[*currentSize].sex = getSex();


		//Inserting the user's date of birth
		users[*currentSize].birthDate = getDate();

		//Inserting the user's email
		getMail(users[*currentSize].email);

		users[*currentSize].numberOfFavouriteSongs = 0;

		addToUsersFile(&users[*currentSize]);

		if(*currentLoggedUser == NULL)
		{
			*currentLoggedUser = users + (*currentSize);
			printf("-----------------------------------------------------------------------------------------------------------------------\n"
					"Ciao, %s\n", (*currentLoggedUser)->name);
		}

		return ++(*currentSize);
	}
	else if(strcmp((*currentLoggedUser)->nickname, "admin") != 0)
	{
		printf("Spiacenti: serve avere i privilegi di amministratore per aggiungere indefinitamente utenti\n");
		system("pause");
		fflush(stdin);
	}
	else
	{
		printf("Impossibile aggiungere altri utenti: si e' raggiunta la quantita' limite\n");
		system("pause");
		fflush(stdin);
	}
	return -1;
}

/**
* This function adds a given user to the `.csv` user file.
* It uses a pointer to the user that it should save
* for optimization purposes.
*/
void addToUsersFile(User *user)
{
	int creatingFile = 0;
	FILE* usersFile;

	creatingFile = !doesFileExists("files/bin/usersList.csv");

	if((usersFile = fopen("files/bin/usersList.csv", "a")) != NULL)
	{
		//if we are creating the file we must set the header of the .csv
		if(creatingFile)
			fprintf(usersFile, "nickname,email,surname,name,sex,birthDate,favSongs\n");

		fprintf(usersFile, "%s,%s,%s,%s,%c,%d/%d/%d,",
				user->nickname,
				user->email,
				user->surname,
				user->name,
				user->sex,
				user->birthDate.day,
				user->birthDate.month,
				user->birthDate.year);

		for(int i = 0; i < user->numberOfFavouriteSongs; i++)
			fprintf(usersFile, "%s-", user->favouriteSongs[i]->code);

		fprintf(usersFile, "\n");

		fclose(usersFile);
	}
	else //if we couldn't open or create the file, print an error message and close the program.
	{
		printf("[404] ERRORE! IMPOSSIBILE MODIFICARE IL FILE DEGLI UTENTI (files/bin/usersList.csv)\n");
		system("pause");
		fflush(stdin);
		exit(404);
	}
}

/**
* This function finds the user with the given username.
*
* @post If the nickname isn't used, the function returns -1
*/
int searchInUsers(User users[], int size, const char* nickname)
{
	string givenNickname, userNickname;
	strcpy(givenNickname, nickname);

	for(int i=0;i<size;i++)
	{
		strcpy(userNickname, users[i].nickname);
		if(strcmp(strupr(userNickname), strupr(givenNickname)) == 0)
			return i;
	}

	return -1;
}

/**
* This function loads users' data from a file.
* The informations are stored in an array (a void array,
* so that it can be used for various data structures).
*
* @pre The file should exist.
* @post If the file doesn't exist the function will return -1.
*/
int loadUsersFromFile(const char* path, Song* songs, int currentSongNumber, User array[], int maxSize)
{
	int currentSize = -1;
	FILE* inputFile;
	char admin[] = "admin,francesco.serafino@uniba.it,Serafino,Francesco,M,00/00/0000,\n"; //admin informations

	if(doesFileExists(path))
	{
		printf("Caricamento utenti da file...\n");
		currentSize = 0;

		inputFile = fopen(path, "r");

		char line[5 * MAX_STRING_LENGTH + MAX_SONGS_NUMBER * MAX_CODE_LENGTH + 21]; //exact maximum length
		fgets(line, sizeof(line), inputFile);

		while(fgets(line, sizeof(line), inputFile) && currentSize < maxSize)
		{
			strtok(line, "\n");

			char *token = strtok(line, ",");

			strcpy(array[currentSize].nickname, token);
			strcpy(array[currentSize].email, strtok(NULL, ","));
			strcpy(array[currentSize].surname, strtok(NULL, ","));
			strcpy(array[currentSize].name, strtok(NULL, ","));
			array[currentSize].sex = strtok(NULL, ",")[0];

			char *date = strtok(NULL, ",");
			char* fav = strtok(NULL, ",");

			//Get date of birth
			array[currentSize].birthDate.day = atoi(strtok(date, "/"));
			array[currentSize].birthDate.month = atoi(strtok(NULL, "/"));
			array[currentSize].birthDate.year = atoi(strtok(NULL, "/"));

			char* code  = strtok(fav, "-");

			//Get the list of favorite songs
			array[currentSize].numberOfFavouriteSongs = 0;
			while(code != NULL)
			{
				array[currentSize].numberOfFavouriteSongs++;
				array[currentSize].favouriteSongs[array[currentSize].numberOfFavouriteSongs - 1] = *(searchSong(songs, currentSongNumber, NULL, 1, code));
				code  = strtok(NULL, "-");
			}

			currentSize++;
		}

		fclose(inputFile);

		if(searchInUsers(array, currentSize, "admin") == -1)
		{
			if(currentSize ==  maxSize)
			{
				printf("ATTENZIONE! Non e' stato trovato l'admin, ma il programma ha gia' raggiunto il massimo numero di utenti.\n"
						"Verra' eliminato l'ultimo utente registrato (%s) per far posto all'amministratore del sistema.\n",
						array[currentSize - 1].nickname);
				system("pause");
				fflush(stdin);
				currentSize--;
			}

			printf("Aggiungendo le credenziali dell'admin...\n");

			inputFile = fopen(path, "a");

			char adminCopy[sizeof(admin)];
			strcpy(adminCopy, admin);

			fprintf(inputFile, adminCopy);

			strcpy(array[currentSize].nickname, strtok(adminCopy, ","));
			strcpy(array[currentSize].email, strtok(NULL, ","));
			strcpy(array[currentSize].surname, strtok(NULL, ","));
			strcpy(array[currentSize].name, strtok(NULL, ","));
			array[currentSize].sex = strtok(NULL, ",")[0];

			char *date = strtok(NULL, ",");

			//Get date of birth
			array[currentSize].birthDate.day = atoi(strtok(date, "/"));
			array[currentSize].birthDate.month = atoi(strtok(NULL, "/"));
			array[currentSize].birthDate.year = atoi(strtok(NULL, "/"));


			//Get the list of favorite songs
			array[currentSize].numberOfFavouriteSongs = 0;

			currentSize++;

			fclose(inputFile);
		}

		system("cls");
	}
	else	//if we couldn't open or create the file, print an error message and close the program.
	{
		printf("[404] ERRORE! IMPOSSIBILE MODIFICARE IL FILE DEGLI UTENTI (files/bin/usersList.csv)\n");
		system("pause");
		fflush(stdin);
		exit(404);
	}

	if(currentSize == 0)
	{
		printf("Aggiungendo le credenziali dell'admin...\n");

		inputFile = fopen(path, "w");

		fprintf(inputFile, "nickname,email,surname,name,sex,birthDate,favSongs\n");

		char adminCopy[sizeof(admin)];
		strcpy(adminCopy, admin);

		fprintf(inputFile, adminCopy);

		strcpy(array[currentSize].nickname, strtok(adminCopy, ","));
		strcpy(array[currentSize].email, strtok(NULL, ","));
		strcpy(array[currentSize].surname, strtok(NULL, ","));
		strcpy(array[currentSize].name, strtok(NULL, ","));
		array[currentSize].sex = strtok(NULL, ",")[0];

		char *date = strtok(NULL, ",");

		//Get date of birth
		array[currentSize].birthDate.day = atoi(strtok(date, "/"));
		array[currentSize].birthDate.month = atoi(strtok(NULL, "/"));
		array[currentSize].birthDate.year = atoi(strtok(NULL, "/"));


		//Get the list of favorite songs
		array[currentSize].numberOfFavouriteSongs = 0;

		currentSize++;

		fclose(inputFile);
	}
	return currentSize;
}

///This function gets and prints all the songs of a given genre to screen.
void printSongsOfGenre(User users[], int usersNumber, User **currentLoggedUser)
{
	if(usersNumber > 0)
	{
		if(*currentLoggedUser)
		{
			int userId;
			if(!strcmp((*currentLoggedUser)->nickname, "admin"))
				 getUserIdByNickname(users, usersNumber);
			else
				userId = searchInUsers(users, usersNumber, (*currentLoggedUser)->nickname);

			Song* favouriteSongs = (Song*) calloc(users[userId].numberOfFavouriteSongs, sizeof(Song));
			for(int i = 0; i < users[userId].numberOfFavouriteSongs; i++)
				favouriteSongs[i] = *(users[userId].favouriteSongs[i]);

			int numberOfFoundSongs;
			Song** foundSongs = searchSong(favouriteSongs, users[userId].numberOfFavouriteSongs, &numberOfFoundSongs, 5, "");

			quicksort(foundSongs, 0, numberOfFoundSongs - 1, 0);

			if(foundSongs == NULL)
			{
				printf("-----------------------------------------------------------------------------------------------------------------------\n"
						"Nessun riscontro con i criteri inseriti.\n"
						"-----------------------------------------------------------------------------------------------------------------------\n");
			}
			else
				printSongsArray(foundSongs, numberOfFoundSongs);

			free(favouriteSongs);
			free(foundSongs);
		}
		else
			puts("Effettuare il login!");
	}
	else
	{
		puts("Non vi sono utenti registrati");
	}
}

/**
 * This function prints all users to screen.
 * @warning This function will soon be removed.
 */
void printUsersList(User users[], int usersNumber)
{
	system("cls");
	User** usersPointers = (User**) calloc(usersNumber, sizeof(User*));
	for(int i = 0; i < usersNumber; i++)
		usersPointers[i] = users + i;

	printUsersArray(usersPointers, usersNumber);
}

/**
 * Save all playlists of a user to a file.
 * This function will work only if the current logged user
 * is the admin. It will create a `.csv` file which name is the
 * username of the user of whom the playlists will be printed.
 */
void savePlaylistsToFile(User users[], int usersNumber, User *const *currentLoggedUser)
{
	if(*currentLoggedUser)
	{
		if(!strcmp((*currentLoggedUser)->nickname, "admin"))
		{
			int userIndex = getUserIdByNickname(users, usersNumber);

			char filePath[MAX_STRING_LENGTH + 22];

			strcpy(filePath, "files/adminGenerated/");
			strcat(filePath, (users + userIndex)->nickname);
			strcat(filePath, ".csv");

			FILE* userFile;
			if((userFile = fopen(filePath, "w")) != NULL)
			{
				printf("Salvataggio di %s\n", filePath);
				for(int i = 0; i < (users + userIndex)->numberOfPlaylists; i++)
				{
					fprintf(userFile, "%s,", (users + userIndex)->playlists[i]->name);

					for(int j = 0; j < (users + userIndex)->playlists[i]->numberOfSongs; j++)
					{
						fprintf(userFile, "%s", (users + userIndex)->playlists[i]->songs[j]->code);
						if(j != (users + userIndex)->playlists[i]->numberOfSongs - 1)
							fprintf(userFile, ",");
					}

					fprintf(userFile, "\n");
				}
				system("cls");
			}
			else
			{
				printf("[404] ERRORE! Impossibile scrivere il file delle playlist (%s)!\n", filePath);
				system("pause");
				exit(404);
			}

			printf("\nSalvataggio completato.\n");
		}
		else
			printf("E' impossibile eseguire l'operazione se non si e' amministratori.\n");
	}
	else
		printf("E' impossibile eseguire l'operazione se non si effetua l'accesso.\n");
}

/**
 * Load all playists from a file.
 * This function gets all the playlists from all users' file and load all the stored informations.
 * Then, it eventually link shared playlists.
 */
void loadPlaylistsFromFile(User usersToModify[], int usersNumber, Song songs[], int currentSongsNumber)
{
	char line[MAX_SONGS_NUMBER * MAX_CODE_LENGTH + MAX_STRING_LENGTH + 1];
	char playlistPath[MAX_STRING_LENGTH + 26];

	Playlist **playlistOfOthers[MAX_PLAYLIST_NUMBER * MAX_USERS_NUMBER];
	int indexes[MAX_PLAYLIST_NUMBER * MAX_USERS_NUMBER][2];
	int numberOfPlaylistToCheck = 0;

	for(int i = 0; i < usersNumber; i++)
	{
		strcpy(playlistPath, "files/bin/playlists/");
		strcat(playlistPath, usersToModify[i].nickname);
		strcat(playlistPath, ".csv");
		if(doesFileExists(playlistPath))
		{
			FILE* playlistFile;
			if((playlistFile = fopen(playlistPath, "r")) != NULL)
			{
				fgets(line, sizeof(line), playlistFile); //remove first line

				usersToModify[i].numberOfPlaylists = 0;
				while(fgets(line, sizeof(line), playlistFile) && usersToModify[i].numberOfPlaylists < MAX_PLAYLIST_NUMBER)
				{
					strtok(line, "\n");

					usersToModify[i].playlists[usersToModify[i].numberOfPlaylists] = (Playlist*) calloc(1, sizeof(Playlist));

					strcpy(usersToModify[i].playlists[usersToModify[i].numberOfPlaylists]->name, strtok(line, ","));
					usersToModify[i].playlists[usersToModify[i].numberOfPlaylists]->isPrivate = atoi(strtok(NULL, ","));
					strcpy(usersToModify[i].playlists[usersToModify[i].numberOfPlaylists]->ownerNickname, strtok(NULL, ","));

					//Is this a shared playlist?
					if(strcmp(usersToModify[i].playlists[usersToModify[i].numberOfPlaylists]->ownerNickname, usersToModify[i].nickname))
					{
						//If yes, remember it so we can modify it later.
						playlistOfOthers[numberOfPlaylistToCheck] = &(usersToModify[i].playlists[usersToModify[i].numberOfPlaylists]);
						indexes[numberOfPlaylistToCheck][0] = i;
						indexes[numberOfPlaylistToCheck][1] = usersToModify[i].numberOfPlaylists;
						numberOfPlaylistToCheck++;
					}

					usersToModify[i].playlists[usersToModify[i].numberOfPlaylists]->numberOfSongs = 0;
					char* code = strtok(NULL, ",");
					while(code != NULL)
					{
						usersToModify[i].playlists[usersToModify[i].numberOfPlaylists]->songs[usersToModify[i].playlists[usersToModify[i].numberOfPlaylists]->numberOfSongs] = *searchSong(songs, currentSongsNumber, NULL, 1, code);
						usersToModify[i].playlists[usersToModify[i].numberOfPlaylists]->numberOfSongs++;
						code = strtok(NULL, ",");
					}
					usersToModify[i].numberOfPlaylists++;
				}
				fclose(playlistFile);
			}
		}
		else
		{
			usersToModify[i].numberOfPlaylists = 0;
			free(usersToModify[i].playlists);
		}
	}

	//For all the shared playlist that has been found...
	for(int i = 0; i < numberOfPlaylistToCheck; i++)
	{
		//Get the original owner
		int id = searchInUsers(usersToModify, usersNumber, (*(playlistOfOthers[i]))->ownerNickname);
		int id2;

		string owner, name, searchedOwner, searchedName;
		strcpy(searchedOwner, (*(playlistOfOthers[i]))->ownerNickname);
		strcpy(searchedName, (*(playlistOfOthers[i]))->name);
		strupr(searchedOwner);
		strupr(searchedName);

		//Search for the playlist and save its data.
		for(int i=0;i<usersNumber;i++)
		{
			for(int j=0;j<usersToModify[i].numberOfPlaylists;j++)
			{
				strcpy(owner, usersToModify[i].playlists[j]->ownerNickname);
				strcpy(name, usersToModify[i].playlists[j]->name);
				strupr(owner);
				strupr(name);

				if(!strcmp(searchedOwner, owner) && !strcmp(searchedName, name))
					id2 = j;
			}
		}
		free(usersToModify[indexes[i][0]].playlists[indexes[i][1]]); //remove the old playlist
		usersToModify[indexes[i][0]].playlists[indexes[i][1]] = usersToModify[id].playlists[id2]; //link to the original playlist
	}
}

///This function prints all the informations of all the playlists of a user.
void printPlaylistsToScreen(User* user)
{
	if(user && user->numberOfPlaylists > 0)
	{
		printf("LE PLAYLIST DI %s %s:\n", user->name, user->surname);
		for(int i = 0; i < user->numberOfPlaylists; i++)
		{
			if(strcmp(user->playlists[i]->name, ""))
			{
				printf("- ");
				char* isPrivate = (user->playlists[i]->isPrivate)? "SI" : "NO";
				printf("NOME: %s\n"
						"\tE' PRIVATA? %s\n"
						"\tPROPRIETARIO: %s\n"
						"\tBRANI:\n",
						user->playlists[i]->name,
						isPrivate,
						user->playlists[i]->ownerNickname);
				for(int j = 0; j < user->playlists[i]->numberOfSongs; j++)
					printf("\t- %s (by: %s)\n", user->playlists[i]->songs[j]->name, user->playlists[i]->songs[j]->artist);
			}

		}
	}
	else if(user)
		printf("\tNessuna playlist trovata.\n");
	else
		printf("Si deve prima effettuare l'accesso.\n");
}

///This function creates and adds a new playlist to a user.
void addPlaylist(User *user, Song songs[], int currentSongSize)
{
	printf("-----------------------------------------------------------------------------------------------------------------------\n"
			"\tCREAZIONE NUOVA PLAYLIST\n"
			"-----------------------------------------------------------------------------------------------------------------------\n");
	if(user)
	{
		if(user->numberOfPlaylists < MAX_PLAYLIST_NUMBER)
		{

			user->playlists[user->numberOfPlaylists] = (Playlist*) calloc(1, sizeof(Playlist));
			int isFirstTimeInput = 1;
			Playlist **foundPlaylist;
			do
			{
				if(!isFirstTimeInput)
				{
					free(foundPlaylist);
					printf("Il nome inserito e' gia' presente. Re");
				}
				printf("Inserire il nome della playlist: ");
				gets(user->playlists[user->numberOfPlaylists]->name);
				isFirstTimeInput = 0;
				foundPlaylist = searchInPlaylists(user, 1, user->playlists[user->numberOfPlaylists]->name, user->nickname); //TODO: CHECK
			} while(foundPlaylist != NULL);
			free(foundPlaylist);

			char choice;

			choice = getChoice("La playlist e' privata?");

			user->playlists[user->numberOfPlaylists]->isPrivate = (choice == 'Y')? 1 : 0;

			strcpy(user->playlists[user->numberOfPlaylists]->ownerNickname, user->nickname);

			choice = 'Y';
			Song** foundSongs;
			char repeat;
			do
			{
				int numberOfFoundSongs;

				do
				{
					printf("Si vuole ricercare per:\n"
							"\t 1- Codice\n"
							"\t 2- Titolo\n"
							"Inserisci una scelta: ");

					int researchType = getInt(1,2);
					foundSongs = searchSong(songs, currentSongSize, &numberOfFoundSongs, researchType, "");
					if(*foundSongs == NULL)
						printf("Nessun riscontro con i criteri inseriti.\n");
				} while(*foundSongs == NULL);

				if(numberOfFoundSongs > 1)
				{
					printSongsArray(foundSongs, numberOfFoundSongs);
					printf("Inserire il codice del brano da aggiungere tra i vari \"%s\"\n", foundSongs[0]->name);
					foundSongs = searchSong(songs, currentSongSize, &numberOfFoundSongs, 1, "");
				}


				Song **favouriteWithCode = (Song**) calloc(1, sizeof(Song*));
				*favouriteWithCode = NULL;

				for(int i=0;i<user->playlists[user->numberOfPlaylists]->numberOfSongs;i++)
					if(strcmp(user->playlists[user->numberOfPlaylists]->songs[i]->code, (*foundSongs)->code) == 0)
					{
						*favouriteWithCode = user->favouriteSongs[i];
						break;
					}

				if((*favouriteWithCode) == NULL)
				{
					user->playlists[user->numberOfPlaylists]->songs[user->playlists[user->numberOfPlaylists]->numberOfSongs] = *foundSongs;
					user->playlists[user->numberOfPlaylists]->numberOfSongs++;

					printf("Brano aggiunto alla playlist.\n");
				}
				else
					puts("Brano gia' presente nella playlist.");
				free(favouriteWithCode);

				repeat = getChoice("Si vuole inserire un altro brano?");
			} while(repeat == 'Y' && user->playlists[user->numberOfPlaylists]->numberOfSongs < MAX_SONGS_NUMBER);
			free(foundSongs);
			user->numberOfPlaylists++;

			writePlaylistToFile(user);
		}
		else
		{
			printf("Impossibile aggiungere altre playlist: si e' raggiunta la quantita' limite\n");
			system("pause");
		}
	}
	else
	{
		printf("Si deve essere loggati per aggiungere una playlist!\n");
		system("pause");
		fflush(stdin);
	}
}

///This function removes a playlist from a user.
void removePlaylist(User *user, Song songs[], int currentSongSize)
{

	printf("-----------------------------------------------------------------------------------------------------------------------\n"
			"\tRIMOZIONE PLAYLIST\n"
			"-----------------------------------------------------------------------------------------------------------------------\n");
	if(user)
		{
		int isFirstTimeInput = 1;
		Playlist **foundPlaylist;
		string playlistToRemoveName;
		do
		{
			if(!isFirstTimeInput)
			{
				free(foundPlaylist);
				printf("Il nome inserito non e' presente. Re");
			}
			printf("Inserire il nome della playlist: ");
			gets(playlistToRemoveName);
			isFirstTimeInput = 0;
			foundPlaylist = searchInPlaylists(user, 1, playlistToRemoveName, user->nickname); //TODO: CHECK
		} while(*foundPlaylist == NULL);

		int foundIndex = -1;
		for(int i = 0; i < user->numberOfPlaylists && foundIndex == -1; i++)
			if(!strcmp(user->playlists[i]->name, (*foundPlaylist)->name))
				foundIndex = i;

		for(int i = foundIndex; i < user->numberOfPlaylists - 1; i++)
			user->playlists[i] = user->playlists[i + 1];

		printf("E' stata rimossa la playlist %s (di %s)\n", (*foundPlaylist)->name, user->nickname);

		free(*foundPlaylist);
		free(foundPlaylist);

		user->numberOfPlaylists--;
		system("pause");
		fflush(stdin);

		writePlaylistToFile(user);
	}
	else
	{
		printf("Si deve essere loggati per rimuovere una playlist!\n");
		system("pause");
		fflush(stdin);
	}
}

///This function gives the user the ability to modify one of his/her playlists.
void modifyPlaylist(User *user, Song songs[], int currentSongSize)
{

	printf("-----------------------------------------------------------------------------------------------------------------------\n"
			"\tMODIFICA DATI PLAYLIST\n"
			"-----------------------------------------------------------------------------------------------------------------------\n");
	if(user)
	{
		int isFirstTimeInput = 1;
		Playlist **playlistToModify, **foundPlaylist;
		string playlistToModifyName;
		do
		{
			if(!isFirstTimeInput)
			{
				free(playlistToModify);
				printf("Il nome inserito non e' presente. Re");
			}
			printf("Inserire il nome della playlist da modificare: ");
			gets(playlistToModifyName);
			isFirstTimeInput = 0;
			playlistToModify = searchInPlaylists(user, 1, playlistToModifyName, user->nickname); //TODO: CHECK
		} while(playlistToModify == NULL);

		isFirstTimeInput = 1;
		do
		{
			if(!isFirstTimeInput)
			{
				free(foundPlaylist);
				printf("Il nome inserito e' gia' presente. Re");
			}
			printf("Inserire il nuovo nome della playlist: ");
			gets(playlistToModifyName);
			isFirstTimeInput = 0;
			puts(playlistToModifyName);
					puts((*playlistToModify)->name);
			foundPlaylist = searchInPlaylists(user, 1, playlistToModifyName, user->nickname); //TODO: CHECK
		} while(foundPlaylist != NULL);

		strcpy((*playlistToModify)->name, playlistToModifyName);
		free(foundPlaylist);

		char choice;

		choice = getChoice("La playlist e' privata?");

		(*playlistToModify)->isPrivate = (choice == 'Y')? 1 : 0;

		strcpy((*playlistToModify)->ownerNickname, user->nickname);

		printf("Modifica completata!\n");
		system("pause");
		fflush(stdin);

		free(playlistToModify);

		writePlaylistToFile(user);
	}
	else
	{
		printf("Si deve essere loggati per modificare una playlist!\n");
		system("pause");
		fflush(stdin);
	}
}

///This function adds a song to a playlist.
void addSongToPlaylist(User *user, Song songs[], int currentSongSize)
{
	printf("-----------------------------------------------------------------------------------------------------------------------\n"
			"\tAGGIUNTA DI UN BRANO A UNA PLAYLIST\n"
			"-----------------------------------------------------------------------------------------------------------------------\n");
	if(user)
	{
		int isFirstTimeInput = 1;
		Playlist **playlistToModify;
		string playlistToModifyName;
		do
		{
			if(!isFirstTimeInput)
			{
				free(playlistToModify);
				printf("Il nome inserito non e' presente. Re");
			}
			printf("Inserire il nome della playlist da modificare: ");
			gets(playlistToModifyName);
			isFirstTimeInput = 0;
			playlistToModify = searchInPlaylists(user, 1, playlistToModifyName, user->nickname); //TODO: CHECK
		} while(playlistToModify == NULL);
		Song** foundSongs;
		char repeat;
		do
		{
			int numberOfFoundSongs;

			do
			{
				printf("Si vuole ricercare per:\n"
						"\t 1- Codice\n"
						"\t 2- Titolo\n"
						"Inserisci una scelta: ");

				int researchType = getInt(1,2);
				foundSongs = searchSong(songs, currentSongSize, &numberOfFoundSongs, researchType, "");
				if(foundSongs == NULL)
					printf("Nessun riscontro con i criteri inseriti.\n");
			} while(foundSongs == NULL);

			if(numberOfFoundSongs > 1)
			{
				printSongsArray(foundSongs, numberOfFoundSongs);
				printf("Inserire il codice del brano da aggiungere tra i vari \"%s\"\n", foundSongs[0]->name);
				foundSongs = searchSong(songs, currentSongSize, &numberOfFoundSongs, 1, "");
			}


			Song **favouriteWithCode = (Song**) calloc(1, sizeof(Song*));
			*favouriteWithCode = NULL;

			for(int i=0;i<(*playlistToModify)->numberOfSongs;i++)
				if(strcmp((*playlistToModify)->songs[i]->code, (*foundSongs)->code) == 0)
				{
					*favouriteWithCode = user->favouriteSongs[i];
					break;
				}

			if((*favouriteWithCode) == NULL)
			{
				(*playlistToModify)->songs[(*playlistToModify)->numberOfSongs] = *foundSongs;
				(*playlistToModify)->numberOfSongs++;

				printf("Brano aggiunto alla playlist.\n");
			}
			else
				puts("Brano gia' presente nella playlist.");
			free(favouriteWithCode);

			repeat = getChoice("Si vuole inserire un altro brano?");
		} while(repeat == 'Y' && (*playlistToModify)->numberOfSongs < MAX_SONGS_NUMBER);
		free(foundSongs);
		system("pause");
		fflush(stdin);
		writePlaylistToFile(user);
	}
	else
	{
		printf("Si deve essere loggati per modificare una playlist!\n");
		system("pause");
		fflush(stdin);
	}
}

///This function removes a song from a playlist.
void removeSongFromPlaylist(User *user, Song songs[], int currentSongSize)
{
	printf("-----------------------------------------------------------------------------------------------------------------------\n"
				"\tRIMOZIONE DI UN BRANO DA UNA PLAYLIST\n"
				"-----------------------------------------------------------------------------------------------------------------------\n");
	if(user)
	{
		int isFirstTimeInput = 1;
		Playlist **playlistToModify;
		string playlistToModifyName;
		do
		{
			if(!isFirstTimeInput)
			{
				free(playlistToModify);
				printf("Il nome inserito non e' presente. Re");
			}
			printf("Inserire il nome della playlist da modificare: ");
			gets(playlistToModifyName);
			isFirstTimeInput = 0;
			playlistToModify = searchInPlaylists(user, 1, playlistToModifyName, user->nickname); //TODO: CHECK
		} while(playlistToModify == NULL);
		char repeat;
		do
		{
			int numberOfFoundSongs;
			Song** foundSongs;
			Song playlistSongs[(*playlistToModify)->numberOfSongs];
			do
			{
				printf("Si vuole ricercare per:\n"
						"\t 1- Codice\n"
						"\t 2- Titolo\n"
						"Inserisci una scelta: ");

				int researchType = getInt(1,2);

				for(int i = 0; i < (*playlistToModify)->numberOfSongs; i++)
					playlistSongs[i] = *((*playlistToModify)->songs[i]);

				foundSongs = searchSong(playlistSongs, (*playlistToModify)->numberOfSongs, &numberOfFoundSongs, researchType, "");
				if(foundSongs == NULL)
					printf("Nessun riscontro con i criteri inseriti.\n");
			} while(foundSongs == NULL);

			if(numberOfFoundSongs > 1)
			{
				printSongsArray(foundSongs, numberOfFoundSongs);
				printf("Inserire il codice del brano da rimuovere tra i vari \"%s\"\n", foundSongs[0]->name);
				foundSongs = searchSong(playlistSongs, (*playlistToModify)->numberOfSongs, &numberOfFoundSongs, 1, "");
			}

			int favouritePosition;

			for(int i=0;i<(*playlistToModify)->numberOfSongs;i++)
					if(strcmp((*playlistToModify)->songs[i]->code, (*foundSongs)->code) == 0)
					{
						favouritePosition = i;
						break;
					}

			for(int i=favouritePosition;i<(*playlistToModify)->numberOfSongs - 1;i++)
				(*playlistToModify)->songs[i] = (*playlistToModify)->songs[i + 1];

			free((*playlistToModify)->songs[(*playlistToModify)->numberOfSongs - 1]);
			(*playlistToModify)->numberOfSongs--;

			printf("\nBrano rimosso dalla playlist:\n");
			printSongToScreen(*foundSongs);

			free(foundSongs);
			repeat = getChoice("Si vuole rimuovere un altro preferito?");
		} while(repeat == 'Y');

		system("pause");
		fflush(stdin);
		writePlaylistToFile(user);
	}
	else
	{
		printf("Si deve essere loggati per modificare una playlist!\n");
		system("pause");
		fflush(stdin);
	}
}

///This function asks for the login informations to the user, and eventually it logs in with the chosen account.
User* logIn(User users[], int usersNumber)
{
	User *loggedUser = NULL;
	char email[2 * MAX_STRING_LENGTH + 2];

	int isFirstTimeInput = 1;
	do
	{
		if(!isFirstTimeInput)
			printf("Spiacenti: email e nickname non combaciano.\n");
		loggedUser = users + getUserIdByNickname(users, usersNumber);
		getMail(email);
		isFirstTimeInput = 0;
	} while(strcmp(email, loggedUser->email));

	return loggedUser;
}

///This function gives the user the ability to log out immediately.
void logOut(User **currentLoggedUser)
{
	if(*currentLoggedUser)
	{
		printf("Arrivederci, %s\n", (*currentLoggedUser)->name);
		*currentLoggedUser = NULL;
	}
	else
		printf("Non si e' ancora effettuato il login!\n");
}

///This function gives the user the ability to modify his/her datas (except the username).
void modifyUser(User users[], int currentSize, User **currentLoggedUser)
{
	User *userToModify = NULL;

	if(*currentLoggedUser == NULL)
		printf("Impossibile modificare un utente: si deve effettuare l'accesso.\n");
	else if(!strcmp((*currentLoggedUser)->nickname, "admin"))
	{
		printf("Quale utente si vuole modificare? ");
		userToModify = users + getUserIdByNickname(users, currentSize);
	}
	else
		userToModify = *currentLoggedUser;

	if(userToModify)
	{
		printf("UTENTE DA MODIFICARE: %s\n",
				userToModify->nickname);

		//Inserting the user's name
		getName(userToModify->name, "nuovo nome dell'utente");

		//Inserting the user's surname
		getName(userToModify->surname, "nuovo cognome dell'utente");

		//Inserting the user's sex
		userToModify->sex = getSex();


		//Inserting the user's date of birth
		userToModify->birthDate = getDate();

		//Inserting the user's email
		getMail(userToModify->email);

		modifyUsersFile(users, currentSize);
	}
}

///This function updates the users' file.
void modifyUsersFile(User users[], int size)
{
	FILE* usersFile;

	if((usersFile = fopen("files/bin/usersList.csv", "w")) != NULL)
	{
		fprintf(usersFile, "nickname,email,surname,name,sex,birthDate,favSongs\n");
		for(int i = 0; i < size; i++)
		{
			fprintf(usersFile, "%s,%s,%s,%s,%c,%d/%d/%d,",
					(users + i)->nickname,
					(users + i)->email,
					(users + i)->surname,
					(users + i)->name,
					(users + i)->sex,
					(users + i)->birthDate.day,
					(users + i)->birthDate.month,
					(users + i)->birthDate.year);

			for(int j = 0; j < (users + i)->numberOfFavouriteSongs; j++)
				fprintf(usersFile, "%s-", (users + i)->favouriteSongs[j]->code);

			fprintf(usersFile, "\n");
		}

		fclose(usersFile);
	}
	else //if we couldn't open or create the file, print an error message and close the program.
	{
		printf("[404] ERRORE! IMPOSSIBILE MODIFICARE IL FILE DEGLI UTENTI (files/bin/usersList.csv)\n");
		system("pause");
		fflush(stdin);
		exit(404);
	}
}

///This function writes the data of all the playlists of a user to a `.csv` file.
void writePlaylistToFile(User *user)
{
	char filePath[MAX_STRING_LENGTH + 26];

	strcpy(filePath, "files/bin/playlists/");
	strcat(filePath, user->nickname);
	strcat(filePath, ".csv");

	FILE* userFile;
	if((userFile = fopen(filePath, "w")) != NULL)
	{
		printf("Salvataggio di %s\n", filePath);
		fprintf(userFile, "playlistName,isPrivate,ownerName,codes\n");
		for(int i = 0; i < user->numberOfPlaylists; i++)
		{
			if(strcmp(user->playlists[i]->name, ""))
			{
				fprintf(userFile, "%s,%d,%s,",
						user->playlists[i]->name,
						user->playlists[i]->isPrivate,
						user->playlists[i]->ownerNickname);

				for(int j = 0; j < user->playlists[i]->numberOfSongs; j++)
				{
					fprintf(userFile, "%s", user->playlists[i]->songs[j]->code);
					if(j != user->playlists[i]->numberOfSongs - 1)
						fprintf(userFile, ",");
				}

				fprintf(userFile, "\n");
			}
		}
		system("cls");
	}
	else
	{
		printf("[404] ERRORE! Impossibile scrivere il file delle playlist (%s)!\n", filePath);
		system("pause");
		exit(404);
	}
}

/**
* This function inserts a nickname.
*/
void getNickname(User* usersList, int size, char *nickname)
{
	int isFirstTimeInput = 1;	//Used for error messages
	int nicknamePosition = -1;

	do
	{
		if(!isFirstTimeInput && nicknamePosition == -1)
			printf("Inserire un nickname valido! Re");
		else if(!isFirstTimeInput)
			printf("Nickname gia' in uso! Re");

		printf("Inserire un nickname (che abbia meno di 20 caratteri e che inizi con una lettera): ");
		scanf("%20s", nickname);

		fflush(stdin);

		isFirstTimeInput = 0;
	} while((nicknamePosition = searchInUsers(usersList, size, nickname)) != -1 || !isalpha(nickname[0]));
}

/**
 * Show all public playlists.
 * This function gets and prints to screen all public playlists. Then it asks o the user if he/she
 * wants to add one of those playist to his/her own playlists' list.
 */
void printPublicPlaylistsToScreen(User user[], int usersNumber, User *currentLoggedUser)
{
	unsigned short int printedPlaylist = 0;
	for(int i = 0; i < usersNumber; i++)
	{
		if(user[i].numberOfPlaylists > 0)
		{
			for(int j = 0; j < user[i].numberOfPlaylists; j++)
			{
				printf("- ");
				if(!user[i].playlists[j]->isPrivate && strcmp(user[i].nickname, user[i].playlists[j]->ownerNickname) == 0 )
				{
					printedPlaylist = 1;

					printf("NOME: %s\n"
							"\tPROPRIETARIO: %s\n"
							"\tBRANI:\n",
							user[i].playlists[j]->name,
							user[i].playlists[j]->ownerNickname);

					for(int k = 0; k < user[i].playlists[j]->numberOfSongs; k++)
						printf("\t- %s (by: %s)\n", user[i].playlists[j]->songs[k]->name, user[i].playlists[j]->songs[k]->artist);
				}
			}
		}
	}
	if(!printedPlaylist)
		printf("Nessuna playlist pubblica.\n");

	printf("-----------------------------------------------------------------------------------------------------------------------\n");
	if(currentLoggedUser && printedPlaylist)
	{
		char wantsToSave = getChoice("Si vuole salvare una playlist fra le proprie?");
		if(wantsToSave == 'Y')
		{
			string playlistName, ownerName;
			char repeat;
			Playlist** foundPlaylist;
			do
			{
				if(currentLoggedUser->numberOfPlaylists < MAX_PLAYLIST_NUMBER)
				{
					int isFirstTimeInput = 1;
					do
					{
						if(!isFirstTimeInput)
							printf("Impossibile salvare una propria playlist.\n");
						printf("Inserire il nome della playlist da salvare: ");
						scanf("%s", playlistName);
						printf("Chi e' il suo proprietario? ");
						strcpy(ownerName, user[getUserIdByNickname(user, usersNumber)].nickname);
						isFirstTimeInput = 0;
					} while(strcmp(ownerName, currentLoggedUser->nickname) == 0);

					foundPlaylist = searchInPlaylists(user, usersNumber, playlistName, ownerName);
					if(!foundPlaylist)
						printf("Playlist non esistente\n");
					else if(!searchInPlaylists(currentLoggedUser, 1, playlistName, ownerName)) //check if the playlist has already been saved.
					{
						currentLoggedUser->playlists[currentLoggedUser->numberOfPlaylists] = *foundPlaylist;
						currentLoggedUser->numberOfPlaylists++;

						repeat = getChoice("Salvare un'altra playlist?");
					}
					else
					{
						puts("La playlist scelta e' gia' nella propria lista di playlist.");

						repeat = getChoice("Salvare un'altra playlist?");
					}
				}
				else
					printf("Si e' raggiunto il limite massimo di playlist.\n");
			} while((!foundPlaylist || repeat == 'Y') && currentLoggedUser->numberOfPlaylists < MAX_PLAYLIST_NUMBER);
		}
	}
	else if(currentLoggedUser && !printedPlaylist)
		puts("Non c'e' nessuna playlist da poter salvare.");
	else
		puts("Si deve essere loggati per salvare una playlist!");
}

/**
 * Search for a playlist.
 * This function searches trough all the  playlists. A playlist is identified by
 * its name and its owner's nickname. The function doesn't search only public
 * or private playlists.
 *
 * @pre The searched playlist should exist
 * @post If the playlist doesn't exist, the function will return a null pointer.
 */
Playlist **searchInPlaylists(User users[], int usersNumber, const char *playlistName, const char *ownerName)
{
	string owner, name, searchedOwner, searchedName;
	strcpy(searchedOwner, ownerName);
	strcpy(searchedName, playlistName);
	strupr(searchedOwner);
	strupr(searchedName);

	for(int i=0;i<usersNumber;i++)
	{
		for(int j=0;j<users[i].numberOfPlaylists;j++)
		{
			strcpy(owner, users[i].playlists[j]->ownerNickname);
			strcpy(name, users[i].playlists[j]->name);
			strupr(owner);
			strupr(name);

			if(!strcmp(searchedOwner, owner) && !strcmp(searchedName, name))
				return &users[i].playlists[j];
		}
	}
	return NULL;
}
