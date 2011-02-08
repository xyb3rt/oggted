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
#include <vector>
#include <cstdlib>
#include <cstring>

#include "oggted.h"
#include "fileio.h"
#include "oggfile.h"
#include "options.h"
#include "pattern.h"

const char *command;

/* return values:
 *   0: everything went fine
 *   1: faulty command line arguments required abort
 *   2: at least one error processing files
 */
int main(int argc, char **argv) {
	uint fileIdx;
	const char *filename;
	bool firstOutput = true;
	bool preserveTimes;
	FileTimes ptimes;
	uint retCode = 0;

	command = FileIO::_basename(argv[0]);
	if (strcmp(command, ".") == 0)
		command = PROGNAME;

	if (Options::parseCommandLine(argc, argv)) {
		cerr << "Try `" << argv[0] << " --help' for more information." << endl;
		exit(1);
	}

	for (fileIdx = 0; fileIdx < Options::fileCount; ++fileIdx) {
		filename = Options::filenames[fileIdx];

		if (!FileIO::isRegular(filename)) {
			cerr << command << ": " << filename << ": Not a regular file" << endl;
			retCode |= 2;
			continue;
		}

		if (!FileIO::isReadable(filename)) {
			cerr << command << ": " << filename
			     << ": Could not open file for reading" << endl;
			retCode |= 2;
			continue;
		}

		if (Options::write && !FileIO::isWritable(filename)) {
			cerr << command << ": " << filename
			     << ": Could not open file for writing" << endl;
			retCode |= 2;
			continue;
		}

		preserveTimes = Options::preserveTimes &&
				FileIO::saveTimes(filename, ptimes) == FileIO::Success;

		OggFile file(filename);
		if (!file.isValid()) {
			retCode |= 2;
			continue;
		}

		if (Options::fromFilename) {
			uint matches = Options::inPattern.match(filename);
			for (uint i = 0; i < matches; ++i)
				file.apply(Options::inPattern.getMatch(i));
		}

		vector<char*>::const_iterator eachR = Options::removals.begin();
		for (; eachR != Options::removals.end(); ++eachR)
			file.removeFields(*eachR);

		vector<GenericInfo>::const_iterator eachG = Options::generics.begin();
		for (; eachG != Options::generics.end(); ++eachG)
			file.apply(*eachG);

		vector<FieldInfo>::const_iterator eachF = Options::fields.begin();
		for (; eachF != Options::fields.end(); ++eachF)
			file.apply(*eachF);

		if (Options::write)
			file.save();

		if (Options::info || Options::list) {
			if (Options::fileCount > 1) {
				if (!firstOutput)
					cout << endl;
				else
					firstOutput = false;
				cout << filename << ":" << endl;
			}
			if (Options::info)
				file.printInfo();
			if (Options::list)
				file.listTag();
		}

		if (Options::organize) {
			for (uint i = 0; i < Options::outPattern.count(); ++i) {
				MatchInfo minfo = Options::outPattern.getMatch(i);
				file.fill(minfo);
				Options::outPattern.setMatch(i, minfo);
			}
			Options::outPattern.replaceSpecialChars(REPLACE_CHAR);
			string newPath = Options::outPattern.getText();
			if (!newPath.empty()) {
				FileIO::Status ret = FileIO::copy(filename, newPath.c_str());
				if (ret == FileIO::Error) {
					cerr << command << ": " << filename << ": Could not organize file"
					     << endl;
					retCode |= 2;
				} else if (ret == FileIO::Success && preserveTimes) {
					FileIO::resetTimes(newPath.c_str(), ptimes);
				}
			}
		}

		if (preserveTimes && (!Options::organize || !Options::move))
			FileIO::resetTimes(filename, ptimes);
	}

	return retCode;
}

