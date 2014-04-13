#include <glib.h>
#include <liblistserv.h>

char
sub_lists_valid_LTX_changed_list (struct listserv *l,
				  const char const * listname)
{
  char **kw = listserv_getlist_keyword (l, listname, "Sub-lists");
  int i = 0;
  while (kw[i]) {
    char *sublist = listserv_get (l, kw[i], "HDR");
    if (sublist[0] == 'T')
      return 1;
    i++;
  }
  return 0;
}
