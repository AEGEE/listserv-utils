//make sure that Subscribe is either closed, or contains Confirm
#include <glib.h>
#include "util.h"

static inline int core (struct listserv *l, const char const *listname) {
  return (keyword_does_contain (l, listname, "Validate", "Yes")
	  || keyword_does_contain (l, listname, "Validate", "No")
	  || keyword_doesnot_contain (l, listname, "Validate", "Confirm"));
}

char
validate_LTX_changed_list (struct listserv *l, const char const * listname) {
    if (core (l, listname)) {
      keyword_try_update (l, listname, "Validate", "All, Confirm");
      return core (l, listname);
    }
    return 0;
}
