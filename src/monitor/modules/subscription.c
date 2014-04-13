//make sure that Subscribe is either closed, or contains Confirm
#include <glib.h>
#include <string.h>
#include "util.h"

char
subscription_LTX_changed_list (struct listserv *l,
			       const char const * listname)
{
  char **keywords = listserv_getlist_keyword(l, listname, "Subscription");
  if (strcasecmp(keywords[0], "Open") != 0
      && strcasecmp(keywords[0], "By_Owner") != 0
      && strcasecmp(keywords[0], "Closed") != 0)
    return 1;
  return 0;
}
