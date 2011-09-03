/* oggted: options.h
 * Copyright (c) 2011 Bert Muennich <muennich at informatik.hu-berlin.de>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <getopt.h>
#include <vector>

#include "oggted.h"
#include "types.h"
#include "pattern.h"

enum LongOptOnly {
	OPT_LO_FORCE = 129,
	OPT_LO_ORG_MOVE
};

class Options {
	public:
		static bool write;
		static bool strip;
		static bool info;
		static bool list;
		static bool force;
		static bool preserveTimes;
		static bool move;
		static bool fromFilename;
		static IPattern inPattern;
		static bool organize;
		static OPattern outPattern;
		static vector<GenericInfo> generics;
		static vector<char*> removals;
		static vector<FieldInfo> fields;

		static uint fileCount;
		static char **filenames;
		
		static bool parseCommandLine(int, char**);
		static void printVersion();
		static void printUsage();
		
	private:
		static const char *options;
		static const struct option longOptions[];
};

#endif /* __OPTIONS_H__ */

