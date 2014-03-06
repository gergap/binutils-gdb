/* source-id-related functions.

   Copyright (C) 2014 Gerhard Gappmeier, ascolab GmbH

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef SOURCE_ID_H
#define SOURCE_ID_H

extern char *source_lookup;
extern void _initialize_source_id (void);

/* This function calls an external script which fetches the source for
 * filename from a source server. The source version is identified by
 * the source-id field.
 */
extern int fetch_source (bfd *abfd, const char *filename,
    char *cachefile, size_t len);

#endif /* SOURCE_ID_H */
