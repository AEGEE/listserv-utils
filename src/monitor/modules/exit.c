//make sure that Exit is MAILING_LISTS_SIEVE
#include <glib.h>
#include "util.h"

char
exit_LTX_changed_list (struct listserv *l, const char const * listname) {
  if (keyword_doesnot_contain (l, listname, "Exit", "MAILING_LISTS_SIEVE"))
    return 1;
  return 0;
}
