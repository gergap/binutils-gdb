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

#include "defs.h"
#include "source-id.h"
#include "command.h"
#include "cli/cli-cmds.h"
#include "elf-bfd.h"

/* source lookup hook script */
char *source_lookup = NULL;

/* Handler for "set source-lookup path" command. */

static void
set_source_lookup_command (char *args, int from_tty)
{
    if (source_lookup)
        xfree(source_lookup);
    source_lookup = xstrdup(args);
}

/* Handler for "unset source-lookup" command. */

static void
unset_source_lookup_command (char *args, int from_tty)
{
    if (source_lookup)
        xfree(source_lookup);
    source_lookup = NULL;
}

/* Handler for "show source-lookup" command.  */

static void
show_source_lookup_command (char *args, int from_tty)
{
  printf_filtered (_("source-lookup: %s\n"), source_lookup);
}


void
_initialize_source_id (void)
{
  add_cmd ("source-lookup", class_files, set_source_lookup_command,
    _("\
Usage: set source-lookup FETCH_SCRIPT\n\
GDB calls this script to fech the correct source file for the executable\n\
from a version control system (e.g. git). GDB only calls this script\n\
if the executable being debugged contains a source-id section with\n\
version information."),
    &setlist);

  add_cmd ("source-lookup", class_files, unset_source_lookup_command,
    _("\
Usage: unset source-lookup\n\
Disables source fetching."),
    &unsetlist);

  add_cmd ("source-lookup", class_files, show_source_lookup_command,
    _("\
Usage: show source-lookup\n\
Shows the currently active source-lookup fetch script."),
    &showlist);
}

int
fetch_source (bfd *abfd, const char *filename, char *cachefile, size_t len)
{
    int ret;
    struct elf_obj_tdata *t;
    char command[PATH_MAX];
    const char *filehash = "hash"; /* TODO: add file hash */
    FILE *f;

    /*printf_unfiltered (_("fetch_source: %p %s\n"), abfd, filename);*/

    /* return if now source-lookup hook was configured */
    if (source_lookup == NULL)
      return 0;

    /* return if now source-id info is available */
    t = elf_tdata (abfd);
    if (t->source_id == NULL)
      return 0;

    /*
    printf_unfiltered (_("source_id: %s %s %s\n"),
        t->source_id->vcs_type,
        t->source_id->vcs_url,
        t->source_id->vcs_version
    );
     */
    ret = snprintf (command, PATH_MAX, "%s %s %s %s %s %s",
        source_lookup,
        t->source_id->vcs_type,
        t->source_id->vcs_url,
        t->source_id->vcs_version,
        filename,
        filehash);
    if (ret >= PATH_MAX)
      {
        printf_unfiltered("Fetch command was truncated. Aim for shorter path names.\n");
        return -1;
      }

    /*printf_unfiltered("Calling system %s\n", command);*/
    f = popen (command, "r");
    if (f)
      {
        if (fgets(cachefile, len, f) != NULL)
          {
            char *tmp = strchr (cachefile, '\n');
            if (tmp)
              *tmp = 0;
          }
        else
          {
            cachefile[0] = 0;
          }
        ret = pclose(f); /* close stream and get exit code */
      }

    if (ret != 0)
      {
        printf_unfiltered("fetching sources failed: system call result=%i, exitcode=%i\n",
                          ret, WEXITSTATUS(ret));
      }

    return ret;
}

