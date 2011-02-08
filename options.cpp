/* oggted: options.cpp
 * Copyright (c) 2011 Bert Muennich <muennich at informatik.hu-berlin.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <iostream>
#include <cstring>
#include <cstdlib>

#include "options.h"

bool Options::parseCommandLine(int argc, char **argv) {
	int opt, nlen, vlen;
	GenericInfo ginfo;
	FieldInfo finfo;
	char *sep;
	bool error = false;

	while (!error) {
		opt = getopt_long(argc, argv, options, longOptions, NULL);

		if (opt == -1)
			break;

		switch (opt) {
			/* invalid option given */
			case '?':
				error = true;
				break;
			/* help, general info & others */
			case 'h':
				printUsage();
				exit(0);
			case 'v':
				printVersion();
				exit(0);
			case 'p':
				preserveTimes = true;
				break;
			/* generic tag infomation */
			case 'a':
			case 'A':
			case 't':
			case 'c':
			case 'g':
			case 'T':
			case 'y': {
				ginfo.id = opt;
				ginfo.value = optarg;
				generics.push_back(ginfo);
				write = true;
				break;
			}
			/* modification of fields */
			case 'F':
			case 'f':
				if (!(sep = strchr(optarg, '=')) || !(vlen = strlen(sep + 1)) || strchr(sep + 1, '=')) {
					cerr << command << ": invalid argument for option -f: " << optarg << endl;
					exit(1);
				}
				nlen = sep - optarg;
				finfo.name = string(optarg, nlen);
				finfo.value = string(sep + 1, vlen);
				finfo.replace = opt == 'F';
				fields.push_back(finfo);
				write = true;
				break;
			/* information from the files */
			case 'i':
				info = true;
				break;
			case 'l':
				list = true;
				break;
			/* tag removal & version to write */
			case 'r':
				removals.push_back(optarg);
				write = true;
				break;
			/* filename <-> tag information */
			case 'N':
			case 'n':
				fromFilename = inPattern.setPattern(optarg, opt == 'N');
				write = true;
				break;
			case 'o':
				organize = outPattern.setPattern(optarg);
				break;
			case OPT_LO_FORCE:
				force = true;
				break;
			case OPT_LO_ORG_MOVE:
				move = true;
				break;
		}
	}

	fileCount = argc - optind;
	filenames = argv + optind;

	if (!error) {
		// check for missing mandatory arguments
		if (optind == 1) {
			cerr << command << ": missing arguments" << endl;
			error = true;
		} else if (fileCount == 0) {
			cerr << command << ": missing <FILES>" << endl;
			error = true;
		}
	}

	return error;
}

void Options::printVersion() {
	cout << PROGNAME << " - command line ogg vorbis tag editor\n"
	     << "Version " << VERSION << ", written by Bert Muennich\n"
	     << "Uses TagLib v" << TAGLIB_MAJOR_VERSION << "."
	     << TAGLIB_MINOR_VERSION << "." << TAGLIB_PATCH_VERSION
	     << ", written by Scott Wheeler" << endl;
}

void Options::printUsage() {
	cout << "Usage: " << command << " [OPTIONS]... <FILES>\n\n"
	     << "OPTIONS:\n"
	     << "If a long option shows an argument as mandatory,\n"
	     << "then it is also mandatory for the equivalent short option.\n\n"
	     << "  -h, --help             display this help and exit\n"
	     << "  -v, --version          display version information and exit\n"
	     << "  -p, --preserve-times   preserve access and modification times of the files\n\n";
	cout << "To alter the most common tag information:\n"
	     << "  -a, --artist ARTIST    set the artist information\n"
	     << "  -A, --album ALBUM      set the album title information\n"
	     << "  -t, --title SONG       set the song title information\n"
	     << "  -c, --comment COMMENT  set the comment information\n"
	     << "  -g, --genre NUM        set the genre number\n"
	     << "  -T, --track NUM        set the track number\n"
	     << "  -y, --year NUM         set the year\n\n";
	cout << "To add/modify all fields:\n"
	     << "  -f, --add-field NAME=VALUE\n"
	     << "                         add field with name NAME and content VALUE to the tags\n"
	     << "  -F, --replace-field NAME=VALUE\n"
	     << "                         same as -f, but remove all other fields with the same\n"
	     << "                         name from the tags first\n\n";
	cout << "Get information from the files:\n"
	     << "  -i, --info             display general information for the files\n"
	     << "  -l, --list             list the tag contents of the files\n\n"
	     << "To remove fields and whole tags\n"
	     << "  -r, --remove NAME      remove all fields with the given name from the tags\n"
	     << "  -D, --delete-all       delete the whole tags of the files\n\n";
	cout << "Filename <-> tag information:\n"
	     << "  -n, --file-pattern PATTERN\n"
	     << "                         extract tag information from the given filenames,\n"
	     << "                         using PATTERN (for supported wildcards see below)\n"
	     << "  -N, --file-regex PATTERN\n"
	     << "                         same as -n, but interpret PATTERN as an extended regex\n"
	     << "  -o, --organize PATTERN organize files into directory structure specified\n"
	     << "                         by PATTERN (for supported wildcards see below)\n"
	     << "      --force            overwrite existing files without asking (-o,-x)\n"
	     << "      --move             when using -o, move files instead of copying them\n\n";
	cout << "The following wildcards are supported for the -o,-n,-N option arguments:\n"
	     << "    %a: Artist, %A: album, %t: title, %g: genre, %y: year,\n"
	     << "    %d: disc number, %T: track number, %%: percent sign" << endl;
}

bool Options::write = false;
bool Options::info = false;
bool Options::list = false;
bool Options::force = false;
bool Options::preserveTimes = false;
bool Options::move = false;
bool Options::fromFilename = false;
IPattern Options::inPattern;
bool Options::organize = false;
OPattern Options::outPattern;
vector<GenericInfo> Options::generics;
vector<char*> Options::removals;
vector<FieldInfo> Options::fields;
uint Options::fileCount = 0;
char **Options::filenames = NULL;

const char* Options::options = "hvpa:A:t:c:g:T:y:F:f:ilr:Dn:N:o:";
const struct option Options::longOptions[] = {
  /* help, general info & others */
  { "help",           no_argument,       NULL, 'h' },
  { "version",        no_argument,       NULL, 'v' },
  { "preserve-times", no_argument,       NULL, 'p' },
  /* alter generic tag infomation */
  { "artist",         required_argument, NULL, 'a' },
  { "album",          required_argument, NULL, 'A' },
  { "title",          required_argument, NULL, 't' },
  { "comment",        required_argument, NULL, 'c' },
  { "genre",          required_argument, NULL, 'g' },
  { "track",          required_argument, NULL, 'T' },
  { "year",           required_argument, NULL, 'y' },
	/* add/modify fields */
	{ "add-field",      required_argument, NULL, 'f' },
	{ "replace-field",  required_argument, NULL, 'F' },
  /* get information from the files */
  { "info",           no_argument,       NULL, 'i' },
  { "list",           no_argument,       NULL, 'l' },
  /* Remove fields and whole tags */
  { "remove",         required_argument, NULL, 'r' },
  { "delete-all",     no_argument,       NULL, 'D' },
	/* Filename <-> tag information */
  { "file-pattern",   required_argument, NULL, 'n' },
  { "file-regex",     required_argument, NULL, 'N' },
  { "organize",       required_argument, NULL, 'o' },
  { "force",          no_argument,       NULL, OPT_LO_FORCE },
  { "move",           no_argument,       NULL, OPT_LO_ORG_MOVE },
  { 0, 0, 0, 0 },
};

