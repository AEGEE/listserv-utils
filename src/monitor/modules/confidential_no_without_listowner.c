#include <glib.h>
#include <liblistserv.h>
#include <string.h>
#include "util.h"

static inline int core(struct listserv *l,
		       const char const *listname) {
  return keyword_does_contain (l, listname, "Confidential", "No")
    && keyword_does_contain(l, listname, "Owner", "nichts@aegee.org");
}

char
unused_confidential_no_LTX_changed_list (struct listserv *l,
					  const char const * listname)
{
  if (core (l, listname)) {
    keyword_try_update (l, listname, "Confidential", "Yes");
    return core (l, listname);
  }
  return 0;
}
