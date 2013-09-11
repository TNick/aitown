/**
 * @file  cfgpath.c
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

#include "cfgpath.h"
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

void get_user_config_file(char *out, unsigned int maxlen, const char *appname)
{
#ifdef __linux__
	const char *out_orig = out;
	char *home = getenv("XDG_CONFIG_HOME");
	unsigned int config_len = 0;
	if (!home) {
		home = getenv("HOME");
		if (!home) {
			// Can't find home directory
			out[0] = 0;
			return;
		}
		config_len = strlen(".config/");
	}

	unsigned int home_len = strlen(home);
	unsigned int appname_len = strlen(appname);
	const int ext_len = strlen(".conf");

	/* first +1 is "/", second is terminating null */
	if (home_len + 1 + config_len + appname_len + ext_len + 1 > maxlen) {
		out[0] = 0;
		return;
	}

	memcpy(out, home, home_len);
	out += home_len;
	*out = '/';
	out++;
	if (config_len) {
		memcpy(out, ".config/", config_len);
		out += config_len;
		/* Make the .config folder if it doesn't already exist */
		mkdir(out_orig, 0755);
	}
	memcpy(out, appname, appname_len);
	out += appname_len;
	memcpy(out, ".conf", ext_len);
	out += ext_len;
	*out = 0;
#elif defined(WIN32)
	if (maxlen < MAX_PATH) {
		out[0] = 0;
		return;
	}
	if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, out))) {
		out[0] = 0;
		return;
	}
	/* We don't try to create the AppData folder as it always exists already */
	unsigned int appname_len = strlen(appname);
	if (strlen(out) + 1 + appname_len + strlen(".conf") + 1 > maxlen) {
		out[0] = 0;
		return;
	}
	strcat(out, "\\");
	strcat(out, appname);
	strcat(out, ".conf");
#endif
}

void get_user_config_folder(char *out, unsigned int maxlen, const char *appname)
{
#ifdef __linux__
	const char *out_orig = out;
	char *home = getenv("XDG_CONFIG_HOME");
	unsigned int config_len = 0;
	if (!home) {
		home = getenv("HOME");
		if (!home) {
			// Can't find home directory
			out[0] = 0;
			return;
		}
		config_len = strlen(".config/");
	}

	unsigned int home_len = strlen(home);
	unsigned int appname_len = strlen(appname);

	/* first +1 is "/", second is trailing "/", third is terminating null */
	if (home_len + 1 + config_len + appname_len + 1 + 1 > maxlen) {
		out[0] = 0;
		return;
	}

	memcpy(out, home, home_len);
	out += home_len;
	*out = '/';
	out++;
	if (config_len) {
		memcpy(out, ".config/", config_len);
		out += config_len;
		/* Make the .config folder if it doesn't already exist */
		*out = '\0';
		mkdir(out_orig, 0755);
	}
	memcpy(out, appname, appname_len);
	out += appname_len;
	/* Make the .config/appname folder if it doesn't already exist */
	*out = '\0';
	mkdir(out_orig, 0755);
	*out = '/';
	out++;
	*out = 0;
#elif defined(WIN32)
	if (maxlen < MAX_PATH) {
		out[0] = 0;
		return;
	}
	if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, out))) {
		out[0] = 0;
		return;
	}
	/* We don't try to create the AppData folder as it always exists already */
	unsigned int appname_len = strlen(appname);
	if (strlen(out) + 1 + appname_len + 1 + 1 > maxlen) {
		out[0] = 0;
		return;
	}
	strcat(out, "\\");
	strcat(out, appname);
	/* Make the AppData\appname folder if it doesn't already exist */
	mkdir(out);
	strcat(out, "\\");
#endif
}

void get_user_data_folder(char *out, unsigned int maxlen, const char *appname)
{
#ifdef __linux__
	const char *out_orig = out;
	char *home = getenv("XDG_DATA_HOME");
	unsigned int config_len = 0;
	if (!home) {
		home = getenv("HOME");
		if (!home) {
			// Can't find home directory
			out[0] = 0;
			return;
		}
		config_len = strlen(".local/share/");
	}

	unsigned int home_len = strlen(home);
	unsigned int appname_len = strlen(appname);

	/* first +1 is "/", second is trailing "/", third is terminating null */
	if (home_len + 1 + config_len + appname_len + 1 + 1 > maxlen) {
		out[0] = 0;
		return;
	}

	memcpy(out, home, home_len);
	out += home_len;
	*out = '/';
	out++;
	if (config_len) {
		memcpy(out, ".local/share/", config_len);
		out += config_len;
		/* Make the .local/share folder if it doesn't already exist */
		*out = '\0';
		mkdir(out_orig, 0755);
	}
	memcpy(out, appname, appname_len);
	out += appname_len;
	/* Make the .local/share/appname folder if it doesn't already exist */
	*out = '\0';
	mkdir(out_orig, 0755);
	*out = '/';
	out++;
	*out = 0;
#elif defined(WIN32)
	/* No distinction under Windows */
	return get_user_config_folder(out, maxlen, appname);
#endif
}

void get_user_cache_folder(char *out, unsigned int maxlen, const char *appname)
{
#ifdef __linux__
	const char *out_orig = out;
	char *home = getenv("XDG_CACHE_HOME");
	unsigned int config_len = 0;
	if (!home) {
		home = getenv("HOME");
		if (!home) {
			// Can't find home directory
			out[0] = 0;
			return;
		}
		config_len = strlen(".cache/");
	}

	unsigned int home_len = strlen(home);
	unsigned int appname_len = strlen(appname);

	/* first +1 is "/", second is trailing "/", third is terminating null */
	if (home_len + 1 + config_len + appname_len + 1 + 1 > maxlen) {
		out[0] = 0;
		return;
	}

	memcpy(out, home, home_len);
	out += home_len;
	*out = '/';
	out++;
	if (config_len) {
		memcpy(out, ".cache/", config_len);
		out += config_len;
		/* Make the .cache folder if it doesn't already exist */
		*out = '\0';
		mkdir(out_orig, 0755);
	}
	memcpy(out, appname, appname_len);
	out += appname_len;
	/* Make the .cache/appname folder if it doesn't already exist */
	*out = '\0';
	mkdir(out_orig, 0755);
	*out = '/';
	out++;
	*out = 0;
#elif defined(WIN32)
	if (maxlen < MAX_PATH) {
		out[0] = 0;
		return;
	}
	if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, out))) {
		out[0] = 0;
		return;
	}
	/* We don't try to create the AppData folder as it always exists already */
	unsigned int appname_len = strlen(appname);
	if (strlen(out) + 1 + appname_len + 1 + 1 > maxlen) {
		out[0] = 0;
		return;
	}
	strcat(out, "\\");
	strcat(out, appname);
	/* Make the AppData\appname folder if it doesn't already exist */
	mkdir(out);
	strcat(out, "\\");
#endif
}
