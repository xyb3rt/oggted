/* oggted: oggfile.h
 * Copyright (c) 2010 Bert Muennich <muennich at informatik.hu-berlin.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 * USA.
 */

#ifndef __OGGFILE_H__
#define __OGGFILE_H__

#include "oggted.h"
#include "types.h"

class OggFile {
	public:
		explicit OggFile(const char*);
		~OggFile();

		bool isValid() const { return file.isValid(); }
		bool isReadOnly() const { return file.readOnly(); }
		const char* filename() const { return file.name(); }

		void apply(GenericInfo&);
		void apply(FieldInfo&);
		void apply(const MatchInfo&);
		void fill(MatchInfo&);
		void removeFields(const char*);
		bool save();

		void showInfo() const;
		void listTag() const;

		int filenameToTag(const char*);
		int organize(const char*, bool = false, bool = false, struct timeval* = NULL);

	private:
		Ogg::Vorbis::File file;
		Ogg::XiphComment *tag;
};

#endif /* __OGGFILE_H__ */
