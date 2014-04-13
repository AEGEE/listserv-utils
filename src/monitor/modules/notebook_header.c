//make sure that Subscribe is either closed, or contains Confirm
#include <glib.h>
#include "util.h"

char
notebook_header_LTX_changed_list (struct listserv *l,
				  const char const * listname)
{
  if (keyword_doesnot_contain (l, listname, "Notebook-Header", "Full"))
    return keyword_try_update (l, listname, "Notebook-Header", "Full")
      ||  keyword_doesnot_contain (l, listname, "Notebook-Header", "Full");
  return 0;
}
