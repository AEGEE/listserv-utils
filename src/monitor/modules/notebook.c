#include <glib.h>
#include <string.h>
#include "util.h"
#include <glib/gstdio.h>

char
notebook_LTX_changed_list (struct listserv *l, const char const * listname) {
  if (keyword_does_contain (l, listname, "Notebook", "No"))
  return 0;
  if (keyword_doesnot_contain(l, listname, "Notebook", "Monthly"))
    return 1;
  char *path = g_alloca (strlen (listname) + 16);
  char *list2 = g_ascii_strdown (listname, -1);
  g_sprintf (path, "/home/listserv/%s", list2);
  g_free (list2);
  char **kw = listserv_getlist_keyword (l, listname, "Notebook");
  if (g_strcasecmp (kw[0], "Yes")
      || g_strcasecmp (kw[1], path))
    return 1;
  return 0;
}
