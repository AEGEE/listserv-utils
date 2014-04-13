//make sure that Subscribe is either closed, or contains Confirm
#include <glib.h>
#include "util.h"

char
review_LTX_changed_list (struct listserv *l, const char const *listname) {
  return keyword_does_contain(l, listname, "Review", "Public");
}
