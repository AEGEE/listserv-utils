//make sure that X-Tags is not Comment
#include <glib.h>
#include <liblistserv.h>

char
x_tags_LTX_changed_list (struct listserv *l, const char const *listname) {
  char **kw  = listserv_getlist_keyword (l, listname, "X-Tags");
  if (g_ascii_strcasecmp (kw[0], "Comment") == 0)
    return 1;
  return 0;
}
