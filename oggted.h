/* oggted: oggted.h
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

#ifndef __OGGTED_H__
#define __OGGTED_H__

#include <taglib/taglib.h>
#include <taglib/tstring.h>

#include "config.h"

#define PROGNAME "oggted"
#define VERSION  "git-20111006"

using namespace std;
using namespace TagLib;
using TagLib::uint;

extern const char *command;

static const String::Type DEF_TSTR_ENC = USE_UTF8 ? String::UTF8
                                                  : String::Latin1;

#endif /* __OGGTED_H__ */

