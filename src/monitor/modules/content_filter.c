#include "util.h"
#include <liblistserv.h>
#include <glib.h>

char
content_filter_LTX_changed_list (struct listserv* l,
				 const char const * listname)
{
  if (keyword_does_contain (l, listname, "Send", "Public")
      && keyword_doesnot_contain (l, listname, "Send", "Non-member")) {
    char **moderators = listserv_getlist_keyword (l, listname, "Moderator");
    if (moderators[0] == NULL) {
      struct listserv_content_filter** cf = listserv_getcontent_filter(l, listname);
      if (cf[0] == NULL
	  || cf[0]->action != 'R')
	return 1;
    }
  }
  return 0;
}
