//make sure that Change-log is only Yes
#include <glib.h>
#include <liblistserv.h>

char
change_log_LTX_changed_list (struct listserv *l, const char const *listname) {
  char **kw  = listserv_getlist_keyword (l, listname, "Change-Log");
  if (kw[1] != NULL
      || g_ascii_strcasecmp (kw[0], "Yes"))
    return 1;
  return 0;
}
