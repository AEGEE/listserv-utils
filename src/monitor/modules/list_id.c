#include <glib.h>
#include <liblistserv.h>
#include "util.h"

char
list_id_LTX_changed_list (struct listserv *l, const char const * listname) {
  char **list_id = listserv_getlist_keyword (l, listname, "List-Id");
  if (list_id[0] != NULL) {
    if (keyword_try_update (l, listname, "List-Id", NULL) == 1)
      return 1;
    list_id = listserv_getlist_keyword (l, listname, "List-Id");
    return (list_id[0] != NULL);
  }
  return 0;
}
