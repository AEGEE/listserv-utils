#include <glib.h>
#include <liblistserv.h>
#include <string.h>
#include <unistd.h>
#include <glib/gstdio.h>
#include "util.h"

char
web_archives_LTX_changed_list (struct listserv *l, const char const * listname)
{
  if (keyword_doesnot_contain (l, listname, "Notebook", "No")) {
    int len = strlen (listname) * 2;
    char *path = g_alloca (38 + len);
    g_sprintf (path, "/home/listserv/public_html/%s/%s.private",
	       listname, listname);
    for (; len >= 0; len--)
      path[27+len] = g_ascii_tolower (path[27+len]);
    if (access (path, F_OK) == -1)
      return 1;
  }
  return 0;
}
