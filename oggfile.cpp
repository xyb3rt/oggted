/* oggted: oggfile.cpp
 * Copyright (c) 2011 Bert Muennich <be.muennich at googlemail.com>
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

#include <iostream>
#include <sstream>
#include <cstdio>

#include <taglib/vorbisproperties.h>

#include "oggfile.h"

OggFile::OggFile(const char *filename) : file(filename), tag(NULL) {
	if (file.isValid())
		tag = file.tag();
}

void OggFile::apply(const GenericInfo &info) {
	if (!file.isValid() || file.readOnly() || tag == NULL)
		return;

	switch(info.id) {
		case 'a':
			tag->setArtist(info.value);
			break;
		case 'A':
			tag->setAlbum(info.value);
			break;
		case 't':
			tag->setTitle(info.value);
			break;
		case 'c':
			tag->setComment(info.value);
			break;
		case 'g':
			tag->setGenre(info.value);
			break;
		case 'T':
			tag->setTrack(info.value.toInt());
			break;
		case 'y':
			tag->setYear(info.value.toInt());
			break;
	}
}

void OggFile::apply(const FieldInfo &info) {
	if (!file.isValid() || file.readOnly() || tag == NULL)
		return;
	if (info.name.isEmpty())
		return;

	tag->addField(info.name, info.value, info.replace);
}

void OggFile::apply(const MatchInfo &info) {
	GenericInfo ginfo;
	FieldInfo finfo;

	if (!file.isValid() || file.readOnly() || tag == NULL)
		return;
	if (info.id == 0 || info.text.length() == 0)
		return;

	switch (info.id) {
		case 'a':
		case 'A':
		case 't':
		case 'c':
		case 'g':
		case 'T':
		case 'y':
			ginfo.id = info.id;
			ginfo.value = String(info.text, DEF_TSTR_ENC);
			apply(ginfo);
			break;
		case 'd':
			finfo.name = "DISCNUMBER";
			finfo.value = String(info.text, DEF_TSTR_ENC);
			apply(finfo);
			break;
	}
}

void OggFile::fill(MatchInfo &info) {
	string &text = info.text;
	ostringstream tmp;
	uint track, year;
	Ogg::FieldListMap map;

	tmp.fill('0');

	if (!file.isValid())
		return;
	if (info.id == 0)
		return;

	switch (info.id) {
		case 'a':
			text = tag->artist().toCString(USE_UTF8);
			if (text.empty())
				text = "Unknown Artist";
			break;
		case 'A':
			text = tag->album().toCString(USE_UTF8);
			if (text.empty())
				text = "Unknown Album";
			break;
		case 't':
			text = tag->title().toCString(USE_UTF8);
			if (text.empty())
				text = "Unknown Title";
			break;
		case 'g':
			text = tag->genre().toCString(USE_UTF8);
			break;
		case 'y':
			year = tag->year();
			if (year) {
				tmp << year;
				text = tmp.str();
			}
			break;
		case 'T':
			tmp.width(2);
			track = tag->track();
			if (track)
				tmp << track;
			else
				tmp << 0;
			text = tmp.str();
			break;
		case 'd':
			map = tag->fieldListMap();
			if (map.contains("DISCNUMBER"))
				text = map["DISCNUMBER"].front().toCString(USE_UTF8);
			if (text.empty())
				text = "0";
			break;
	}
}

void OggFile::removeFields(const char *id) {
	if (id == NULL)
		return;
	if (!file.isValid() || file.readOnly() || tag == NULL)
		return;

	tag->removeField(id);
}

void OggFile::strip() {
	if (!file.isValid() || file.readOnly() || tag == NULL)
		return;

	while (!tag->isEmpty())
		tag->removeField(tag->fieldListMap().begin()->first);
}

bool OggFile::save() {
	if (!file.isValid() || file.readOnly() || tag == NULL)
		return false;

	return file.save();
}

void OggFile::printInfo() const {
	Vorbis::Properties *prop;
	int len;

	if (!file.isValid())
		return;

	if ((prop = file.audioProperties()) == NULL)
		return;

	len = prop->length();
	printf("Ogg Vorbis version %d, length: %02d:%02d:%02d\n",
	       prop->vorbisVersion(), len / 3600, len / 60, len % 60);
	printf("Nominal bitrate: %d kBit/s, sample rate: %d Hz, %d channels\n",
				 prop->bitrate(), prop->sampleRate(), prop->channels());
}

void OggFile::listTag() const {
	Ogg::FieldListMap map;
	Ogg::FieldListMap::Iterator eachKey;
	StringList values;
	StringList::ConstIterator eachVal;

	if (!file.isValid() || tag == NULL)
		return;

	map = tag->fieldListMap();
	for (eachKey = map.begin(); eachKey != map.end(); ++eachKey) {
		values = eachKey->second;
		for (eachVal = values.begin(); eachVal != values.end(); ++eachVal)
			cout << eachKey->first << "=" << eachVal->toCString(USE_UTF8) << endl;
	}
}
