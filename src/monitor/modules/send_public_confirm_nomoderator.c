//make sure that for lists with Send = Public, Confidential is not No
#include "util.h"
#include <glib.h>

char
send_public_confirm_nomoderator_LTX_changed_list (struct listserv *l,
						  const char const * listname)
{
  if (keyword_does_contain (l, listname, "Send", "Public")
      && keyword_doesnot_contain (l, listname, "Send", "Non-Member")
      && keyword_does_contain (l, listname, "Send", "Confirm")) {
    char **moderators = listserv_getlist_keyword (l, listname, "Moderator");
    if (moderators[0] == NULL)
      return 1;
  }
  return 0;
}
