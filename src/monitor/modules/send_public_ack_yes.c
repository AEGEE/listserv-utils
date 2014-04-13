//make sure that for lists with Send = Public, Ack is not Yes
#include "util.h"
#include <glib.h>

char
send_public_ack_yes_LTX_changed_list (struct listserv *l,
				      const char const * listname)
{
  if (keyword_does_contain (l, listname, "Send", "Public")
      && keyword_doesnot_contain (l, listname, "Send", "Non-member")
      && keyword_does_contain (l, listname, "Ack", "Yes")) {
    char **moderators = listserv_getlist_keyword (l, listname, "Moderator");
    if (moderators[0] == NULL)
      return 1;
  }
  return 0;
}
/*
char*
send_public_ack_yes_LTX_changed_list (const char const *listname)
{
  return g_strconcat("\
== =============================================================================\r\n\
", NULL);
}
*/
