/**
 * @file  cfgpath.h
 * @brief Cross platform methods for obtaining paths to configuration files.
 *
 * Copyright (C) 2013 Adam Nielsen <malvineous@shikadi.net>
 *
 * This code is placed in the public domain.  You are free to use it for any
 * purpose.  If you add new platform support, please contribute a patch!
 *
 * Example use:
 *
 * char cfgdir[256];
 * get_user_config_file(cfgdir, sizeof(cfgdir), "myapp");
 * if (cfgdir[0] == 0) {
 *     printf("Unable to find home directory.\n");
 *     return 1;
 * }
 * printf("Saving configuration file to %s\n", cfgdir);
 *
 * A number of constants are also defined:
 *
 *  - MAX_PATH: Maximum length of a path, in characters.  Used to allocate a
 *      char array large enough to hold the returned path.
 *
 *  - PATH_SEPARATOR_CHAR: The separator between folders.  This will be either a
 *      forward slash or a backslash depending on the platform.  This is a
 *      character constant.
 *
 *  - PATH_SEPARATOR_STRING: The same as PATH_SEPARATOR_CHAR but as a C string,
 *      to make it easier to append to other string constants.
 */

#ifndef CFGPATH_H_
#define CFGPATH_H_

#ifdef __linux__
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#define MAX_PATH 512  /* arbitrary value */
#define PATH_SEPARATOR_CHAR '/'
#define PATH_SEPARATOR_STRING "/"
#elif defined(WIN32)
#include <shlobj.h>
#include <direct.h>
/* MAX_PATH is defined by the Windows API */
#define PATH_SEPARATOR_CHAR '\\'
#define PATH_SEPARATOR_STRING "\\"
#else
#error cfgpath.h functions have not been implemented for your platform!  Please send patches.
#endif

#ifdef __cplusplus
extern "C" {
#endif 

/** Get an absolute path to a single configuration file, specific to this user.
 *
 * This function is useful for programs that need only a single configuration
 * file.  The file is unique to the user account currently logged in.
 *
 * Output is typically:
 *
 *   Windows: C:\Users\jcitizen\AppData\Roaming\appname.ini
 *   Linux: /home/jcitizen/.config/appname.conf
 *   Mac: <not implemented, please send patches>
 *
 * @param out
 *   Buffer to write the path.  On return will contain the path, or an empty
 *   string on error.
 *
 * @param maxlen
 *   Length of out.  Must be >= MAX_PATH.
 *
 * @param appname
 *   Short name of the application.  Avoid using spaces or version numbers, and
 *   use lowercase if possible.
 *
 * @post The file may or may not exist.
 * @post The folder holding the file is created if needed.
 */
extern void get_user_config_file(char *out, unsigned int maxlen, const char *appname);

/** Get an absolute path to a configuration folder, specific to this user.
 *
 * This function is useful for programs that need to store multiple
 * configuration files.  The output is a folder (which may not exist and will
 * need to be created) suitable for storing a number of files.
 *
 * The returned path will always end in a platform-specific trailing slash, so
 * that a filename can simply be appended to the path.
 *
 * Output is typically:
 *
 *   Windows: C:\Users\jcitizen\AppData\Roaming\appname\
 *   Linux: /home/jcitizen/.config/appname/
 *   Mac: <not implemented, please send patches>
 *
 * @param out
 *   Buffer to write the path.  On return will contain the path, or an empty
 *   string on error.
 *
 * @param maxlen
 *   Length of out.  Must be >= MAX_PATH.
 *
 * @param appname
 *   Short name of the application.  Avoid using spaces or version numbers, and
 *   use lowercase if possible.
 *
 * @post The folder is created if needed.
 */
extern void get_user_config_folder(char *out, unsigned int maxlen, const char *appname);

/** Get an absolute path to a data storage folder, specific to this user.
 *
 * This function is useful for programs that need to store larger amounts of
 * data specific to each user.  The output is a folder (which may not exist and
 * will need to be created) suitable for storing a number of data files.
 *
 * This path should be used for persistent, important data files the user would
 * want to keep.  Do not use this path for temporary files, cache files, or
 * other files that can be recreated if they are deleted.  Use
 * get_user_cache_folder() for those instead.
 *
 * The returned path will always end in a platform-specific trailing slash, so
 * that a filename can simply be appended to the path.
 *
 * Output is typically:
 *
 *   Windows: C:\Users\jcitizen\AppData\Roaming\appname-data\
 *   Linux: /home/jcitizen/.local/share/appname/
 *   Mac: <not implemented, please send patches>
 *
 * @param out
 *   Buffer to write the path.  On return will contain the path, or an empty
 *   string on error.
 *
 * @param maxlen
 *   Length of out.  Must be >= MAX_PATH.
 *
 * @param appname
 *   Short name of the application.  Avoid using spaces or version numbers, and
 *   use lowercase if possible.
 *
 * @post The folder is created if needed.
 */
extern void get_user_data_folder(char *out, unsigned int maxlen, const char *appname);

/** Get an absolute path to a temporary storage folder, specific to this user.
 *
 * This function is useful for programs that temporarily need to store larger
 * amounts of data specific to each user.  The output is a folder (which may
 * not exist and will need to be created) suitable for storing a number of
 * temporary files.  The files may be lost or deleted when the program
 * terminates.
 *
 * This path should be used for temporary, unimportant data files that can
 * safely be deleted after the program terminates.  Do not use this path for
 * any important files the user would want to keep.  Use get_user_data_folder()
 * for those instead.
 *
 * The returned path will always end in a platform-specific trailing slash, so
 * that a filename can simply be appended to the path.
 *
 * Output is typically:
 *
 *   Windows: C:\Users\jcitizen\AppData\Local\appname\
 *   Linux: /home/jcitizen/.cache/appname/
 *   Mac: <not implemented, please send patches>
 *
 * @param out
 *   Buffer to write the path.  On return will contain the path, or an empty
 *   string on error.
 *
 * @param maxlen
 *   Length of out.  Must be >= MAX_PATH.
 *
 * @param appname
 *   Short name of the application.  Avoid using spaces or version numbers, and
 *   use lowercase if possible.
 *
 * @post The folder is created if needed.
 */
extern void get_user_cache_folder(char *out, unsigned int maxlen, const char *appname);

#ifdef __cplusplus
}
#endif 
#endif /* CFGPATH_H_ */
