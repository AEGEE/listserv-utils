//make sure that for lists with Send = Public, Confidential is not No
#include "util.h"
#include <glib.h>

char
send_public_confidential_no_LTX_changed_list (struct listserv *l,
					      const char const * listname)
{
  if (keyword_does_contain (l, listname, "Send", "Public")
      && keyword_doesnot_contain (l, listname, "Send", "Non-Member")
      && keyword_does_contain (l, listname, "Confidential", "No"))
    return 0;
  return 0;
}
/*
char*
send_public_confidential_no_LTX_listowners_mailing (const char const *listname)
{
  return g_strconcat("\
== List Visibility and Posting Rights =========================================\r\n\
Your list is visible at https://lists.aegee.org and everybody can post to it.\r\n\
This makes your list visible to spammers and will lead to increse of the\r\n\
incoming spam.  To keep your list visible, it must satisfy some more criteria:\r\n\
  * Only one list per antenna, project or working groups\r\n\
  * Only lists where everybody can subscribe\r\n\
  * No lists for events\r\n\
  * No lists where everybody can post\r\n\
You must decide if you want to \"hide\" your list, or to change the settings,\r\n\
so that only a limited number of subscribers can post to the list.\r\n", NULL);
}
*/
