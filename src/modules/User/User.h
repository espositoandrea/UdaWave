/**
* @file User.h
* Functions to manipulate users and playlists.
*
* This library contains functions that work on users, users' arrays and on playlists,
* Along with the definitions of users and playlists-related data structures.
* Some of this functions are based on functions defined in Utilities.h and in Song.h.
* @version 0.1
* @date 08/may/2018
* @authors Andrea Esposito, Graziano Montanaro
* @copyright No copyright informations provided.
*/


#ifndef USER_H
#define USER_H

#include "../Song/Song.h"
#include "../Utilities/Utilities.h"

/**
 * The maximum number of users the software can manage.
 */
#define MAX_USERS_NUMBER 20
/**
 * The maximum number of playlist (per user) the software can manage.
 */
#define MAX_PLAYLIST_NUMBER 15

/**
 * A playlist.
 * The data structure that contains all the informations about a playlist.
 */
typedef struct Playlist
{
	string name;						///< The name of the playlist.
	Song* songs[MAX_SONGS_NUMBER];		///< The list of songs of the playlist.
	int numberOfSongs;					///< The number of songs of the playlist.
	int isPrivate; 						///< Is the playlist private? (1 = True, 0 = False).
	string ownerNickname;				///< The nickname of the playlist's owner.
} Playlist;

/**
 * A user.
 * The data structure that contains all the informations about a user.
 */
typedef struct User
{
	string nickname;							///< The user's nickname.
	char email[2 * MAX_STRING_LENGTH + 2];		///< The user's email (its size is: string + '@' + string + '\0').
	string surname;								///< The user's surname.
	string name;								///< The user's name.
	Sex sex;									///< The user's sex.
	Date birthDate;								///< The user's date of birth.

	Song* favouriteSongs[MAX_SONGS_NUMBER];		///< The user's list of favorite songs.
	int numberOfFavouriteSongs;					///< The number of the user's favorite songs.

	Playlist* playlists[MAX_PLAYLIST_NUMBER];	///< The list of playlists of the user.
	int numberOfPlaylists;						///< The number of playlists of the user.
} User;

/**
 * Print all users data to screen.
 * @warning This function will soon be removed.
 * @param users The array of users.
 * @param size The number of users.
 */
void printUsersArray(User* users[], int size);
/**
 * Print a single user to screen
 * @param user The user that will be printed.
 */
void printUserToScreen(User *user);

/**
 * Print a user's playlist's list to screen.
 * @param user The "owner" of the playlists that will be printed.
 */
void printPlaylistsToScreen(User* user);

/**
 * Saves all playlists of a user to file.
 * @param[in] users The array of users.
 * @param[in] usersNumber The number of users.
 * @param[in] currentLoggedUser The current logged user
 */
void savePlaylistsToFile(User users[], int usersNumber, User *const *currentLoggedUser);

/**
 * Load all playlists from a file.
 *
 * @param[in] usersToModify The array of users.
 * @param[in] usersNumber The number of users.
 * @param[in] songs The array of songs.
 * @param[in] currentSongsNumber The number of songs.
 */
void loadPlaylistsFromFile(User usersToModify[], int usersNumber, Song songs[], int currentSongsNumber);

/**
 * Print all songs of a genre.
 *
 * @param[in] users The array of users.
 * @param[in] usersNumber The number of users.
 * @param[in] currentLoggedUser The current logged user.
 */
void printSongsOfGenre(User users[], int usersNumber, User **currentLoggedUser);

/**
 * Write all the playlists of a user to a file.
 * @param[in] user The user of whom the playlists will be printed.
 */
void writePlaylistToFile(User *user);

/**
 * Add a playlist.
 *
 * @param[in] user The user whose playlist will be added.
 * @param[in] songs The array of songs.
 * @param[in] currentSongSize The number of songs.
 */
void addPlaylist(User *user, Song songs[], int currentSongSize);

/**
 * Remove a playlist.
 *
 * @param[in] user The user whose playlist will be removed.
 * @param[in] songs The array of songs.
 * @param[in] currentSongSize The number of songs.
 */
void removePlaylist(User *user, Song songs[], int currentSongSize);

/**
 * Remove a playlist.
 *
 * @param[in] user The user whose playlist will be modified.
 * @param[in] songs The array of songs.
 * @param[in] currentSongSize The number of songs.
 */
void modifyPlaylist(User *user, Song songs[], int currentSongSize);

/**
 * Add a song to a playlist.
 *
 * @param[in] user The user whose playlist will be modified.
 * @param[in] songs The array of songs.
 * @param[in] currentSongSize The number of songs.
 */
void addSongToPlaylist(User *user, Song songs[], int currentSongSize);


/**
 * Remove a song from a playlist.
 *
 * @param[in] user The user whose playlist will be modified.
 * @param[in] songs The array of songs.
 * @param[in] currentSongSize The number of songs.
 */
void removeSongFromPlaylist(User *user, Song songs[], int currentSongSize);

/**
 * Log a user in.
 *
 * @param users The array of users.
 * @param usersNumber The number of users.
 * @return A pointer to the new logged user.
 */
User* logIn(User users[], int usersNumber);

/**
 * Log out from current session.
 *
 * @param[out] currentLoggedUser The current logged user that has to be logged out.
 */
void logOut(User **currentLoggedUser);

/**
 * Modify favourite list.
 *
 * @param[in] userToModify The user that will be modified.
 * @param[in] users The array of users
 * @param[in] usersNumber The size of the users' array.
 * @param[in] songs The catalog of songs.
 * @param[in] songsNumber The number of songs in the catalog.
 */
void modifyFavourites(User **userToModify, User users[], int usersNumber, Song songs[], int songsNumber);
/**
 * Add a favourite to the favourite list.
 *
 * @param[in] userToModify The user that will be modified.
 * @param[in] users The array of users
 * @param[in] usersNumber The size of the users' array.
 * @param[in] songs The catalog of songs.
 * @param[in] songsNumber The number of songs in the catalog.
 */
void addFavourite(User **userToModify, User users[], int usersNumber, Song songs[], int songsNumber);
/**
 * Remove a favourite from the favourite list.
 *
 * @param[in] userToModify The user that will be modified.
 * @param[in] users The array of users
 * @param[in] usersNumber The size of the users' array.
 * @param[in] songs The catalog of songs.
 * @param[in] songsNumber The number of songs in the catalog.
 */
void removeFavourite(User **userToModify, User users[], int usersNumber, Song songs[], int songsNumber);

/**
 * Modify a user.
 *
 * @param[in] users The array of users.
 * @param[in] currentSize The size of the users' array.
 * @param[in] currentLoggedUser The current logged user.
 */
void modifyUser(User users[], int currentSize, User **currentLoggedUser);

/**
 * Update users' file.
 *
 * @param[in] users The array of users.
 * @param[in] size The size of the users' array.
 */
void modifyUsersFile(User users[], int size);

/**
 * Print all public playlists.
 *
 * @param[in] user The array of users from which it will get all the playlists.
 * @param[in] usersNumber The size of the users' array.
 * @param[in] currentLoggedUser The current logged user.
 */
void printPublicPlaylistsToScreen(User* user, int usersNumber, User *currentLoggedUser);

/**
 * Search a playlist.
 *
 * @param[in] users The array of users from which it will get all the playlists.
 * @param[in] usersNumber The size of the users' array.
 * @param[in] playlistName The name of the playlist to search.
 * @param[in] ownerName The nickname of the searched playlit's owner.
 * @return A dinamically mapped pointer to a pointer to a Playlist.
 */
Playlist **searchInPlaylists(User users[], int usersNumber, const char *playlistName, const char *ownerName);

/**
* Get the id of a user.
*
* @param[in] users Array of Users in which to search.
* @param[in] size Size of the array
* @return It returns the index of the user. It returns -1 if there were errors.
*/
int getUserIdByNickname(User users[], int size);

/**
* Load users' datas from file.
*
* @param[in] path The path of the file.
* @param[in] songs Songs' array, used to get the list of favourite songs.
* @param[in] currentSongNumber Size of the array of songs.
* @param[in] array The array where datas should be saved.
* @param[in] maxSize The maximum size of the array.
* @return The function returns the number of read elements. It return -1 in case of any error.
*/
int loadUsersFromFile(const char* path, Song* songs, int currentSongNumber, User array[], int maxSize);

/**
* Find the user with the given username.
*
* @param[in] users The array of user in which the function will search.
* @param[in] size The size of the array.
* @param[in] nickname The nickname that has to be searched.
* @return The function returns -1 if the nickname isn't in the array.
* If it is, the function will return the position of the user in the array.
*/
int searchInUsers(User users[], int size, const char* nickname);

/**
* Insert a nickname;
*
* @param[in] usersList The list of user where the nickname has to be checked.
* @param[in] size The size of the users' list.
* @param[in] nickname String to where the nickname will be saved.
*/
void getNickname(User* usersList, int size, char *nickname);

/**
* Add a user.
*
* @param[in] users Array of users.
* @param[in] currentSize Pointer to the current dimension of the array.
* @param[in] currentLoggedUser The current logged user
* @return The function returns the new dimension of the array if it succeed. If it fails, it returns -1.
*/
int addUser(User users[], int *currentSize, User **currentLoggedUser);

/**
* Add a user to the users' .csv file.
*
* @param[in] user User to add to the file.
*/
void addToUsersFile(User *user);

#endif
