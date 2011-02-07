/* oggted: main.cpp
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
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>

#include "oggted.h"
#include "fileio.h"
#include "oggfile.h"
#include "options.h"

const char *command;

int main(int argc, char **argv) {
	uint fileIdx;
	bool firstOutput = true;

	command = FileIO::_basename(argv[0]);
	if (strcmp(command, ".") == 0)
		command = PROGNAME;

	if (Options::parseCommandLine(argc, argv)) {
		cerr << "Try `" << argv[0] << " --help' for more information." << endl;
		exit(1);
	}

	for (fileIdx = 0; fileIdx < Options::fileCount; ++fileIdx) {
		const char *filename = Options::filenames[fileIdx];
		OggFile file(filename);

		if (Options::listTag) {
			if (Options::fileCount > 1) {
				if (!firstOutput)
					cout << endl;
				else
					firstOutput = false;
				cout << filename << ":" << endl;
			}

			file.listTag();
		}
	}

	return 0;
}

