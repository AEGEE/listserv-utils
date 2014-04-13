#include <glib.h>
#include "util.h"
#include <liblistserv.h>

char
list_address_LTX_changed_list (struct listserv *l,
			       const char const * listname)
{
  char **list_address = listserv_getlist_keyword (l, listname, "List-Address");
  if (list_address[0] != NULL) {
    if (keyword_try_update (l, listname, "List-Address", NULL) == 1)
      return 1;
    list_address = listserv_getlist_keyword (l, listname, "List-Address");
    return (list_address[0] != NULL);
  }
  return 0;
}
