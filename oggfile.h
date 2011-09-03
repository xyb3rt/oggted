/* oggted: oggfile.h
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

#ifndef __OGGFILE_H__
#define __OGGFILE_H__

#include <taglib/vorbisfile.h>
#include <taglib/xiphcomment.h>

#include "oggted.h"
#include "pattern.h"
#include "types.h"

class OggFile {
	public:
		explicit OggFile(const char*);

		bool isValid() const { return file.isValid(); }
		bool isReadOnly() const { return file.readOnly(); }
		const char* filename() const { return file.name(); }

		void apply(const GenericInfo&);
		void apply(const FieldInfo&);
		void apply(const MatchInfo&);
		void fill(MatchInfo&);
		void removeFields(const char*);
		void strip();
		bool save();

		void printInfo() const;
		void listTag() const;

	private:
		Ogg::Vorbis::File file;
		Ogg::XiphComment *tag;
};

#endif /* __OGGFILE_H__ */
